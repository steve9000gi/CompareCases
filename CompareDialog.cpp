////////////////////////////////////////////////////////////////////////////////
//
// CompareDialog.cpp: Qt-based GUI for prostate cancer radiotherapy planning 
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
// primary collaborator: Vorakarn Chanyavanich, Duke Medical Center
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
#include "vtkTextProperty.h"
#include "vtkChartLegend.h"
#include "vtkBrush.h"

#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"
#include "vtkImageActor.h"
#include "vtkPen.h"

#include "Patient.h"
#include "Projector.h"
#include "MainWindow.h"
#include "CaseSpaceDialog.h"
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
	0.9, 0.0, 0.0,			// PTV red     
	0.545, 0.271, 0.075,	// rectum "saddle brown"
	1.0, 0.84, 0.0,			// bladder "golden yellow"
	//0.33, 0.33, 0.4,		// left femoral head blue-gray
	//0.67, 0.67, 0.67		// right femoral head lite gray
	0.5, 0.5, 0.6,		// left femoral head blue-gray
	0.75, 0.75, 0.75		// right femoral head lite gray
};

// rgb values equivalent to those in structureColor, but in the 0-255 range:
static const double color[CompareDialog::numStructures][3] =
{
	230, 0.0, 0.0,			// PTV red     
	139, 69, 19,			// rectum "saddle brown"
	255, 215, 0.0,			// bladder "golden yellow"
	//85, 85, 102,			// left femoral head blue-gray
	//171, 171, 171			// right femoral head lite gray
	127, 127, 153,			// left femoral head blue-gray
	191, 191, 191			// right femoral head lite gray
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
		gantryAngleMenu(NULL),
		gantryAngleActionGroup(NULL),
		queryDVH(NULL),
		matchDVH(NULL),
		queryDVHView(NULL),
		matchDVHView(NULL),
		caseSpaceDialog(NULL),
		queryProjector(NULL),
		matchProjector(NULL),
		queryProjectionRenWin(NULL),
		matchProjectionRenWin(NULL),
		matchHistoryMenu(NULL),
		currMatchHistoryNum(0),
		currHistoryActionNum(0),
		overlaySelectionMenu(NULL),
		matchDVHDataExists(false)
{
	setupUi(this);
	setupVTKUI();
	setupProjectionGantryAngleMenu();
	setupMatchHistoryMenu();
	setupOverlaySelectionMenu();
	createActions();

	viewFemoralHeadsCheckBox->setChecked(true);

	legendGroupBox->setStyleSheet("QGroupBox { color: solid black; }");
	legendGroupBox->setStyleSheet("QGroupBox { border: 2px solid black; }");
}

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CompareDialog::CompareDialog(CaseSpaceDialog * csDlog)
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
		gantryAngleMenu(NULL),
		gantryAngleActionGroup(NULL),
		queryDVH(NULL),
		matchDVH(NULL),
		queryDVHView(NULL),
		matchDVHView(NULL),
		caseSpaceDialog(csDlog),
		queryProjector(NULL),
		matchProjector(NULL),
		queryProjectionRenWin(NULL),
		matchProjectionRenWin(NULL),
		matchHistoryMenu(NULL),
		currMatchHistoryNum(0),
		currHistoryActionNum(0),
		overlaySelectionMenu(NULL),
		matchDVHDataExists(false)
{
	setupUi(this);
	setupVTKUI();
	setupProjectionGantryAngleMenu();
	createActions();

	viewFemoralHeadsCheckBox->setChecked(true);
	flatShadedCheckBox->setChecked(true);

	legendGroupBox->setStyleSheet("QGroupBox { color: solid black; }");
	legendGroupBox->setStyleSheet("QGroupBox { border: 2px solid black; }");
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

	if (queryProjector)
	{
		delete queryProjector;
	}

	if (matchProjector)
	{
		delete matchProjector;
	}

	if (queryProjectionRenWin)
	{
		queryProjectionRenWin->Delete();
	}

	if (matchProjectionRenWin)
	{
		matchProjectionRenWin->Delete();
	}

	// 2do: delete all the match history menu stuff: actions, etc.

	// Re-enable the "Compare Cases" button back on the Case Space dialog when
	// closing this (Compare dialog):
	if (caseSpaceDialog)
	{
		caseSpaceDialog->enableCompareCasesButton(true);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setQuery(Patient *patient)
{
	queryPatient = patient;
	querySelectSpinBox->setValue(queryPatient->getNumber());
	selectQueryCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
	selectQueryProjection();
	displayQueryDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
// NOTE: setMatch(...) is called to set the match for the CompareDialog from the
// CaseSpaceDialog. selectMatch(...) is called as a slot for the match spin box.
// 
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setMatch(Patient *patient)
{
	matchPatient = patient;
	matchSelectSpinBox->setValue(matchPatient->getNumber());
	selectMatchCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
	selectMatchProjection();
	displayMatchDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
// NOTE:  Currently checks for the existence of the directories that are
// expected to hold the CT data, but not for the presence of correct data within
// those directories.  And it doesn't check for the existence of even the 
// directories for projection data, much less for the existence of that data 
// within those directories.
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::dataExistsFor(Patient *patient)
{
	QFile CTDir(patient->getPathToCTData());

	if (!CTDir.exists())
	{
		return false;

	}

// Because there's currently (2011/06/24) still such a paucity of DVH data,
// don't check for its existence.  That way the Compare dialog can still open:
#undef CHECKFORDVHDATA

#ifdef CHECKFORDVHDATA
	QFile DVHDataFile(patient->getPathToDVHData());

	return (DVHDataFile.exists());
#else
	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
// Sets up CT, projection, and DVH displays for query patient with id == param
// patientNumber.
//
// 2do: Need to select the patient from pointer/reference to array[s] of
// patients from way back in the MainWindow.  This method just changes the
// number on the Query Patient passed in from CaseSpaceDialog.  That may not
// matter at this point (2011/03/29) because everything currently being used
// for displaying patient data is accessed based on globally set paths plus
// the patient number, but all the other data values accumulated earlier in
// the Patient object are almost certainly not correct for a Patient with a
// changed number.
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
// NOTE: selectMatch(...) is called as a slot for the match spin box; 
// setMatch(...) is called to set the match for the CompareDialog from the
// CaseSpaceDialog.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatch(int patientNumber)
{
	if (!caseSpaceDialog) return;

	if (patientNumber == queryPatient->getNumber())
	{
		QString warn =
			"Match case must differ from query case." ;
		QMessageBox::warning(this, tr("Query and match must differ"), warn);

		// Restore previous value:
		matchSelectSpinBox->setValue(matchPatient->getNumber());
		
		return;
	}

	Patient *matchPatientCandidate =
		caseSpaceDialog->getDukePatientFrom(patientNumber);

	if (!matchPatientCandidate)
	{
		QString warn =
			"No overlap data for Duke Patient #"
			+ (QString::number(patientNumber))
			+ " exists." ;
		QMessageBox::warning(this, tr("Nonexistent Patient Data"), warn);

		// Restore previous value:
		matchSelectSpinBox->setValue(matchPatient->getNumber());
		
		return;
	}

	if (!caseSpaceDialog->isNewMatchCaseSelectedHere())
	{
		caseSpaceDialog->setMatchCaseFromCompareDialog(patientNumber);
		caseSpaceDialog->setLastMatchCase(matchPatient);
	}

	matchPatient = matchPatientCandidate;

	selectMatchCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
	selectMatchProjection();

	if (!matchHistoryMenu)
	{
		setupMatchHistoryMenu();
		setupOverlaySelectionMenu();
	}
	else
	{
		addMatchHistoryItem();
	}

	displayMatchDVHData();

	if (overlayDVHCheckBox->isChecked())
	{
		displayQueryDVHData();
	}
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
// Concerning image orientation, here's an email to the VTK users group:
//
// Date: Tue, 1 Mar 2011 10:45:24 -0700
// From: David Gobbi <david.gobbi@gmail.com>
// Subject: Re: [vtkusers] Rotate vtkImage with vtkmatrix4x4
// To: Jothy <jothybasu@gmail.com>
// Cc: VTK Mailing List <vtkusers@vtk.org>
// ...
// Hi Jothy,
//
// If you work with sagittal and coronal image you will have to worry about
// image orientation, but with axial images everything is identity.
// 
// For axial images, just make sure that your camera ViewUp is (0, -1, 0)
// and DirectionOfProjection is (0, 0, 1).  The direction of projection for
// the camera is the (FocalPoint - Position) direction.
// 
//  - David
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setSliceAxis(bool val /* = true */)
{
	if (axialRadioButton->isChecked()) 
	{
		sliceSelectionLabel->setText(tr("Z slice:"));
		if (queryPatient) queryPatient->setSliceOrientation(Patient::Z);
		if (matchPatient) matchPatient->setSliceOrientation(Patient::Z);

		queryCTImageFlip->SetFilteredAxis(0); // Flip about x (i.e., no flip)
		matchCTImageFlip->SetFilteredAxis(0); // Flip about x (i.e., no flip)
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
// NOTE:  Currently does not update slider or spin box until animation is
// complete.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::autoplay()
{
	int maxSliceNum = sliceSelectionSlider->maxValue();

	//setSliceAxis();

	//bool hasTracking = sliceSelectionSlider->hasTracking();

	bool updatesEnabled = sliceSelectionSlider->updatesEnabled();

	for (int i = 0; i <= maxSliceNum; i++)
	{
		changeSlice(i);

		this->setWindowTitle(QString::number(i));
		maxSliceLabel->setText(QString::number(i));

		sliceSelectionSlider->setSliderPosition(i);
		sliceSelectionSlider->setValue(i);
		sliceSelectionSlider->update();
		//int val = sliceSelectionSlider->value();
		//int pos = sliceSelectionSlider->sliderPosition();

		sliceSelectionSpinBox->setValue(i);
		sliceSelectionSpinBox->update();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle25()
{
	angle25Action->setChecked(true);
	queryAngle = matchAngle = 25;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle75()
{
	angle75Action->setChecked(true);
	queryAngle = matchAngle = 75;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle130()
{
	angle130Action->setChecked(true);
	queryAngle = matchAngle = 130;
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
void CompareDialog::setGantryAngle230()
{
	angle230Action->setChecked(true);
	queryAngle = matchAngle = 230;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle280()
{
	angle280Action->setChecked(true);
	queryAngle = matchAngle = 280;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setGantryAngle335()
{
	angle335Action->setChecked(true);
	queryAngle = matchAngle = 335;
	selectQueryProjection();
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::changeTransparency(int transp)
{
	queryProjector->setTransparency(transp);
	matchProjector->setTransparency(transp);

	selectQueryProjection();
	selectMatchProjection();

	transparencySlider->setValue(transp);
	transparencySpinBox->setValue(transp);

}

////////////////////////////////////////////////////////////////////////////////
//
// For projections
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::toggleFlatShadedStructures(bool checked)
{
	queryProjector->setFlatShaded(checked);
	selectQueryProjection();
	matchProjector->setFlatShaded(checked);
	selectMatchProjection();
}

////////////////////////////////////////////////////////////////////////////////
//
// For projections
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::toggleOrigin(bool checked)
{
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

	queryProjector->setNoFemoralHeads(!checked);
	matchProjector->setNoFemoralHeads(!checked);

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
	initQueryCTPipeLine();
	initMatchCTPipeLine();

	QString dataDir = caseSpaceDialog->getMainWindow()->getDataDir();
	queryProjector = new Projector(dataDir);
	matchProjector = new Projector(dataDir);

	queryProjectionRenWin = vtkRenderWindow::New();
	matchProjectionRenWin = vtkRenderWindow::New();

	queryProjector->WindowInit(queryProjectionRenWin, queryProjectionWidget);
	matchProjector->WindowInit(matchProjectionRenWin, matchProjectionWidget);

	queryProjector->InitExtrema();
	matchProjector->InitExtrema();

	queryProjector->TextInit();
	matchProjector->TextInit();

	queryProjectionWidget->SetRenderWindow(queryProjectionRenWin);
	matchProjectionWidget->SetRenderWindow(matchProjectionRenWin);

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
	connect(querySelectSpinBox, SIGNAL(valueChanged(int)), this, 
		SLOT(selectQuery(int)));
	connect(matchSelectSpinBox, SIGNAL(valueChanged(int)), this, 
		SLOT(selectMatch(int)));

	connect(axialRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setSliceAxis(bool)));
	connect(sagittalRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setSliceAxis(bool)));
	connect(coronalRadioButton, SIGNAL(toggled(bool)), this, 
		SLOT(setSliceAxis(bool)));

	connect(sliceSelectionSlider, SIGNAL(valueChanged(int)), this, 
		SLOT(changeSlice(int)));
	connect(autoPlayPushButton, SIGNAL(clicked()), this, SLOT(autoplay()));

	connect(viewFemoralHeadsCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleFemoralHeads(bool)));
	connect(flatShadedCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleFlatShadedStructures(bool)));
	connect(originCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleOrigin(bool)));

	connect(transparencySlider, SIGNAL(valueChanged(int)), this, 
		SLOT(changeTransparency(int)));
	connect(transparencySpinBox, SIGNAL(valueChanged(int)), this, 
		SLOT(changeTransparency(int)));

	connect(overlayDVHCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleOverlayDVH(bool)));

	connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::initQueryCTPipeLine()
{
	if (queryDICOMReader)
	{
		queryDICOMReader->Delete();
	}
		
	if (queryCTImageFlip)
	{
		queryCTImageFlip->Delete();
	}
		
	if (queryCTImageViewer)
	{
		queryCTImageViewer->Delete();
	}
		
	queryDICOMReader = vtkDICOMImageReader::New();
	queryCTImageFlip = vtkImageFlip::New();
	queryCTImageFlip->SetInputConnection(queryDICOMReader->GetOutputPort());
	queryCTImageViewer = vtkImageViewer2::New();
	queryCTImageViewer->SetInputConnection(queryCTImageFlip->GetOutputPort());
	queryCTImageWidget->SetRenderWindow(queryCTImageViewer->GetRenderWindow());
	queryCTImageViewer->SetupInteractor(
		queryCTImageWidget->GetRenderWindow()->GetInteractor());

	queryCTImageViewer->SetColorLevel(40);		// Set to increase contrast...
	queryCTImageViewer->SetColorWindow(350);	// ...at Vorakarn's request
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::initMatchCTPipeLine()
{
	if (matchDICOMReader)
	{
		matchDICOMReader->Delete();
	}
		
	if (matchCTImageFlip)
	{
		matchCTImageFlip->Delete();
	}
		
	if (matchCTImageViewer)
	{
		matchCTImageViewer->Delete();
	}
		
	matchDICOMReader = vtkDICOMImageReader::New();
	matchCTImageFlip = vtkImageFlip::New();
	matchCTImageFlip->SetInputConnection(matchDICOMReader->GetOutputPort());
	matchCTImageViewer = vtkImageViewer2::New();
	matchCTImageViewer->SetInputConnection(matchCTImageFlip->GetOutputPort());
	matchCTImageWidget->SetRenderWindow(matchCTImageViewer->GetRenderWindow());
	matchCTImageViewer->SetupInteractor(
		matchCTImageWidget->GetRenderWindow()->GetInteractor());

	matchCTImageViewer->SetColorLevel(40);		// Set to increase contrast...
	matchCTImageViewer->SetColorWindow(350);	// ...at Vorakarn's request
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::accept()
{
	caseSpaceDialog->enableCompareCasesButton(true);
	QDialog::accept();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::reject()
{
	caseSpaceDialog->enableCompareCasesButton(true);
	QDialog::reject();
}

////////////////////////////////////////////////////////////////////////////////
//
// Set the slice for the query patient's CT image display.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectQueryCTSlice(int slice)
{
	initQueryCTPipeLine();

	QFile file(queryPatient->getPathToCTData());

	if (file.exists())
	{
		queryDICOMReader->SetDirectoryName(
			(queryPatient->getPathToCTData()).toAscii());
		queryDICOMReader->Update();
		queryCTImageViewer->SetSlice(slice);
	}
	else
	{
		QString warn =
			"CompareDialog::selectQueryCTSlice(int): Failed to open \""
			+ queryPatient->getPathToCTData() + "\"";
		//QMessageBox::warning(this, tr("File Open Failed"), warn);
		queryDICOMReader = NULL;
	}
	
	queryCTImageViewer->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Set the slice for the match patient's CT image display.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatchCTSlice(int slice)
{
	initMatchCTPipeLine();

	QFile file(matchPatient->getPathToCTData());

	if (file.exists())
	{
		matchDICOMReader->SetDirectoryName(
			(matchPatient->getPathToCTData()).toAscii());
		matchDICOMReader->Update();
		matchCTImageViewer->SetSlice(slice);
	}
	else
	{
		QString warn =
			"CompareDialog::selectMatchCTSlice(int): Failed to open \""
			+ matchPatient->getPathToCTData() + "\"";
		//QMessageBox::warning(this, tr("File Open Failed"), warn);
		matchCTImageViewer->SetColorLevel(0);
		matchCTImageViewer->SetColorWindow(0);

		matchDICOMReader = NULL;
	}

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
	int pNum = queryPatient->getNumber();

    if (!queryProjector->BuildStructuresForPatient(pNum))
	{	
		QString warn =
			"CompareDialog::selectQueryProjection(): Failed to build structures for patient #"
			+ QString::number(pNum);
		QMessageBox::warning(this, tr("File Open Failed"), warn);		
	}

	if (originCheckBox->isChecked())
	{
		queryProjector->AddOrigin(queryProjector->GetRenderer());
	}

	queryProjector->ComputeAvgZ();
	queryProjector->SetProjection(pNum, queryAngle);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatchProjection()
{
	int pNum = matchPatient->getNumber();

    if (!matchProjector->BuildStructuresForPatient(pNum))
	{	
		QString warn =
			"CompareDialog::selectQueryProjection(): Failed to build structures for patient #"
			+ QString::number(pNum);
		QMessageBox::warning(this, tr("File Open Failed"), warn);		
	}

	if (originCheckBox->isChecked())
	{
		matchProjector->AddOrigin(matchProjector->GetRenderer());
	}

	matchProjector->ComputeAvgZ();
	matchProjector->SetProjection(pNum, matchAngle);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupProjectionGantryAngleMenu()
{
	gantryAngleMenu = new QMenu(this);
	angle25Action = 
		gantryAngleMenu->addAction("25 degrees", this, SLOT(setGantryAngle25()));
	angle75Action = 
		gantryAngleMenu->addAction("75 degrees", this, SLOT(setGantryAngle75()));
	angle130Action = 
		gantryAngleMenu->addAction("130 degrees", this, SLOT(setGantryAngle130()));
	angle180Action = 
		gantryAngleMenu->addAction("180 degrees", this, SLOT(setGantryAngle180()));
	angle230Action = 
		gantryAngleMenu->addAction("230 degrees", this, SLOT(setGantryAngle230()));
	angle280Action = 
		gantryAngleMenu->addAction("280 degrees", this, SLOT(setGantryAngle280()));
	angle335Action = 
		gantryAngleMenu->addAction("335 degrees", this, SLOT(setGantryAngle335()));

	angle25Action->setCheckable(true);
	angle75Action->setCheckable(true);
	angle130Action->setCheckable(true);
	angle180Action->setCheckable(true);
	angle230Action->setCheckable(true);
	angle280Action->setCheckable(true);
	angle335Action->setCheckable(true);

	gantryAngleActionGroup = new QActionGroup(this);
	gantryAngleActionGroup->add(angle25Action);
	gantryAngleActionGroup->add(angle75Action);
	gantryAngleActionGroup->add(angle130Action);
	gantryAngleActionGroup->add(angle180Action);
	gantryAngleActionGroup->add(angle230Action);
	gantryAngleActionGroup->add(angle280Action);
	gantryAngleActionGroup->add(angle335Action);

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

	queryDVH->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetColor(1, 1, 1);
	queryDVH->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetColor(1, 1, 1);

	queryDVH->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetColor(1, 1, 1);
	queryDVH->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetColor(1, 1, 1);

	queryDVH->GetTitleProperties()->SetColor(1, 1, 1);

	vtkChartLegend *l = queryDVH->GetLegend();
	l->GetBrush()->SetColorF(0, 0, 0, 0.67);
	l->GetLabelProperties()->SetColor(1, 1, 1);

	if (queryDVHView) queryDVHView->Delete();
	queryDVHView = vtkContextView::New();
	queryDVHView->GetRenderer()->SetBackground(0, 0, 0);

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
	matchDVHView->GetRenderer()->SetBackground(0, 0, 0);

	matchDVH->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetColor(1, 1, 1);
	matchDVH->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetColor(1, 1, 1);

	matchDVH->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetColor(1, 1, 1);
	matchDVH->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetColor(1, 1, 1);

	matchDVH->GetTitleProperties()->SetColor(1, 1, 1);

	vtkChartLegend *l = matchDVH->GetLegend();
	l->GetBrush()->SetColorF(0, 0, 0, 0.67);
	l->GetLabelProperties()->SetColor(1, 1, 1);

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
bool CompareDialog::readQueryDVHData(Patient &patient)
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
		in >> dose[i] >> queryVolumes[PTV][i] >> queryVolumes[rectum][i] 
		   >> queryVolumes[bladder][i]  >> queryVolumes[leftFem][i] 
		   >> queryVolumes[rightFem][i];
		i++;  // Hack: won't work unless incremented as separate statement [?]
	}

	in.flush();
	file.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//
// Assumes that each DVH data file has two lines of (discardable) title and
// column header info, followed by exactly numDVHPoints rows of data (although
// there is a check for premature end of file, overall program behavior is
// unpredictable if it occurs).
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::readMatchDVHData(Patient &patient)
{
	QString path = patient.getPathToDVHData();
	QFile file(path);

	// Silently fail if the data isn't available: at this point (2/06/11) it
	// often isn't, that may continue to be the case indefinitely into the
	// future, and modally announcing the fact is an annoying interruption
	// in workflow.
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
		in >> dose[i] >> matchVolumes[PTV][i] >> matchVolumes[rectum][i] 
		   >> matchVolumes[bladder][i]  >> matchVolumes[leftFem][i] 
		   >> matchVolumes[rightFem][i];
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
void CompareDialog::setupQueryDVHChart(vtkChartXY *chart, char *title)
{
	chart->ClearPlots();

	chart->SetTitle(title);
	chart->GetAxis(vtkAxis::LEFT)->SetTitle("% volume");
	chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("% dose");
	chart->SetShowLegend(true);

	vtkFloatArray *xCoords = vtkFloatArray::New();
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
		yCoords[i]->SetArray(queryVolumes[i], numDVHPoints, 1);

		if (overlayDVHCheckBox->isChecked())
		{
			QString name = "query ";
			name.append(structureName[i]);
			yCoords[i]->SetName(name);
		}
		else
		{
			yCoords[i]->SetName(structureName[i]);
		}

		table->AddColumn(yCoords[i]);
		table->Update();
		line = chart->AddPlot(vtkChart::LINE);
		int numCols = table->GetNumberOfColumns();
		line->SetInput(table, 0, i + 1);
		line->SetColor(color[i][0], color[i][1], color[i][2], 255);
		yCoords[i]->Delete();
	}

	if (overlayDVHCheckBox->isChecked() &&	matchDVHDataExists)
	{
		vtkFloatArray *matchYCoords[numStructures];

		for (int i = 0; i < numStructuresToDisplay; i++)
		{
			matchYCoords[i] = vtkFloatArray::New();
			matchYCoords[i]->SetArray(matchVolumes[i], numDVHPoints, 1);
			QString name = "match ";
			name.append(structureName[i]);
			matchYCoords[i]->SetName(name);
			table->AddColumn(matchYCoords[i]);
			table->Update();
			line = chart->AddPlot(vtkChart::LINE);
			line->SetInput(table, 0, table->GetNumberOfColumns() - 1);
			line->SetColor(color[i][0], color[i][1], color[i][2], 255);
			line->GetPen()->SetLineType(vtkPen::DASH_LINE);
			line->GetPen()->SetWidth(4.0);
			matchYCoords[i]->Delete();
		}
	}

	chart->Update();
	chart->RecalculateBounds();

	xCoords->Delete();
}

////////////////////////////////////////////////////////////////////////////////
//
// Assumes that parameter chart points to a previously instantiated vtkChartXY
// object.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupMatchDVHChart(vtkChartXY *chart, char *title)
{
	chart->ClearPlots();

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
		yCoords[i]->SetArray(matchVolumes[i], numDVHPoints, 1);
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
// Proposed: new and modified functionality for the Compare dialog (based on a
// conversation with Shiva Das and Joseph Lo on 8/30/11):
//
// 1. An item is deleted from the Match History menu by control-clicking on it.
// 
// 2. New DVH display behavior:
//
// What has been "Query DVH" (top right panel in Compare dialog) becomes 
// "Match candidate DVH overlays" (so therefore what was "Match" in the Compare
// dialog becomes "Match candidate"). In the larger scheme the Query case can't
// be expected to *have* a DVH prior to the completion of his treatment plan.
//
// When the Compare dialog first appears, the Match candidate's DVH appears in 
// both DVH panels.  Then, as each additional Match candidates is selected for
// viewing its identifying institution/case number is displayed as a new menu
// item in the history menu, ordered with the most recently selected at the top.
//
// This selection may occur by any of three mechanisms: 1) by picking in the
// Case Space dialog; 2) by typing or spinning in a value using the
// matchSelectSpinBox; or 3) by selecting from the Match History menu.
//
// As each match candidate selection is made, its DVH is displayed in the lower
// DVH panel.  The upper DVH panel still shows the initial Match candidate's
// DVH until the user clicks on the Overlay button, at which point the current
// Match candidate's DVH is overlaid on what's currently displayed in the
// Overlay DVH panel.
//
// Maximum number of overlays option #1: max 5:
//
// There is also an Overlay History menu that displays the sequence of Match
// candidate DVH's whose DVH's have been added to the Overlay panel, also 
// ordered with the most recent first.  
//
// For both of these menus, control-clicking on an item removes it.  If an item
// is removed from the History menu, it's also removed from the Overlay History
// menu.  If an item is removed from the Overlay History menu, however, it's not
// removed from the Match History menu.
//
// There is a maximum of 5 overlaid DVH's that will be displayed simultaneously.
// There are two options for handling a user attempt to add a 6th: 1) the
// Overlay button is disabled until the user removes at least one item from
// the Overlay menu; or 2) the bottom item on the Overlay menu is bumped and
// the remaining items all move down to make room for the new selection.  The
// Overlay panel is updated accordingly.
//
// Maximum number of overlays option #2: max 2:
//
// Either one or two DVH's are displayed in the Overlay Panel. One is always the
// DVH associated with the Match candidate currently displayed in the lower half
// of the Compare dialog.  The other is selected using an Overlay menu,
// identical to the History menu, except that selecting an item determines the
// second DVH displayed in the DVH Overlay panel.
// 
// There are two check boxes, one of which is disabled if there are fewer than
// two Match candidates (there will always be at least one).  If one or both is
// checked, then the DVH is displayed for the Match candidate corresponding to
// the label for that check box.  The labels for the two check boxes are set to 
// match the most recently selected items in the Match History and Overlay
// menus.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::displayQueryDVHData()
{
	if (readQueryDVHData(*queryPatient))
	{	
		QString title;

		if ((overlayDVHCheckBox->isChecked()) && matchDVHDataExists)
		{
			title = "Query DVH: query patient " + 
			QString(("%1")).arg(queryPatient->getNumber(), 3, 10, QLatin1Char('0'))
			+ " + match patient " +
			QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'))
			+ " overlay";
		}
		else
		{
			title = "Query DVH: patient " + 
			QString(("%1")).arg(queryPatient->getNumber(), 3, 10, QLatin1Char('0'));
		}

		QByteArray ba = title.toLatin1();
		setupQueryDVHChart(queryDVH, ba.data());
	}
	else
	{
		cout << "Failed to read DVH data for patient #"
			 << queryPatient->getNumber() << endl;

		initQueryDVHObjects();
	}
	
	queryDVHView->Render();

}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::displayMatchDVHData()
{
	if (matchDVHDataExists = readMatchDVHData(*matchPatient))
	{
		QString title = "Match DVH: patient " + 
		QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'));

		QByteArray ba = title.toLatin1();
		setupMatchDVHChart(matchDVH, ba.data());
		matchDVHView->Render();
	}
	else
	{
		cout << "Failed to read DVH data for patient #"
			 << matchPatient->getNumber() << endl;

		initMatchDVHObjects();

		if (overlayDVHCheckBox->isChecked())
		{
			displayQueryDVHData();
		}

		matchDVHView->Render();
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// 
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::toggleOverlayDVH(bool checked)
{
	QString title;

	if (overlayDVHCheckBox->isChecked())
	{
		title = "Query DVH: query patient " + 
		QString(("%1")).arg(queryPatient->getNumber(), 3, 10, QLatin1Char('0'))
		+ " + match patient " +
		QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'))
		+ " overlay";
	}
	else
	{
		title = "Query DVH: patient " + 
		QString(("%1")).arg(queryPatient->getNumber(), 3, 10, QLatin1Char('0'));
	}

	QByteArray ba = title.toLatin1();
	setupQueryDVHChart(queryDVH, ba.data());
	queryDVHView->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Using the dangerous (because of the cast) "sender" approach.  
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::itemTriggered(bool checked)
{
	QAction *action = (QAction *)sender();
	emit selectHistoryMatch(action->text());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectHistoryMatch(QString text)
{
	caseSpaceDialog->setIsNewMatchCaseSelectedHere(false);

	QByteArray a = text.toLatin1();
	//cout << "selectHistoryMatch(" << a.data() << ")" << endl;

	QString nAsText = text.section('#', 1);
	int num = nAsText.toInt();
	matchSelectSpinBox->setValue(num);

	removeRedundantHistoryItem(text);
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

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupMatchHistoryMenu()
{
	matchHistoryMenu = new QMenu(this);
	historyPushButton->setMenu(matchHistoryMenu);

	currMatchHistoryNum = 0;

	for (int i = 0; i < maxMatchHistoryNum; i++)
	{
		matchHistoryAction[i] = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupOverlaySelectionMenu()
{
	overlaySelectionMenu = new QMenu(this);
	overlaySelectionPushButton->setMenu(overlaySelectionMenu);

	overlaySelectionIndex = -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Don't add if the current candidate is already in the list.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::addMatchHistoryItem()
{
	if ((!matchHistoryMenu) || (!caseSpaceDialog)) return;

	// If the new match (an old history item?) is in the history, remove it:
	int oldHistoryPatientNum = caseSpaceDialog->getMatchCase()->getNumber();

	QString oldHistoryItemText = /* matchPatient->getInstitution() + */
		"Duke Patient #" + QString::number(oldHistoryPatientNum);

	removeRedundantHistoryItem(oldHistoryItemText);

	int newHistoryPatientNum = caseSpaceDialog->getLastMatchCase()->getNumber();

	QString newHistoryItemText = /* matchPatient->getInstitution() + */
		"Duke Patient #" + QString::number(newHistoryPatientNum);

	currMatchHistoryNum = matchHistoryMenu->actions().size();

	for (int i = 0; i < currMatchHistoryNum; i++)
	{
		QAction *currAction = matchHistoryMenu->actions().at(i);
		QString currText = currAction->text();

		if (!newHistoryItemText.compare(currText))	// If they are the same...
		{
			return;	 // ...don't add (because it's already there)
		}
	}

	matchHistoryAction[currHistoryActionNum] =
		new QAction(newHistoryItemText, this);
	connect(matchHistoryAction[currHistoryActionNum], SIGNAL(triggered(bool)),
		    this, SLOT(itemTriggered(bool)));

	QAction *firstAction = NULL;

	if (!matchHistoryMenu->isEmpty())
	{
		firstAction = matchHistoryMenu->actions().first();
	}

	matchHistoryMenu->insertAction(firstAction,
		matchHistoryAction[currHistoryActionNum++]);
	currMatchHistoryNum = matchHistoryMenu->actions().size();
}

////////////////////////////////////////////////////////////////////////////////
//
// Look through the items in the match history menu.  For each that has the same
// text field as the text input parameter.  Return true if any actions were
// removed from the menu, else return false.
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::removeRedundantHistoryItem(QString text)
{
	bool wasActionRemoved = false;
	currMatchHistoryNum = matchHistoryMenu->actions().size();

	for (int i = 0; i < currMatchHistoryNum; i++)
	{
		QAction *currAction = matchHistoryMenu->actions().at(i);
		QString currText = currAction->text();

		if (!text.compare(currText))
		{
			matchHistoryMenu->removeAction(currAction);
			currMatchHistoryNum = matchHistoryMenu->actions().size();

			wasActionRemoved = true;
		}
	}

	return wasActionRemoved;
}

