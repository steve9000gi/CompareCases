////////////////////////////////////////////////////////////////////////////////
//
// CompareDialog.cpp: Qt-based GUI for prostate cancer radiation therapy  
// planning tool. This window displays six panels: the left-top two display data
// for the "query" patient, i.e., the current patient for whom treatment is 
// being planned. The lower three display data for the "match" patient,
// that is, data associated with a prior case who has completed a successful
// program of radiation therapy and has been selected by an algorithm external
// to this code as one of the cases in the database most like the query case.
//
// Each row of panels consists of three widgets, in order left to right: 1) a CT 
// (Computerized Tomography) image data display, including controls for axis
// selection, slice selection, and auto-play (image animation along the current
// slice axis); 2) a projection display, which shows an image projection of the
// overlap or degree of occlusion between the various anatomical structures in
// the treatment area from any of the seven (user-selectable) standard gantry
// angles ("standard" with respect to the system used at Duke Medical Center,
// namely the Eclipse treatment planning system from Palo Alto-based Varian
// Medical Systems); and 3) Dose Volume Histogram (DVH) data as a cartesian
// graph for the selected anatomical structures (femoral head data display
// may be toggled on or off for projection and DVH displays). The match DVH 
// panel displays DVH data for the match patient.  The overlay DVH panel 
// departs from the general approach of top row for query, bottom row for 
// match, by showing the match DVH with an optional overlay of DVH data for
// one of the previously selected match cases, selected from an overlay menu.
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
// primary collaborators: Joseph Lo, Shiva Das, and Vorakarn Chanyavanich,
//						  Duke Medical Center
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

// rgb values, 0.0-1.0 range (used by Projector class):
static const double structureColor[CompareDialog::numStructures][3] =
{
	0.9, 0.0, 0.0,			// PTV red     
	0.545, 0.271, 0.075,	// rectum "saddle brown"
	1.0, 0.84, 0.0,			// bladder "golden yellow"
	//0.33, 0.33, 0.4,		// left femoral head blue-gray
	//0.67, 0.67, 0.67		// right femoral head lite gray
	0.5, 0.5, 0.6,			// left femoral head blue-gray
	0.75, 0.75, 0.75		// right femoral head lite gray
};

// rgb values equivalent to those in structureColor, but in the 0-255 range:
static const double color[CompareDialog::numStructures][3] =
{
	230, 0, 0,				// PTV red     
	139, 69, 19,			// rectum "saddle brown"
	255, 215, 0,			// bladder "golden yellow"
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
		overlayPatient(NULL),
		queryDICOMReader(NULL),
		matchDICOMReader(NULL),
		queryCTImageFlip(NULL),
		matchCTImageFlip(NULL),
		queryCTImageViewer(NULL),
		matchCTImageViewer(NULL),
		gantryAngleMenu(NULL),
		gantryAngleActionGroup(NULL),
		overlayDVH(NULL),
		matchDVH(NULL),
		overlayDVHView(NULL),
		matchDVHView(NULL),
		caseSpaceDialog(NULL),
		queryProjector(NULL),
		matchProjector(NULL),
		queryProjectionRenWin(NULL),
		matchProjectionRenWin(NULL),
		matchHistoryMenu(NULL),
		overlaySelectionMenu(NULL),
		matchDVHDataExists(false),
		matchXYDataExists(true)
{
	setupUi(this);
	setupVTKUI();
	initScrollArea();
	setupProjectionGantryAngleMenu();
	setupMatchHistoryMenu();
	setupOverlaySelectionMenu();
	createActions();

	viewStructureCheckBox[PTV] = viewPTVCheckBox;
	viewStructureCheckBox[rectum] = viewRectumCheckBox;
	viewStructureCheckBox[bladder] = viewBladderCheckBox;
	viewStructureCheckBox[leftFem] = viewFemoralHeadsCheckBox;
	viewStructureCheckBox[rightFem] = viewFemoralHeadsCheckBox;

	setViewCheckboxColors();

	viewRectumCheckBox->setChecked(true);
	viewBladderCheckBox->setChecked(true);
	viewPTVCheckBox->setChecked(true);
	viewFemoralHeadsCheckBox->setChecked(true);
	flatShadedCheckBox->setChecked(true);
	historyPushButton->setEnabled(false);
	removeCurrentMatchPushButton->setEnabled(false);
	overlaySelectionPushButton->setEnabled(false);
	removeSelectedOverlayPushButton->setEnabled(false);
	overlayDVHCheckBox->setEnabled(false);
	querySelectSpinBox->setHidden(true);
	queryInstitutionPushButton->setHidden(true);
	originCheckBox->setHidden(true);

	sliceSelectionSpinBox->setKeyboardTracking(false);
	querySelectSpinBox->setKeyboardTracking(false);
	matchSelectSpinBox->setKeyboardTracking(false);
	transparencySpinBox->setKeyboardTracking(false);
}

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CompareDialog::CompareDialog(CaseSpaceDialog * csDlog)
	:	queryAngle(Patient::defaultGantryAngle),
		matchAngle(Patient::defaultGantryAngle),
		queryPatient(NULL),
		matchPatient(NULL),
		overlayPatient(NULL),
		queryDICOMReader(NULL),
		matchDICOMReader(NULL),
		queryCTImageFlip(NULL),
		matchCTImageFlip(NULL),
		queryCTImageViewer(NULL),
		matchCTImageViewer(NULL),
		gantryAngleMenu(NULL),
		gantryAngleActionGroup(NULL),
		overlayDVH(NULL),
		matchDVH(NULL),
		overlayDVHView(NULL),
		matchDVHView(NULL),
		caseSpaceDialog(csDlog),
		queryProjector(NULL),
		matchProjector(NULL),
		queryProjectionRenWin(NULL),
		matchProjectionRenWin(NULL),
		matchHistoryMenu(NULL),
		overlaySelectionMenu(NULL),
		matchDVHDataExists(false),
		matchXYDataExists(true)
{
	setupUi(this);
	setupVTKUI();
	initScrollArea();
	setupProjectionGantryAngleMenu();
	createActions();

	viewStructureCheckBox[PTV] = viewPTVCheckBox;
	viewStructureCheckBox[rectum] = viewRectumCheckBox;
	viewStructureCheckBox[bladder] = viewBladderCheckBox;
	viewStructureCheckBox[leftFem] = viewFemoralHeadsCheckBox;
	viewStructureCheckBox[rightFem] = viewFemoralHeadsCheckBox;

	setViewCheckboxColors();

	viewRectumCheckBox->setChecked(true);
	viewBladderCheckBox->setChecked(true);
	viewPTVCheckBox->setChecked(true);
	viewFemoralHeadsCheckBox->setChecked(true);
	flatShadedCheckBox->setChecked(true);
	historyPushButton->setEnabled(false);
	removeCurrentMatchPushButton->setEnabled(false);
	overlaySelectionPushButton->setEnabled(false);
	removeSelectedOverlayPushButton->setEnabled(false);
	overlayDVHCheckBox->setEnabled(false);
	querySelectSpinBox->setHidden(true);
	queryInstitutionPushButton->setHidden(true);
	originCheckBox->setHidden(true);
	sliceSelectionSpinBox->setHidden(true);
	maxSliceLabel->setHidden(true);
	dvhLabel->setHidden(true);
	flatShadedCheckBox->setHidden(true);

	sliceSelectionSpinBox->setKeyboardTracking(false);
	querySelectSpinBox->setKeyboardTracking(false);
	matchSelectSpinBox->setKeyboardTracking(false);
	transparencySpinBox->setKeyboardTracking(false);
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
		
	if (overlayDVH)
	{
		overlayDVH->Delete();
	}
		
	if (matchDVH)
	{
		matchDVH->Delete();
	}
	
	if (overlayDVHView)
	{
		overlayDVHView->Delete();
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
void CompareDialog::initScrollArea()
{
/*	QScrollArea *scrollArea = new QScrollArea(this);
	scrollArea->setBackgroundRole(QPalette::Dark);
	QWidget *viewport = new QWidget(this);
	QHBoxLayout *dialog_layout = new QHBoxLayout(this);
    dialog_layout->addWidget(scroll); // add scroll to the QDialog's layout
    setLayout(dialog_layout);
	viewport->setLayout(this->layout());
	scrollArea->setWidget(viewport);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
*/
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
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setQuery(Patient *patient)
{
	queryPatient = patient;
	QString text = "patient #"
				 + QString::number(queryPatient->getNumber());
	queryCaseNumberLabel->setText(text);
	querySelectSpinBox->setValue(queryPatient->getNumber());
	selectQueryCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
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
bool CompareDialog::CTDataExistsFor(Patient *patient)
{
	QFile CTDir(patient->getPathToCTData());

	QString path = patient->getPathToCTData();

	const char *s = path.ascii();

	//cout << "CompareDialog::CTDataExistsFor(patient #" << patient->getNumber() << "): path: " << s << endl;

	if (!CTDir.exists())
	{
		return false;
	}

#define CHECKFORDVHDATA

#ifdef CHECKFORDVHDATA
	QFile DVHDataFile(patient->getPathToDVHData());

	return (DVHDataFile.exists());
#else
	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
// NOTE:  Currently checks for the existence of the file that is expected to
// hold the DVH data, but not for the presence of correct data within that file.
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::DVHDataExistsFor(Patient *patient)
{
#define CHECKFORDVHDATA

#ifdef CHECKFORDVHDATA
	QFile DVHDataFile(patient->getPathToDVHData());

	QString path = patient->getPathToCTData();

	//const char *s = path.ascii();
	//cout << "CompareDialog::DVHDataExistsFor(patient #" << patient->getNumber() << "): path: " << s << endl;


	return (DVHDataFile.exists());
#else
	return true;
#endif
}

////////////////////////////////////////////////////////////////////////////////
// 
// Make sure that queryPatient has been initialized before you call this method.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::readIsocenters()
{
	QString isoCenterFilePath = queryPatient->getDataDir() + "/isocenters.txt";

	QFile file(isoCenterFilePath);

	if (!file.open(QIODevice::ReadOnly))
	{
		QString warn = 
			"CompareDialog::readIsocenters(): Failed to open \""
			+ isoCenterFilePath + "\"";

		QMessageBox::warning(this, tr("File Open Failed"), warn);
		return;
	}

	QTextStream in(&file);
	
	in.readLine(); // skip first row: column headers;

	int i = 0;

	while (!in.atEnd())
	{
		in.readLine();
		i++;
	}

	numIsocenters = i;

	in.flush();

	if (!in.seek(0)) // back to the top
	{
		QString warn = 
			"CompareDialog::readIsocenters(): seek(0) failed for file \""
			+ isoCenterFilePath + "\"";
		QMessageBox::warning(this, 
			tr("failure attempting to return to file start"), warn);
		return; 
	}

	in.readLine(); // skip first row

	isocenterIndex = new int[numIsocenters];

	for (i = 0; i < numIsocenters; i++)
	{
		in >> isocenterIndex[i]
		   >> isocenter[i][0]
		   >> isocenter[i][1]
		   >> isocenter[i][2];
	}
/*
	for (i = 0; i < numIsocenters; i++)
	{
		cout << isocenterIndex[i] << ": "
		   << isocenter[i][0] << ", "
		   << isocenter[i][1] << ", "
		   << isocenter[i][2] << ", " << endl;
	}
*/
	file.close();
}

////////////////////////////////////////////////////////////////////////////////
//
// Sets up CT, projection, and DVH displays for query patient with id == param
// patientNumber.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectQuery(int patientNumber)
{
	if (!caseSpaceDialog) return;

	queryPatient = caseSpaceDialog->getDukePatientFrom(patientNumber);
	queryPatient->setNumber(patientNumber);
	selectQueryCTSlice(sliceSelectionSlider->value());
	setSliceAxis();
	selectQueryProjection(true);
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

	matchXYDataExists = matchXYDataExists && (matchPatientCandidate != NULL);

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
	selectMatchProjection(true);

	if (!matchHistoryMenu)
	{
		setupMatchHistoryMenu();
	}
	else if (matchXYDataExists)
	{
		addMatchHistoryItem();
	}

	if (!overlaySelectionMenu)
	{
		setupOverlaySelectionMenu();
	}
	else if (matchXYDataExists)
	{
		addOverlaySelectionItem();
	}
	else matchXYDataExists = true;

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
	int orientation; 

	if (axialRadioButton->isChecked()) 
	{
		sliceSelectionLabel->setText(tr("Z slice:"));
		if (queryPatient) queryPatient->setSliceOrientation(Patient::Z);
		if (matchPatient) matchPatient->setSliceOrientation(Patient::Z);

		queryCTImageFlip->SetFilteredAxis(0); // Flip about x (i.e., no flip)
		matchCTImageFlip->SetFilteredAxis(0); // Flip about x (i.e., no flip)
		queryCTImageViewer->SetSliceOrientationToXY();
		matchCTImageViewer->SetSliceOrientationToXY();
		orientation = 2;
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
		orientation = 0;
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
		orientation = 1;
	}

	int qSliceMin = queryCTImageViewer->GetSliceMin();
	int qSliceMax = queryCTImageViewer->GetSliceMax();
	int mSliceMin = matchCTImageViewer->GetSliceMin();
	int mSliceMax = matchCTImageViewer->GetSliceMax();

	sliceSelectionSlider->setMaxValue(max(qSliceMax, mSliceMax));
	sliceSelectionSpinBox->setMaxValue(max(qSliceMax, mSliceMax));
	sliceSelectionSlider->setMinValue(min(qSliceMin, mSliceMin));
	sliceSelectionSpinBox->setMinValue(min(qSliceMin, mSliceMin));
	// Here's where we'll set the two CT displays to their isocenters bydefault 
	sliceSelectionSlider->setValue(max(qSliceMax, mSliceMax) / 2); // ~ middle
	maxSliceLabel->setText(QString::number(max(qSliceMax, mSliceMax)));

	float *qImagePosition = queryDICOMReader->GetImagePositionPatient();
	float *mImagePosition = matchDICOMReader->GetImagePositionPatient();

	//cout << "qImagePosition: " << qImagePosition[0] << ", " << qImagePosition[1] << ", " << qImagePosition[2] << endl;
	//cout << "mImagePosition: " << mImagePosition[0] << ", " << mImagePosition[1] << ", " << mImagePosition[2] << endl;

#if USE_PROJECTOR_SLICE_PLANE
	queryProjector->PositionSlicePlane(orientation,
		queryCTImageViewer->GetSlice(), qSliceMax - qSliceMin);
	matchProjector->PositionSlicePlane(orientation,
		matchCTImageViewer->GetSlice(), mSliceMax - mSliceMin);
#endif

/*	queryProjector->PositionSlicePlane(orientation,
		queryCTImageViewer->GetSlice(), queryDICOMReader->GetDataSpacing());
	matchProjector->PositionSlicePlane(orientation,
		matchCTImageViewer->GetSlice(), matchDICOMReader->GetDataSpacing());
*/
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::changeSlice(int slice)
{
	queryCTImageViewer->SetSlice(slice);
	matchCTImageViewer->SetSlice(slice);

#if USE_PROJECTOR_SLICE_PLANE
	if (queryPatient && queryDICOMReader)
	{
		queryProjector->PositionSlicePlane(queryPatient->getSliceOrientation(),
			queryCTImageViewer->GetSlice(), 
			queryCTImageViewer->GetSliceMax() - queryCTImageViewer->GetSliceMin());
	}

	if (matchPatient && matchDICOMReader)
	{
		matchProjector->PositionSlicePlane(matchPatient->getSliceOrientation(),
			matchCTImageViewer->GetSlice(), 
			matchCTImageViewer->GetSliceMax() - matchCTImageViewer->GetSliceMin());
	}
#endif

	sliceSelectionSlider->setSliderPosition(slice);
	sliceSelectionSlider->setValue(slice);
	sliceSelectionSlider->update();
	sliceSelectionSpinBox->setValue(slice);
	sliceSelectionSpinBox->update();

	//float *qImagePosition = queryDICOMReader->GetImagePositionPatient();
	//float *mImagePosition = matchDICOMReader->GetImagePositionPatient();

	//cout << "slice " << slice << " qImagePosition: " << qImagePosition[0] << ", " << qImagePosition[1] << ", " << qImagePosition[2] << endl;
	//cout << "slice " << slice << " mImagePosition: " << mImagePosition[0] << ", " << mImagePosition[1] << ", " << mImagePosition[2] << endl;

	//extractQueryDICOMData();
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
// Slot for transparencySlider, to match argless valueChanged(int) signal.
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
// Slot for transparencySpinBox, to match argless editingFinished() signal.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::changeTransparency()
{
	changeTransparency(transparencySpinBox->value());
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
// Force update of structure projections and DVH displays.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::toggleViewStructure(bool checked)
{
	queryProjector->setDrawStructure(Projector::ekBladder,
		viewBladderCheckBox->isChecked());
	queryProjector->setDrawStructure(Projector::ekRectum,
		viewRectumCheckBox->isChecked());
	queryProjector->setDrawStructure(Projector::ekPTV,
		viewPTVCheckBox->isChecked());
	queryProjector->setDrawStructure(Projector::ekLtFem,
		viewFemoralHeadsCheckBox->isChecked());
	queryProjector->setDrawStructure(Projector::ekRtFem,
		viewFemoralHeadsCheckBox->isChecked());

	matchProjector->setDrawStructure(Projector::ekBladder,
		viewBladderCheckBox->isChecked());
	matchProjector->setDrawStructure(Projector::ekRectum,
		viewRectumCheckBox->isChecked());
	matchProjector->setDrawStructure(Projector::ekPTV,
		viewPTVCheckBox->isChecked());
	matchProjector->setDrawStructure(Projector::ekLtFem,
		viewFemoralHeadsCheckBox->isChecked());
	matchProjector->setDrawStructure(Projector::ekRtFem,
		viewFemoralHeadsCheckBox->isChecked());

	selectQueryProjection();
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

	originCheckBox->setChecked(false);

	queryProjector->WindowInit(queryProjectionRenWin, queryProjectionWidget);
	matchProjector->WindowInit(matchProjectionRenWin, matchProjectionWidget);

	queryProjector->InitExtrema();
	matchProjector->InitExtrema();

	queryProjector->InitLegend();

	queryProjector->TextInit();
	matchProjector->TextInit();

#if USE_PROJECTOR_SLICE_PLANE
	queryProjector->InitSlicePlane();
	matchProjector->InitSlicePlane();
#endif

	queryProjectionWidget->SetRenderWindow(queryProjectionRenWin);
	matchProjectionWidget->SetRenderWindow(matchProjectionRenWin);

	initOverlayDVHObjects();
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
	connect(sliceSelectionSpinBox, SIGNAL(valueChanged(int)), this, 
		SLOT(changeSlice(int)));
	connect(autoPlayPushButton, SIGNAL(clicked()), this, SLOT(autoplay()));

	connect(viewBladderCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleViewStructure(bool)));
	connect(viewRectumCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleViewStructure(bool)));
	connect(viewPTVCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleViewStructure(bool)));
	connect(viewFemoralHeadsCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleViewStructure(bool)));
	
	connect(flatShadedCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleFlatShadedStructures(bool)));
	connect(originCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleOrigin(bool)));

	connect(transparencySlider, SIGNAL(valueChanged(int)), this, 
		SLOT(changeTransparency(int)));
	connect(transparencySpinBox, SIGNAL(editingFinished()), this, 
		SLOT(changeTransparency()));

	connect(removeCurrentMatchPushButton, SIGNAL(released()), this,
		SLOT(removeCurrentMatch()));

	connect(overlayDVHCheckBox, SIGNAL(clicked(bool)), this, 
		SLOT(toggleOverlayDVH(bool)));
	connect(removeSelectedOverlayPushButton, SIGNAL(released()), this,
		SLOT(removeSelectedOverlayMenuItem()));

	//connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	//connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}

////////////////////////////////////////////////////////////////////////////////
//
// Call *after* the viewStructureCheckBox array has been set up
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setViewCheckboxColors()
{
	for (int s = PTV; s < rightFem; s++)
	{
		QColor clr((int)(color[s][0]), (int)(color[s][1]), (int)(color[s][2]));
		viewStructureCheckBox[s]->setPaletteForegroundColor(clr);
	}

	QColor shadowClr((int)(color[bladder][0]) / 2, (int)(color[bladder][1]) / 2,
					 (int)(color[bladder][2]) / 2);
	viewBladderCheckboxShadowLabel->setPaletteForegroundColor(shadowClr);
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

	queryCTImageViewer->GetRenderWindow()->Render();
	
	//extractQueryDICOMData();
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
void CompareDialog::extractQueryDICOMData()
{
	cout << "|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-" << endl;
	int dataExtent[6];
	double dataSpacing[3];
	double dataOrigin[3];
	queryDICOMReader->GetDataExtent(dataExtent);
	queryDICOMReader->GetDataSpacing(dataSpacing);
	queryDICOMReader->GetDataOrigin(dataOrigin);
	float *qImagePosition = queryDICOMReader->GetImagePositionPatient();
	double *pixelSpacing = queryDICOMReader->GetPixelSpacing();
	int w = queryDICOMReader->GetWidth();
	int h = queryDICOMReader->GetHeight();

	cout << "CT slice #" << queryCTImageViewer->GetSlice() << ": " << endl;
	cout << "orientation: " << queryCTImageViewer->GetSliceOrientation() << endl;
	cout << "extent: " << dataExtent[0] << ", "
					   << dataExtent[1] << ", "
					   << dataExtent[2] << ", "
					   << dataExtent[3] << ", "
					   << dataExtent[4] << ", "
					   << dataExtent[5] << endl;
	cout << "spacing: " << dataSpacing[0] << ", "
					    << dataSpacing[1] << ", "
					    << dataSpacing[2] << endl;
	cout << "data origin: " << dataOrigin[0] << ", "
					    << dataOrigin[1] << ", "
					    << dataOrigin[2] << endl;
	cout << "qImagePosition: " << qImagePosition[0] << ", "
						<< qImagePosition[1] << ", "
						<< qImagePosition[2] << endl;
	cout << "pixelSpacing: " << pixelSpacing[0] << ", "
							 << pixelSpacing[1] << ", "
							 << pixelSpacing[2] << endl;
	cout << "width, height: " << w << ", " << h << endl;
	cout << "|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-" << endl;
	

	const char *xferSyntaxUID = queryDICOMReader->GetTransferSyntaxUID();
	const char *patientName = queryDICOMReader->GetPatientName();
	const char *studyUID = queryDICOMReader->GetStudyUID();
	const char *studyID = queryDICOMReader->GetStudyID();
	const char *fileX = queryDICOMReader->GetFileExtensions();
	float rescaleSlope = queryDICOMReader->GetRescaleSlope();
	float rescaleOffset = queryDICOMReader->GetRescaleOffset();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectQueryProjection(bool newQuery /* = false */)
{
	int pNum = queryPatient->getNumber();

    if (!queryProjector->BuildStructuresForPatient(pNum, newQuery))
	{	
		QString warn =
			"CompareDialog::selectQueryProjection(): Failed to build structures for patient #"
			+ QString::number(pNum);
		//QMessageBox::warning(this, tr("File Open Failed"), warn);		
	}

	if (originCheckBox->isChecked())
	{
		queryProjector->AddOrigin(queryProjector->GetRenderer());
	}

	// Don't compute a new avgZ value if all you're doing is showing or hiding 
	// structures, because then the collection of visible structures is likely
	// to move around:
	if (newQuery)
	{
		queryProjector->ComputeAverages();
	}

	queryProjector->PositionSlicePlane(
		queryCTImageViewer->GetSliceOrientation(),
		queryCTImageViewer->GetSlice(),
		queryCTImageViewer->GetSliceMax() - queryCTImageViewer->GetSliceMin()); // TEMP
//		queryDICOMReader->GetDataSpacing());

	queryProjector->SetProjection(pNum, queryAngle);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectMatchProjection(bool newMatch /* = false */ )
{
	int pNum = matchPatient->getNumber();

	if (newMatch)
	{
		matchProjector->InitExtrema();
	}

    if (!matchProjector->BuildStructuresForPatient(pNum, newMatch))
	{	
		QString warn =
			"CompareDialog::selectQueryProjection(): Failed to build structures for patient #"
			+ QString::number(pNum);
		//QMessageBox::warning(this, tr("File Open Failed"), warn);		
	}

	if (originCheckBox->isChecked())
	{
		matchProjector->AddOrigin(matchProjector->GetRenderer());
	}
	
	// Don't compute a new avgZ value if all you're doing is showing or hiding 
	// structures, because then the collection of visible structures is likely
	// to move around:
	if (newMatch)
	{
		matchProjector->ComputeAverages();
	}

	matchProjector->PositionSlicePlane(
		matchCTImageViewer->GetSliceOrientation(),
		matchCTImageViewer->GetSlice(),
		matchCTImageViewer->GetSliceMax() - matchCTImageViewer->GetSliceMin()); // TEMP 
		//matchDICOMReader->GetDataSpacing());
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
void CompareDialog::initOverlayDVHObjects()
{
	if (overlayDVH) overlayDVH->Delete();
	overlayDVH = vtkChartXY::New();
	setDVHYAxisTicks(overlayDVH);

	overlayDVH->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetColor(1, 1, 1);
	overlayDVH->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetColor(1, 1, 1);

	overlayDVH->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetColor(1, 1, 1);
	overlayDVH->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetColor(1, 1, 1);

	overlayDVH->GetTitleProperties()->SetColor(1, 1, 1);

	vtkChartLegend *l = overlayDVH->GetLegend();
	l->GetBrush()->SetColorF(0, 0, 0, 0.67);
	l->GetLabelProperties()->SetColor(1, 1, 1);

	if (overlayDVHView) overlayDVHView->Delete();
	overlayDVHView = vtkContextView::New();
	overlayDVHView->GetRenderer()->SetBackground(0, 0, 0);

	overlayDVHView->GetScene()->AddItem(overlayDVH);
	overlayDVHView->SetInteractor(overlayDVHWidget->GetInteractor());
	overlayDVHWidget->SetRenderWindow(overlayDVHView->GetRenderWindow());
	overlayDVHView->GetRenderWindow()->SetSize(xDVHWidget, yDVHWidget);
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
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::initVolumesArray(
	float volumes[numStructures][maxNumDVHPoints])
{

	for (int i = 0; i < numStructures; i++)
	{
		for (int j = 0; j < maxNumDVHPoints; j++)
		{
			volumes[i][j] = 0.0;
		}
	}
}

///readDVHData///////////////////////////////////////////////////////////////////
//
// File format (as of 2011-09-20):
//	1. bladder:
//		30 lines to skip over;
//		variable number of lines of numeric data for bladder; each line:
//			1) relative dose (%) in 0.1% increments: x axis;
//			2) dose (cGy == centi-Grays): we're not using this here;
//			3) ratio of total structure volume (%): y-axis;
//		last line of data followed by blank line (carriage return only).
//  2, 3, 4, 5. rectum, left femoral head, right femoral head, PTV:
//		18 lines to skip over (not including blank line that terminated data for
//			previous structure)
//		data as above
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::readDVHData(Patient &patient,
	float volumes[numStructures][maxNumDVHPoints], int &numPoints)
{
	QString path = patient.getPathToDVHData();
	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
	{
		QString warn = "Failed to open \"" + path + "\"";
		QMessageBox::warning(this, tr("File Open Failed"), warn);
		return false;
	}

	QTextStream in(&file);
	QString line;
	
	for (int i = 0; i < 12; i++) // Skip 12 lines of file header
	{
		line = in.readLine();
	}

	initVolumesArray(volumes);
	numPoints = readDVHStructureData(in, bladder, volumes);
	readDVHStructureData(in, rectum, volumes);
	readDVHStructureData(in, leftFem, volumes);
	readDVHStructureData(in, rightFem, volumes);
	readDVHStructureData(in, PTV, volumes);

	in.flush();
	file.close();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// 
// Returns the number of data points read from file.
//
// Assumes parameter "in" connects to an open file of the correct format.
//
////////////////////////////////////////////////////////////////////////////////
int CompareDialog::readDVHStructureData(QTextStream &in, int structureNum,
		float volumes[numStructures][maxNumDVHPoints])
{
	QString line;
	const static int numStructureHeaderLines = 18;

	for (int i = 0; i < numStructureHeaderLines; i++) // Skip structure header
	{
		line = in.readLine();
	}

	line = in.readLine(); // Initialize line for the while loop
	int lineNum = 0;
	QStringList list;

	// readLine() trims endline chars so returns empty QString for blank line:
	while ((!line.isEmpty()) && (lineNum < maxNumDVHPoints)) 
	{
		list = line.split(" ", QString::SkipEmptyParts);
		//list = line.split(QRegExp("\\s+"));

		if (!list.at(0).contains(QChar('.'))) // Skip fractional % doses
		{
			dose[lineNum] = list.at(0).toDouble();
			int size = list.size();
			float vol = list.at(2).toDouble();
			volumes[structureNum][lineNum++] = list.at(2).toDouble();
		}

		line = in.readLine();
	}

	return lineNum - 1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Assumes that parameter "chart" points to a previously instantiated vtkChartXY
// object.
//
// Also assumes that match DVH data has already been read in.
//
// Note: vtkChartXY requires that all the arrays that are used to construct 
// plots have the same number of elements.  However, in the DVH data acquired
// 2011/09/19, different cases have different numbers of values.  The current
// hack is to set the number of plot points for both match and overlay to the
// number of match points.  The volumes array elements are all set to 0.0 before
// each read, so that if the match case has more points than the overlay case,
// the extra overlay values will just be 0.0 since the initialization for those
// elements will not have been overwritten during file read.  If the overlay
// case has more points than the match case, then those "extras" will simply
// not be displayed.  This should not be more than a very few points, judging
// from the data I've looked at, and this way there won't be any discrepancy
// in the sizes and formats of the two DVH graphs.  An alternative would be 
// to set both to the max of the two numbers of points, but that could entail
// resizing the match DVH depending on which overlay case is selected, which
// I'd rather avoid unless someone misses the few data points lopped off the
// nds of the overlay curves in some cases.  Everything tends to be at or near
// 0.0 out there in any case.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupOverlayDVHChart(vtkChartXY *chart, char *title)
{
	chart->ClearPlots();

	if (title) chart->SetTitle(title);
	chart->GetAxis(vtkAxis::LEFT)->SetTitle("% volume");
	chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("% dose");
	chart->SetShowLegend(true);

	vtkFloatArray *xCoords = vtkFloatArray::New();
	vtkFloatArray *yCoords[numStructures];
	vtkTable *table = vtkTable::New();
	vtkPlot *line;

	xCoords->SetArray(dose, numMatchDVHPoints, 1);
	xCoords->SetName("% dose");
	table->AddColumn(xCoords);
	table->SetNumberOfRows(numMatchDVHPoints);

	int currStructureCount = 0;

	for (int i = 0; i < numStructures; i++)
	{
		if (!viewStructureCheckBox[i]->isChecked()) continue;

		yCoords[currStructureCount] = vtkFloatArray::New();
		yCoords[currStructureCount]->SetArray(matchVolumes[i], numMatchDVHPoints, 1);

		if (overlayDVHCheckBox->isChecked())
		{
			QString name = "match ";
			name.append(structureName[i]);
			yCoords[currStructureCount]->SetName(name);
		}
		else
		{
			yCoords[currStructureCount]->SetName(structureName[i]);
		}

		table->AddColumn(yCoords[currStructureCount]);
		table->Update();
		line = chart->AddPlot(vtkChart::LINE);
		line->SetInput(table, 0, currStructureCount + 1);
		line->SetColor(color[i][0], color[i][1], color[i][2], 255);
		yCoords[currStructureCount++]->Delete();
	}

	if (overlayDVHCheckBox->isChecked() && overlayPatient)
	{
		vtkFloatArray *overlayYCoords[numStructures];
		currStructureCount = 0;

		for (int i = 0; i < numStructures; i++)
		{
			if (!viewStructureCheckBox[i]->isChecked()) continue;

			overlayYCoords[currStructureCount] = vtkFloatArray::New();

			overlayYCoords[currStructureCount]->SetArray(overlayVolumes[i],
				numMatchDVHPoints, 1);
			QString name = "overlay ";
			name.append(structureName[i]);
			overlayYCoords[currStructureCount]->SetName(name);
			table->AddColumn(overlayYCoords[currStructureCount]);
			table->Update();
			line = chart->AddPlot(vtkChart::LINE);
			line->SetInput(table, 0, table->GetNumberOfColumns() - 1);
			line->SetColor(color[i][0], color[i][1], color[i][2], 255);
			line->GetPen()->SetLineType(vtkPen::DASH_LINE);
			line->GetPen()->SetWidth(4.0);
			overlayYCoords[currStructureCount++]->Delete();
		}
	}

	chart->Update();
	chart->RecalculateBounds();

	xCoords->Delete();
}

////////////////////////////////////////////////////////////////////////////////
//
// Assumes that parameter "chart" points to a previously instantiated vtkChartXY
// object.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::setupMatchDVHChart(vtkChartXY *chart, char *title)
{
	chart->ClearPlots();

	if (title) chart->SetTitle(title);
	chart->GetAxis(vtkAxis::LEFT)->SetTitle("% volume");
	chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("% dose");
	chart->SetShowLegend(true);

	vtkFloatArray *xCoords = vtkFloatArray::New();;
	vtkFloatArray *yCoords[numStructures];
	vtkTable *table = vtkTable::New();
	vtkPlot *line;

	xCoords->SetArray(dose, numMatchDVHPoints, 1);
	xCoords->SetName("% dose");
	table->AddColumn(xCoords);
	table->SetNumberOfRows(numMatchDVHPoints);

	int currStructureCount = 0;

	for (int i = 0; i < numStructures; i++)
	{
		if (!viewStructureCheckBox[i]->isChecked()) continue;

		yCoords[currStructureCount] = vtkFloatArray::New();
		yCoords[currStructureCount]->SetArray(matchVolumes[i], numMatchDVHPoints, 1);
		yCoords[currStructureCount]->SetName(structureName[i]);
		table->AddColumn(yCoords[currStructureCount]);
		table->Update();
		line = chart->AddPlot(vtkChart::LINE);
		line->SetInput(table, 0, currStructureCount + 1);
		line->SetColor(color[i][0], color[i][1], color[i][2], 255);
		yCoords[currStructureCount++]->Delete();
	}

	chart->Update();
	chart->RecalculateBounds();

	xCoords->Delete();
}

///displayOverlayDVHData////////////////////////////////////////////////////////
//
// Proposed: new and modified functionality for the Compare dialog (based on a
// conversation with Shiva Das and Joseph Lo on 8/30/11):
//
// New DVH display behavior:
//
// What had been "Query DVH" (top right panel in Compare dialog) has become
// the "Overlay DVH".  In the larger scheme the Query case can't be expected to
// *have* a DVH prior to the completion of his treatment plan.
//
// When the Compare dialog first appears, the Match candidate's DVH appears in 
// both DVH panels.  Then, as each additional Match candidate is selected for
// viewing its identifying institution/case number is displayed as a new menu
// item in the history menu, ordered with the most recently selected at the top.
//
// This selection may occur by any of three mechanisms: 1) by picking in the
// Case Space dialog; 2) by typing or spinning in a value using the
// matchSelectSpinBox; or 3) by selecting from the Match History menu.
//
// As each match candidate selection is made, its DVH is displayed in both
// DVH panels.  If the Overlay button is checked and an item is selected in the
// overlay menu, that item's DVH is overlaid on the match DVH currently 
// displayed in the Overlay DVH panel.
//
// Maximum number of overlays option #1: max 5 (rejected):
//
// There is also an Overlay History menu that displays the sequence of Match
// candidate DVH's whose DVH's have been added to the Overlay panel, also 
// ordered with the most recent first.  
//
// For both of these menus, clicking on the associated "Remove selected"
// button rmoves the selected item (and selects the topmost item remaining?).  
// If an item is removed from the History menu, it's also removed from the
// Overlay menu.  If an item is removed from the Overlay menu, however,
// it's not removed from the Match History menu. [???]
//
// There is a maximum of 5 overlaid DVH's that will be displayed simultaneously.
// There are two options for handling a user attempt to add a 6th: 1) the
// Overlay button is disabled until the user removes at least one item from
// the Overlay menu; or 2) the bottom item on the Overlay menu is bumped and
// the remaining items all move down to make room for the new selection.  The
// Overlay panel is updated accordingly.
//
// Maximum number of overlays 2 (option #2: implemented starting 2011/09/21):
//
// Either one or two DVH's are displayed in the Overlay Panel. One is always the
// DVH associated with the Match candidate currently displayed in the lower half
// of the Compare dialog.  The other is selected using an Overlay menu,
// identical to the History menu, except that selecting an item determines the
// second DVH displayed in the DVH Overlay panel.
// 
// Algorithm:  
// 
// If it's a new dialog, overlayPatient is NULL and the overlayDVHCheckBox isn't
// checked: just display the match DVH.
//
// If the overlayDVHCheckBox isn't checked, if no item in the overlay menu is
// selected, or if the overlay menu is empty, just display the match DVH in
// the overlay DVH display.
//
// If the overlayPatient has a value, an item is selected by the user in the
// overlay menu, and the overlayDVHCheckBox is checked, then read the DVH data
// for the overlay patient and, if successful (as it should be: every item in
// the overlay menu has already been displayed as a match. However, I'm
// reluctant to go without checking for success) then display the match and
// overlay data, the overlay data shoown with dashed lines.  if not successful,
// report the error and just display the match DVH.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::displayOverlayDVHData()
{
	QString title;

	if (overlayPatient && overlayDVHCheckBox->isChecked())
	{	
		if (readDVHData(*overlayPatient, overlayVolumes, numOverlayDVHPoints))
		{
			title = "DVH: match -- Duke patient #" + 
			QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'))
			+ " + overlay -- Duke patient #" +
			QString(("%1")).arg(overlayPatient->getNumber(), 3, 10, QLatin1Char('0'));
		}
		else
		{
			cout << "Failed to read DVH data for patient #"
				 << overlayPatient->getNumber() << endl;

			initOverlayDVHObjects();
			return;
		}
	}
	else
	{
		title = "Match DVH: Duke patient " + 
		QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'));
	}

	QByteArray ba = title.toLatin1();
	setupOverlayDVHChart(overlayDVH, ba.data());
	overlayDVHView->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::displayMatchDVHData()
{
	if (matchDVHDataExists = readDVHData(*matchPatient, matchVolumes, 
		numMatchDVHPoints))
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

		matchDVHView->Render();
	}

	displayOverlayDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::toggleOverlayDVH(bool checked)
{
	if (!overlayPatient || !matchPatient) return;

	QString title;

	if (overlayDVHCheckBox->isChecked())
	{
		title = "DVH: match -- Duke patient " + 
		QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'))
		+ " + overlay -- Duke patient " +
		QString(("%1")).arg(overlayPatient->getNumber(), 3, 10, QLatin1Char('0'));
	}
	else
	{
		title = "Match DVH: Duke patient " + 
		QString(("%1")).arg(matchPatient->getNumber(), 3, 10, QLatin1Char('0'));
	}

	QByteArray ba = title.toLatin1();

	setupOverlayDVHChart(overlayDVH, ba.data());
	overlayDVHView->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Using the "sender" approach, considered dangerous because of the cast.  
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::historyItemTriggered(bool checked)
{
	QAction *action = (QAction *)sender();
	emit selectHistoryMatch(action->text());
}

////////////////////////////////////////////////////////////////////////////////
//
// Remove the current match and replace it with the top (most recently viewed)
// item in the match history menu.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::removeCurrentMatch()
{
	if (!matchHistoryMenu || matchHistoryMenu->actions().isEmpty()) return;

	// Replace the current match with the item on top of the history menu;
	QAction *action = matchHistoryMenu->actions().first();
	selectHistoryMatch(action->text());

	// Now the new item on top of the history menu is the former match that
	// we want to remove:
	action = matchHistoryMenu->actions().first();
	matchHistoryMenu->removeAction(action);

	removeCurrentMatchPushButton->setEnabled(
		!matchHistoryMenu->actions().isEmpty());

	historyPushButton->setEnabled(!matchHistoryMenu->actions().isEmpty());

	if (caseSpaceDialog) caseSpaceDialog->resetDukeDataPositions();
}


////////////////////////////////////////////////////////////////////////////////
//
// Using the "sender" approach, considered dangerous because of the cast.  
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::overlayItemTriggered(bool checked)
{
	QAction *action = (QAction *)sender();
	emit selectOverlay(action->text());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectHistoryMatch(QString text)
{
	caseSpaceDialog->setIsNewMatchCaseSelectedHere(false);

	QString nAsText = text.section('#', 1);
	int num = nAsText.toInt();
	matchSelectSpinBox->setValue(num);

	removeRedundantMenuItem(matchHistoryMenu, text);

	if (caseSpaceDialog) caseSpaceDialog->resetDukeDataPositions();

}

////////////////////////////////////////////////////////////////////////////////
//
// 2011/09/21:  Behavior wrt the DVH overlay pane:
//
// Option 1: the overlay is from the overlay menu, the underlay is the match:
//
// When the Compare Dialog is opened, the DVH overlay pane displays a 
// duplicate of the match DVH. The overlay check box is unchecked and disabled.
// Both the match and overlay DVH titles read: "DVH for match <institution>
// patient<number>".
//
// When the user changes the match, the DVH underlay stays (goes) with the  
// current match, the previous match is added to the top of both match history 
// and overlay menus, and the overlay check box is enabled.
//
// By default, no item in the overlay menu is checked.  The user may select an
// in the overlay menu by clicking on it.  When the user has made a selection
// in the overlay menu, the overlay checkbox is automatically checked, the 
// corresponding DVH overlay is displayed, and that item remains selected
// until it's deleted or until the user chooses another overlay menu item.
//
// Checking the overlay check box overlays the DVH for the currently selected
// item in the overlay menu onto the match DVH.  The overlay DVH title reads
// "DVH for match <institution> patient<number>; overlay: <institution>
// patient<number>".
//
// The match history menu includes the current overlay selection, but not the 
// current match.  The overlay menu does not include the current match either.
//
// The selected item may be removed from the overlay menu by clicking on the
// corresponding "Remove Current" button.  When that happens, no overlay item is
// selected, no overlay is displayed, and the overlay checkbox is unchecked.
// If the overlay menu is empty, the overlay check box is disabled.
//
// If the user selects from the overlay menu, the overlay check box is checked
// automatically.  Why else would a user select an overlay item except to
// display it.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::selectOverlay(QString text)
{
	QByteArray a = text.toLatin1();
	QString nAsText = text.section('#', 1);
	int num = nAsText.toInt();
	overlayPatient = caseSpaceDialog->getDukePatientFrom(num);
	int numItems = overlaySelectionMenu->actions().size();

	for (int i = 0; i < numItems; i++)
	{
		QAction *currAction = overlaySelectionMenu->actions().at(i);
		QString currText = currAction->text();
		currAction->setChecked(!text.compare(currText));
	}

	overlaySelectionPushButton->setEnabled(true);
	removeSelectedOverlayPushButton->setEnabled(true);
	overlayDVHCheckBox->setEnabled(true);
	overlayDVHCheckBox->setChecked(true);
	displayOverlayDVHData();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::removeSelectedOverlayMenuItem()
{
	if (!overlaySelectionMenu || overlaySelectionMenu->isEmpty()) return;

	bool deleted = false;
	
	QList<QAction *> actions = overlaySelectionMenu->actions(); 
	QList<QAction *>::Iterator i;

	for (i = actions.begin(); i != actions.end(); ++i)
	{
		if ((*i)->isChecked())
		{
			overlaySelectionMenu->removeAction(*i);
		}
	}

	if (overlaySelectionMenu->isEmpty())
	{
		overlaySelectionPushButton->setEnabled(false);
		overlayDVHCheckBox->setEnabled(false);
	}

	removeSelectedOverlayPushButton->setEnabled(false); // Nothing selected

	overlayDVHCheckBox->setChecked(false);
	overlayPatient = NULL;
	toggleOverlayDVH(false);
	displayOverlayDVHData();
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
	int maybeRedundantPatientNum = caseSpaceDialog->getMatchCase()->getNumber();

	QString maybeRedundantItemText = /* matchPatient->getInstitution() + */
		"Duke Patient #" + QString::number(maybeRedundantPatientNum);

	removeRedundantMenuItem(matchHistoryMenu, maybeRedundantItemText);

	int newHistoryPatientNum = caseSpaceDialog->getLastMatchCase()->getNumber();

	newHistoryItemText = /* matchPatient->getInstitution() + */
		"Duke Patient #" + QString::number(newHistoryPatientNum);

	int currMatchHistoryNum = matchHistoryMenu->actions().size();

	for (int i = 0; i < currMatchHistoryNum; i++)
	{
		QAction *currAction = matchHistoryMenu->actions().at(i);
		QString currText = currAction->text();

		if (!newHistoryItemText.compare(currText))	// If they are the same...
		{
			return;	 // ...don't add (because it's already there)
		}
	}

	QAction *matchHistoryAction = new QAction(newHistoryItemText, this);
	connect(matchHistoryAction, SIGNAL(triggered(bool)),
		this, SLOT(historyItemTriggered(bool)));

	QAction *firstAction = NULL;

	if (!matchHistoryMenu->isEmpty())
	{
		firstAction = matchHistoryMenu->actions().first();
	}

	matchHistoryMenu->insertAction(firstAction, matchHistoryAction);
	
	historyPushButton->setEnabled(!matchHistoryMenu->actions().isEmpty());

	removeCurrentMatchPushButton->setEnabled(
		!matchHistoryMenu->actions().isEmpty());

	if (caseSpaceDialog) caseSpaceDialog->resetDukeDataPositions();
}

////////////////////////////////////////////////////////////////////////////////
//
// Make sure addMatchHistoryItem() is called before calling this method so that
// newHistoryItemText is current.
//
// Don't add if the current match candidate is already in the list.
//
////////////////////////////////////////////////////////////////////////////////
void CompareDialog::addOverlaySelectionItem()
{
	if ((!overlaySelectionMenu) || (!caseSpaceDialog)) return;

	// If the new match is in the overlay menu, remove it:
	int maybeRedundantPatientNum = caseSpaceDialog->getMatchCase()->getNumber();

	QString maybeRedundantItemText = /* matchPatient->getInstitution() + */
		"Duke Patient #" + QString::number(maybeRedundantPatientNum);

	removeRedundantMenuItem(overlaySelectionMenu, maybeRedundantItemText);

	int newHistoryPatientNum = caseSpaceDialog->getLastMatchCase()->getNumber();

	int currNumOverlayMenuItems = overlaySelectionMenu->actions().size();

	for (int i = 0; i < currNumOverlayMenuItems; i++)
	{
		QAction *currAction = overlaySelectionMenu->actions().at(i);
		QString currText = currAction->text();

		if (!newHistoryItemText.compare(currText))	// If they are the same...
		{
			return;	 // ...don't add (because it's already there)
		}
	}

	QAction *overlaySelectionAction = new QAction(newHistoryItemText, this);
	overlaySelectionAction->setCheckable(true);
	connect(overlaySelectionAction, SIGNAL(triggered(bool)),
		this, SLOT(overlayItemTriggered(bool)));

	QAction *firstAction = NULL;

	if (!overlaySelectionMenu->isEmpty())
	{
		firstAction = overlaySelectionMenu->actions().first();
	}

	overlaySelectionMenu->insertAction(firstAction, overlaySelectionAction);

	overlayDVHCheckBox->setEnabled(!overlaySelectionMenu->isEmpty());
	overlaySelectionPushButton->setEnabled(!overlaySelectionMenu->isEmpty());

	/* No, no autoselection.  If an item is selected it's because the user did it.
	// Select the first item:
	overlaySelectionMenu->actions().first()->setChecked(true);

	currNumOverlayMenuItems = overlaySelectionMenu->actions().size();

	// And deselect all the others:
	for (int i = 1; i < currNumOverlayMenuItems; i++)
	{
		QAction *currAction = overlaySelectionMenu->actions().at(i);
		currAction->setChecked(false);
	}

	displayOverlayDVHData();
	*/
}

////////////////////////////////////////////////////////////////////////////////
//
// Look through the items in the menu.  Remove each that has the same
// text field as the text input parameter.  Return true if any actions were
// removed from the menu, else return false.
//
////////////////////////////////////////////////////////////////////////////////
bool CompareDialog::removeRedundantMenuItem(QMenu *menu, QString text)
{
	bool wasActionRemoved = false;
	int numMenuItems = menu->actions().size();

	int i = 0;

	while (i < numMenuItems)
	{
		QAction *currAction = menu->actions().at(i++);
		QString currText = currAction->text();

		if (!text.compare(currText)) // if they're the same...
		{
			menu->removeAction(currAction);
			numMenuItems--;
			wasActionRemoved = true;
		}
	}

	return wasActionRemoved;
}

