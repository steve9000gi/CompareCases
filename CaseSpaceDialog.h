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

// Forward declarations:
class ccChartXY;
class vtkContextView;
class vtkPlot;
class vtkTable;
class vtkVector2f;
class MainWindow;
class Patient;
class CompareDialog;


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
	Patient *getPatientFromCoodinates(float *xArray, float *yArray, float x, float y);
	Patient *getDukePatientFromCoodinates(float posX, float posY);
	int getNumDukePatients() { return numDukePatients; };

	void enableCompareCasesButton(bool isEnabled)
	{
		compareCasesPushButton->setEnabled(isEnabled);
	};

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
	float *dX;	// Duke
	float *dY;
	float *pX;	// Pocono
	float *pY;
	float *hpX;	// High Point
	float *hpY;

	int numDukePatients;
	int numPoconoPatients;
	int numHighPointPatients;

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

	CompareDialog *compareDialog;

private:
	void createActions();
	void setupCaseSpaceChart();

	void addDukeDataToChart();
	void addDummyDukeDataToChart();
	void addPoconoDataToChart();
	void addHighPointDataToChart();
	void addQueryCaseToChart();

	void drawSelectedCase();

private:
	CaseSpaceDialog(const CaseSpaceDialog&);		// Not implemented.
	void operator=(const CaseSpaceDialog&);			// Not implemented.
};

#endif
