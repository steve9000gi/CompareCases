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
#include <QFile>

#include "ui_MainWindow.h"

#define XVGA_RESOLUTION // If the target display is 1024 x 768

// Forward declaration:
class CaseSpaceDialog;

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
	~MainWindow()
	{
	};

private slots:
	void openCaseSpaceDialog();	
	void selectDukeDirectory();
	void selectPoconoDirectory();
	void selectHighPointDirectory();
	void selectDukeQueryCase(int index);
	void selectPoconoQueryCase(int index);
	void selectHighPointQueryCase(int index);
	void viewDocumentation();
	void about();

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

	QFile dukeDataDirectoryPath;

	QString dukeDir;
	QString dukeXYDataPath;
	QString poconoDir;
	QString highPointDir;

	SourceInstitution queryCaseSourceInstitution;
	int queryCasePatientNumber;

	CaseSpaceDialog *caseSpaceDialog;

	int versionNumber;

public:
	QString getDukeDir() { return dukeDir; };
	 // TEMP hardwired to Duke pending introduction of data from other institutions:
	QString getDataDir() { return dukeDir; };
	QString &getDukeXYDataPath() { return dukeXYDataPath; };
	SourceInstitution getQueryCaseSourceInstitution() { return queryCaseSourceInstitution; };
	int getQueryCasePatientNumber() { return queryCasePatientNumber; };

private:
	MainWindow(const MainWindow&);		// Not implemented.
	void operator=(const MainWindow&);  // Not implemented.
};

#endif
