////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.cpp: Qt-based GUI for prostate cancer radiation therapy 
// planning tool. This window displays a cartesian graph showing the relative
// distances of the selected sets of cases from the query case with respect to
// several criteria, e.g., PTV + bladder overlap for the x axis and PTV + 
// rectum overlap for the y axis.
//
// author: Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyavanich, Duke Medical Center
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

#include "ccChartXY.h"
#include "CaseSpaceDialog.h"
#include "CompareDialog.h"
#include "MainWindow.h"
#include "Patient.h"
#include "Projector.h"
#include "Axes.h"

#include <vector>
#include <stack>
#include <hash_map>
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
    CaseSpaceDialog::ReportCameraPosition(r);
	//cout << "Hi from callback." << endl;
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
      cout << "Picked a cell" << endl;
	  csDlg->pickPatient();
    }
  }

private:
	CaseSpaceDialog *csDlg;
};

PickCallback *pickCallback = NULL;
vtkCellPicker *picker = NULL;


// HoverCallback: intended to be called for pick events.
//
class HoverCallback : public vtkCommand
{
public:
  static HoverCallback *New() { return new HoverCallback; }

  void SetCaseSpaceDialog(CaseSpaceDialog *dlg) { csDlg = dlg; };

  ///Execute////////////////////////////////////////////////////////////////////
  //
  // Gets called for hover events.
  //
  //////////////////////////////////////////////////////////////////////////////
  virtual void Execute(vtkObject *caller, unsigned long, void *)
  {
    vtkCellPicker *picker = vtkCellPicker::SafeDownCast(caller);

    if (picker)
    {
      cout << "Hovering" << endl;
    }
  }

private:
	CaseSpaceDialog *csDlg;
};

HoverCallback *hoverCallback = NULL;


///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CaseSpaceDialog::CaseSpaceDialog(MainWindow *mw)
	:	mainWindow(mw),
		dukeOverlapDataPath(mainWindow->getDukeOverlapDataPath()),
		caseSpaceChart(NULL),
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
		compareDialog(NULL),
		MIMax(-FLT_MAX),
		MIMin(FLT_MAX),
		MIRange(0.0),
		queryCaseIndex(-1),		// Impossible dummy value -> uninitialized,
		caseSpaceRenWin(NULL),
		zMult(10000.0),
		axes(NULL),
		stdCamDist(33700.0),	// Empricially determined
		stdVertShift(2500.0),	// Add to move camera up, oblique & MI views
		parallelScale(5300.0),
		thresholdPlaneThickness(15.0)
{
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
	QString matchInstitution = "Duke"; // TEMP SAC 2011/07/15

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

	matchCase =
		getPatientFromCoodinates(
		x, y, selectedMatchPlotPosition->X(), selectedMatchPlotPosition->Y());

	if (matchCase)
	{
		matchCaseNameLabel->setText(matchInstitution + " patient #"
			+ QString::number(matchCase->getNumber()));
		//drawSelectedCase();

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
Patient *CaseSpaceDialog::getDukePatientFromCoodinates(double posX, double posY)
{
	return this->getPatientFromCoodinates(dX, dY, posX, posY);
};

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
	}

	return foundPatient;
}

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate code to the GUI controls.
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
	stack<int> intStack;

	for (int i = 0; i < 123; i++)
	{
		intStack.push(i);
	}

	for (int i = 0; i < 125; i++)
	{
		if (!intStack.empty())
		{
			int j = intStack.top();
			intStack.pop();
		}
	}

	hash_map<int, QString> stringHashMap;
	QString s = "QWERTY12345";

	hash_map<int, int> intHashMap;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setupCaseSpaceChart()
{
	if (caseSpaceChart) caseSpaceChart->Delete();
	caseSpaceChart = ccChartXY::New();
	caseSpaceChart->SetCaseSpaceDialog(this);

	if (caseSpaceView) caseSpaceView->Delete();
	caseSpaceView = vtkContextView::New();

	caseSpaceView->GetScene()->AddItem(caseSpaceChart);
	caseSpaceView->SetInteractor(caseSpaceViewWidget->GetInteractor());
	caseSpaceViewWidget->SetRenderWindow(caseSpaceView->GetRenderWindow());
	//caseSpaceView->GetRenderWindow()->SetSize(920, 569);
	caseSpaceView->GetRenderWindow()->SetSize(1600, 996);

	caseSpaceChart->SetTitle("");
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->SetTitle("PTV + rectum overlap");
	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->SetTitle( /* Hack to position x-axis title on right of graph */
	".                                                                                                  PTV + bladder overlap");

	//caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetColor(1, 0, 0);
	//caseSpaceChart->GetAxis(vtkAxis::LEFT)->GetPen()->SetWidth(3);  
	//caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->GetPen()->SetWidth(3);

	caseSpaceChart->SetShowLegend(true);

	if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kDuke)
	{
		addDukeDataToChart();
	}
	else if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kHighPoint)
	{
		addHighPointDataToChart();
	}
	else if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kPocono)
	{
		addPoconoDataToChart();
	}

	addQueryCaseToChart();

	caseSpaceChart->SetDrawAxesAtOrigin(true);

	// No ticks, no tick labels, please:
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->SetNumberOfTicks(0);
	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->SetNumberOfTicks(0);

	// That didn't work: how about empty arrays?:
	vtkDoubleArray *tickPositions = vtkDoubleArray::New();
	vtkStringArray *tickLabels = vtkStringArray::New();
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->SetTickPositions(tickPositions);
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->SetTickLabels(tickLabels);
	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->SetTickPositions(tickPositions);
	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->SetTickLabels(tickLabels);

	// (Empty arrays work if the range is explicitly set:)
	// Need to put query case at origin.  Shift the ranges negative by query
	// case values:
	double xMax = maxPTVPlusBladder - queryCase->getPTVPlusBladder();
	double xMin = minPTVPlusBladder - queryCase->getPTVPlusBladder();
	double yMax = maxPTVPlusRectum - queryCase->getPTVPlusRectum();
	double yMin = minPTVPlusRectum - queryCase->getPTVPlusRectum();

	// Set the ranges a little bigger (* 0.75 and 1.333) to make sure all points
	//are well inside:
	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->SetRange(xMin * 0.75, xMax * 1.333);
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->SetRange(yMin * 0.75, yMax * 1.333);

	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetJustificationToLeft();
	//caseSpaceChart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetOrientation(90.0); // Doesn't work
	//double xOr = caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->GetOrientation();
	//double yOr = caseSpaceChart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->GetOrientation();

	// Raise the y-axis title
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetLineOffset(-193);

	caseSpaceChart->Update();
	caseSpaceChart->RecalculateBounds();

	selectedMatchPlotPosition = new vtkVector2f;
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
	ren->SetBackground2(0.1, 0.1, 0.1);
	ren->SetGradientBackground(true);
	ren->SetRenderWindow(caseSpaceRenWin);
	caseSpaceRenWin->AddRenderer(ren);

	RendererCallback *callback = RendererCallback::New();
	ren->AddObserver(vtkCommand::StartEvent, callback);
	callback->Delete();

	pickCallback = PickCallback::New();
	pickCallback->SetCaseSpaceDialog(this);

	hoverCallback = HoverCallback::New();
	hoverCallback->SetCaseSpaceDialog(this);

	picker = vtkCellPicker::New();
	picker->SetTolerance(0.0);
	picker->AddObserver(vtkCommand::EndPickEvent, pickCallback);
	picker->AddObserver(vtkCommand::HoverEvent, hoverCallback);

	renderWindowInteractor = caseSpaceViewWidget->GetInteractor();
	renderWindowInteractor->SetRenderWindow(caseSpaceRenWin);
	renderWindowInteractor->SetPicker(picker);

	selectedMatchPlotPosition = new vtkVector2f;

	if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kDuke)
	{
		addDukeDataToChart();
	}
	else if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kHighPoint)
	{
		addHighPointDataToChart();
	}
	else if (mainWindow->getQueryCaseSourceInstitution() == MainWindow::kPocono)
	{
		addPoconoDataToChart();
	}
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addDukeDataToChart()
{
	vtkDoubleArray *dukeXCoords = vtkDoubleArray::New();
	vtkDoubleArray *dukeYCoords = vtkDoubleArray::New();
	dukeTable = vtkTable::New();
//	vtkPlot *dukePoints;

	QFile file(dukeOverlapDataPath);

	if (!file.open(QIODevice::ReadOnly))
	{
		QString warn = "Failed to open \"" + dukeOverlapDataPath + "\"";
		QMessageBox::warning(this, tr("File Open Failed"), warn);
		return; // false;
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
		if (i == queryCaseIndex) continue;

		dX[i] = dukePatientList[i].getPTVPlusBladder();
		dY[i] = dukePatientList[i].getPTVPlusRectum();

		dukePoint[i] = vtkCubeSource::New();
		dukePoint[i]->SetCenter(dX[i], dY[i], MIval[queryCaseIndex][i] * zMult);
		dukePoint[i]->SetXLength(100.0);
		dukePoint[i]->SetYLength(100.0);
		dukePoint[i]->SetZLength(100.0);

		dukePointMapper[i] = vtkPolyDataMapper::New();
		dukePointMapper[i]->SetInputConnection(dukePoint[i]->GetOutputPort());

		dukePointActor[i] = vtkActor::New();
		dukePointActor[i]->SetMapper(dukePointMapper[i]);
		dukePointActor[i]->GetProperty()->SetColor(100 / 255.0 , 120 / 255.0 , 165 / 255.0);

		ren->AddActor(dukePointActor[i]);
	}

	ren->Render();
}

void CaseSpaceDialog::addQueryCase()
{
	queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum();

	vtkCubeSource *queryPoint = vtkCubeSource::New();
	xCenterQueryPt = queryCase->getPTVPlusBladder();
	yCenterQueryPt = queryCase->getPTVPlusRectum();
	zCenterQueryPt = MIMin * zMult;
	//queryPoint->SetCenter(queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(), MIMin * zMult);
	queryPoint->SetCenter(xCenterQueryPt, yCenterQueryPt, zCenterQueryPt);
	queryPoint->SetXLength(200.0);
	queryPoint->SetYLength(200.0);
	queryPoint->SetZLength(200.0);

	vtkPolyDataMapper *queryPointMapper = vtkPolyDataMapper::New();
	queryPointMapper->SetInputConnection(queryPoint->GetOutputPort());

	vtkActor *queryPointActor = vtkActor::New();
	queryPointActor->SetMapper(queryPointMapper);
	queryPointActor->GetProperty()->SetColor(1.0, 0.5, 0.0);
	queryPointActor->PickableOff();

	ren->AddActor(queryPointActor);
}

////////////////////////////////////////////////////////////////////////////////
//
// Currrently using dummy data (random number tables).
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addDummyDukeDataToChart()
{
	vtkDoubleArray *dukeXCoords = vtkDoubleArray::New();
	vtkDoubleArray *dukeYCoords = vtkDoubleArray::New();
	dukeTable = vtkTable::New();
	vtkPlot *dukePoints;
	const int numCases = 200;
	
	// Random numbers from 
	// http://www.rand.org/content/dam/rand/pubs/monograph_reports/2005/digits.txt.zip:

	double dukeX[numCases] = 
	{
		10097, 32533, 76520, 13586, 34673, 54876, 80959, 9117, 39292, 74945,
		37542, 4805, 64894, 74296, 24805, 24037, 20636, 10402, 822, 91665,
		8422, 68953, 19645, 9303, 23209, 2560, 15953, 34764, 35080, 33606,
		99019, 2529, 9376, 70715, 38311, 31165, 88676, 74397, 4436, 27659,
		12807, 99970, 80157, 36147, 64032, 36653, 98951, 16877, 12171, 76833,
		66065, 74717, 34072, 76850, 36697, 36170, 65813, 39885, 11199, 29170,
		31060, 10805, 45571, 82406, 35303, 42614, 86799, 7439, 23403, 9732,
		85269, 77602, 2051, 65692, 68665, 74818, 73053, 85247, 18623, 88579,
		63573, 32135, 5325, 47048, 90553, 57548, 28468, 28709, 83491, 25624,
		73796, 45753, 3529, 64778, 35808, 34282, 60935, 20344, 35273, 88435,
		98520, 17767, 14905, 68607, 22109, 40558, 60970, 93433, 50500, 73998,
		11805, 5431, 39808, 27732, 50725, 68248, 29405, 24201, 52775, 67851,
		83452, 99634, 6288, 98083, 13746, 70078, 18475, 40610, 68711, 77817,
		88685, 40200, 86507, 58401, 36766, 67951, 90364, 76493, 29609, 11062,
		99594, 67348, 87517, 64969, 91826, 8928, 93785, 61368, 23478, 34113,
		65481, 17674, 17468, 50950, 58047, 76974, 73039, 57186, 40218, 16544,
		80124, 35635, 17727, 8015, 45318, 22374, 21115, 78253, 14385, 53763,
		74350, 99817, 77402, 77214, 43236, 210, 45521, 64237, 96286, 2655,
		69916, 26803, 66252, 29148, 36936, 87203, 76621, 13990, 94400, 56418,
		9893, 20505, 14225, 68514, 46427, 56788, 96297, 78822, 54382, 14598,
	};

	dX = new double[numCases];
	memcpy(dX, dukeX, numCases * sizeof(double));

	double dukeY[numCases] =
	{
		80508, 76285, 17630, 9429, 30293, 16391, 87516, 20628, 53159, 80261,
		12043, 94593, 2328, 43332, 83707, 12201, 23088, 39829, 76777, 55495,
		41717, 72807, 33686, 73225, 30173, 5410, 91541, 45387, 48084, 21855,
		54866, 57899, 13389, 68475, 77825, 1301, 74831, 15970, 68803, 14519,
		12030, 92278, 86864, 4430, 50868, 4949, 8820, 98949, 33713, 87279,
		71744, 72285, 82724, 45846, 69682, 89838, 70910, 26386, 16527, 21698,
		7607, 46148, 29548, 8230, 93459, 69788, 43771, 50812, 60337, 40035,
		25584, 34039, 92437, 61873, 7874, 43107, 56212, 48897, 48008, 83125,
		64572, 2625, 39993, 32573, 88828, 19036, 19394, 51921, 68629, 84838,
		22089, 96239, 65157, 3977, 92561, 41314, 80082, 60159, 74429, 34535,
		58590, 90320, 67095, 28958, 62803, 5097, 8269, 63296, 92249, 80332,
		21640, 45655, 94143, 89051, 22782, 29086, 38014, 11641, 54398, 85092,
		81936, 35183, 97146, 90677, 41012, 62425, 19569, 40059, 32565, 23037,
		34506, 67652, 56534, 21287, 58697, 36165, 43304, 52134, 22272, 75345,
		64575, 80559, 38389, 21713, 36749, 30055, 39889, 4287, 21294, 77790,
		28400, 71414, 73453, 62631, 85191, 18446, 81309, 33305, 67816, 56922,
		797, 10584, 63075, 31922, 48847, 34738, 32528, 1884, 71241, 34618,
		39544, 26038, 86456, 29624, 76562, 21853, 31395, 81509, 72150, 35599,
		94881, 65970, 22406, 21125, 41074, 63283, 61007, 22211, 21082, 73175,
		74166, 39761, 35695, 43436, 38419, 937, 68925, 63631, 90667, 15306,
	};

	dY = new double[numCases];
	memcpy(dY, dukeY, numCases * sizeof(double));

	for (int i = 0; i < numCases; i++)
	{
		//dX[i] = ((int)dX[i] % 200) - 100; // range -100 - 100
		//dY[i] = ((int)dY[i] % 200) - 100;
		dX[i] *= 20; 
		dY[i] *= 20;
	}

	dukeXCoords->SetArray(dX, numCases, 1);
	dukeXCoords->SetName("Duke X");
	dukeTable->AddColumn(dukeXCoords);
	dukeTable->SetNumberOfRows(numCases);

	dukeYCoords->SetArray(dY, numCases, 1);
	dukeYCoords->SetName("Duke");
	dukeTable->AddColumn(dukeYCoords);

	dukePoints = caseSpaceChart->AddPlot(vtkChart::POINTS);
	dukePoints->SetInput(dukeTable, 0, 1);
	dukePoints->SetColor(190, 210, 255, 255);

	dukeTable->Update();
}

////////////////////////////////////////////////////////////////////////////////
//
// Currrently using dummy data (random number tables).
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addPoconoDataToChart()
{
	vtkDoubleArray *poconoXCoords = vtkDoubleArray::New();
	vtkDoubleArray *poconoYCoords = vtkDoubleArray::New();
	poconoTable = vtkTable::New();
	vtkPlot *poconoPoints;
	const int numCases = 100;
	
	double poconoX[numCases] =
	{
		50024, 77973, 37215, 15317, 29627, 6296, 96124, 55171, 80151, 44481,
		18570, 28827, 78396, 43240, 17224, 45981, 6678, 81875, 30469, 99142,
		20691, 32627, 35801, 23684, 69732, 70960, 51639, 86304, 41947, 8067,
		85335, 27321, 42201, 37768, 63889, 53600, 17937, 17356, 80823, 71697,
		25903, 52768, 8732, 21795, 21741, 29862, 8352, 82046, 84754, 43423,
		21618, 52678, 27782, 26434, 61535, 81543, 6911, 44126, 66259, 32488,
		35480, 76486, 4346, 58586, 86336, 61404, 22280, 75605, 90585, 66989,
		86683, 23437, 67517, 51046, 52570, 11244, 7739, 61841, 39524, 30022,
		72515, 93576, 42242, 82876, 71525, 14, 21422, 79154, 85360, 43334,
		92252, 57535, 15295, 52671, 13826, 96554, 15538, 71169, 41268, 54695
	};

	pX = new double[numCases];
	memcpy(pX, poconoX, numCases * sizeof(double));

	double poconoY[numCases] =
	{
		53315, 11477, 50231, 13235, 82006, 70697, 14795, 13947, 20448, 55934,
		13499, 89995, 97861, 7538, 56643, 9798, 70722, 54503, 24026, 42450,
		85877, 23805, 14300, 50880, 92451, 30768, 23415, 33343, 72804, 67861,
		34908, 18299, 46846, 57902, 4238, 25327, 99436, 92607, 78585, 86239,
		84571, 66256, 48074, 14004, 29033, 87385, 57711, 26809, 76019, 56539,
		56096, 40158, 54990, 82584, 37565, 97506, 73915, 38026, 77124, 2718,
		99653, 28840, 45119, 69474, 28090, 65074, 73212, 45844, 73078, 61180,
		38431, 83402, 10664, 4062, 42474, 65487, 7735, 59046, 97560, 74934,
		8045, 41653, 2516, 22537, 44439, 69606, 36451, 91106, 40561, 63178,
		33695, 45328, 55442, 82857, 1995, 4935, 27406, 98715, 11578, 68614
	};

	pY = new double[numCases];
	memcpy(pY, poconoY, numCases * sizeof(double));

	for (int i = 0; i < numCases; i++)
	{
		//pX[i] = ((int)pX[i] % 200) - 100; 
		//pY[i] = ((int)pY[i] % 200) - 100; 
		pX[i] *= 40; 
		pY[i] *= 40;
	}

	poconoXCoords->SetArray(pX, numCases, 1);
	poconoXCoords->SetName("Pocono X");
	poconoTable->AddColumn(poconoXCoords);

	poconoYCoords->SetArray(pY, numCases, 1);
	poconoYCoords->SetName("Pocono");
	poconoTable->AddColumn(poconoYCoords);

	poconoPoints = caseSpaceChart->AddPlot(vtkChart::POINTS, vtkPlotPoints::CIRCLE);
	poconoPoints->SetInput(poconoTable, 0, 1);
	poconoPoints->SetColor(170, 210, 170, 255);

	poconoTable->Update();
}

////////////////////////////////////////////////////////////////////////////////
//
// Currrently using dummy data (random number tables).
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addHighPointDataToChart()
{
	vtkTable *highPointTable = vtkTable::New();
	vtkDoubleArray *highPointXCoords = vtkDoubleArray::New();
	vtkDoubleArray *highPointYCoords = vtkDoubleArray::New();
	vtkPlot *highPointPoints;

	const int numCases = 100;
	
	// Random numbers from 
	// http://www.rand.org/content/dam/rand/pubs/monograph_reports/2005/digits.txt.zip:

	double highPointX[numCases] =
	{
		74125, 63878, 62461, 97700, 85648, 77945, 45938, 29687, 84670, 78942,
		01242, 89212, 9406, 805, 27040, 88349, 44212, 50771, 90802, 61844,
		87211, 88979, 30871, 42152, 66203, 70042, 20081, 53303, 83833, 56055,
		99234, 1805, 45968, 51687, 90510, 68644, 37801, 93993, 1582, 30518,
		40746, 1986, 21367, 8836, 20769, 41922, 942, 66958, 23396, 15179,
		24721, 70747, 39288, 55537, 43789, 14763, 62121, 95101, 58731, 19242,
		92266, 71948, 72155, 77906, 97614, 71391, 32732, 35174, 30051, 77022,
		74694, 95394, 17888, 81692, 86792, 65238, 42213, 23398, 91036, 54574,
		52416, 3438, 44596, 33646, 10381, 84556, 50472, 50740, 35319, 30296,
		81207, 59017, 63439, 68594, 31557, 74216, 84924, 96216, 65668, 83454
	};

	hpX = new double[numCases];
	memcpy(hpX, highPointX, numCases * sizeof(double));

	double highPointY[numCases] =
	{
		41438, 85843, 28227, 64869, 83324, 49508, 6473, 70409, 59405, 24722,
		88974, 25138, 87231, 79127, 27017, 24909, 49727, 3647, 4352, 97628,
		43798, 77277, 25911, 32258, 76543, 79623, 43188, 72371, 6679, 78217,
		69242, 57630, 81782, 55356, 20390, 97076, 13951, 74935, 23149, 9426,
		34956, 2777, 36601, 47136, 37106, 37695, 43441, 68595, 69325, 82013,
		22426, 25510, 12424, 46241, 3842, 64733, 31261, 54377, 9723, 46531,
		83640, 32307, 71622, 79741, 9116, 92140, 17179, 20002, 86698, 71965,
		24711, 92166, 81452, 66002, 22805, 4605, 51317, 25123, 80477, 40963,
		28674, 43328, 18536, 22962, 79363, 33062, 93835, 96797, 52795, 12977,
		93353, 84149, 57433, 48999, 26980, 90777, 67457, 54214, 93754, 4276
	};

	hpY = new double[numCases];
	memcpy(hpY, highPointY, numCases * sizeof(double));

	for (int i = 0; i < numCases; i++)
	{
		//hpX[i] = ((int)hpX[i] % 200) - 100; 
		//hpY[i] = ((int)hpY[i] % 200) - 100; 
		hpX[i] *= 40; 
		hpY[i] *= 40;
	}

	highPointXCoords->SetArray(hpX, numCases, 1);
	highPointXCoords->SetName("High Point X");
	highPointTable->AddColumn(highPointXCoords);

	highPointYCoords->SetArray(hpY, numCases, 1);
	highPointYCoords->SetName("High Point");
	highPointTable->AddColumn(highPointYCoords);

	highPointPoints = caseSpaceChart->AddPlot(vtkChart::POINTS, vtkPlotPoints::DIAMOND);
	highPointPoints->SetInput(highPointTable, 0, 1);
	highPointPoints->SetColor(180, 130, 255, 255);

	highPointTable->Update();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addQueryCaseToChart()
{
	const int numCases = 28;
	double queryX[numCases];
	double queryY[numCases];

	for (int i = 0; i < numCases; i++) 
	{
		queryX[i] = 10.0; 
		queryY[i] = 10.0;
	}

	queryCaseTable = vtkTable::New();
	vtkDoubleArray *queryXCoords = vtkDoubleArray::New();
	vtkDoubleArray *queryYCoords = vtkDoubleArray::New();
	vtkPlot *queryPoint = caseSpaceChart->AddPlot(vtkChart::POINTS); //, vtkPlotPoints::CROSS);
	queryXCoords->SetArray(queryX, numCases, 1);
	queryXCoords->SetName("qcx");
	queryYCoords->SetArray(queryY, numCases, 1);
	queryYCoords->SetName("Query case");

	queryCaseTable->AddColumn(queryXCoords);
	queryCaseTable->AddColumn(queryYCoords);

	queryPoint->SetInput(queryCaseTable, 0, 1);
	queryPoint->SetColor(255, 0, 0, 255); // 148, 128, 255);
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

			//if (MIval[row][col] > MIMax) MIMax = MIval[row][col];
			//if (MIval[row][col] < MIMin) MIMin = MIval[row][col];
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
		if (MIval[queryCaseIndex][i] > MIMax) MIMax = MIval[queryCaseIndex][i];
		if (MIval[queryCaseIndex][i] < MIMin) MIMin = MIval[queryCaseIndex][i];
	}

	// Then get the fraction from min to max for each case:
	MIRange = MIMax - MIMin;

	this->ReportCameraPosition(ren);
	axes = new Axes();
	//double shaftLength = 6000.0;
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

	xf->SetScale(shaftLength / 35.0);
	yf->SetScale(shaftLength / 35.0);
	zf->SetScale(shaftLength / 35.0);

	ren->AddActor(xf);
	ren->AddActor(yf);
	ren->AddActor(zf);

	for (int i = 0; i < numMICases; i++)
	{
		if (i == queryCaseIndex) continue;

		MIFraction[i] = (MIval[queryCaseIndex][i] - MIMin) / MIRange;
		dukePointActor[i]->GetProperty()->SetColor(MIFraction[i] * 1.2, MIFraction[i] * 1.2, 1.0 - (MIFraction[i] * 1.2));
	}

	// Set up the MI Range Slider with current values:
	QString maxAsText;
	maxAsText.setNum(MIMax, 'g', 3);
	MIMaxLabel->setText(maxAsText);
	QString minAsText;
	minAsText.setNum(MIMin, 'g', 3);
	MIMinLabel->setText(minAsText);
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
	//mitpActor->GetProperty()->SetColor(0.4, 0.4, 0.4);

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
// Broken.  Pretty sure it's a bug in ccChartXY, i.e., vtkChartXY.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::drawSelectedCase()
{
	if (selectedMatchCaseTable)
	{
		if (selectedPoint)
		{
			caseSpaceChart->RemovePlotInstance(selectedPoint);
		}

		selectedMatchCaseTable->Delete();
	}

	const int numDuplicates = 1;

	double selectedX[numDuplicates]; 
	double selectedY[numDuplicates];

	// ccChartXY doesn't place points correctly if there are "too 
	// few" (i.e., from informal tests, about 27) elements in the
	// columns added, so the hack is to draw a bunch, all at the 
	// same place.  However, with the current VTK build (5.6.1)
	// that doesn't seem to help either (selected case point is
	// always drawn at the origin) so this loop is being set to 
	// a single iteration but with the hacked code still here if I
	// want to resurrect it:
	for (int i = 0; i < numDuplicates; i++) 
	{
		selectedX[i] = selectedMatchPlotPosition->X();
		selectedY[i] = selectedMatchPlotPosition->Y();
		// Even when the following two lines are substituted for the
		// previous two, the selected case point still shows up at
		// the origin.  That's why I think the bug is in ccChartXY
		// (SAC 2011/06/26):
		//selectedX[i] = (i + 1) * 1000000.0;
		//selectedY[i] = (-i - 1) * 1000000.0;
	}

	vtkDoubleArray *selectedXCoords = vtkDoubleArray::New();
	vtkDoubleArray *selectedYCoords = vtkDoubleArray::New();
	selectedXCoords->SetArray(selectedX, numDuplicates, 1);
	selectedXCoords->SetName("sel");
	selectedYCoords->SetArray(selectedY, numDuplicates, 1);
	selectedYCoords->SetName("Selected case");

	selectedMatchCaseTable = vtkTable::New();
	selectedMatchCaseTable->AddColumn(selectedXCoords);
	selectedMatchCaseTable->AddColumn(selectedYCoords);

	selectedPoint = caseSpaceChart->AddPlot(vtkChart::POINTS, vtkPlotPoints::DIAMOND);
	selectedPoint->SetInput(selectedMatchCaseTable, 0, 1);
	selectedPoint->SetColor(255, 185, 0, 255);
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
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::pickPatient()
{
	double pickPos[3];
	picker->GetPickPosition(pickPos);

	cout << "Patient at (" << pickPos[0] << ", " << pickPos[1] << ", "
	<< pickPos[2] << ")" << endl;

	vtkActor *pickedActor = picker->GetActor();

	if (pickedActor)
	{
		double *center = new double[3];
		center = pickedActor->GetCenter();

		cout << "Center at (" << center[0] << ", " << center[1] << ", "
		<< center[2] << ")" << endl;

		selectedMatchPlotPosition->SetX(center[0]);
		selectedMatchPlotPosition->SetY(center[1]);
		identifyMatchCase();
	}
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
	cout << "MI threshold = " << MIThresholdVal
		 << "; threshold plane z val = " << thresholdPlaneZVal << endl;
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

