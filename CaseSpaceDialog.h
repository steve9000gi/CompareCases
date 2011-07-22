////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.h: Qt-based GUI for prostate cancer radiation therapy
// planning tool. This window displays a cartesian graph showing the relative
// distances of the selected sets of cases from the query case with respect to
// several criteria, e.g., PTV + bladder overlap for the x axis and PTV + 
// rectum overlap for the y axis.
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyavanich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CASE_SPACE_DIALOG_H
#define CASE_SPACE_DIALOG_H

#include <QDialog>

#include "ui_CaseSpaceDialog.h"  // auto generated from XML output from QT Designer

// TEMP hardwired based on size of MI data files:
static const int numMICases = 100;

// Forward declarations:
class ccChartXY;
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
class vtkCellPicker;


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
	Patient *getDukePatientFromCoodinates(double posX, double posY);
	int getNumDukePatients() { return numDukePatients; };
	MainWindow *getMainWindow() { return mainWindow; };

	void enableCompareCasesButton(bool isEnabled)
	{
		compareCasesPushButton->setEnabled(isEnabled);
	};

	const static void ReportCameraPosition(vtkRenderer *r);
	void SetCameraPosition(double pos[3], double fp[3], double vUp[3], double clip[2], double zoom);
	void SetCameraPosition(double az);

	void pickPatient();

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

private: // Objects
	ccChartXY *caseSpaceChart;
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
	double zMult;
	Axes *axes;
	vtkAssembly *axesAssembly;

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

	// Zero-based index into array of Patients: the first position
	// is 0 even though the first patient number is 3 or something;
	// Also, patient numbers do not have to be sequential:
	int queryCaseIndex; 

	CompareDialog *compareDialog;

private:
	void createActions();
	void setupCaseSpaceChart();
	void setupCaseSpaceRenWin();
	void addQueryCase();


	void addDukeDataToChart();
	void addDummyDukeDataToChart();
	void addPoconoDataToChart();
	void addHighPointDataToChart();
	void addQueryCaseToChart();

	bool readMIData();
	bool averageMIData();
	void prepareMIDisplay();
	void addMIThresholdPlane();

	void drawSelectedCase();

private:
	CaseSpaceDialog(const CaseSpaceDialog&);		// Not implemented.
	void operator=(const CaseSpaceDialog&);			// Not implemented.
};

#endif
