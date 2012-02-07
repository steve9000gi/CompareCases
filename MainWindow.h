////////////////////////////////////////////////////////////////////////////////
//
// MainWindow.h: Qt-based GUI for prostate cancer radiation therapy treatment 
// tool. This window allows the user to select top-level data source
// directories for the current three institutions providing that data: Duke, 
// Pocono, and High Point. The data must be in the specified directory
// organization beneath that top level. Once one or more of these directories
// are determined, the user may then select a particular patient case from the
// established institutions for use as query case. Once a specific query case
// is selected, the View Case Space button, which opens the Case Space dialog
// for selecting a match case, is enabled.
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

#ifndef CCMW_H
#define CCMW_H

#include <QDialog>
#include <QFile>

#include "ui_MainWindow.h"

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
  ~MainWindow();

  void setCaseSpaceDialogPointerToNULL() { caseSpaceDialog = NULL; };

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
  void close();

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

  // Object for accessing a text file that contains the path to the top level
  // of the directory structure that holds the Duke data:
  QFile dukeDataDirFile;

  QString dukeDataDirName;
  QString dukeXYDataPath;
  QString poconoDir;
  QString highPointDir;

  SourceInstitution queryCaseSourceInstitution;
  int queryCasePatientNumber;

  CaseSpaceDialog *caseSpaceDialog;

  int versionNumber;

public:
  QString getDukeDir() { return dukeDataDirName; };
  // TEMP hardwired to Duke pending introduction of data from other institutions:
  QString getDataDir() { return dukeDataDirName; };
  QString &getDukeXYDataPath() { return dukeXYDataPath; };
  SourceInstitution getQueryCaseSourceInstitution()
  { return queryCaseSourceInstitution; };
  int getQueryCasePatientNumber() { return queryCasePatientNumber; };

private:
  MainWindow(const MainWindow&);      // Not implemented.
  void operator=(const MainWindow&);  // Not implemented.
};

#endif
