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
// Author:    Steve Chall, RENCI
//
// Primary collaborators: 
//            Joseph Lo, Shiva Das, Vorakarn Chanyavanich, Duke Medical Center
//
// Copyright: The Renaissance Computing Institute (RENCI)
//
// License:   Licensed under the RENCI Open Source Software License v. 1.0
//
//            See http://www.renci.org/resources/open-source-software-license
//            for details.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CASE_SPACE_DIALOG_H
#define CASE_SPACE_DIALOG_H

#include <QDialog>

// auto generated from XML output from QT Designer:
#include "ui_CaseSpaceDialog.h"

// TEMP hardwired based on size of MI data files:
static const int numMICases = 100;
static const int maxNumMICases = 200; // Just in case we get more

static const int numMatchIconLevels = 35;
static const double cubeSize = 100.0;

static const double colorMult = 1.2;

// Forward declarations:
class vtkContextView;
class vtkPlot;
class vtkTable;
class vtkVector2f;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkRenderer;
class vtkCubeSource;
class vtkSphereSource;
class vtkPolyDataMapper;
class vtkActor;
class vtkAssembly;
class vtkBalloonWidget;
class vtkBalloonRepresentation;
class vtkScalarBarActor;
class vtkLookupTable;
class vtkPolyDataAlgorithm;

class MainWindow;
class Patient;
class CompareDialog;
class Axes;
class ccInteractorStyleTrackballCamera;


class CaseSpaceDialog : public QDialog, public Ui_CaseSpaceDialog
{
  Q_OBJECT

public:
  enum XYValueType
  {
    PTVSize = 0,
    rectumSize,
    bladderSize,
    PTVPlusRectum,
    PTVPlusBladder,
    PTVPlusRectumPlusBladder,
    numXYValueTypes
  };

  CaseSpaceDialog(MainWindow *mw);
  ~CaseSpaceDialog();

  void setQueryCaseText(QString &text);
  void setSelectedMatchPlotPos(vtkVector2f *pos);
  void setDukeXYDataPath(QString &path) { dukeXYDataPath = path; };
  void identifyMatchCase();
  void setMatchCaseFromCompareDialog(int patientNumber);
  int getIndexFrom(Patient *p);
  Patient *getDukePatientFromCoodinates(double posX, double posY);
  Patient *getDukePatientFrom(int patientNumber);
  Patient *getPatientFromCoodinates(
    double *xArray, double *yArray, double x, double y);
  int getNumDukePatients() { return numDukePatients; };
  MainWindow *getMainWindow() { return mainWindow; };

  Patient *getLastMatchCase() { return lastMatchCase; };
  Patient *getMatchCase() { return matchCase; };
  void setLastMatchCase(Patient *lmc) { lastMatchCase = lmc; };
  bool isNewMatchCaseSelectedHere();
  void setIsNewMatchCaseSelectedHere(bool here)
    { newMatchCaseSelectedHere = here; };

  void enableCompareCasesButton(bool isEnabled)
  {
    compareCasesPushButton->setEnabled(isEnabled);
  };

  void setCompareDialogPointer(CompareDialog *ptr) { compareDialog = ptr; };

  bool pickPatient();
  const static void ReportCameraPosition(vtkRenderer *r);
  double getMIValue(int queryIx, int matchIx)
  {
    return MIval[queryIx][matchIx];
  };

private slots:
  void compareCases();
  void testFunction();
  void setXYView(bool checked = true);
  void setXMIView(bool checked = true);
  void setYMIView(bool checked = true);
  void setObliqueView(bool checked = true);
  void resetView();
  void setThresholdPlanePosition(int zVal);
  void setBackgroundBlack(bool checked = true);
  void setBackgroundWhite(bool checked = true);
  void setBackgroundRamped(bool checked = true);
  void XYDataAngleItemTriggered(bool checked);
  void selectXYDataAngle(QString text);
  void XValuesItemTriggered(bool checked);
  void selectXValues(QString text);
  void YValuesItemTriggered(bool checked);
  void selectYValues(QString text);
  void toggleAxisPlanes(bool checked);
  void toggleThresholdAxes(bool checked);
  void toggleThresholdPlane(bool checked);

private: // methods
  void SetCameraPosition(double pos[3], double fp[3], double vUp[3],
    double clip[2], double zoom);

  void createActions();
  void setupCaseSpaceRenWin();
  void addQueryCase();

  void initializeBalloonStuff();
  void addBalloon(int index);
  void updateBalloon(int i, int xVal, int yVal);

  void initializeMatchIcon();
  void setMatchIconLocation(double x, double y, double z);

  void initializeMILegend();
  void getColorCorrespondingToMIvalue(
    double MIVal, double &r, double &g, double &b);

  void initXYValExtrema();
  bool readDukeXYData();
  void addDukeDataToGraph();

  void makeSphereIfCube(int ix);
  void makeCubeIfSphere(int ix);
  void setGeometryAccordingToMatchHistory(int ix);
  void resetAppurtenances();

public:
  void resetDukeDataPositions(bool needToRead = false);

private:
  double getXYValueFromIndex(int index, XYValueType valueType);
  double getXYValueFromNumber(int patientNum, XYValueType valueType);
  void averageDukeXYData();

  // Axis values selection menus:
  void setupXYDataAngleMenu();
  void setupXValuesMenu();
  void setupYValuesMenu();

  void addPoconoDataToGraph();
  void addHighPointDataToGraph();

  void displayQueryCaseData();
  void displayMatchCaseData();

  bool readMIData();
  bool averageMIData();
  void prepareMIDisplay();
  void addMIThresholdPlane();
  void addAxisPlanes();
  void setAxisPlanes(int thresholdPlaneZVal);

private: // Objects
  vtkContextView *caseSpaceView;

  // Pointer to data corresponding to the plot point the user
  // has clicked on:
  vtkVector2f *selectedMatchPlotPosition;

  // Stucture XY data for chart display:
  double *dX;   // Duke
  double *dY;
  double *pX;   // Pocono
  double *pY;
  double *hpX;  // High Point
  double *hpY;

  vtkRenderWindow *caseSpaceRenWin;
  vtkRenderWindowInteractor *renderWindowInteractor;
  vtkRenderer *ren;
  ccInteractorStyleTrackballCamera *iStyle;
  vtkPolyDataAlgorithm *dukePoint[maxNumMICases];
  vtkPolyDataMapper *dukePointMapper[maxNumMICases];
  vtkActor *dukePointActor[maxNumMICases];

  vtkCubeSource *queryPoint;
  vtkPolyDataMapper *queryPointMapper;
  vtkActor *queryPointActor;

  vtkSphereSource *matchIcon[numMatchIconLevels];
  vtkPolyDataMapper *matchIconMapper[numMatchIconLevels];
  vtkActor *matchIconActor[numMatchIconLevels];

  // Tell us about the patient over which we're hovering:
  vtkBalloonWidget *balloonWidget;
  vtkBalloonRepresentation *balloonRep;

  vtkScalarBarActor *MILegend;
  vtkLookupTable *MILookupTable;

  double zMult;
  Axes *axes;
  vtkAssembly *axesAssembly;
  vtkAssembly *thresholdAxesAssembly;
  double axisTextOffset;

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
  double MIMax;
  double MIMin;
  double MIRange;
  vtkCubeSource *MIThresholdPlane;
  vtkActor *mitpActor;
  double MIThresholdVal;
  double thresholdPlaneZVal;
  double thresholdPlaneThickness;
  vtkCubeSource *xAxisPlane;
  vtkCubeSource *yAxisPlane;
  vtkActor *xAxisPlaneActor;
  vtkActor *yAxisPlaneActor;

  static const double numMIColors;

  MainWindow *mainWindow;

  // Path to data for structure XY values: used for placing patient 
  // points in Case Space:
  QString dukeXYDataPath;
  QString dukeXYDataSuffix;

  // XY Values:
  QMenu *XYDataAngleMenu;
  QMenu *XValuesMenu;
  QMenu *YValuesMenu;
  int XYValue[numXYValueTypes];
  int minVal[numXYValueTypes];
  int maxVal[numXYValueTypes];
  XYValueType xValueType;
  XYValueType yValueType;

  Patient *dukePatientList;
  Patient *queryCase;
  Patient *matchCase;
  Patient *lastMatchCase;

  bool newMatchCaseSelectedHere;

  // Zero-based indices into array of Patients: the first position
  // is of course 0 even though the first patient number is 3 or
  // something. Also, there may be gaps between patient numbers.
  int queryCaseIndex; 
  int lastMatchCaseIndex;
  int currMatchCaseIndex;

  CompareDialog *compareDialog;

private:
  CaseSpaceDialog(const CaseSpaceDialog&);  // Not implemented.
  void operator=(const CaseSpaceDialog&);   // Not implemented.
};

#endif
