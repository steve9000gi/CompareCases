////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.cpp: Qt-based GUI for prostate cancer radiation therapy 
// planning tool. This window displays a 3D graph showing the relative
// distances of the selected sets of cases from the query case with respect to
// several criteria, i.e., PTV + bladder overlap for the x axis, PTV + 
// rectum overlap for the y axis.  The z axis shows MI values, arranged min to
// max.  Because maximizing MI is desirable as opposed to proximity to the
// query case's MI value (which is always the maximum) the query case is 
// placed at the min MI value in the z direction.
//
// author: Steve Chall, RENCI
// primary collaborators: Joseph Lo, Shiva Das, Vorakarn Chanyavanich,
// Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCubeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkAssembly.h"
#include "vtkFollower.h"
#include "vtkCellPicker.h"
#include "vtkBalloonWidget.h"
#include "vtkBalloonRepresentation.h"
#include "vtkScalarBarActor.h"
#include "vtkLookupTable.h"

#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkDoubleArray.h"
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"
#include "vtkTextProperty.h"
#include "vtkPen.h"

#include "CaseSpaceDialog.h"
#include "CompareDialog.h"
#include "MainWindow.h"
#include "Patient.h"
#include "Projector.h"
#include "Axes.h"

#include <stdint.h>

using namespace std;

static const char *MIAvgFileName = "/MI/mi_avg1-7.txt";


class RendererCallback : public vtkCommand
{
public:
  static RendererCallback *New() { return new RendererCallback; }

  ///Execute////////////////////////////////////////////////////////////////////
  // 
  // Gets called whenever the user interactively manipulates the camera.  
  //
  //////////////////////////////////////////////////////////////////////////////
  virtual void Execute(vtkObject *caller, unsigned long, void *)
  {
    vtkRenderer *r = vtkRenderer::SafeDownCast(caller);
    //CaseSpaceDialog::ReportCameraPosition(r);
  }
};


// PickCallback: intended to be called for pick events.
//
class PickCallback : public vtkCommand
{
public:
	static PickCallback *New() { return new PickCallback; }

	void SetCaseSpaceDialog(CaseSpaceDialog *dlg) { csDlg = dlg; };

	///Execute////////////////////////////////////////////////////////////////////
	//
	// Gets called for pick events.
	//
	//////////////////////////////////////////////////////////////////////////////
	virtual void Execute(vtkObject *caller, unsigned long, void *)
	{
		vtkCellPicker *picker = vtkCellPicker::SafeDownCast(caller);

		if (picker)
		{
			csDlg->setIsNewMatchCaseSelectedHere(true);
			csDlg->pickPatient();
			//picker->RemoveAllLocators();
			//picker->FastDelete();
			//picker = NULL;
		}
	}

private:
	CaseSpaceDialog *csDlg;
};

PickCallback *pickCallback = NULL;
vtkCellPicker *picker = NULL;


///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CaseSpaceDialog::CaseSpaceDialog(MainWindow *mw)
	:	mainWindow(mw),
		dukeOverlapDataPath(mainWindow->getDukeOverlapDataPath()),
		caseSpaceView(NULL),
		dX(NULL),
		dY(NULL),
		hpX(NULL),
		hpY(NULL),
		dukeTable(NULL),
		poconoTable(NULL),
		highPointTable(NULL),
		selectedMatchCaseTable(NULL),
		selectedPoint(NULL),
		minPTVSize(INT32_MAX),
		minRectumSize(INT32_MAX),
		minBladderSize(INT32_MAX),
		minPTVPlusRectum(INT32_MAX),
		minPTVPlusBladder(INT32_MAX),
		minPTVPlusRectumPlusBladder(INT32_MAX),
		maxPTVSize(-1),
		maxRectumSize(-1),
		maxBladderSize(-1),
		maxPTVPlusRectum(-1),
		maxPTVPlusBladder(-1),
		maxPTVPlusRectumPlusBladder(-1),
		dukePatientList(NULL),
		queryCase(NULL),
		matchCase(NULL),
		lastMatchCase(NULL),
		compareDialog(NULL),
		MIMax(-FLT_MAX),
		MIMin(FLT_MAX),
		MIRange(0.0),
		queryCaseIndex(-1),		// Impossible dummy value -> uninitialized,
		caseSpaceRenWin(NULL),
		balloonWidget(NULL),
		balloonRep(NULL),
		zMult(10000.0),
		axes(NULL),
		stdCamDist(33700.0),	// Empricially determined
		stdVertShift(2500.0),	// Add to move camera up, oblique & MI views
		parallelScale(5300.0),
		thresholdPlaneThickness(15.0),
		queryPointActor(NULL),
		lastMatchCaseIndex(-1),	// Negative until initialized
		currMatchCaseIndex(-1),	// Negative until initialized
		MILegend(NULL),
		MILookupTable(NULL),
		newMatchCaseSelectedHere(false)
{
	matchGlow[0] = NULL;		// We just check the 0th element

	this->setupUi(this);
	this->readMIData();
	this->setupCaseSpaceRenWin();
	this->prepareMIDisplay();
	this->addQueryCase();
	this->addMIThresholdPlane();
	// this->averageMIData(); Only needed to run it once
	this->createActions();

	testPushButton->hide();
	compareCasesPushButton->setEnabled(false);

	ren->GetActiveCamera()->ParallelProjectionOn(); 
	setObliqueView(true);
}

///dtor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CaseSpaceDialog::~CaseSpaceDialog()
{
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setQueryCaseText(QString &text)
{
	queryCaseNameLabel->setText(text);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setSelectedMatchPlotPos(vtkVector2f *pos)
{
	selectedMatchPlotPosition->SetX(pos->GetX());
	selectedMatchPlotPosition->SetY(pos->GetY());
}

////////////////////////////////////////////////////////////////////////////////
//
// Find which institution the selected case is from, then linearly search the
// unsorted (small array of) x values until a match is found.  Then verify
// that the corresponding y value also matches that of the selected case.
// If the y value doesn't match then we have multiple data pairs that just
// happen to share the same x value (not likely but possible).  Continue and
// repeat until match is found (and it "must" be found because what we're
// looking for has been selected by the user from existing points).  Set the
// match text accordingly.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::identifyMatchCase()
{
	//QString matchInstitution = selectedMatchPlot->GetLabel();
	/*QString */matchInstitution = "Duke"; // TEMP SAC 2011/07/15

	double *x; // Pointers to arrays of values of which the selected point...
	double *y; // ...is a member.

	if (matchInstitution == "Duke")
	{
		x = dX;
		y = dY;
	}
	else if (matchInstitution == "High Point")
	{
		x = hpX;
		y = hpY;
	}
	else if (matchInstitution == "Pocono")
	{
		x = pX;
		y = pY;
	}
	else if (matchInstitution == "Query case")
	{
		QMessageBox::warning(this, "Query case",
			"You've selected the query case");
		return;
	}
	else if (matchInstitution == "Selected case")
	{
		// QMessageBox::warning(this, "Selected case", "Already selected");
		return;
	}
	else // Can't get here :-)
	{
		QMessageBox::warning(this, "Not found", "Match case not found");
		return;
	}

	if (newMatchCaseSelectedHere)
	{
		lastMatchCase = matchCase;
	}

	if (lastMatchCase)
	{
		lastMatchCaseIndex = getIndexFrom(lastMatchCase);
	}

	matchCase =
		getPatientFromCoodinates(
		x, y, selectedMatchPlotPosition->X(), selectedMatchPlotPosition->Y());

	if (matchCase)
	{
		matchCaseNameLabel->setText(matchInstitution + " patient #"
			+ QString::number(matchCase->getNumber()));

		displayMatchCaseData();

		if (lastMatchCaseIndex != currMatchCaseIndex)
		{
			lastMatchCaseIndex = currMatchCaseIndex;
			newMatchCaseSelectedHere = true;

			if (!matchGlow[0])
			{
				initializeMatchGlow();
			}

			double *center = new double[3];
			center = dukePointActor[currMatchCaseIndex]->GetCenter();

			setMatchGlowLocation(center[0], center[1], center[2]);
		}

		if (!compareDialog)
		{
			compareCasesPushButton->setEnabled(true);
		}
		else
		{	
			if (compareDialog->dataExistsFor(matchCase))
			{
				compareDialog->setMatch(matchCase);
			}
			else
			{
				QString warn = "Can't access data for " + matchInstitution
					+ " patient #" + QString::number(matchCase->getNumber());
				QMessageBox::warning(this, tr("Missing data"), warn);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// Hardwired to Duke.  2do:  Generalize to all institutions.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setMatchCaseFromCompareDialog(int patientNumber)
{
	lastMatchCase = matchCase;

	matchCase = getDukePatientFrom(patientNumber);

	if (matchCase)
	{
		matchCaseNameLabel->setText(matchInstitution + " patient #"
			+ QString::number(matchCase->getNumber()));

		displayMatchCaseData();

		lastMatchCaseIndex = currMatchCaseIndex;
		currMatchCaseIndex = getIndexFrom(matchCase);

		if (!matchGlow[0])
		{
			initializeMatchGlow();
		}

		double *center = new double[3];
		center = dukePointActor[currMatchCaseIndex]->GetCenter();

		setMatchGlowLocation(center[0], center[1], center[2]);

		if (picker) 
		{
			//picker->DeletePickList(picker->GetActor());
			//picker->PickFromListOff();
/*
			vtkCoordinate* c = vtkCoordinate::New();
			c->SetCoordinateSystemToWorld();
			c->SetValue(center);
			int *i = c->GetComputedDisplayValue(ren);

			picker->Pick(i[0], i[1], 0.0, ren);
			caseSpaceRenWin->Render();
			//picker->FastDelete();
			//picker->Delete();
			//picker = vtkCellPicker::New();
			//renderWindowInteractor->SetPicker(picker);
			//picker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
*/
		}


		vtkActor *pickedActor = picker->GetActor();

		picker->InitializePickList();

		if (pickedActor)
		{
			pickedActor->GetProperty()->SetEdgeColor(0, 0, 1);
			int cellID = picker->GetCellId();
			int x = cellID;
			//pickedActor->GetProperty()->EdgeVisibilityOff();
		}

		caseSpaceRenWin->Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// Hardwired to Duke.  2do:  Generalize to all institutions.
//
////////////////////////////////////////////////////////////////////////////////
int CaseSpaceDialog::getIndexFrom(Patient *p)
{
	for (int i = 0; i < numDukePatients; i++)
	{
		if (p == &(dukePatientList[i])) return i;
	}

	return -1;
}


////////////////////////////////////////////////////////////////////////////////
//
// 2do:  Generalize to all institutions.
//
////////////////////////////////////////////////////////////////////////////////
Patient *CaseSpaceDialog::getDukePatientFromCoodinates(double posX, double posY)
{
	return this->getPatientFromCoodinates(dX, dY, posX, posY);
};

////////////////////////////////////////////////////////////////////////////////
//
// returns Duke Patient with number == patientNumber.  If not found, returns 
// NULL.
//
// 2do:  Generalize to all institutions.
//
////////////////////////////////////////////////////////////////////////////////
Patient *CaseSpaceDialog::getDukePatientFrom(int patientNumber)
{
	int ix = -1;
	bool found = false;

	while ((!found) && (++ix < numDukePatients))
	{
		found = (dukePatientList[ix].getNumber() == patientNumber);
	}

	return (found) ? &dukePatientList[ix] : NULL;
}

////////////////////////////////////////////////////////////////////////////////
// 
// Returns a pointer to the Patient at (x, y).  If not found, returns NULL.
//
// 2do: Generalize to all institutions. For now assumes it's a Duke patient.	`
//
////////////////////////////////////////////////////////////////////////////////
Patient *CaseSpaceDialog::getPatientFromCoodinates(double *xArray, double *yArray,
												   double x, double y)
{
	int ix = 0;
	bool found = false;
	Patient *foundPatient = NULL;

	while ((!found) && (ix < numDukePatients)) // Hack SAC
	{
		if (!(found = (xArray[ix] == x)))
		{
			ix++;
		}
		else // x value match; now check for y value
		{
			if (!(found = (yArray[ix] == y)))
			{
				ix++;
			}
		}
	}

	if (found) 
	{
		foundPatient = &(dukePatientList[ix]);
		currMatchCaseIndex = ix;
	}

	return foundPatient;
}

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate code with the GUI controls.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::createActions()
{
	connect(compareCasesPushButton, SIGNAL(clicked()), this, SLOT(compareCases()));
	connect(testPushButton, SIGNAL(clicked()), this, SLOT(testFunction()));
	connect(XYViewRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setXYView(bool)));
	connect(XMIViewRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setXMIView(bool)));
	connect(YMIViewRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setYMIView(bool)));
	connect(obliqueViewRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setObliqueView(bool)));
	connect(MIRangeSlider, SIGNAL(valueChanged(int)), this, 
		SLOT(setThresholdPlaneZVal(int)));
	connect(backgroundBlackRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setBackgroundBlack(bool)));	
	connect(backgroundWhiteRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setBackgroundWhite(bool)));	
	connect(backgroundRampRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setBackgroundRamped(bool)));	
}

////////////////////////////////////////////////////////////////////////////////
//
// If Match Case data exists then instantiate and display the Compare Cases 
// dialog, else display a warning message.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::compareCases()
{
	if (!compareDialog)
	{
		compareDialog = new CompareDialog(this);

		if (compareDialog->dataExistsFor(matchCase))
		{
			QString selectedMatchPlotLabel = "Duke"; // TEMP SAC 2011/07/15 selectedMatchPlot->GetLabel();

			compareDialog->setQuery(queryCase);
			compareDialog->setMatch(matchCase);

			compareCasesPushButton->setEnabled(false);

			compareDialog->show();
		}
		else
		{
			QString warn = "Can't access data for patient #" +
				QString::number(matchCase->getNumber());
			QMessageBox::warning(this, tr("Missing data"), warn);

			// The code depends on compareDialog being either fully 
			// instantiated or NULL:
			CompareDialog *cDlg = compareDialog;
			cDlg->deleteLater();
			compareDialog = NULL;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::testFunction()
{
}

////////////////////////////////////////////////////////////////////////////////
//
// 3-D case space.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setupCaseSpaceRenWin()
{
	caseSpaceRenWin = vtkRenderWindow::New();
	caseSpaceViewWidget->SetRenderWindow(caseSpaceRenWin);
	caseSpaceRenWin->SetSize(920, 569);

	ren = vtkRenderer::New();
	ren->SetBackground(0, 0, 0);
	ren->SetBackground2(0, 0, 0);
	ren->SetGradientBackground(true);
	ren->SetRenderWindow(caseSpaceRenWin);
	caseSpaceRenWin->AddRenderer(ren);

	RendererCallback *callback = RendererCallback::New();
	ren->AddObserver(vtkCommand::StartEvent, callback);
	callback->Delete();

	pickCallback = PickCallback::New();
	pickCallback->SetCaseSpaceDialog(this);

	picker = vtkCellPicker::New();
	picker->SetTolerance(0.0);
	picker->AddObserver(vtkCommand::EndPickEvent, pickCallback);

	renderWindowInteractor = caseSpaceViewWidget->GetInteractor();
	renderWindowInteractor->SetRenderWindow(caseSpaceRenWin);
	renderWindowInteractor->SetPicker(picker);

	initializeBalloonStuff();

	selectedMatchPlotPosition = new vtkVector2f;

	if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kDuke)
	{
		queryInstitution = "Duke";
		addDukeDataToGraph();
	}
	else if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kHighPoint)
	{
		queryInstitution = "High Point";
		addHighPointDataToGraph();
	}
	else if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kPocono)
	{
		queryInstitution = "Pocono";
		addPoconoDataToGraph();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::initializeBalloonStuff()
{
	balloonWidget = vtkBalloonWidget::New();
	balloonRep = vtkBalloonRepresentation::New();
	balloonRep->SetBalloonLayoutToImageRight();
	balloonWidget->SetInteractor(renderWindowInteractor);
	balloonWidget->SetRepresentation(balloonRep);
	balloonWidget->SetTimerDuration(300);
	balloonWidget->EnabledOn();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addBalloon(int i)
{
	QString n;
	n.setNum(dukePatientList[i].getNumber());

	QString xString;
	xString.setNum(dukePatientList[i].getPTVPlusBladder());

	QString yString;
	yString.setNum(dukePatientList[i].getPTVPlusRectum());

	QString zString;
	zString.setNum(MIval[queryCaseIndex][i], 'g', 3);

	QString s = "Duke patient #" + n + ":\n" + xString + ", " + 
				yString + ", " + zString;
	QByteArray a = s.toAscii();

	balloonWidget->AddBalloon(dukePointActor[i], a.data(), NULL);

	ren->AddActor(dukePointActor[i]);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::initializeMatchGlow()
{
	static const double increment = 0.06;

	for (int i = 0; i < numGlowLevels; i++)
	{
		matchGlow[i] = vtkCubeSource::New();
		double length = cubeSize * (1.0 + ((i + 1.0) * increment));
		matchGlow[i]->SetYLength(length);
		matchGlow[i]->SetXLength(length);
		matchGlow[i]->SetZLength(length);

		matchGlowMapper[i] = vtkPolyDataMapper::New();
		matchGlowMapper[i]->SetInputConnection(matchGlow[i]->GetOutputPort());

		matchGlowActor[i] = vtkActor::New();
		matchGlowActor[i]->SetMapper(matchGlowMapper[i]);
		matchGlowActor[i]->PickableOff();
		matchGlowActor[i]->GetProperty()->SetAmbient(1.0);
		matchGlowActor[i]->GetProperty()->SetDiffuse(0.0);
		matchGlowActor[i]->GetProperty()->SetSpecular(0.0);
		matchGlowActor[i]->GetProperty()->SetAmbientColor(1, 0, 0);

		double halfPi = 1.57079632679489661923;

		double mult = (i == 0) ? 1.0 : ((i < 4) ? 0.6 : ((i < 8) ? 0.45 : 0.3));
		double opacity = mult * (1.0 - sin(halfPi * ((i + 1.0) / numGlowLevels))); 
		matchGlowActor[i]->GetProperty()->SetOpacity(opacity);

		ren->AddActor(matchGlowActor[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setMatchGlowLocation(double x, double y, double z)
{
	for (int i = 0; i < numGlowLevels; i++)
	{
		matchGlow[i]->SetCenter(x, y, z);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// Make sure the MI extrema have been generated before calling this method.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::initializeMILegend()
{
	if (!MILegend)
	{
		MILegend = vtkScalarBarActor::New();
		MILookupTable = vtkLookupTable::New();
	}

	MILookupTable->SetScaleToLinear();
	MILookupTable->SetRange(MIMin, MIMax);

	static const int numColors = 100;
	MILookupTable->SetNumberOfTableValues(numColors);
	
	double r, g, b;
	static const double denom = numColors - 1.0;

	for (int i = 0; i < numColors; i++)
	{
		r = (i / denom) * colorMult;

		if (r > 1.0) r = 1.0;
		g = r;
		b = 1.0 - r;
		MILookupTable->SetTableValue(i, r, g, b);
	}

	MILookupTable->Build();

	MILegend->SetLookupTable(MILookupTable);
	MILegend->SetTitle("MI Values");
	MILegend->SetOrientationToHorizontal();
	MILegend->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
	MILegend->GetPositionCoordinate()->SetValue(0.814, 0.94);
	MILegend->SetWidth(0.1515);
	MILegend->SetHeight(0.05);
	MILegend->GetTitleTextProperty()->ItalicOff();
	MILegend->GetLabelTextProperty()->ItalicOff();
	ren->AddActor2D(MILegend);
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addDukeDataToGraph()
{
	vtkDoubleArray *dukeXCoords = vtkDoubleArray::New();
	vtkDoubleArray *dukeYCoords = vtkDoubleArray::New();
	dukeTable = vtkTable::New();

	QFile file(dukeOverlapDataPath);

	if (!file.open(QIODevice::ReadOnly))
	{
		QString warn = "Failed to open \"" + dukeOverlapDataPath + "\"";
		QMessageBox::warning(this, tr("File Open Failed"), warn);
		return;
	}

	QTextStream in(&file);
	
	in.readLine();		// Skip over the first line (column headers):
	numDukePatients = 0;

	while (!in.atEnd())
	{
		in.readLine();
		numDukePatients++;
	}

	in.flush();

	if (!in.seek(0))
	{
		QString warn = "Failed to seek(0) \"" + dukeOverlapDataPath + "\"";
		QMessageBox::warning(this, 
			tr("failure attempting to return to file beginning"), warn);
		return; 
	}

	dX = new double[numDukePatients];
	dY = new double[numDukePatients];

	int n, PTVSize, rectumSize, bladderSize, PTVPlusRectum, PTVPlusBladder,
		PTVPlusRectumPlusBladder;
	dukePatientList = new Patient[numDukePatients];

	for (int i = 0; i < numDukePatients; i++)
	{
		(dukePatientList[i]).setDataDir(mainWindow->getDataDir());
	}

	in.readLine();		// Skip over the first line (column headers):

	MainWindow::SourceInstitution sourceInstitution =
		mainWindow->getQueryCaseSourceInstitution();
	int queryCasePatientNumber = 
		mainWindow->getQueryCasePatientNumber();
	int i = 0;

	// Read overlap data for all Duke cases, getting extrema while we're at it.
	// Also note which Patient object in the array is the Query Case:
	//
	while ((!in.atEnd()) && (i < numDukePatients)) // else 1 too far: final newline?
	{
		in >> n >> PTVSize >> rectumSize >> bladderSize >> PTVPlusRectum
		   >> PTVPlusBladder >> PTVPlusRectumPlusBladder;

		if ((n == queryCasePatientNumber) && (sourceInstitution == MainWindow::kDuke))
		{
			queryCase = &(dukePatientList[i]);
			queryCaseIndex = i;
		}

		dukePatientList[i].setNumber(n);
		dukePatientList[i].setIndex(i);
		dukePatientList[i].setPTVSize(PTVSize);
		dukePatientList[i].setRectumSize(rectumSize);
		dukePatientList[i].setBladderSize(bladderSize);
		dukePatientList[i].setPTVPlusRectum(PTVPlusRectum);
		dukePatientList[i].setPTVPlusBladder(PTVPlusBladder);
		dukePatientList[i].setPTVPlusRectumPlusBladder(PTVPlusRectumPlusBladder);

		if (PTVSize < minPTVSize)
			minPTVSize = PTVSize;
		if (rectumSize < minRectumSize) minRectumSize = rectumSize;
		if (bladderSize < minBladderSize) minBladderSize = bladderSize;
		if (PTVPlusRectum < minPTVPlusRectum) minPTVPlusRectum = PTVPlusRectum;
		if (PTVPlusBladder < minPTVPlusBladder) minPTVPlusBladder = PTVPlusBladder;
		if (PTVPlusRectumPlusBladder < minPTVPlusRectumPlusBladder)
			minPTVPlusRectumPlusBladder = PTVPlusRectumPlusBladder;

		if (PTVSize > maxPTVSize) maxPTVSize = PTVSize;
		if (rectumSize > maxRectumSize) maxRectumSize = rectumSize;
		if (bladderSize > maxBladderSize) maxBladderSize = bladderSize;
		if (PTVPlusRectum > maxPTVPlusRectum) maxPTVPlusRectum = PTVPlusRectum;
		if (PTVPlusBladder > maxPTVPlusBladder) maxPTVPlusBladder = PTVPlusBladder;
		if (PTVPlusRectumPlusBladder > maxPTVPlusRectumPlusBladder)
			maxPTVPlusRectumPlusBladder = PTVPlusRectumPlusBladder;
		i++;
	}

	file.close();

	for (i = 0; i < numDukePatients; i++)
	{
		dX[i] = dukePatientList[i].getPTVPlusBladder();
		dY[i] = dukePatientList[i].getPTVPlusRectum();

		if (i == queryCaseIndex) continue;

		dukePoint[i] = vtkCubeSource::New();
		dukePoint[i]->SetCenter(dX[i], dY[i], MIval[queryCaseIndex][i] * zMult);
		dukePoint[i]->SetXLength(cubeSize);
		dukePoint[i]->SetYLength(cubeSize);
		dukePoint[i]->SetZLength(cubeSize);

		dukePointMapper[i] = vtkPolyDataMapper::New();
		dukePointMapper[i]->SetInputConnection(dukePoint[i]->GetOutputPort());

		dukePointActor[i] = vtkActor::New();
		dukePointActor[i]->SetMapper(dukePointMapper[i]);
		dukePointActor[i]->GetProperty()->SetColor(100 / 255.0 , 120 / 255.0 , 165 / 255.0);

		addBalloon(i);
	}

	ren->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addQueryCase()
{
	vtkCubeSource *queryPoint = vtkCubeSource::New();
	xCenterQueryPt = queryCase->getPTVPlusBladder();
	yCenterQueryPt = queryCase->getPTVPlusRectum();
	zCenterQueryPt = MIMin * zMult;
	queryPoint->SetCenter(xCenterQueryPt, yCenterQueryPt, zCenterQueryPt);
	queryPoint->SetXLength(200.0);
	queryPoint->SetYLength(200.0);
	queryPoint->SetZLength(200.0);

	vtkPolyDataMapper *queryPointMapper = vtkPolyDataMapper::New();
	queryPointMapper->SetInputConnection(queryPoint->GetOutputPort());

	queryPointActor = vtkActor::New();
	queryPointActor->SetMapper(queryPointMapper);
	queryPointActor->GetProperty()->SetColor(1.0, 0.5, 0.0);
	queryPointActor->PickableOff();

	QString xString;
	xString.setNum(queryCase->getPTVPlusBladder());

	QString yString;
	yString.setNum(queryCase->getPTVPlusRectum());

	QString zString;
	zString.setNum(MIval[queryCaseIndex][queryCaseIndex], 'g', 3);

	QString s = "Query case:\n" + xString + ", " + 
			yString + ", " + zString;
	QByteArray a = s.toAscii();

	balloonWidget->AddBalloon(queryPointActor, a.data(), NULL);

	displayQueryCaseData();

	ren->AddActor(queryPointActor);
}

////////////////////////////////////////////////////////////////////////////////
//
// stub.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addPoconoDataToGraph()
{
}

////////////////////////////////////////////////////////////////////////////////
//
// stub.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addHighPointDataToGraph()
{
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::displayQueryCaseData()
{
	queryCaseNameLabel->setText(queryInstitution + " patient #"
		+ QString::number(queryCase->getNumber())
		+ "; PTV plus bladder: "
		+ QString::number(queryCase->getPTVPlusBladder())
		+ "; PTV plus rectum: "
		+ QString::number(queryCase->getPTVPlusRectum())
		+ "; MI: "
		+ QString::number(MIval[queryCaseIndex][queryCaseIndex])
		);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::displayMatchCaseData()
{
	matchCaseNameLabel->setText(matchInstitution + " patient #"
		+ QString::number(matchCase->getNumber())
		+ "; PTV plus bladder: "
		+ QString::number(matchCase->getPTVPlusBladder())
		+ "; PTV plus rectum: "
		+ QString::number(matchCase->getPTVPlusRectum())
		+ "; MI: "
		+ QString::number(MIval[queryCaseIndex][currMatchCaseIndex])
	);
}

////////////////////////////////////////////////////////////////////////////////
//
// TEMP hardwired for 100 Duke cases with MI values averaged over the seven
// angles 25, 75, 130, 180, 230, 280, and 335 degrees.
//
////////////////////////////////////////////////////////////////////////////////
bool CaseSpaceDialog::readMIData()
{
	QString MIPath = mainWindow->getDataDir() + MIAvgFileName;
	QByteArray pathArray = MIPath.toAscii();
	char *pathString = pathArray.data();

	ofstream MIofs;

	ifstream MIfs;
	MIfs.open(pathString, ifstream::in);

	if (!MIfs.is_open())
	{
	cout << "Failed to open MI file " << pathString << endl;
	return false;
	}

	for (int row = 0; row < numMICases; row++)
	{
		MIval[row] = new double[numMICases];

		for (int col = 0; col < numMICases; col++)
		{
			if (MIfs.eof())
			{
				cout << "Unexpected end of file reading " << pathString << endl;
				MIfs.close();
				return false;
			}
			
			MIfs >> MIval[row][col];
		}
	}

	MIfs.close();
	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// Note the difference between queryCaseIndex which is the index into arrays
// of data (sequential: 0, 1, 2...) vs. the query case Patient number, which 
// will probably *not* be sequential.  Right now (2011/07/10), for example, the
// Duke patient numbers start 3, 4, 6, 7, 9...  They don't start at 0 (or at 1)
// and they aren't sequential.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::prepareMIDisplay()
{
	// Get extrema for all cases wrt query case:
	MIMax = -FLT_MAX;
	MIMin = FLT_MAX;

	for (int i = 0; i < numMICases; i++)
	{
		// Query case always has the max MI.  Don't include it here:
		if ((queryCaseIndex != i) && (MIval[queryCaseIndex][i] > MIMax))
		{
			MIMax = MIval[queryCaseIndex][i];
		}

		if (MIval[queryCaseIndex][i] < MIMin) MIMin = MIval[queryCaseIndex][i];
	}

	// Then get the fraction from min to max for each case:
	MIRange = MIMax - MIMin;

	//this->ReportCameraPosition(ren);
	axes = new Axes();
	double shaftLength = 4000.0;
	double textOffset = 1.05 * shaftLength;
	axesAssembly = axes->InsertThis(ren, shaftLength);

	axesAssembly->SetPosition(queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(), MIMin * zMult + thresholdPlaneThickness * 5);

	vtkFollower *xf = axes->AddFollowingText("PTV + bladder overlap", 
		queryCase->getPTVPlusBladder() + shaftLength * 1.1, queryCase->getPTVPlusRectum(), MIMin * zMult + thresholdPlaneThickness * 5, 
		1, 0, 0, ren);
	vtkFollower *yf = axes->AddFollowingText("PTV + rectum overlap",
		queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum() + shaftLength * 1.1, MIMin * zMult + thresholdPlaneThickness * 5,
		0, 1, 0, ren);
	vtkFollower *zf = axes->AddFollowingText("MI", 
		queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(), MIMin * zMult + shaftLength * 1.1,
		 0, 0, 1, ren);

	const double scale = shaftLength / 35.0; // Eyeballed 
	xf->SetScale(scale);
	yf->SetScale(scale);
	zf->SetScale(scale);

	ren->AddActor(xf);
	ren->AddActor(yf);
	ren->AddActor(zf);

	for (int i = 0; i < numMICases; i++)
	{
		if (i == queryCaseIndex) continue;

		MIFraction[i] = (MIval[queryCaseIndex][i] - MIMin) / MIRange;

		double r, g, b;

		r = MIFraction[i] * colorMult;
		if (r > 1.0) r = 1.0;
		g = r;
		b = 1.0 - r;
		dukePointActor[i]->GetProperty()->SetColor(r, g, b);
		//dukePointActor[i]->GetProperty()->SetColor(MIFraction[i] * 1.2, MIFraction[i] * 1.2, 1.0 - (MIFraction[i] * 1.2));
	}

	// Set up the MI Range Slider with current values:
	QString maxAsText;
	maxAsText.setNum(MIMax, 'g', 3);
	MIMaxLabel->setText(maxAsText);
	QString minAsText;
	minAsText.setNum(MIMin, 'g', 3);
	MIMinLabel->setText(minAsText);

	initializeMILegend();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addMIThresholdPlane()
{
	MIThresholdPlane = vtkCubeSource::New();
	MIThresholdPlane->SetXLength(maxPTVPlusBladder - minPTVPlusBladder);
	MIThresholdPlane->SetYLength(maxPTVPlusRectum - minPTVPlusRectum);
	MIThresholdPlane->SetZLength(thresholdPlaneThickness);

	//MIThresholdPlane->SetCenter(queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(), MIMin * zMult);
	MIThresholdPlane->SetCenter((maxPTVPlusBladder - minPTVPlusBladder) / 2.0,
								(maxPTVPlusRectum - minPTVPlusRectum) / 2.0,
								MIMin * zMult);

	vtkPolyDataMapper *mitpMapper = vtkPolyDataMapper::New();
	mitpMapper->SetInputConnection(MIThresholdPlane->GetOutputPort());

	vtkActor *mitpActor = vtkActor::New();
	mitpActor->SetMapper(mitpMapper);
	mitpActor->GetProperty()->SetOpacity(0.33);
	mitpActor->GetProperty()->SetSpecular(0);
	mitpActor->GetProperty()->SetDiffuse(0);
	mitpActor->GetProperty()->SetAmbient(1);
	mitpActor->GetProperty()->SetAmbientColor(0.2, 0.2, 0.2);
	mitpActor->PickableOff();

	ren->AddActor(mitpActor);

	setThresholdPlaneZVal(MIRangeSlider->value());
}

////////////////////////////////////////////////////////////////////////////////
//
// One-off for 100 Duke cases with the seven angles 25, 75, 130, 180,
// 230, 280, and 335 degrees.
//
////////////////////////////////////////////////////////////////////////////////
bool CaseSpaceDialog::averageMIData()
{
	// Instantiate and initialize array:
	for (int i = 0; i < numMICases; i++)
	{
		MIval[i] = new double[numMICases];

		for (int j = 0; j < numMICases; j++)
		{
			MIval[i][j] = 0.0;
		}
	}

	int numAngles = 7;	// TEMP: hardwired for Duke cases
	const int kMaxChars = 255;

	// Add up the values for each element (filenames are 1-based):
	for (int currAngleNum = 1; currAngleNum <= numAngles; currAngleNum++)
	{
		QString MIInPathFormat = mainWindow->getDataDir() + "/MI/mi_%01d.txt";
		QByteArray formatArray = MIInPathFormat.toAscii();
		char *formatString = formatArray.data();
		char MIPath[kMaxChars];

		sprintf_s(MIPath, formatString, currAngleNum);

		ifstream MIfs;
		MIfs.open(MIPath, ifstream::in);

		if (!MIfs.is_open())
		{
			cout << "Failed to open MI file " << MIPath << endl;
			return false;
		}

		for (int row = 0; row < numMICases; row++)
		{

			for (int col = 0; col < numMICases; col++)
			{
				if (MIfs.eof())
				{
					cout << "Unexpected end of file reading " << MIPath << endl;
					MIfs.close();
					return false;
				}
				
				double val;
				MIfs >> val;
				MIval[row][col] += val;
			}
		}

		MIfs.close();
	}

	// Compute the average for each element, and track extrema:
	for (int i = 0; i < numMICases; i++)
	{
		for (int j = 0; j < numMICases; j++)
		{
			MIval[i][j] /= numAngles;

			if (MIval[i][j] > MIMax) MIMax = MIval[i][j];
			if (MIval[i][j] < MIMin) MIMin = MIval[i][j];
		}
	}

	// Write the averages to file:
	QString MIOutPath = mainWindow->getDataDir() + MIAvgFileName;
	QByteArray outPathArray = MIOutPath.toAscii();
	char *outPathString = outPathArray.data();

	ofstream MIofs;
	MIofs.open(outPathString, ifstream::out);

	if (!MIofs.is_open())
	{
		cout << "Failed to open MI file " << outPathString << endl;
		return false;
	}

	for (int row = 0; row < numMICases; row++)
	{
		for (int col = 0; col < numMICases; col++)
		{
			MIofs << MIval[row][col] << ' ';
		}

		MIofs << "\n";
	}

	MIofs.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// When the match case is changed in caseSpaceDialog, the compareDialog's
// matchSelectSpinBox is updated, which calls the associated slot, that is, 
// selectMatch(int patientNumber), which updates the caseSpaceDialog's 
// lastMatchCase variable to the current matchCase.  This is necessary when 
// selectMatch(int patientNumber) is invoked by direct user manipulation of the
// matchSelectSpinBox, but ends up with an incorrect patient (the newly selected)
// being placed in the matchHistory if the selection was made from Case Space.
// Hence this variable used to skip the incorrect lastMatchCase update for the 
// latter case.
//
////////////////////////////////////////////////////////////////////////////////
bool CaseSpaceDialog::isNewMatchCaseSelectedHere()
{
	bool returnVal = newMatchCaseSelectedHere;
	newMatchCaseSelectedHere = false;

	return returnVal;
}

///ReportCameraPosition/////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
const void CaseSpaceDialog::ReportCameraPosition(vtkRenderer *r)
{
	double pos[3], fp[3], vUp[3], clipRange[2];
	r->GetActiveCamera()->GetPosition(pos);
	r->GetActiveCamera()->GetFocalPoint(fp);  
	r->GetActiveCamera()->GetViewUp(vUp);
	r->GetActiveCamera()->GetClippingRange(clipRange);
	double parallelScale = r->GetActiveCamera()->GetParallelScale();
	double viewAngle = r->GetActiveCamera()->GetViewAngle();


	cout << "\n///Active Camera////////////////////////////////////" << endl;
	cout << "// position: (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")"
		<< endl;
	cout << "// focal point: (" << fp[0] << ", " << fp[1] << ", " << fp[2] << ")"
		<< endl;
	cout << "// view up: (" << vUp[0] << ", " << vUp[1] << ", " << vUp[2] << ")"
		<< endl;
	cout << "// clipping range: (" << clipRange[0] << ", " << clipRange[1] << ")"
		<< endl; 
	cout << "// parallel scale: " << parallelScale << endl;
	cout << "////////////////////////////////////////////////////" << endl;
}

///SetCameraPosition////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::SetCameraPosition(double pos[3], double fp[3],
	double vUp[3], double clip[2], double zoom)
{
	vtkCamera *cam = ren->GetActiveCamera();
	cam->SetPosition(pos);
	cam->SetFocalPoint(fp);  
	cam->SetViewUp(vUp); 
	cam->SetClippingRange(clip);
	cam->SetParallelScale(1.0 / zoom);

	caseSpaceRenWin->Render();
}

///SetCameraPosition////////////////////////////////////////////////////////////
//
// Setting camera position and orientation with azimuth defined by argument az.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::SetCameraPosition(double az)
{
  ren->GetActiveCamera()->Azimuth(-az);
}

////////////////////////////////////////////////////////////////////////////////
//
// If invalid object was picked (notably the query case point) return false,
// else return true.
//
////////////////////////////////////////////////////////////////////////////////
bool CaseSpaceDialog::pickPatient()
{
	double pickPos[3];
	picker->GetPickPosition(pickPos);

	//cout << "Patient at (" << pickPos[0] << ", " << pickPos[1] << ", "
	//<< pickPos[2] << ")" << endl;

	vtkActor *pickedActor = picker->GetActor();

	if (pickedActor)
	{
		if (pickedActor == queryPointActor)
		{
			QMessageBox::warning(this, "Query case",
			"You've selected the query case");
			return false;
		}

		double *center = new double[3];
		center = pickedActor->GetCenter();

		//cout << "Picked actor center at (" << center[0] << ", " << center[1] << ", "
		//<< center[2] << ")" << endl;

		selectedMatchPlotPosition->SetX(center[0]);
		selectedMatchPlotPosition->SetY(center[1]);
		identifyMatchCase();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setXYView(bool /* checked = true */ )
{
	double pos[] = { xCenterQueryPt,
					 yCenterQueryPt /*+ stdVertShift */,
					 zCenterQueryPt + stdCamDist };
	double fp[] = { xCenterQueryPt,
					yCenterQueryPt /*+ stdVertShift */,
					zCenterQueryPt };
	double vUp[] = {0, 1, 0 };
	double clip[] = { 1, 75760 };
	double zoom = 1.0 / parallelScale;

	SetCameraPosition(pos, fp, vUp, clip, zoom);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setXMIView(bool /* checked = true */ )
{
	double pos[] = { xCenterQueryPt,
					 yCenterQueryPt - stdCamDist,
					 zCenterQueryPt + stdVertShift };
	double fp[] = { xCenterQueryPt,
					yCenterQueryPt,
					zCenterQueryPt + stdVertShift };
	double vUp[] = {0, 0, 1 };
	double clip[] = { 1, 75760 };
	double zoom = 1.0 / parallelScale;

	SetCameraPosition(pos, fp, vUp, clip, zoom);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setYMIView(bool /* checked = true */ )
{
	double pos[] = { stdCamDist + xCenterQueryPt,
					 yCenterQueryPt,
					 zCenterQueryPt + stdVertShift };
	double fp[] = { xCenterQueryPt,
					yCenterQueryPt,
					zCenterQueryPt + stdVertShift };
	double vUp[] = { 0, 0, 1 };
	double clip[] = { 1, 75760 };
	double zoom = 1.0 / parallelScale;

	SetCameraPosition(pos, fp, vUp, clip, zoom);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setObliqueView(bool /* checked = true */ )
{
	// Normalized view vector, empricially determined:
	double nVV[3] = { 0.759418157,	-0.628362612,	0.168655249 }; 

	double pos[] = { nVV[0] * stdCamDist + xCenterQueryPt,
					 nVV[1] * stdCamDist + yCenterQueryPt,
					 (nVV[2] * stdCamDist + zCenterQueryPt) + stdVertShift };
	double fp[] = { xCenterQueryPt, yCenterQueryPt,
					zCenterQueryPt + stdVertShift };
	double vUp[] = { 0, 0, 1 };
	double clip[] = { 1, 75760 };
	double zoom = 1.0 / parallelScale;

	SetCameraPosition(pos, fp, vUp, clip, zoom);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setThresholdPlaneZVal(int val)
{
	double sliderRange = MIRangeSlider->maxValue() - MIRangeSlider->minValue();
	MIThresholdVal = (MIMin + (MIRange * val / sliderRange));
	thresholdPlaneZVal = MIThresholdVal * zMult;
	//cout << "MI threshold = " << MIThresholdVal
	//	 << "; threshold plane z val = " << thresholdPlaneZVal << endl;
	MIThresholdPlane->SetCenter(queryCase->getPTVPlusBladder(),
								queryCase->getPTVPlusRectum(),
								thresholdPlaneZVal);

	for (int i = 0; i < numMICases; i++)
	{
		if (i == queryCaseIndex) continue;

		double center[3];
		dukePoint[i]->GetCenter(center);

		bool belowThreshold = (center[2] < thresholdPlaneZVal);
		dukePointActor[i]->GetProperty()->SetOpacity(belowThreshold ? 0.0 : 1.0);

		if (belowThreshold) dukePointActor[i]->PickableOff();
		else dukePointActor[i]->PickableOn();

		QString thresholdAsText;
		thresholdAsText.setNum(MIThresholdVal, 'g', 3);

		MIRangeGroupBox->setTitle("MI threshold: " + thresholdAsText);
	}

	caseSpaceRenWin->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setBackgroundBlack(bool checked /* = true */ )
{
	if (checked)
	{
		ren->SetBackground(0, 0, 0);
		ren->SetBackground2(0, 0, 0);
	}

	caseSpaceRenWin->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setBackgroundWhite(bool checked /* = true */ )
{
	if (checked)
	{
		ren->SetBackground(1, 1, 1);
		ren->SetBackground2(1, 1, 1);
	}

	caseSpaceRenWin->Render();
}
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setBackgroundRamped(bool checked /* = true */ )
{
	if (checked)
	{
		ren->SetBackground(0, 0, 0);
		ren->SetBackground2(1, 1, 1);
	}

	caseSpaceRenWin->Render();
}
