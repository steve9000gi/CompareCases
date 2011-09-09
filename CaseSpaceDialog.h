////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.h: Qt-based GUI for prostate cancer radiation therapy 
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

#ifndef CASE_SPACE_DIALOG_H
#define CASE_SPACE_DIALOG_H

#include <QDialog>

#include "ui_CaseSpaceDialog.h"  // auto generated from XML output from QT Designer

// TEMP hardwired based on size of MI data files:
static const int numMICases = 100;

static const int numGlowLevels = 20;
static const double cubeSize = 100.0;

static const double colorMult = 1.2;



// Forward declarations:
class vtkContextView;
class vtkPlot;
class vtkTable;
class vtkVector2f;
class MainWindow;
class Patient;
class CompareDialog;
class Axes;

class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkRenderer;
class vtkCubeSource;
class vtkPolyDataMapper;
class vtkActor;
class vtkAssembly;
class vtkBalloonWidget;
class vtkBalloonRepresentation;
class vtkScalarBarActor;
class vtkLookupTable;


class CaseSpaceDialog : public QDialog, public Ui_CaseSpaceDialog
{
	Q_OBJECT

public:
	CaseSpaceDialog(MainWindow *mw);
	~CaseSpaceDialog();

	void setQueryCaseText(QString &text);
	void setSelectedMatchPlot(vtkPlot *plot) { selectedMatchPlot = plot; };
	void setSelectedMatchPlotPos(vtkVector2f *pos);
	void setDukeOverlapDataPath(QString &path) { dukeOverlapDataPath = path; };
	void identifyMatchCase();
	Patient *getPatientFromCoodinates(double *xArray, double *yArray, double x, double y);
	void setMatchCaseFromCompareDialog(int patientNumber);
	int getIndexFrom(Patient *p);
	Patient *getDukePatientFromCoodinates(double posX, double posY);
	Patient *getDukePatientFrom(int patientNumber);
	int getNumDukePatients() { return numDukePatients; };
	MainWindow *getMainWindow() { return mainWindow; };

	Patient *getLastMatchCase() { return lastMatchCase; };
	Patient *getMatchCase() { return matchCase; };
	void setLastMatchCase(Patient *lmc) { lastMatchCase = lmc; };
	bool isNewMatchCaseSelectedHere();
	void setIsNewMatchCaseSelectedHere(bool here) { newMatchCaseSelectedHere = here; };

	void enableCompareCasesButton(bool isEnabled)
	{
		compareCasesPushButton->setEnabled(isEnabled);
	};

	const static void ReportCameraPosition(vtkRenderer *r);
	void SetCameraPosition(double pos[3], double fp[3], double vUp[3], double clip[2], double zoom);
	void SetCameraPosition(double az);

	bool pickPatient();

	int minPTVSize;
	int minRectumSize;
	int minBladderSize;
	int minPTVPlusRectum;
	int minPTVPlusBladder;
	int minPTVPlusRectumPlusBladder;

	int maxPTVSize;
	int maxRectumSize;
	int maxBladderSize;
	int maxPTVPlusRectum;
	int maxPTVPlusBladder;
	int maxPTVPlusRectumPlusBladder;

private slots:
	void compareCases();
	void testFunction();
	void setXYView(bool checked = true);
	void setXMIView(bool checked = true);
	void setYMIView(bool checked = true);
	void setObliqueView(bool checked = true);
	void setThresholdPlaneZVal(int val);
	void setBackgroundBlack(bool checked = true);
	void setBackgroundWhite(bool checked = true);
	void setBackgroundRamped(bool checked = true);

private: // Objects
	vtkContextView *caseSpaceView;

	// Pointers to data corresponding to the plot point the user
	// has clicked on:
	vtkPlot *selectedMatchPlot;
	vtkVector2f *selectedMatchPlotPosition;

	// Stucture overlap data for chart display.  NOTE: this
	// values are shifted by the Query Case values so that
	// the Query Case will be at the origin.
	double *dX;	// Duke
	double *dY;
	double *pX;	// Pocono
	double *pY;
	double *hpX;	// High Point
	double *hpY;

	vtkRenderWindow *caseSpaceRenWin;
	vtkRenderWindowInteractor *renderWindowInteractor;
	vtkRenderer *ren;
	vtkCubeSource *dukePoint[numMICases];
	vtkPolyDataMapper *dukePointMapper[numMICases];
	vtkActor *dukePointActor[numMICases];
	vtkActor *queryPointActor;

	vtkCubeSource *matchGlow[numGlowLevels];
	vtkPolyDataMapper *matchGlowMapper[numGlowLevels];
	vtkActor *matchGlowActor[numGlowLevels];

	// Tell us about the patient over which we're hovering:
	vtkBalloonWidget *balloonWidget;
	vtkBalloonRepresentation *balloonRep;

	vtkScalarBarActor *MILegend;
	vtkLookupTable *MILookupTable;

	double zMult;
	Axes *axes;
	vtkAssembly *axesAssembly;

	QString matchInstitution;
	QString queryInstitution;

	// Use these to set the camera to center at origin, 
	// i.e., query point:
	double xCenterQueryPt;
	double yCenterQueryPt;
	double zCenterQueryPt;
	double stdCamDist;
	double stdVertShift;
	double parallelScale;

	int numDukePatients;
	int numPoconoPatients;
	int numHighPointPatients;

	double *MIval[numMICases];
	double MIFraction[numMICases];
	double MIMax;
	double MIMin;
	double MIRange;
	vtkCubeSource *MIThresholdPlane;
	double MIThresholdVal;
	double thresholdPlaneZVal;
	double thresholdPlaneThickness;

	vtkTable *dukeTable;
	vtkTable *poconoTable;
	vtkTable *highPointTable;
	vtkTable *queryCaseTable;
	vtkTable *selectedMatchCaseTable;

	vtkPlot *selectedPoint;

	MainWindow *mainWindow;

	// Path to data for structure overlaps: used for placing patient 
	// points in Case Space:
	QString dukeOverlapDataPath;	

	Patient *dukePatientList;
	Patient *queryCase;
	Patient *matchCase;
	Patient *lastMatchCase;

	int lastMatchCaseIndex;
	int currMatchCaseIndex;
	bool newMatchCaseSelectedHere;

	// Zero-based index into array of Patients: the first position
	// is 0 even though the first patient number is 3 or something;
	// Also, patient numbers do not have to be sequential:
	int queryCaseIndex; 

	CompareDialog *compareDialog;

private:
	void createActions();
	void setupCaseSpaceRenWin();
	void addQueryCase();

	void initializeBalloonStuff();
	void addBalloon(int index);

	void initializeMatchGlow();
	void setMatchGlowLocation(double x, double y, double z);

	void initializeMILegend();

	void addDukeDataToGraph();
	void addPoconoDataToGraph();
	void addHighPointDataToGraph();

	void displayQueryCaseData();
	void displayMatchCaseData();

	bool readMIData();
	bool averageMIData();
	void prepareMIDisplay();
	void addMIThresholdPlane();

private:
	CaseSpaceDialog(const CaseSpaceDialog&);		// Not implemented.
	void operator=(const CaseSpaceDialog&);			// Not implemented.
};

#endif
