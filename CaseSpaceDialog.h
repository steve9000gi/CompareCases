////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.h: Qt-based GUI for prostate cancer radiation therapy
// planning tool. This window displays ...
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyanavich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CASE_SPACE_DIALOG_H
#define CASE_SPACE_DIALOG_H

#include <QDialog>

#include "ui_CaseSpaceDialog.h"  // auto generated from XML output from QT Designer

// Forward declarations:
class vtkChartXY;
class vtkContextView;


class CaseSpaceDialog : public QDialog, public Ui_CaseSpaceDialog
{
	Q_OBJECT

public:
	CaseSpaceDialog();
	~CaseSpaceDialog();

private slots:
	void compareCases();
	void testTemplate();

private: // Objects
	vtkChartXY *caseSpaceChart;
	vtkContextView *caseSpaceView;

private:
	void createActions();
	void setupVTKUI();

private:
	CaseSpaceDialog(const CaseSpaceDialog&);				// Not implemented.
	void operator=(const CaseSpaceDialog&);			// Not implemented.
};

#endif
