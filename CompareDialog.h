////////////////////////////////////////////////////////////////////////////////
//
// CompareDialog.h: Qt-based GUI for prostate cancer radiation therapy planning 
// tool. This window displays six panels: the left-top two display data
// for the "query" patient, i.e., the current patient for whom treatment is 
// being planned.  The lower three display data for the "match" patient,
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

#ifndef COMPARE_DIALOG_H
#define COMPARE_DIALOG_H

#include <QDialog>

#include "ui_CompareDialog.h"  // auto generated from XML output from QT Designer


static const int numDVHPoints = 103; // Based on DVH input data format
static const int maxNumDVHPoints = 200;

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
//class QSignalMapper;

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

	void initScrollArea();
	virtual void accept();
	virtual void reject();


	void setQuery(Patient *patient);
	void setMatch(Patient *patient);

	bool CTDataExistsFor(Patient *patient);
	bool DVHDataExistsFor(Patient *patient);

	QMenu *getMatchHistoryMenu() { return matchHistoryMenu; };

signals:
	void historyMenuItemSelected(int number);

private slots:
	void selectQuery(int patientNumber);			// For query CT and projection displays
	void selectMatch(int patientNumber);			// For match CT, projection, DVH displays
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
	void changeTransparency();
	void toggleFlatShadedStructures(bool checked);	// For projections
	void toggleOrigin(bool checked);
	void toggleViewStructure(bool checked);			// For projection and DVH data. q. and m.
	void toggleOverlayDVH(bool checked);
	void historyItemTriggered(bool checked);
	void selectHistoryMatch(QString text);
	void removeCurrentMatch();
	void overlayItemTriggered(bool checked);
	void selectOverlay(QString text);
	void removeSelectedOverlayMenuItem();

private:
	// General setup methods:
	void setupVTKUI();
	void createActions();
	void setViewCheckboxColors();

	// CT data display methods:
	void initQueryCTPipeLine();
	void initMatchCTPipeLine();
	void selectQueryCTSlice(int slice);
	void selectMatchCTSlice(int slice);
	void extractQueryDICOMData();

	// Projection data display methods:
	void selectQueryProjection(bool newQuery = false);
	void selectMatchProjection(bool newMatch = false);
	void setupProjectionGantryAngleMenu();

	// Dose Volume Histogram data display methods:
	void initOverlayDVHObjects();
	void initMatchDVHObjects();
	void initVolumesArray(float volumes[numStructures][maxNumDVHPoints]);
	bool readDVHData(Patient &patient,
		float volumes[numStructures][maxNumDVHPoints], int &numPoints);
	int readDVHStructureData(QTextStream &in, int structureNum,
		float volumes[numStructures][maxNumDVHPoints]);
	void setupOverlayDVHChart(vtkChartXY *chart, char *title);
	void setupMatchDVHChart(vtkChartXY *chart, char *title);

	void displayOverlayDVHData();
	void displayMatchDVHData();
	void setDVHYAxisTicks(vtkChartXY *chart);

	// Match history methods:
	void setupMatchHistoryMenu();
	void addMatchHistoryItem();
	bool removeRedundantMenuItem(QMenu *menu, QString text);

	// Overlay selection methods:
	void setupOverlaySelectionMenu();
	void addOverlaySelectionItem();

	// Objects:
	Patient *queryPatient;
	Patient *matchPatient;
	Patient *overlayPatient;

	// CT data display objects:
	vtkDICOMImageReader *queryDICOMReader;
	vtkDICOMImageReader *matchDICOMReader;
	vtkImageFlip *queryCTImageFlip;	// CT image orientation appropriate to prostate data
	vtkImageFlip *matchCTImageFlip;	// "
	vtkImageViewer2 *queryCTImageViewer;
	vtkImageViewer2 *matchCTImageViewer;

	// Projection objects:
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

	// To determine which structures to show (projection & DVH):
	(QCheckBox *) viewStructureCheckBox[numStructures];

	// Dose Volume Histogram data display objects:
	vtkChartXY *overlayDVH;
	vtkChartXY *matchDVH;
	vtkContextView *overlayDVHView;
	vtkContextView *matchDVHView;

	float dose[maxNumDVHPoints];							// x-axis DVH values 	
	float overlayVolumes[numStructures][maxNumDVHPoints];	// y-axis DVH values
	float matchVolumes[numStructures][maxNumDVHPoints];	// y-axis DVH values
	int numMatchDVHPoints;
	int numOverlayDVHPoints;

	static const int xDVHWidget;						// DVH display width
	static const int yDVHWidget;						// DVH display height

	CaseSpaceDialog *caseSpaceDialog;

	// Match history:
	QMenu *matchHistoryMenu;
	QString newHistoryItemText;							// Need for overlay menu

	// DVH overlay selection:
	QMenu *overlaySelectionMenu;
	int overlaySelectionIndex;
	bool matchDVHDataExists;							// Don't overlay nonexistent match data
	bool matchXYDataExists;						// If not, don't add to menus

private:
	CompareDialog(const CompareDialog&);				// Not implemented.
	void operator=(const CompareDialog&);				// Not implemented.
};

#endif
