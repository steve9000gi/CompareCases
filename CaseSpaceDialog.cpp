////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.cpp: Qt-based GUI for prostate cancer radiation therapy 
// planning tool. This window displays a 3D graph showing the relative
// distances of the selected sets of cases from the query case with respect to
// several criteria, e.g., PTV + bladder overlap for the x axis, PTV + 
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

#include <QtGui>

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkCubeSource.h"
#include "vtkSphereSource.h"
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
#include "vtkStringArray.h"
#include "vtkTextProperty.h"
#include "vtkPen.h"

#include "CaseSpaceDialog.h"
#include "CompareDialog.h"
#include "MainWindow.h"
#include "Patient.h"
#include "Projector.h"
#include "Axes.h"

#include "ccInteractorStyleTrackballCamera.h"

#include <stdint.h>

using namespace std;

static const char *MIAvgFileName = "/MI/mi_avg1-7.txt";

const double CaseSpaceDialog::numMIColors = 100;
const double nearClip = 1.0;
const double farClip = 75760.0;
const int numXYDataAngleMenuItems = 8;

QString angleItemText[numXYDataAngleMenuItems] =
{  
  "Average over all gantry angles",
  "025 degrees",
  "075 degrees",
  "130 degrees",
  "180 degrees",
  "230 degrees",
  "280 degrees",
  "330 degrees"
};

char *XYValuesItemText[CaseSpaceDialog::numXYValueTypes] =
{  
  "PTV size",
  "Rectum size",
  "Bladder size",
  "PTV + rectum",
  "PTV + bladder",
  "PTV + rectum + bladder"
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
  : mainWindow(mw),
    dukeXYDataPath(mainWindow->getDukeXYDataPath()),
    dukeXYDataSuffix("Avg.txt"),
    caseSpaceView(NULL),
    selectedMatchPlotPosition(NULL),
    dX(NULL),
    dY(NULL),
    pX(NULL),
    pY(NULL),
    hpX(NULL),
    hpY(NULL),
    caseSpaceRenWin(NULL),
    renderWindowInteractor(NULL),
    ren(NULL),
    iStyle(NULL),
    dukePatientList(NULL),
    queryCase(NULL),
    matchCase(NULL),
    lastMatchCase(NULL),
    compareDialog(NULL),
    MIMax(-FLT_MAX),
    MIMin(FLT_MAX),
    MIRange(0.0),
    MIThresholdPlane(NULL),
    mitpActor(NULL),
    queryCaseIndex(-1),     // Impossible dummy value -> uninitialized
    balloonWidget(NULL),
    balloonRep(NULL),
    zMult(10000.0),
    axes(NULL),
    stdCamDist(33700.0),    // Empiricially determined
    stdVertShift(2500.0),   // Add to move camera up, oblique & MI views
    parallelScale(5300.0),
    thresholdPlaneThickness(15.0),
    queryPoint(NULL),
    queryPointMapper(NULL),
    queryPointActor(NULL),
    lastMatchCaseIndex(-1), // Negative until initialized
    currMatchCaseIndex(-1), // Negative until initialized
    MILegend(NULL),
    MILookupTable(NULL),
    newMatchCaseSelectedHere(false),
    xValueType(PTVPlusBladder),
    yValueType(PTVPlusRectum),
    xAxisPlane(NULL),
    yAxisPlane(NULL),
    XYDataAngleMenu(NULL),
    XValuesMenu(NULL),
    YValuesMenu(NULL)
{
  setAttribute(Qt::WA_DeleteOnClose);

  matchIcon[0] = NULL;    // We'll just check the 0th element

  initXYValExtrema();

  this->setupUi(this);
  this->readMIData();
  this->setupCaseSpaceRenWin();
  this->prepareMIDisplay();
  this->addQueryCase();
  this->addMIThresholdPlane();
  // this->averageMIData(); Only needed to run it once
  this->createActions();
  setupXYDataAngleMenu();
  setupXValuesMenu();
  setupYValuesMenu();

  testPushButton->hide();

  compareCasesPushButton->setEnabled(false);

  ren->GetActiveCamera()->ParallelProjectionOn(); 
  setObliqueView(true);
  viewThresholdAxesCheckBox->setChecked(true);
    viewAxisPlanesCheckBox->setChecked(true);
  viewThresholdPlaneCheckBox->setChecked(true);
}

///dtor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CaseSpaceDialog::~CaseSpaceDialog()
{
  if (caseSpaceView) caseSpaceView->Delete();
  delete selectedMatchPlotPosition;
  delete[] dX;
  delete[] dY;
  delete[] pX;
  delete[] pY;
  delete[] hpX;
  delete[] hpY;

  if (ren) ren->Delete();
  if (iStyle) iStyle->Delete();

  for (int i = 0; i < numDukePatients; i++)
  {
    if (dukePoint[i]) dukePoint[i]->Delete();
    if (dukePointMapper[i]) dukePointMapper[i]->Delete();
    if (dukePointActor[i]) dukePointActor[i]->Delete();
  }

  if (queryPoint) queryPoint->Delete();
  if (queryPointMapper) queryPointMapper->Delete();
  if (queryPointActor) queryPointActor->Delete();

  if (matchIcon[0])
  {
    for (int i = 0; i < numMatchIconLevels; i++)
    {
      matchIcon[i]->Delete();
      matchIconMapper[i]->Delete();
      matchIconActor[i]->Delete();
    }
  }

  if (balloonWidget)
  {
    balloonWidget->Delete();
    balloonRep->Delete();
  }

  if (MILegend) MILegend->Delete();
  if (MILookupTable) MILookupTable->Delete();

  delete axes;
  if (axesAssembly) axesAssembly->Delete();
  if (thresholdAxesAssembly) thresholdAxesAssembly->Delete();

  for (int i = 0; i < numMICases; i++)
  {
    delete[] MIval[i];
  }

  if (MIThresholdPlane) MIThresholdPlane->Delete();
  if (mitpActor) mitpActor->Delete();

  if (xAxisPlane)
  {
    xAxisPlane->Delete();
    xAxisPlaneActor->Delete();
  }

  if (yAxisPlane)
  {
    yAxisPlane->Delete();
    yAxisPlaneActor->Delete();
  }

  if (XYDataAngleMenu) delete XYDataAngleMenu;
  if (XValuesMenu) delete XValuesMenu;
  if (YValuesMenu) delete YValuesMenu;

  if (dukePatientList) delete[] dukePatientList;

  mainWindow->setCaseSpaceDialogPointerToNULL();

  if (compareDialog) compareDialog->close();
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

    if (lastMatchCaseIndex != currMatchCaseIndex)
    {
      lastMatchCaseIndex = currMatchCaseIndex;
      newMatchCaseSelectedHere = true;

      if (!matchIcon[0])
      {
        initializeMatchIcon();
      }

      makeCubeIfSphere(currMatchCaseIndex);

      double *center = dukePointActor[currMatchCaseIndex]->GetCenter();
      setMatchIconLocation(center[0], center[1], center[2]);
    }

    displayMatchCaseData();

    if (!compareDialog)
    {
      compareCasesPushButton->setEnabled(true);
    }
    else
    {  
      bool CTDataOK, DVHDataOK;

      if (!(CTDataOK = compareDialog->CTDataExistsFor(matchCase)))
      {
        QString warn = "Can't access CT data for " + matchInstitution
          + " patient #" + QString::number(matchCase->getNumber());
        QMessageBox::warning(this, tr("Missing data"), warn);
      }

      if (!(DVHDataOK = compareDialog->DVHDataExistsFor(matchCase)))
      {
        QString warn = "Can't access DVH data for " + matchInstitution
          + " patient #" + QString::number(matchCase->getNumber());
        QMessageBox::warning(this, tr("Missing data"), warn);
      }

      if (CTDataOK && DVHDataOK)
      {
        compareDialog->setMatch(matchCase);
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
    lastMatchCaseIndex = currMatchCaseIndex;
    currMatchCaseIndex = getIndexFrom(matchCase);

    if (!matchIcon[0])
    {
      initializeMatchIcon();
    }

    makeCubeIfSphere(currMatchCaseIndex);

    double *center = dukePointActor[currMatchCaseIndex]->GetCenter();
    setMatchIconLocation(center[0], center[1], center[2]);

    matchCaseNameLabel->setText(matchInstitution + " patient #"
      + QString::number(matchCase->getNumber()));

    displayMatchCaseData();

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
// 2do: Generalize to all institutions. For now assumes it's a Duke patient.  `
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
// When the match case is changed in caseSpaceDialog, the compareDialog's
// matchSelectSpinBox is updated, which calls the associated slot, that is, 
// selectMatch(int patientNumber), which updates the caseSpaceDialog's 
// lastMatchCase variable to the current matchCase.  This is necessary when 
// selectMatch(int patientNumber) is invoked by direct user manipulation of the
// matchSelectSpinBox, but would end with an incorrect patient (the newly
// selected) being placed in the matchHistory if the selection was made from
// Case Space. Hence this variable is used to skip the incorrect lastMatchCase
// update for the latter case.
//
////////////////////////////////////////////////////////////////////////////////
bool CaseSpaceDialog::isNewMatchCaseSelectedHere()
{
  bool returnVal = newMatchCaseSelectedHere;
  newMatchCaseSelectedHere = false;

  return returnVal;
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

    double *center = pickedActor->GetCenter();

    //cout << "Picked actor center at (" << center[0] << ", " << center[1]
    //     << ", " << center[2] << ")" << endl;

    selectedMatchPlotPosition->SetX(center[0]);
    selectedMatchPlotPosition->SetY(center[1]);
    identifyMatchCase();
  }

  return true;
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

////////////////////////////////////////////////////////////////////////////////
//
// If compareDialog already exists then don't do anything.
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

    bool missingCTData, missingDVHData;

    if (missingCTData = !compareDialog->CTDataExistsFor(matchCase))
    {
      QString warn = "Can't access CT data for " + matchInstitution
        + " patient #" + QString::number(matchCase->getNumber());
      QMessageBox::warning(this, tr("Missing data"), warn);
    }

    if (missingDVHData = !compareDialog->DVHDataExistsFor(matchCase))
    {
      QString warn = "Can't access DVH data for " + matchInstitution
        + " patient #" + QString::number(matchCase->getNumber());
      QMessageBox::warning(this, tr("Missing data"), warn);
    }

    if (missingCTData || missingDVHData)
    {  
      // The code depends on compareDialog being either fully 
      // instantiated or NULL:
      CompareDialog *cd = compareDialog;
      delete cd;
      compareDialog = NULL;
    }
    else
    {
      compareDialog->readIsocenters(queryCase);
      compareDialog->setQuery(queryCase);
      compareDialog->setMatch(matchCase);
      compareDialog->show();
    }
    
    compareCasesPushButton->setEnabled(false);
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
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setXYView(bool /* checked = true */ )
{
  double pos[] = { xCenterQueryPt,
           yCenterQueryPt,
           zCenterQueryPt + stdCamDist };
  double fp[] = { xCenterQueryPt,
          yCenterQueryPt,
          zCenterQueryPt };
  double vUp[] = {0, 1, 0 };
  double clip[] = { nearClip, farClip };
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
  double clip[] = { nearClip, farClip };
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
  double clip[] = { nearClip, farClip };
  double zoom = 1.0 / parallelScale;

  SetCameraPosition(pos, fp, vUp, clip, zoom);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setObliqueView(bool /* checked = true */ )
{
  // Normalized view vector, empirically determined:
  double nVV[3] = { 0.759418157,  -0.628362612,  0.168655249 }; 

  double pos[] = { nVV[0] * stdCamDist + xCenterQueryPt,
                   nVV[1] * stdCamDist + yCenterQueryPt,
                  (nVV[2] * stdCamDist + zCenterQueryPt) + stdVertShift };
  double fp[] = { xCenterQueryPt, 
                  yCenterQueryPt, 
                  zCenterQueryPt + stdVertShift };
  double vUp[] = { 0, 0, 1 };
  double clip[] = { nearClip, farClip };
  double zoom = 1.0 / parallelScale;

  SetCameraPosition(pos, fp, vUp, clip, zoom);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::resetView()
{
  if (XYViewRadioButton->isChecked())
  {
    setXYView();
  }
  else if (XMIViewRadioButton->isChecked())
  {
    setXMIView();
  }
  else if (YMIViewRadioButton->isChecked())
  {
    setYMIView();
  }
  else if (obliqueViewRadioButton->isChecked())
  {
    setObliqueView();
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setThresholdPlanePosition(int zVal)
{
  double sliderRange = MIRangeSlider->maxValue() - MIRangeSlider->minValue();
  MIThresholdVal = (MIMin + (MIRange * zVal / sliderRange));
  thresholdPlaneZVal = MIThresholdVal * zMult;

  //cout << "MI threshold = " << MIThresholdVal
  //   << "; threshold plane z val = " << thresholdPlaneZVal << endl;

  MIThresholdPlane->SetCenter(
    (maxVal[xValueType] - minVal[xValueType]) / 2.0 + minVal[xValueType],
    (maxVal[yValueType] - minVal[yValueType]) / 2.0 + minVal[yValueType],
    thresholdPlaneZVal);

  for (int i = 0; i < numMICases; i++)
  {
    if (i == queryCaseIndex) continue;

    double center[3];
    vtkCubeSource *cube = vtkCubeSource::SafeDownCast(dukePoint[i]);

    if (cube)
    {
      cube->GetCenter(center);
    }
    else
    {
      vtkSphereSource *sphere = vtkSphereSource::SafeDownCast(dukePoint[i]);

      if (sphere)
      {
        sphere->GetCenter(center);
      }
    }

    bool belowThreshold = (center[2] < thresholdPlaneZVal);
    dukePointActor[i]->GetProperty()->SetOpacity(belowThreshold ? 0 : 1);

    if (belowThreshold) dukePointActor[i]->PickableOff();
    else dukePointActor[i]->PickableOn();

    QString thresholdAsText;
    thresholdAsText.setNum(MIThresholdVal, 'g', 3);

    MIRangeGroupBox->setTitle("MI threshold: " + thresholdAsText);
  }

  if (!xAxisPlane) addAxisPlanes();

  setAxisPlanes(thresholdPlaneZVal);

  thresholdAxesAssembly->SetPosition(
    getXYValueFromIndex(queryCaseIndex, xValueType),
    getXYValueFromIndex(queryCaseIndex, yValueType),
    thresholdPlaneZVal);

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
    ren->SetBackground(1, 1, 1);
    ren->SetBackground2(0, 0, 0);
  }

  caseSpaceRenWin->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Using the "sender" approach, considered dangerous because of the cast.  
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::XYDataAngleItemTriggered(bool checked)
{
  QAction *action = (QAction *)sender();
  emit selectXYDataAngle(action->text());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::selectXYDataAngle(QString text)
{
  QString nAsText = text.section(' ', 0, 0);
  bool isNum;
  int num = nAsText.toInt(&isNum);
  QByteArray ba = nAsText.toUtf8();
  dukeXYDataSuffix = ((isNum) ? (nAsText) : "Avg") + ".txt";  

  resetDukeDataPositions(true);
  setAxisPlanes(thresholdPlaneZVal);
  thresholdAxesAssembly->SetPosition(
    getXYValueFromIndex(queryCaseIndex, xValueType),
    getXYValueFromIndex(queryCaseIndex, yValueType),
    thresholdPlaneZVal);
  resetView();

  for (int i = 0; i < numXYDataAngleMenuItems; i++)
  {
    QString currText = XYDataAngleMenu->actions().at(i)->text();;
    XYDataAngleMenu->actions().at(i)->setChecked(!text.compare(currText));
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::XValuesItemTriggered(bool checked)
{
  QAction *action = (QAction *)sender();
  emit selectXValues(action->text());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::selectXValues(QString text)
{
  XYValueType lastValueType = xValueType;

  for (int i = PTVSize; i < numXYValueTypes; i++)
  {
    QString currText = XValuesMenu->actions().at(i)->text();;
    XValuesMenu->actions().at(i)->setChecked(!text.compare(currText));

    if (XValuesMenu->actions().at(i)->isChecked())
    {
      xValueType = (XYValueType)i;
    }
  }

  if (lastValueType != xValueType)
  {
    resetDukeDataPositions();
    setAxisPlanes(thresholdPlaneZVal);
    thresholdAxesAssembly->SetPosition(
      getXYValueFromIndex(queryCaseIndex, xValueType),
      getXYValueFromIndex(queryCaseIndex, yValueType),
      thresholdPlaneZVal);
    resetView();
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::YValuesItemTriggered(bool checked)
{
  QAction *action = (QAction *)sender();
  emit selectYValues(action->text());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::selectYValues(QString text)
{
  //QMessageBox::information(this, "selectYValues", text);
  XYValueType lastValueType = yValueType;

  for (int i = PTVSize; i < numXYValueTypes; i++)
  {
    QString currText = XValuesMenu->actions().at(i)->text();
    YValuesMenu->actions().at(i)->setChecked(!text.compare(currText));

    if (YValuesMenu->actions().at(i)->isChecked())
    {
      yValueType = (XYValueType)i;
    }
  }

  if (lastValueType != yValueType)
  {
    resetDukeDataPositions();
    setAxisPlanes(thresholdPlaneZVal);
    thresholdAxesAssembly->SetPosition(
      getXYValueFromIndex(queryCaseIndex, xValueType),
      getXYValueFromIndex(queryCaseIndex, yValueType),
      thresholdPlaneZVal);
    resetView();
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::toggleAxisPlanes(bool checked)
{
  if (checked)
  {
    ren->AddActor(xAxisPlaneActor);
    ren->AddActor(yAxisPlaneActor);
  }
  else
  {
    ren->RemoveActor(xAxisPlaneActor);
    ren->RemoveActor(yAxisPlaneActor);
  }

  caseSpaceRenWin->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::toggleThresholdAxes(bool checked)
{
  if (checked)
  {
    ren->AddActor(thresholdAxesAssembly);
  }
  else
  {
    ren->RemoveActor(thresholdAxesAssembly);
  }

  caseSpaceRenWin->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::toggleThresholdPlane(bool checked)
{
  if (checked)
  {
    ren->AddActor(mitpActor);
  }
  else
  {
    ren->RemoveActor(mitpActor);
  }

  caseSpaceRenWin->Render();
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

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate code with the GUI controls.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::createActions()
{
  connect(compareCasesPushButton, SIGNAL(released()), this, SLOT(compareCases()));
  connect(testPushButton, SIGNAL(clicked()), this, SLOT(testFunction()));
  connect(XYViewRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setXYView(bool)));
  connect(XMIViewRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setXMIView(bool)));
  connect(YMIViewRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setYMIView(bool)));
  connect(obliqueViewRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setObliqueView(bool)));
  connect(resetViewPushButton, SIGNAL(clicked()), this, 
    SLOT(resetView()));
  connect(MIRangeSlider, SIGNAL(valueChanged(int)), this, 
    SLOT(setThresholdPlanePosition(int)));
  connect(backgroundBlackRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setBackgroundBlack(bool)));  
  connect(backgroundWhiteRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setBackgroundWhite(bool)));  
  connect(backgroundRampRadioButton, SIGNAL(toggled(bool)), this, 
    SLOT(setBackgroundRamped(bool)));

  connect(viewThresholdAxesCheckBox, SIGNAL(toggled(bool)), this, 
    SLOT(toggleThresholdAxes(bool)));
  connect(viewThresholdPlaneCheckBox, SIGNAL(toggled(bool)), this, 
    SLOT(toggleThresholdPlane(bool)));
  connect(viewAxisPlanesCheckBox, SIGNAL(toggled(bool)), this, 
    SLOT(toggleAxisPlanes(bool)));

  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
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
  //caseSpaceRenWin->SetSize(920, 569);
  caseSpaceRenWin->SetSize(1473, 897);

  ren = vtkRenderer::New();
  ren->SetBackground(0, 0, 0);
  ren->SetBackground2(0, 0, 0);
  ren->SetGradientBackground(true);
  ren->SetRenderWindow(caseSpaceRenWin);
  caseSpaceRenWin->AddRenderer(ren);

  pickCallback = PickCallback::New();
  pickCallback->SetCaseSpaceDialog(this);

  picker = vtkCellPicker::New();
  picker->SetTolerance(0.0);
  picker->AddObserver(vtkCommand::EndPickEvent, pickCallback);

  renderWindowInteractor = caseSpaceViewWidget->GetInteractor();
  renderWindowInteractor->SetRenderWindow(caseSpaceRenWin);
  renderWindowInteractor->SetPicker(picker);

  vtkInteractorObserver *o = renderWindowInteractor->GetInteractorStyle();
  const char *className = o->GetClassName();

  iStyle = ccInteractorStyleTrackballCamera::New();
  renderWindowInteractor->SetInteractorStyle(iStyle);

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
void CaseSpaceDialog::addQueryCase()
{
  queryPoint = vtkCubeSource::New();
  xCenterQueryPt = queryCase->getPTVPlusBladder();
  yCenterQueryPt = queryCase->getPTVPlusRectum();
  zCenterQueryPt = MIMin * zMult;
  queryPoint->SetCenter(xCenterQueryPt, yCenterQueryPt, zCenterQueryPt);
  queryPoint->SetXLength(200.0);
  queryPoint->SetYLength(200.0);
  queryPoint->SetZLength(200.0);

  queryPointMapper = vtkPolyDataMapper::New();
  queryPointMapper->SetInputConnection(queryPoint->GetOutputPort());

  queryPointActor = vtkActor::New();
  queryPointActor->SetMapper(queryPointMapper);
  queryPointActor->GetProperty()->SetColor(1.0, 1.0, 0.0); // yellow
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
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::initializeBalloonStuff()
{
  balloonWidget = vtkBalloonWidget::New();
  balloonRep = vtkBalloonRepresentation::New();
  balloonRep->SetBalloonLayoutToImageRight();
  balloonWidget->SetInteractor(renderWindowInteractor);
  balloonWidget->SetRepresentation(balloonRep);
  balloonWidget->SetTimerDuration(200);
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
void CaseSpaceDialog::updateBalloon(int i, int xVal, int yVal)
{
  QString n;
  n.setNum(dukePatientList[i].getNumber());

  QString xString;
  xString.setNum(xVal);

  QString yString;
  yString.setNum(yVal);

  QString zString;
  zString.setNum(MIval[queryCaseIndex][i], 'g', 3);

  QString s = "Duke patient #" + n + ":\n" + xString + ", "
              + yString + ", " + zString;
  QByteArray a = s.toAscii();

  balloonWidget->UpdateBalloonString(dukePointActor[i], a.data());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::initializeMatchIcon()
{
  static const double increment = 0.06;

  for (int i = 0; i < numMatchIconLevels; i++)
  {
    matchIcon[i] = vtkSphereSource::New();
    double radius = cubeSize * 0.7 * (1.0 + ((i + 1.0) * increment));
    matchIcon[i]->SetRadius(radius);
    matchIconMapper[i] = vtkPolyDataMapper::New();
    matchIconMapper[i]->SetInputConnection(matchIcon[i]->GetOutputPort());

    matchIconActor[i] = vtkActor::New();
    matchIconActor[i]->SetMapper(matchIconMapper[i]);
    matchIconActor[i]->PickableOff();
    matchIconActor[i]->GetProperty()->SetAmbient(1.0);
    matchIconActor[i]->GetProperty()->SetDiffuse(0.0);
    matchIconActor[i]->GetProperty()->SetSpecular(0.0);
    matchIconActor[i]->GetProperty()->SetAmbientColor(0, 1, 1);    // cyan

    const double halfPi = 1.57079632679489661923;

    double mult = (i == 0) ? 1.0 : ((i < 6) ? 0.6 : ((i < 15) ? 0.45 : 0.3));
    double opacity = 
      mult * (1.0 - sin(halfPi * ((i + 1.0) / numMatchIconLevels))); 
    matchIconActor[i]->GetProperty()->SetOpacity(opacity);

    ren->AddActor(matchIconActor[i]);
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setMatchIconLocation(double x, double y, double z)
{
  for (int i = 0; i < numMatchIconLevels; i++)
  {
    matchIcon[i]->SetCenter(x, y, z);
  }

  caseSpaceRenWin->Render();
}

////initializeMILegend//////////////////////////////////////////////////////////
//
// Make sure MIMin and MIMax have been generated before calling this method.
//
// Assumes that pointers MILegend and MILookupTable are set to 0 if not already
// instantiated.
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

  MILookupTable->SetNumberOfTableValues(numMIColors);
  
  double r, g, b;

  for (int i = 0; i < numMIColors; i++)
  {
    double MIVal = MIMin + ((double)i / numMIColors) * MIRange;
    getColorCorrespondingToMIvalue(MIVal, r, g, b);
    MILookupTable->SetTableValue(i, r, g, b);
  }

  MILookupTable->Build();

  MILegend->SetLookupTable(MILookupTable);
  MILegend->SetTitle("MI Values");
  MILegend->SetOrientationToHorizontal();
  MILegend->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
  MILegend->GetPositionCoordinate()->SetValue(0.849, 0.95);
  MILegend->SetWidth(0.114);
  MILegend->SetHeight(0.05);
  MILegend->GetTitleTextProperty()->ItalicOff();
  MILegend->GetLabelTextProperty()->ItalicOff();
  ren->AddActor2D(MILegend);
}

///getColorCorrespondingToMIValue///////////////////////////////////////////////
//
// Assumes you've already generated MIMin and MIMax -- the extrema for the data
// to which you're applying the color map. Then define the number of colorNodes
// and make sure there's a row of three double values (representing r, g, and b
// in a 0.0-1.0 range) for each node. Then call this method for with parameter 
// MIVal some double value between MIMin and MIMax inclusive. The corresponding 
// rgb values will be returned in the reference-to-double parameters r, g, and
// b. 
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::getColorCorrespondingToMIvalue(double MIVal,
  double &r, double &g, double &b)
{
/*
  static const int numColorNodes = 5;
  double color[numColorNodes][3] =
  {
    //0.1, 0.1, 0.2,    // dark blue-gray
    0.1, 0.1, 0.35,    // fairly dark blue-gray
    0.1, 0.1, 0.5,    // blue
    0.5, 0, 0.25,    // purple
    0.5, 0, 0,      // maroon
    1, 1, 0        // yellow
  };
*/
/*static const int numColorNodes = 4;
  double color[numColorNodes][3] =
  {
    0.1, 0.1, 0.35,    // fairly dark blue-gray
    0.1, 0.1, 0.5,    // blue
    1.0, 0.5, 0.0,   // orange
    1, 0, 0         // red
  };
*/
/*  // A lighter palette for increased visibility on a (dimmer) projector:
  static const int numColorNodes = 4;
  double color[numColorNodes][3] =
  {
    0.2, 0.2, 0.45,    // fairly dark blue-gray
    0.2, 0.2, 0.6,    // blue
    1.0, 0.6, 0.1,   // orange
    1, 0.1, 0.1     // red
  };
*/
/*  // That's good. Lighter yet:
  static const int numColorNodes = 4;
  double color[numColorNodes][3] =
  {
    0.3, 0.3, 0.55,    // fairly dark blue-gray
    0.3, 0.3, 0.7,    // blue
    1.0, 0.7, 0.2,   // orange
    1, 0.2, 0.2     // red
  };
*/
 // One more iteration (orange is getting a little hot):
  static const int numColorNodes = 4;
  double color[numColorNodes][3] =
  {
    0.4, 0.4, 0.65,    // fairly dark blue-gray
    0.4, 0.4, 0.8,    // blue
    1.0, 0.7, 0.2,   // orange
    1, 0.3, 0.3     // red
  };

/*  static const int numColorNodes = 3;
  double color[numColorNodes][3] =
  {
    0.0, 0.0, 1.0,    // blue
    1.0, 1.0, 1.0,   // white
    1.0, 0.0, 0.0   // red
  };
*/
  for (int i = 0; i < (numColorNodes - 1); i++)
  {
    double currFloor = MIMin + ((double)i / (numColorNodes - 1)) * MIRange;
    double currCeil = MIMin + ((double)(i + 1) / (numColorNodes - 1)) * MIRange;

    if ((MIVal >= currFloor) && (MIVal <= currCeil))
    {
      double currFraction = (MIVal - currFloor) / (currCeil - currFloor);
      r = color[i][0] * (1.0 - currFraction) + color[i + 1][0] * currFraction;
      g = color[i][1] * (1.0 - currFraction) + color[i + 1][1] * currFraction;
      b = color[i][2] * (1.0 - currFraction) + color[i + 1][2] * currFraction;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::initXYValExtrema()
{
  for (int i = PTVSize; i < numXYValueTypes; i++)
  {
    minVal[i] = INT32_MAX;
    maxVal[i] = -1; // All actual values will be non-negative
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Read XY data for all Duke cases, getting extrema while we're at it. Also note
// which Patient object in the array is the Query Case.
//
////////////////////////////////////////////////////////////////////////////////
bool CaseSpaceDialog::readDukeXYData()
{
  QString fullPath = dukeXYDataPath + dukeXYDataSuffix;

  QFile file(fullPath);

  if (!file.open(QIODevice::ReadOnly))
  {
    QString warn = "Failed to open \"" + fullPath + "\"";
    QMessageBox::warning(this, tr("File Open Failed"), warn);
    return false;
  }

  QTextStream in(&file);
  
  in.readLine();    // Skip over the first line (column headers):
  numDukePatients = 0;

  while (!in.atEnd()) // Count the number of patients
  {
    in.readLine();
    numDukePatients++;
  }

  in.flush();

  if (!in.seek(0))
  {
    QString warn = "readDukeXYData(...): seek(0) failed";
    QMessageBox::warning(this, 
      tr("failure attempting to return to file start"), warn);
    return false; 
  }

  if (!dX)
  {
    dX = new double[numDukePatients];
    dY = new double[numDukePatients];
  }

  initXYValExtrema();
  
  int n; 

  if (!dukePatientList) dukePatientList = new Patient[numDukePatients];

  for (int i = 0; i < numDukePatients; i++)
  {
    (dukePatientList[i]).setDataDir(mainWindow->getDataDir());
  }

  in.readLine();    // Skip over the first line (column headers):

  MainWindow::SourceInstitution sourceInstitution =
    mainWindow->getQueryCaseSourceInstitution();
  int queryCasePatientNumber = mainWindow->getQueryCasePatientNumber();
  int ix = 0;

  while ((!in.atEnd()) && (ix < numDukePatients)) // else 1 too far: final newline?
  {
    in >> n >> XYValue[PTVSize] >> XYValue[rectumSize] >> XYValue[bladderSize]
       >> XYValue[PTVPlusRectum] >> XYValue[PTVPlusBladder]
       >> XYValue[PTVPlusRectumPlusBladder];

    if ((n == queryCasePatientNumber) && (sourceInstitution == MainWindow::kDuke))
    {
      queryCase = &(dukePatientList[ix]);
      queryCaseIndex = ix;
    }

    dukePatientList[ix].setNumber(n);
    dukePatientList[ix].setIndex(ix);
    dukePatientList[ix].setPTVSize(XYValue[PTVSize]);
    dukePatientList[ix].setRectumSize(XYValue[rectumSize]);
    dukePatientList[ix].setBladderSize(XYValue[bladderSize]);
    dukePatientList[ix].setPTVPlusRectum(XYValue[PTVPlusRectum]);
    dukePatientList[ix].setPTVPlusBladder(XYValue[PTVPlusBladder]);
    dukePatientList[ix].setPTVPlusRectumPlusBladder(
      XYValue[PTVPlusRectumPlusBladder]);

    for (int vType = PTVSize; vType < numXYValueTypes; vType++)
    {
      if (XYValue[vType] < minVal[vType]) minVal[vType] = XYValue[vType];
      if (XYValue[vType] > maxVal[vType]) maxVal[vType] = XYValue[vType];
    }

    ix++;
  }

  file.close();

  return true;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addDukeDataToGraph()
{
  if (!readDukeXYData()) return;

  for (int i = 0; i < numDukePatients; i++)
  {
    dX[i] = getXYValueFromIndex(i, xValueType);
    dY[i] = getXYValueFromIndex(i, yValueType);

    if (i == queryCaseIndex) 
    {  // Don't draw the usual for the query case
      dukePoint[i] = NULL;
      dukePointMapper[i] = NULL;
      dukePointActor[i] = NULL;
    }
    else
    {
      vtkCubeSource *cube = vtkCubeSource::New();
      dukePoint[i] = cube;
      cube->SetCenter(dX[i], dY[i], MIval[queryCaseIndex][i] * zMult);
      cube->SetXLength(cubeSize);
      cube->SetYLength(cubeSize);
      cube->SetZLength(cubeSize);

      dukePointMapper[i] = vtkPolyDataMapper::New();
      dukePointMapper[i]->SetInputConnection(dukePoint[i]->GetOutputPort());

      dukePointActor[i] = vtkActor::New();
      dukePointActor[i]->SetMapper(dukePointMapper[i]);

      addBalloon(i);
    }
  }

  ren->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
// Is the data point representing the patient with index "ix" a cube?  if so,
// replace it with a sphere.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::makeSphereIfCube(int ix)
{
  vtkCubeSource *cube = vtkCubeSource::SafeDownCast(dukePoint[ix]);
            
  if (cube)
  {
    cube->Delete();
    vtkSphereSource *sphere = vtkSphereSource::New();
    dukePoint[ix] = sphere;
    sphere->SetRadius(cubeSize);
    dukePointMapper[ix]->SetInputConnection(sphere->GetOutputPort());
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Is the data point representing the patient with index "ix" a sphere?  if so,
// replace it with a cube.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::makeCubeIfSphere(int ix)
{
  vtkSphereSource *sphere = vtkSphereSource::SafeDownCast(dukePoint[ix]);

  if (sphere)
  {
    sphere->Delete();
    vtkCubeSource *cube = vtkCubeSource::New();
    dukePoint[ix] = cube;
    cube->SetXLength(cubeSize);
    cube->SetYLength(cubeSize);
    cube->SetZLength(cubeSize);
    dukePointMapper[ix]->SetInputConnection(sphere->GetOutputPort());
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Is the patient with index "ix" in the CompareDialog match history? If so,
// then make sure its Case Space point representation is a sphere, else make
// sure it's a cube.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setGeometryAccordingToMatchHistory(int ix)
{
  if (compareDialog)
  {
    QMenu *matchHistoryMenu = compareDialog->getMatchHistoryMenu();

    if (!matchHistoryMenu) return;

    bool isHistoryItem = false;

    int nHistoryItems = matchHistoryMenu->actions().size();

    for (int currHIx = 0; currHIx < nHistoryItems; currHIx++)
    {
      QString n1AsStr = 
        matchHistoryMenu->actions().at(currHIx)->text().section('#', 1);
      QString n2AsStr = QString::number(dukePatientList[ix].getNumber());
  
      if (!n1AsStr.compare(n2AsStr)) // then dukePatientList[ix] is...  
      {  // ...in the match history, so...
        isHistoryItem  = true;
      }
    }

    if (isHistoryItem) // ...it should be a sphere
    {
      makeSphereIfCube(ix);
    }
    else  // dukePatientList[ix] is not in the match history...
    {    // ...so it should be a cube...
      makeCubeIfSphere(ix);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//
// Put the axes, the query point, the threshold plane, the camera, and the
// current match case point in the right places, oriented correctly, and make
// sure the axis labels and the query and match case data in the upper-right-
// hand corner are correct.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::resetAppurtenances()
{
  xCenterQueryPt = getXYValueFromIndex(queryCaseIndex, xValueType);
  yCenterQueryPt = getXYValueFromIndex(queryCaseIndex, yValueType);
  zCenterQueryPt = MIMin * zMult;

  axes->setAxisLabel(Axes::xAxis, XYValuesItemText[xValueType]);
  axes->setAxisLabel(Axes::yAxis, XYValuesItemText[yValueType]);
  axes->setAxisLabelPosition(Axes::xAxis, xCenterQueryPt + axisTextOffset,
    yCenterQueryPt, zCenterQueryPt);

  axes->setAxisLabelPosition(Axes::yAxis, xCenterQueryPt,
    yCenterQueryPt + axisTextOffset, zCenterQueryPt);
  axes->setAxisLabelPosition(Axes::zAxis, xCenterQueryPt,
    yCenterQueryPt, zCenterQueryPt + axisTextOffset);
  axesAssembly->SetPosition(xCenterQueryPt, yCenterQueryPt, zCenterQueryPt);

  queryPoint->SetCenter(xCenterQueryPt, yCenterQueryPt, zCenterQueryPt);
  
  MIThresholdPlane->SetCenter(
    (maxVal[xValueType] - minVal[xValueType]) / 2.0 + minVal[xValueType],
    (maxVal[yValueType] - minVal[yValueType]) / 2.0 + minVal[yValueType],
    thresholdPlaneZVal);
  MIThresholdPlane->SetXLength(maxVal[xValueType] - minVal[xValueType]);
  MIThresholdPlane->SetYLength(maxVal[yValueType] - minVal[yValueType]);

  displayQueryCaseData();

  if (currMatchCaseIndex != -1)
  {
    displayMatchCaseData();
    makeCubeIfSphere(currMatchCaseIndex);

    double *center = dukePointActor[currMatchCaseIndex]->GetCenter();
    setMatchIconLocation(center[0], center[1], center[2]);
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::resetDukeDataPositions(bool needToRead /* = false */ )
{
  if (needToRead)
  {
    if (!readDukeXYData()) return;
  }

  for (int i = 0; i < numDukePatients; i++)
  {
    dX[i] = getXYValueFromIndex(i, xValueType);
    dY[i] = getXYValueFromIndex(i, yValueType); 

    if (i == queryCaseIndex) continue; // query case drawn differently

    setGeometryAccordingToMatchHistory(i);

    vtkCubeSource *cube = vtkCubeSource::SafeDownCast(dukePoint[i]);

    if (cube)
    {
      cube->SetCenter(dX[i], dY[i], MIval[queryCaseIndex][i] * zMult);
    }
    else
    {
      vtkSphereSource *sphere = vtkSphereSource::SafeDownCast(dukePoint[i]);

      if (sphere)
      {
        sphere->SetCenter(dX[i], dY[i], MIval[queryCaseIndex][i] * zMult);
      }
    }

    updateBalloon(i, dX[i], dY[i]);
  }

  resetAppurtenances();

  renderWindowInteractor->Render();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
double CaseSpaceDialog::getXYValueFromIndex(int ix, XYValueType valueType)
{
  switch (valueType)
  {
    case PTVSize:
      return dukePatientList[ix].getPTVSize();
      break;
    case rectumSize:
      return dukePatientList[ix].getRectumSize();
      break;
    case bladderSize:
      return dukePatientList[ix].getBladderSize();
      break;
    case PTVPlusRectum:
      return dukePatientList[ix].getPTVPlusRectum();
      break;
    case PTVPlusBladder:
      return dukePatientList[ix].getPTVPlusBladder();
      break;
    case PTVPlusRectumPlusBladder:
      return dukePatientList[ix].getPTVPlusRectumPlusBladder();
      break;
    default:
      cout << "CaseSpaceDialog::getXYValueFromIndex(...): "
         << "unknown valueType " << valueType << endl;
      return 0;
      break;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
double CaseSpaceDialog::getXYValueFromNumber(int pNum, XYValueType valType)
{
  return getXYValueFromIndex(getIndexFrom(getDukePatientFrom(pNum)), valType);
}

////////////////////////////////////////////////////////////////////////////////
// 
// Just run this once.  Reads the Duke XY data for each gantry angle and
// outputs a file containing the averages over all angles.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::averageDukeXYData()
{
  QString pre("C:\\Duke_Cases_2011-09-19\\overlap\\Duke_struct_overlap");
  const int kNumFiles = 7;

  QString colHeaders;

  char *post[kNumFiles] =
  { "025.txt", 
    "075.txt", 
    "130.txt", 
    "180.txt", 
    "230.txt", 
    "280.txt", 
    "330.txt" };

  int numDukePatients = 0;
  int prevNumDukePatients = 0;

  // Count how many patients are in the first file:
  QString path0(pre);
  path0.append(post[0]);

  QFile infile0(path0);

  if (!infile0.open(QIODevice::ReadOnly))
  {
    QString warn = "Failed to open \"" + path0 + "\"";
    QMessageBox::warning(this, tr("File Open Failed"), warn);
    return;
  }

  QTextStream in0(&infile0);
  
  in0.readLine();    // Skip over the first line (column headers):

  while (!in0.atEnd()) // Count the number of patients
  {
    in0.readLine();
    numDukePatients++;
  }

  prevNumDukePatients = numDukePatients;

  infile0.close();

  int *n = new int[numDukePatients];
  int *avgPTVSize = new int[numDukePatients];
  int *avgRectumSize = new int[numDukePatients];
  int *avgBladderSize = new int[numDukePatients];
  int *avgPTVPlusRectum = new int[numDukePatients];
  int *avgPTVPlusBladder = new int[numDukePatients];
  int *avgPTVPlusRectumPlusBladder = new int[numDukePatients];

  memset(avgPTVSize, 0, sizeof(int) * numDukePatients);
  memset(avgRectumSize, 0, sizeof(int) * numDukePatients);
  memset(avgBladderSize, 0, sizeof(int) * numDukePatients);
  memset(avgPTVPlusRectum, 0, sizeof(int) * numDukePatients);
  memset(avgPTVPlusBladder, 0, sizeof(int) * numDukePatients);
  memset(avgPTVPlusRectumPlusBladder, 0, sizeof(int) * numDukePatients);

  for (int currFileNum = 0; currFileNum < kNumFiles; currFileNum++)
  {
    QString currPath(pre);
    currPath.append(post[currFileNum]);

    QFile infile(currPath);

    if (!infile.open(QIODevice::ReadOnly))
    {
      QString warn = "Failed to open \"" + currPath + "\"";
      QMessageBox::warning(this, tr("File Open Failed"), warn);
      return;
    }

    QTextStream in(&infile);
  
    in.readLine();    // Skip over the first line (column headers):

    numDukePatients = 0;

    while (!in.atEnd()) // Count the number of patients
    {
      in.readLine();
      numDukePatients++;
    }

    if (numDukePatients != prevNumDukePatients)
    {
      QString warn = "Different numbers of patients";
      QMessageBox::warning(this, tr("File Contents Inconsistent"), warn);
      infile.close();
      return;
    }

    in.flush();

    if (!in.seek(0)) // Go back to the beginning
    {
      QString warn = "Failed to seek(0) \"" + currPath + "\"";
      QMessageBox::warning(this, 
        tr("failure attempting to return to file beginning"), warn);
      return; 
    }

    int PTVSize, rectumSize, bladderSize, PTVPlusRectum, PTVPlusBladder,
      PTVPlusRectumPlusBladder;

     colHeaders = in.readLine();

    int i = 0;

    while ((!in.atEnd()) && (i < numDukePatients)) // else 1 too far: final newline?
    {
      in >> n[i] >> PTVSize >> rectumSize >> bladderSize >> PTVPlusRectum
         >> PTVPlusBladder >> PTVPlusRectumPlusBladder;

      avgPTVSize[i] += PTVSize;
      avgRectumSize[i] += rectumSize;
      avgBladderSize[i] += bladderSize;
      avgPTVPlusRectum[i] += PTVPlusRectum;
      avgPTVPlusBladder[i] += PTVPlusBladder;
      avgPTVPlusRectumPlusBladder[i] += PTVPlusRectumPlusBladder;
      i++;
    }

    infile.close();
  }

  QString outPath(pre);
  outPath.append("Avg.txt");

  QFile outfile(outPath);

  if (!outfile.open(QIODevice::WriteOnly))
  {
    QString warn = "Failed to open \"" + outPath + "\" for writing";
    QMessageBox::warning(this, tr("File Open Failed"), warn);
    return;
  }

  QTextStream out(&outfile);

  out << colHeaders << endl;

  double nF = kNumFiles;

  for (int i = 0; i < numDukePatients; i++)
  {
    out << n[i] << "\t"
      << (int)(avgPTVSize[i] / nF) << "\t"
      << (int)(avgRectumSize[i] / nF) << "\t"      
      << (int)(avgBladderSize[i] / nF) << "\t"      
      << (int)(avgPTVPlusRectum[i] / nF) << "\t"      
      << (int)(avgPTVPlusBladder[i] / nF) << "\t"      
      << (int)(avgPTVPlusRectumPlusBladder[i] / nF) << endl;
  }

  outfile.close();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setupXYDataAngleMenu()
{
  XYDataAngleMenu = new QMenu(this);
  XYDataAnglePushButton->setMenu(XYDataAngleMenu);

  for (int i = 0; i < numXYDataAngleMenuItems; i++)
  {
    QAction *oAction = XYDataAngleMenu->addAction(angleItemText[i]);
    oAction->setCheckable(true);
    connect(oAction, SIGNAL(triggered(bool)),
      this, SLOT(XYDataAngleItemTriggered(bool)));
  }

  XYDataAngleMenu->actions().first()->setChecked(true);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setupXValuesMenu()
{
  XValuesMenu = new QMenu(this);
  XValuesPushButton->setMenu(XValuesMenu);

  for (int v = PTVSize; v < numXYValueTypes; v++)
  {
    QAction *xAction = XValuesMenu->addAction(XYValuesItemText[v]);
    xAction->setCheckable(true);
    connect(xAction, SIGNAL(triggered(bool)),
      this, SLOT(XValuesItemTriggered(bool)));
  }

  XValuesMenu->actions().at(PTVPlusBladder)->setChecked(true);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setupYValuesMenu()
{
  YValuesMenu = new QMenu(this);
  YValuesPushButton->setMenu(YValuesMenu);

  for (int v = PTVSize; v < numXYValueTypes; v++)
  {
    QAction *yAction = YValuesMenu->addAction(XYValuesItemText[v]);
    yAction->setCheckable(true);
    connect(yAction, SIGNAL(triggered(bool)),
      this, SLOT(YValuesItemTriggered(bool)));
  }

  YValuesMenu->actions().at(PTVPlusRectum)->setChecked(true);
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
  int qNum = queryCase->getNumber();

  queryCaseNameLabel->setText(queryInstitution + " patient #"
    + QString::number(qNum)
    + ": " + QString::fromLatin1(XYValuesItemText[xValueType])
    + ": " + QString::number(getXYValueFromNumber(qNum, xValueType))
    + "; " + QString::fromLatin1(XYValuesItemText[yValueType])
    + ": " + QString::number(getXYValueFromNumber(qNum, yValueType))
    + "; MI: "
    + QString::number(MIval[queryCaseIndex][queryCaseIndex], 'g', 3)
    );

  queryCaseNameLabel->adjustSize();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::displayMatchCaseData()
{
  int mNum = matchCase->getNumber();

  matchCaseNameLabel->setText(matchInstitution + " patient #"
    + QString::number(mNum)
    + ": " + QString::fromLatin1(XYValuesItemText[xValueType])
    + ": " + QString::number(getXYValueFromNumber(mNum, xValueType))
    + "; " + QString::fromLatin1(XYValuesItemText[yValueType])
    + ": " + QString::number(getXYValueFromNumber(mNum, yValueType))
    + "; MI: "
    + QString::number(MIval[queryCaseIndex][currMatchCaseIndex], 'g', 3)
  );

  matchCaseNameLabel->adjustSize();
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

  const int numAngles = 7;  // TEMP: hardwired for Duke configuration
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
// Note the difference between queryCaseIndex which is the index into arrays
// of data (sequential: 0, 1, 2...) vs. the query case Patient number, which 
// will probably *not* be sequential.  Right now (2011/07/10), for example, the
// Duke patient numbers start 3, 4, 6, 7, 9...  They don't start at 0 (or at 1)
// and are skipped numbers.
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

  axes = new Axes();
  double shaftLength = 4000.0;
  axesAssembly = axes->InsertThis(ren, shaftLength);
  thresholdAxesAssembly = axes->createGhostAxes(ren, shaftLength);
  double z = MIMin * zMult;

  axesAssembly->SetPosition(
    queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(), z);
  thresholdAxesAssembly->SetPosition(
    queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(), z);

  //const double scale = shaftLength / 35.0;  // Eyeballed 
  const double scale = shaftLength / 30.0;  // Eyeballed 
  axisTextOffset = shaftLength * 1.1;       // Likewise

  vtkFollower *xf = axes->AddFollowingText(Axes::xAxis,
    XYValuesItemText[xValueType],
    queryCase->getPTVPlusBladder() + axisTextOffset,
    queryCase->getPTVPlusRectum(), z, 1, 0, 0, scale, ren);
  vtkFollower *yf = axes->AddFollowingText(Axes::yAxis,
    XYValuesItemText[yValueType],
    queryCase->getPTVPlusBladder(),
    queryCase->getPTVPlusRectum() + axisTextOffset, z, 0, 1, 0, scale, ren);
  vtkFollower *zf = axes->AddFollowingText(Axes::zAxis, "MI", 
    queryCase->getPTVPlusBladder(), queryCase->getPTVPlusRectum(),
    MIMin * zMult + axisTextOffset, 0, 0, 1, scale, ren);

  ren->AddActor(xf);
  ren->AddActor(yf);
  ren->AddActor(zf);

  for (int i = 0; i < numMICases; i++)
  {
    if (i == queryCaseIndex) continue;

    double r, g, b;
    getColorCorrespondingToMIvalue(MIval[queryCaseIndex][i], r, g, b);
    dukePointActor[i]->GetProperty()->SetColor(r, g, b);
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

  MIThresholdPlane->SetCenter(
    (maxVal[xValueType] - minVal[xValueType]) / 2.0 + minVal[xValueType],
    (maxVal[yValueType] - minVal[yValueType]) / 2.0 + minVal[yValueType],
    thresholdPlaneZVal);
  MIThresholdPlane->SetXLength(maxVal[xValueType] - minVal[xValueType]);
  MIThresholdPlane->SetYLength(maxVal[yValueType] - minVal[yValueType]);

  vtkPolyDataMapper *mitpMapper = vtkPolyDataMapper::New();
  mitpMapper->SetInputConnection(MIThresholdPlane->GetOutputPort());

  mitpActor = vtkActor::New();
  mitpActor->SetMapper(mitpMapper);
  mitpActor->GetProperty()->SetOpacity(0.33);
  mitpActor->GetProperty()->SetSpecular(0);
  mitpActor->GetProperty()->SetDiffuse(0);
  mitpActor->GetProperty()->SetAmbient(1);
  mitpActor->GetProperty()->SetAmbientColor(0.2, 0.2, 0.2);
  mitpActor->PickableOff();

  mitpMapper->Delete();

  setThresholdPlanePosition(MIRangeSlider->value());
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::addAxisPlanes()
{
  if (xAxisPlane) return;

  xAxisPlane = vtkCubeSource::New();
  vtkPolyDataMapper *xAxisPlaneMapper = vtkPolyDataMapper::New();
  xAxisPlaneMapper->SetInputConnection(xAxisPlane->GetOutputPort());
  xAxisPlaneActor = vtkActor::New();
  xAxisPlaneActor->SetMapper(xAxisPlaneMapper);
  xAxisPlaneActor->GetProperty()->SetOpacity(0.05);
  xAxisPlaneActor->GetProperty()->SetSpecular(0);
  xAxisPlaneActor->GetProperty()->SetDiffuse(0);
  xAxisPlaneActor->GetProperty()->SetAmbient(1);
  xAxisPlaneActor->GetProperty()->SetAmbientColor(1.0, 0.0, 0.0);
  xAxisPlaneActor->PickableOff();

  yAxisPlane = vtkCubeSource::New();
  vtkPolyDataMapper *yAxisPlaneMapper = vtkPolyDataMapper::New();
  yAxisPlaneMapper->SetInputConnection(yAxisPlane->GetOutputPort());
  yAxisPlaneActor = vtkActor::New();
  yAxisPlaneActor->SetMapper(yAxisPlaneMapper);
  yAxisPlaneActor->GetProperty()->SetOpacity(0.05);
  yAxisPlaneActor->GetProperty()->SetSpecular(0);
  yAxisPlaneActor->GetProperty()->SetDiffuse(0);
  yAxisPlaneActor->GetProperty()->SetAmbient(1);
  yAxisPlaneActor->GetProperty()->SetAmbientColor(0.0, 1.0, 0.0);
  yAxisPlaneActor->PickableOff();
  
  xAxisPlaneMapper->Delete();
  yAxisPlaneMapper->Delete();
}

////////////////////////////////////////////////////////////////////////////////
//
// Make sure this is called *after* setThresholdPlanePosition(...) so that
// MIThresholdVal is up to date.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::setAxisPlanes(int thresholdPlaneZVal)
{
  double width = axes->getShaftLength();
  double height = thresholdPlaneZVal - zCenterQueryPt;
  double thickness = thresholdPlaneThickness;

  xAxisPlane->SetXLength(width);
  xAxisPlane->SetYLength(thickness);
  xAxisPlane->SetZLength(height);

  xAxisPlane->SetCenter(xCenterQueryPt + (width / 2.0),
                yCenterQueryPt + (thickness / 2.0),
                zCenterQueryPt + (height / 2.0));

  yAxisPlane->SetXLength(thickness);
  yAxisPlane->SetYLength(width);
  yAxisPlane->SetZLength(height);

  yAxisPlane->SetCenter(xCenterQueryPt + (thickness / 2.0),
                yCenterQueryPt + (width / 2.0),
                zCenterQueryPt + (height / 2.0));

  caseSpaceRenWin->Render();
}




