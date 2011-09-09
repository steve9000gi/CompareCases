////////////////////////////////////////////////////////////////////////////////
//
// CompareDialog.h: Qt-based GUI for prostate cancer radiation therapy planning 
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

#ifndef COMPARE_DIALOG_H
#define COMPARE_DIALOG_H

#include <QDialog>

#include "ui_CompareDialog.h"  // auto generated from XML output from QT Designer


static const int numDVHPoints = 103; // Based on DVH input data format
static const int maxMatchHistoryNum = 200;

// Forward declarations:
class vtkPNGReader;
class vtkImageViewer;
class vtkImageViewer2;
class vtkDICOMImageReader;
class vtkRenderer;
class vtkRenderWindow;
class vtkImageFlip;
class vtkContextView;
class vtkTable;
class vtkChartXY;
class Patient;
class CaseSpaceDialog;
class Projector;
class QSignalMapper;

class CompareDialog : public QDialog, public Ui_CompareDialog
{
	Q_OBJECT

public:
	enum Structure
	{
		PTV = 0,							// Planning Target Volume
		rectum,
		bladder,
		leftFem,
		rightFem,
		numStructures
	};

	CompareDialog();
	CompareDialog(CaseSpaceDialog *csDlog);
	~CompareDialog();

	virtual void accept();
	virtual void reject();


	void setQuery(Patient *patient);
	void setMatch(Patient *patient);

	bool dataExistsFor(Patient *patient);

signals:
	void historyMenuItemSelected(int number);

private slots:
	void selectQuery(int patientNum);				// For query CT, projection, DVH displays
	void selectMatch(int patientNum);				// For match CT, projection, DVH displays

	void setSliceAxis(bool val = true);				// Both query and match CT data
	void changeSlice(int slice);					// Both query and match CT data
	void autoplay();								// Both query and match CT data
	void setGantryAngle25();						// All these for projection data, ...
	void setGantryAngle75();				
	void setGantryAngle130();
	void setGantryAngle180();
	void setGantryAngle230();
	void setGantryAngle280();
	void setGantryAngle335();						// ...both query and match.
	void changeTransparency(int transp);
	void toggleFlatShadedStructures(bool checked);	// For projections
	void toggleOrigin(bool checked);
	void toggleFemoralHeads(bool checked);			// For projection and DVH data. q. and m.
	void toggleOverlayDVH(bool checked);
	void itemTriggered(bool checked);
	void selectHistoryMatch(QString text);

private:
	// General setup methods:
	void setupVTKUI();
	void createActions();

	// CT data display methods:
	void initQueryCTPipeLine();
	void initMatchCTPipeLine();
	void selectQueryCTSlice(int slice);
	void selectMatchCTSlice(int slice);
	void extractQueryDICOMFileMetaData();

	// Projection data display methods:
	void selectQueryProjection();
	void selectMatchProjection();
	void setupProjectionGantryAngleMenu();

	// Dose Volume Histogram data display methods:
	void initQueryDVHObjects();
	void initMatchDVHObjects();
	bool readQueryDVHData(Patient &patient);
	bool readMatchDVHData(Patient &patient);
	void setupQueryDVHChart(vtkChartXY *chart, char *title);
	void setupMatchDVHChart(vtkChartXY *chart, char *title);

	void displayQueryDVHData();
	void displayMatchDVHData();
	void setDVHYAxisTicks(vtkChartXY *chart);

	// Match history methods:
	void setupMatchHistoryMenu();
	void addMatchHistoryItem();
	bool removeRedundantHistoryItem(QString text);

	// Overlay selection methods:
	void setupOverlaySelectionMenu();

	// Objects:
	Patient *queryPatient;
	Patient *matchPatient;

	// CT data display objects:
	vtkDICOMImageReader *queryDICOMReader;
	vtkDICOMImageReader *matchDICOMReader;
	vtkImageFlip *queryCTImageFlip;	// CT image orientation appropriate to prostate data
	vtkImageFlip *matchCTImageFlip;	// "
	vtkImageViewer2 *queryCTImageViewer;
	vtkImageViewer2 *matchCTImageViewer;

	vtkRenderWindow *queryProjectionRenWin;
	vtkRenderWindow *matchProjectionRenWin;
	Projector *queryProjector;
	Projector *matchProjector;

	// Projection gantry angle drop-down menu elements:
	QMenu *gantryAngleMenu;
	QActionGroup *gantryAngleActionGroup;
	QAction *angle25Action;
	QAction *angle75Action;
	QAction *angle130Action;
	QAction *angle180Action;
	QAction *angle230Action;
	QAction *angle280Action;
	QAction *angle335Action;

	int queryAngle;	// Current gantry angle for query projection display
	int matchAngle; // Current gantry angle for match projection display

	// Dose Volume Histogram data display objects:
	vtkChartXY *queryDVH;
	vtkChartXY *matchDVH;
	vtkContextView *queryDVHView;
	vtkContextView *matchDVHView;

	float dose[numDVHPoints];							// x-axis DVH values 	
	float *volumes;	// y-axis DVH values
	float queryVolumes[numStructures][numDVHPoints];	// y-axis DVH values
	float matchVolumes[numStructures][numDVHPoints];	// y-axis DVH values

	static const int xDVHWidget;						// DVH display width
	static const int yDVHWidget;						// DVH display height

	CaseSpaceDialog *caseSpaceDialog;

	// Match history:
	QMenu *matchHistoryMenu;
	int currMatchHistoryNum;							// # matches currently in history menu
	int currHistoryActionNum;							// How many matches have been created
	QAction *matchHistoryAction[maxMatchHistoryNum];

	// DVH overlay selection:
	QMenu *overlaySelectionMenu;
	int overlaySelectionIndex;
	bool matchDVHDataExists;							// Don't overlay nonexistent match data

private:
	CompareDialog(const CompareDialog&);				// Not implemented.
	void operator=(const CompareDialog&);				// Not implemented.
};

#endif
