////////////////////////////////////////////////////////////////////////////////
//
// MainWindow.h: Qt-based GUI for prostate cancer radiation therapy treatment 
// tool. This window allows the user to select top-level data source
// directories for the current three institutions providing that data: Duke, 
// Pocono, and High Point. The data must be in the specified directory
// organization beneath that top level. Once one or more of these directories
// are determined, the user may then select a particular patient case from the
// established institutions for use as query case. Once a specific query case
// is selected, the View Space Case button, which opens the Case Space dialog
// for selecting a match case, is enabled.
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyavanich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CCMW_H
#define CCMW_H

#include <QDialog>

#include "ui_MainWindow.h"


class MainWindow : public QMainWindow, public Ui_MainWindow
{
	Q_OBJECT

public:
	enum SourceInstitution
	{
		kDuke,
		kPocono,
		kHighPoint
	};

	MainWindow();

private slots:
	void openCaseSpaceDialog();
	void selectDukeDirectory();
	void selectPoconoDirectory();
	void selectHighPointDirectory();
	void selectDukeQueryCase(int index);
	void selectPoconoQueryCase(int index);
	void selectHighPointQueryCase(int index);

protected:
	// General setup methods:
	void createActions();
	bool setupDukeSelectQueryCaseComboBox();
	void setupSelectQueryCaseComboBoxes();
	void setViewCaseSpacePushButtonEnabling();

	// Qt objects:
	QAction *selectDukeQueryCaseAction;
	QAction *selectPoconoQueryCaseAction;
	QAction *selectHighPointQueryCaseAction;

	QString dukeDir;
	QString dukeOverlapDataPath;
	QString poconoDir;
	QString highPointDir;

	SourceInstitution queryCaseSourceInstitution;
	int queryCasePatientNumber;

public:
	QString getDukeDir() { return dukeDir; };
	QString &getDukeOverlapDataPath() { return dukeOverlapDataPath; };
	SourceInstitution getQueryCaseSourceInstitution() { return queryCaseSourceInstitution; };
	int getQueryCasePatientNumber() { return queryCasePatientNumber; };

private:
	MainWindow(const MainWindow&);		// Not implemented.
	void operator=(const MainWindow&);  // Not implemented.
};

#endif
