////////////////////////////////////////////////////////////////////////////////
//
// CompareDialog.cpp: Qt-based GUI for prostate cancer radiation therapy planning 
// tool. This window displays two horizontal "panels": the top displays data
// for the "query" patient, i.e., the current patient for whom treatment is 
// being planned.  The lower "panel" displays data for the "match" patient,
// that is, data associated with a prior case who has completed a successful
// program of radiation therapy and has been selected by an algorithm external
// to this code as one of the cases in the database most like the query case.
//
// Each "panel" consists of three widgets, in order left to right: 1) a CT 
// (Computerized Tomography) image data display, including controls for axis
// selection, slice selection, and auto-play (image animation along the current
// slice axis); 2) a projection display, which shows an image projection of the
// overlap or degree of occlusion between the various anatomical structures in
// the treatment area from any of the seven (user-selectable) standard gantry
// angles ("standard" with respect to the system used at Duke Medical Center,
// namely the Eclipse treatment planning system from Palo Alto-based Varian
// Medical Systems); and 3) Dose Volume Histogram (DVH) data as a cartesian
// graph for the selected anatomical structures (femoral head data display
// may be toggled on or off for projection and DVH displays).  
//
// The various user-selectable display options are synchronized so that both 
// query and match data are shown with identical options set (e.g., CT display
// axis, projection gantry angle).  At some future date some or all aspects
// of these displays may be desynchronized, for example if a planning
// physician chooses to look at a different CT slice in the query patient
// than in a match, so the patient and match functionality has been
// separated out in many cases, even though this code currently doesn't
// exploit that separability.
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyanavich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#include <QtGui>

#include "vtkImageViewer.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkPNGReader.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageFlip.h"
#include "vtkChartXY.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"

#include "Patient.h"
#include "CompareDialog.h"

int min(int a, int b) 
{
	return (a < b) ? a : b;
}

int max(int a, int b) 
{
	return (a > b) ? a : b;
}

// rgb values, 0.0-1.0 range:
static const double structureColor[CompareDialog::numStructures][3] =
{
	0.0, 0.0, 0.9,		// PTV
	0.675, 0.0, 0.675,	// rectum
	0.0, 0.9, 0.0,		// bladder
	0.9, 0.0, 0.0,		// left femoral head
	0.675, 0.675, 0.0	// right femoral head
};

// rgb values equivalent to those in structureColor, but in the 0-255 range:
static const double color[CompareDialog::numStructures][3] =
{
	0.0, 0.0, 229.0,	// PTV
	172.0, 0.0, 172.0,	// rectum
	0.0, 229.0, 0.0,	// bladder
	229.0, 0.0, 0.0,	// left femoral head
	172.0, 172.0, 0.0	// right femoral head
};

static const char *structureName[CompareDialog::numStructures] =
{
	"PTV",		// Planning Target Volume
	"rectum",
	"bladder",
	"left fem",
	"right fem"
};

const int CompareDialog::xDVHWidget = 733; // Width of DVH display
const int CompareDialog::yDVHWidget = 512; // Height of DVH display

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CompareDialog::CompareDialog()
	:	queryAngle(Patient::defaultGantryAngle),
		matchAngle(Patient::defaultGantryAngle),
		queryPatient(NULL),
		matchPatient(NULL),
		queryDICOMReader(NULL),
		matchDICOMReader(NULL),
		queryCTImageFlip(NULL),
		matchCTImageFlip(NULL),
		queryCTImageViewer(NULL),
		matchCTImageViewer(NULL),
		queryProjectionReader(NULL),
		matchProjectionReader(NULL),
		queryProjectionViewer(NULL),
		matchProjectionViewer(NULL),
		gantryAngleMenu(NULL),
		gantryAngleActionGroup(NULL),
		queryDVH(NULL),
		matchDVH(NULL),
		queryDVHView(NULL),
		matchDVHView(NULL)
{
	setupUi(this);
	setupVTKUI();
	setupProjectionGantryAngleMenu();
	createActions();

	viewFemoralHeadsCheckBox->setChecked(true);

	legendGroupBox->setStyleSheet("QGroupBox { color: solid black; }");
	legendGroupBox->setStyleSheet("QGroupBox { border: 2px solid black; }");

	queryPatient = new Patient(Patient::defaultPatientNumber);
	matchPatient = new Patient(Patient::defaultPatientNumber + 1);

	selectQuery(Patient::defaultPatientNumber);
	selectMatch(Patient::defaultPatientNumber + 1);

	querySelectSpinBox->setValue(Patient::defaultPatientNumber);
	matchSelectSpinBox->setValue(Patient::defaultPatientNumber + 1);
}

CompareDialog::~CompareDialog()
{
	if (queryPatient)
	{
		delete queryPatient;
	}

	if (matchPatient)
	{
		delete matchPatient;
	}

	if (queryDICOMReader)
	{
		queryDICOMReader->Delete();
	}
		
	if (matchDICOMReader)
	{
		matchDICOMReader->Delete();
	}
		
	if (queryCTImageFlip)
	{
		queryCTImageFlip->Delete();
	}
		
	if (matchCTImageFlip)
	{
		matchCTImageFlip->Delete();
	}
		
	if (queryCTImageViewer)
	{
		queryCTImageViewer->Delete();
	}
		
	if (matchCTImageViewer)
	{
		matchCTImageViewer->Delete();
	}
		
	if (queryProjectionReader)
	{
		queryProjectionReader->Delete();
	}
		
	if (matchProjectionReader)
	{
		matchProjectionReader->Delete();
	}
		
	if (queryProjectionViewer)
	{
		queryProjectionViewer->Delete();
	}
		
	if (matchProjectionViewer)
	{
		matchProjectionViewer->Delete();
	}
		
	if (gantryAngleMenu)
	{
		delete gantryAngleMenu;
	}
		
	if (gantryAngleActionGroup)
	{
		delete gantryAngleActionGroup;
	}
		
	if (queryDVH)
	{
		queryDVH->Delete();
	}
		
	if (matchDVH)
	{
		matchDVH->Delete();
	}
	
	if (queryDVHView)
	{
		queryDVHView->Delete();
	}
	
	if (matchDVHView)
	{
		matchDVHView->Delete();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// Sets up CT, projection, and DVH displays for query patient with id == param
// patientNumber.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectQuery(int patientNumber)
{
	queryPatient->setNumber(patientNumber);
	selectQueryCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
	selectQueryProjection();
	displayQueryDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
// Sets up CT, projection, and DVH displays for match patient with id == param
// patientNumber.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatch(int patientNumber)
{
	matchPatient->setNumber(patientNumber);
	selectMatchCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
	selectMatchProjection();
	displayMatchDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
// Select display orientation for CT image displays for both query and match
// patients.  
//
// If the query and match CT data sets have different numbers of slices (which 
// often happens, notably in the axial view), set the max slider value to the 
// max of the two numbers of slices.  When the data set with fewer slices gets
// to its last slice, it just continues to display that last slice while the
// other keeps going.  Thus, synchronization is preserved to the degree 
// possible but the user can also see all slices for both data sets.  
//
// When the axis is changed, the slider is set for both data sets to about the
// middle slice for the data set with the largest number of slices, by
// Vorakarn's request: the end slices tend to be devoid of interesting info.
//
// The bool parameter "val" isn't used but it has to be there because this 
// method, as a Qt "slot", has to have the same argument list as the Qt-defined
// "signal" it's being connected with.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setSliceAxis(bool val /* = true */)
{
	if (axialRadioButton->isChecked()) 
	{
		sliceSelectionLabel->setText(tr("Z slice:"));
		queryPatient->setSliceOrientation(Patient::Z);
		matchPatient->setSliceOrientation(Patient::Z);
		queryCTImageViewer->SetSliceOrientationToXY();
		matchCTImageViewer->SetSliceOrientationToXY();
	}
	else if (sagittalRadioButton->isChecked())
	{
		sliceSelectionLabel->setText(tr("X slice:"));
		queryPatient->setSliceOrientation(Patient::X);
		matchPatient->setSliceOrientation(Patient::X);
		queryCTImageFlip->SetFilteredAxis(2); // Flip about z
		matchCTImageFlip->SetFilteredAxis(2); // Flip about z
		queryCTImageViewer->SetSliceOrientationToYZ();
		matchCTImageViewer->SetSliceOrientationToYZ();
	}
	else if (coronalRadioButton->isChecked())
	{
		sliceSelectionLabel->setText(tr("Y slice:"));
		queryPatient->setSliceOrientation(Patient::Y);
		matchPatient->setSliceOrientation(Patient::Y);
		queryCTImageFlip->SetFilteredAxis(2); // Flip about z
		matchCTImageFlip->SetFilteredAxis(2); // Flip about z
		queryCTImageViewer->SetSliceOrientationToXZ();
		matchCTImageViewer->SetSliceOrientationToXZ();
	}

	int qSliceMin = queryCTImageViewer->GetSliceMin();
	int qSliceMax = queryCTImageViewer->GetSliceMax();
	int mSliceMin = matchCTImageViewer->GetSliceMin();
	int mSliceMax = matchCTImageViewer->GetSliceMax();

	sliceSelectionSlider->setMaxValue(max(qSliceMax, mSliceMax));
	sliceSelectionSpinBox->setMaxValue(max(qSliceMax, mSliceMax));
	sliceSelectionSlider->setMinValue(min(qSliceMin, mSliceMin));
	sliceSelectionSpinBox->setMinValue(min(qSliceMin, mSliceMin));
	sliceSelectionSlider->setValue(max(qSliceMax, mSliceMax) / 2); // ~ middle
	maxSliceLabel->setText(QString::number(max(qSliceMax, mSliceMax)));
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::changeSlice(int slice)
{
	queryCTImageViewer->SetSlice(slice);
	matchCTImageViewer->SetSlice(slice);

	//extractQueryDICOMFileMetaData();
}

////////////////////////////////////////////////////////////////////////////////
//
// Animate image display through all the slices with the current axis
// orientation.
//
// Although we're explicitly changing only slider, spinbox, and text label in
// this method, the following behavior is implicit if auto-play is synchronized
// between query and patient:
//
// If the query and match CT data sets have different numbers of slices (which 
// often happens, notably in the axial view), the max slider value is set to the 
// max of the two numbers of slices.  When the data set with fewer slices gets
// to its last slice, it just continues to display that last slice while the
// other keeps going.  Thus, synchronization is preserved as much as possible
// but the user can also see all slices for both data sets.  
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::autoplay()
{
	int maxSliceNum = sliceSelectionSlider->maxValue();

	for (int i = 0; i <= maxSliceNum; i++)
	{
		changeSlice(i);

		this->setWindowTitle(QString::number(i));
		maxSliceLabel->setText(QString::number(i));

		sliceSelectionSpinBox->setValue(i);
		sliceSelectionSpinBox->update();

		sliceSelectionSlider->setValue(i);
		sliceSelectionSlider->update();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle27()
{
	angle27Action->setChecked(true);
	queryAngle = matchAngle = 27;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle78()
{
	angle78Action->setChecked(true);
	queryAngle = matchAngle = 78;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle129()
{
	angle129Action->setChecked(true);
	queryAngle = matchAngle = 129;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle180()
{
	angle180Action->setChecked(true);
	queryAngle = matchAngle = 180;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle231()
{
	angle231Action->setChecked(true);
	queryAngle = matchAngle = 231;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle282()
{
	angle282Action->setChecked(true);
	queryAngle = matchAngle = 282;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle333()
{
	angle333Action->setChecked(true);
	queryAngle = matchAngle = 333;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
// Affects projection and DVH displays for both query and match.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::toggleFemoralHeads(bool checked)
{
	queryPatient->setIsShowingFemoralHeads(checked);
	matchPatient->setIsShowingFemoralHeads(checked);

	selectQueryProjection();
	displayQueryDVHData();

	selectMatchProjection();
	displayMatchDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupVTKUI()
{
	queryProjectionReader = vtkPNGReader::New();
	matchProjectionReader = vtkPNGReader::New();
	queryProjectionViewer = vtkImageViewer::New();
	matchProjectionViewer = vtkImageViewer::New();

	queryProjectionViewer->SetInputConnection(queryProjectionReader->GetOutputPort());

	queryProjectionWidget->SetRenderWindow(queryProjectionViewer->GetRenderWindow());
	queryProjectionViewer->SetupInteractor(queryProjectionWidget->GetRenderWindow()->GetInteractor());

	queryProjectionViewer->SetColorLevel(138.5);	// Default values from...
	queryProjectionViewer->SetColorWindow(233);		// ..."The VTK User's Guide"

	matchProjectionViewer->SetInputConnection(matchProjectionReader->GetOutputPort());

	matchProjectionWidget->SetRenderWindow(matchProjectionViewer->GetRenderWindow());
	matchProjectionViewer->SetupInteractor(matchProjectionWidget->GetRenderWindow()->GetInteractor());

	matchProjectionViewer->SetColorLevel(138.5);	// Default values from...
	matchProjectionViewer->SetColorWindow(233);		// ..."The VTK User's Guide"

	queryDICOMReader = vtkDICOMImageReader::New();
	matchDICOMReader = vtkDICOMImageReader::New();

	queryCTImageFlip = vtkImageFlip::New();
	matchCTImageFlip = vtkImageFlip::New();

	queryCTImageFlip->SetInputConnection(queryDICOMReader->GetOutputPort());
	matchCTImageFlip->SetInputConnection(matchDICOMReader->GetOutputPort());

	queryCTImageViewer = vtkImageViewer2::New();
	matchCTImageViewer = vtkImageViewer2::New();

	queryCTImageViewer->SetInputConnection(queryCTImageFlip->GetOutputPort());
	matchCTImageViewer->SetInputConnection(matchCTImageFlip->GetOutputPort());

	queryCTImageWidget->SetRenderWindow(queryCTImageViewer->GetRenderWindow());
	queryCTImageViewer->SetupInteractor(queryCTImageWidget->GetRenderWindow()->GetInteractor());

	queryCTImageViewer->SetColorLevel(100);			// Selected to increase contrast...
	queryCTImageViewer->SetColorWindow(500);		// ...at Vorakarn's request

	matchCTImageWidget->SetRenderWindow(matchCTImageViewer->GetRenderWindow());
	matchCTImageViewer->SetupInteractor(matchCTImageWidget->GetRenderWindow()->GetInteractor());

	matchCTImageViewer->SetColorLevel(100);			// Selected to increase contrast...
	matchCTImageViewer->SetColorWindow(500);		// ...at Vorakarn's request

	initQueryDVHObjects();
	initMatchDVHObjects();
}

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate responses to user manipulation of the GUI controls.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::createActions()
{
	connect(querySelectSpinBox, SIGNAL(valueChanged(int)), this, SLOT(selectQuery(int)));
	connect(matchSelectSpinBox, SIGNAL(valueChanged(int)), this, SLOT(selectMatch(int)));

	connect(axialRadioButton, SIGNAL(toggled(bool)), this, SLOT(setSliceAxis(bool)));
	connect(sagittalRadioButton, SIGNAL(toggled(bool)), this, SLOT(setSliceAxis(bool)));
	connect(coronalRadioButton, SIGNAL(toggled(bool)), this, SLOT(setSliceAxis(bool)));

	connect(sliceSelectionSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSlice(int)));
	connect(autoPlayPushButton, SIGNAL(clicked()), this, SLOT(autoplay()));

	connect(viewFemoralHeadsCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggleFemoralHeads(bool)));

	connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

////////////////////////////////////////////////////////////////////////////////
//
// Set the slice for the query patient's CT image display.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectQueryCTSlice(int slice)
{
	queryDICOMReader->Delete();
	queryDICOMReader = vtkDICOMImageReader::New();
	queryDICOMReader->SetDirectoryName((queryPatient->getPathToCTData()).toAscii());
	queryDICOMReader->Update();
	queryCTImageFlip->SetInputConnection(queryDICOMReader->GetOutputPort());
	queryCTImageViewer->SetSlice(slice);
	queryCTImageViewer->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Set the slice for the match patient's CT image display.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatchCTSlice(int slice)
{
	matchDICOMReader->Delete();
	matchDICOMReader = vtkDICOMImageReader::New();
	matchDICOMReader->SetDirectoryName((matchPatient->getPathToCTData()).toAscii());
	matchDICOMReader->Update();
	matchCTImageFlip->SetInputConnection(matchDICOMReader->GetOutputPort());
	matchCTImageViewer->SetSlice(slice);
	matchCTImageViewer->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Diagnostics for debug.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::extractQueryDICOMFileMetaData()
{
	double *pixelSpacing = queryDICOMReader->GetPixelSpacing();
	const char *xferSyntaxUID = queryDICOMReader->GetTransferSyntaxUID();
	const char *patientName = queryDICOMReader->GetPatientName();
	const char *studyUID = queryDICOMReader->GetStudyUID();
	const char *studyID = queryDICOMReader->GetStudyID();
	const char *fileX = queryDICOMReader->GetFileExtensions();
	int w = queryDICOMReader->GetWidth();
	int h = queryDICOMReader->GetHeight();
	float rescaleSlope = queryDICOMReader->GetRescaleSlope();
	float rescaleOffset = queryDICOMReader->GetRescaleOffset();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectQueryProjection()
{
	QString fileName = queryPatient->getPathToProjectionImage(queryAngle);

	if (!fileName.isEmpty())
	{
		queryProjectionReader->SetFileName(fileName);
		queryProjectionViewer->Render();
		queryProjectionWidget->show();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatchProjection()
{
	QString fileName = matchPatient->getPathToProjectionImage(matchAngle);

	if (!fileName.isEmpty())
	{
		matchProjectionReader->SetFileName(fileName);
		matchProjectionViewer->Render();
		matchProjectionWidget->show();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupProjectionGantryAngleMenu()
{
	gantryAngleMenu = new QMenu(this);
	angle27Action = 
		gantryAngleMenu->addAction("27 degrees", this, SLOT(setGantryAngle27()));
	angle78Action = 
		gantryAngleMenu->addAction("78 degrees", this, SLOT(setGantryAngle78()));
	angle129Action = 
		gantryAngleMenu->addAction("129 degrees", this, SLOT(setGantryAngle129()));
	angle180Action = 
		gantryAngleMenu->addAction("180 degrees", this, SLOT(setGantryAngle180()));
	angle231Action = 
		gantryAngleMenu->addAction("231 degrees", this, SLOT(setGantryAngle231()));
	angle282Action = 
		gantryAngleMenu->addAction("282 degrees", this, SLOT(setGantryAngle282()));
	angle333Action = 
		gantryAngleMenu->addAction("333 degrees", this, SLOT(setGantryAngle333()));

	angle27Action->setCheckable(true);
	angle78Action->setCheckable(true);
	angle129Action->setCheckable(true);
	angle180Action->setCheckable(true);
	angle231Action->setCheckable(true);
	angle282Action->setCheckable(true);
	angle333Action->setCheckable(true);


	gantryAngleActionGroup = new QActionGroup(this);
	gantryAngleActionGroup->add(angle27Action);
	gantryAngleActionGroup->add(angle78Action);
	gantryAngleActionGroup->add(angle129Action);
	gantryAngleActionGroup->add(angle180Action);
	gantryAngleActionGroup->add(angle231Action);
	gantryAngleActionGroup->add(angle282Action);
	gantryAngleActionGroup->add(angle333Action);

	angle180Action->setChecked(true);

	gantryAnglePushButton->setMenu(gantryAngleMenu);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::initQueryDVHObjects()
{
	if (queryDVH) queryDVH->Delete();
	queryDVH = vtkChartXY::New();
	setDVHYAxisTicks(queryDVH);

	if (queryDVHView) queryDVHView->Delete();
	queryDVHView = vtkContextView::New();

	queryDVHView->GetScene()->AddItem(queryDVH);
	queryDVHView->SetInteractor(queryDVHWidget->GetInteractor());
	queryDVHWidget->SetRenderWindow(queryDVHView->GetRenderWindow());
	queryDVHView->GetRenderWindow()->SetSize(xDVHWidget, yDVHWidget);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::initMatchDVHObjects()
{
	if (matchDVH) matchDVH->Delete();
	matchDVH = vtkChartXY::New();
	setDVHYAxisTicks(matchDVH);

	if (matchDVHView) matchDVHView->Delete();
	matchDVHView = vtkContextView::New();

	matchDVHView->GetScene()->AddItem(matchDVH);
	matchDVHView->SetInteractor(matchDVHWidget->GetInteractor());
	matchDVHWidget->SetRenderWindow(matchDVHView->GetRenderWindow());
	matchDVHView->GetRenderWindow()->SetSize(xDVHWidget, yDVHWidget);
}

////////////////////////////////////////////////////////////////////////////////
//
// Assumes that each DVH data file has two lines of (discardable) title and
// column header info, followed by exactly numDVHPoints rows of data (although
// there is a check for premature end of file, overall program behavior is
// unpredictable if it occurs).
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::readDVHData(Patient &patient)
{
	QString path = patient.getPathToDVHData();
	QFile file(path);

	// Silently fail if the data isn't available: at this point (2/06/11) it
	// often isn't, that may continue to be the case indefinitely into the
	// future, and announcing the fact is an annoying interruption in workflow.
	//
	if (!file.open(QIODevice::ReadOnly))
	{
		//QString warn = "Failed to open \"" + path + "\"";
		//QMessageBox::warning(this, tr("File Open Failed"), warn);
		return false;
	}

	QTextStream in(&file);

	// Skip over the first two lines (title and column headers):
	in.readLine();
	in.readLine();

	int i = 0;

	while ((!in.atEnd()) && (i < numDVHPoints))
	{
		in >> dose[i] >> volumes[PTV][i] >> volumes[rectum][i] 
		   >> volumes[bladder][i]  >> volumes[leftFem][i] 
		   >> volumes[rightFem][i];
		i++;  // Hack: won't work unless incremented as separate statement [?]
	}

	in.flush();
	file.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// Assumes that parameter chart points to a previously instantiated vtkChartXY
// object.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupDVHChart(vtkChartXY *chart, char *title)
{
	this->clearDVHPlots(chart);

	chart->SetTitle(title);
	chart->GetAxis(vtkAxis::LEFT)->SetTitle("% volume");
	chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("% dose");
	chart->SetShowLegend(true);

	vtkFloatArray *xCoords = vtkFloatArray::New();;
	vtkFloatArray *yCoords[numStructures];
	vtkTable *table = vtkTable::New();
	vtkPlot *line;

	xCoords->SetArray(dose, numDVHPoints, 1);
	xCoords->SetName("% dose");
	table->AddColumn(xCoords);
	table->SetNumberOfRows(numDVHPoints);

	int numStructuresToDisplay = viewFemoralHeadsCheckBox->isChecked() 
							   ? numStructures : 3;

	for (int i = 0; i < numStructuresToDisplay; i++)
	{
		yCoords[i] = vtkFloatArray::New();
		yCoords[i]->SetArray(volumes[i], numDVHPoints, 1);
		yCoords[i]->SetName(structureName[i]);
		table->AddColumn(yCoords[i]);
		table->Update();
		line = chart->AddPlot(vtkChart::LINE);
		line->SetInput(table, 0, i + 1);
		line->SetColor(color[i][0], color[i][1], color[i][2], 255);
		yCoords[i]->Delete();
	}

	chart->Update();
	chart->RecalculateBounds();

	xCoords->Delete();
}

////////////////////////////////////////////////////////////////////////////////
// 
// This function is a hack: chart->ClearPlots() currently crashes the app (using
// VTK 5.6.1).  We finally perform the same behavior by looping through the 
// various plots associated with the chart object and redundantly remove them
// individually, one by one, over and over again.  Hopefully the imminent :-)
// VTK 5.8 release will obviate the need for this crude method.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::clearDVHPlots(vtkChartXY *chart)
{
	int numPlots = chart->GetNumberOfPlots();
	vtkPlot *oldPlot;

	for (int i = 0; i < numPlots * 4; i++) 
	{	
		oldPlot = chart->GetPlot(i);
		chart->RemovePlotInstance(oldPlot);
		chart->RemovePlot(i);
		chart->RemovePlotInstance(oldPlot);
		chart->RemovePlot(i);
	}

	// That's not enough (empricially determined).  Again:

	for (int i = 0; i < numPlots * 4; i++) // Why 4?  It works
	{	
		oldPlot = chart->GetPlot(i);
		chart->RemovePlotInstance(oldPlot);
		chart->RemovePlot(i);
		chart->RemovePlotInstance(oldPlot);
		chart->RemovePlot(i);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::displayQueryDVHData()
{
	if (readDVHData(*queryPatient))
	{	
		QString title = "Query DVH: patient " + 
		QString(("%1")).arg(queryPatient->getNumber(), 3, 10, QLatin1Char('0'));
		QByteArray ba = title.toLatin1();
		setupDVHChart(queryDVH, ba.data());
		queryDVHView->Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::displayMatchDVHData()
{
	if (readDVHData(*matchPatient))
	{
		QString title = "Match DVH: patient " + 
		QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'));

		QByteArray ba = title.toLatin1();
		setupDVHChart(matchDVH, ba.data());
		matchDVHView->Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// Hardwired for now.  May be a user-selected option at some point.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setDVHYAxisTicks(vtkChartXY *chart)
{
	static const int numTicks = 21;
	static const double maxY = 110.0;
	static const double tickIncrement = (maxY - 10.0) / (numTicks - 1);
	char tickLabel[15]; // Can't possibly need more than 15 chars :-)

	vtkDoubleArray *tickPositions = vtkDoubleArray::New();
	vtkStringArray *tickLabels = vtkStringArray::New();

	for (int i = 0; i < numTicks; i++) 
	{
		tickPositions->InsertNextValue(i * tickIncrement);
		sprintf_s(tickLabel, "%d", (i * (int)tickIncrement));
		tickLabels->InsertNextValue(tickLabel);
	}

	chart->GetAxis(vtkAxis::LEFT)->SetTickPositions(tickPositions);
	chart->GetAxis(vtkAxis::LEFT)->SetTickLabels(tickLabels);
	chart->GetAxis(vtkAxis::LEFT)->SetRange(0, maxY);
}

