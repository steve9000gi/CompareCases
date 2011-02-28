////////////////////////////////////////////////////////////////////////////////
//
// MainWindow.h: Qt-based GUI for prostate cancer radiation therapy treatment 
// tool.  This window displays ...
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyanavich, Duke Medical Center
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
	MainWindow();

private slots:
	void openCaseSpaceDialog();
	void selectDukeDirectory();
	void selectPoconoDirectory();
	void selectHighPointDirectory();

private:
	// General setup methods:
	void createActions();

	QString dukeDir;
	QString poconoDir;
	QString highPointDir;


private:
	MainWindow(const MainWindow&);		// Not implemented.
	void operator=(const MainWindow&);  // Not implemented.
};

#endif
