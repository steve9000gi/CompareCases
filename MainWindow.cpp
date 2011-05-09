////////////////////////////////////////////////////////////////////////////////
//
// MainWindow.cpp: Qt-based GUI for prostate cancer radiation therapy treatment 
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

#include <QtGui>
#include "CaseSpaceDialog.h"
#include "MainWindow.h"

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
	:	queryCaseSourceInstitution(kDuke),
		queryCasePatientNumber(-1),
		dukeDir("C:/Users/Steve/Documents/IMRT/Duke_Cases_2011-02-24")
{
	setupUi(this);
	setupSelectQueryCaseComboBoxes();
	createActions();
	//dukeQueryCaseComboBox->setDisabled(true);
	loadDukeLineEdit->setText(dukeDir);
	dukeQueryCaseComboBox->setDisabled(false);
	poconoQueryCaseComboBox->setDisabled(true);
	highPointQueryCaseComboBox->setDisabled(true);
	viewCaseSpaceButton->setDisabled(true);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::openCaseSpaceDialog()
{
	CaseSpaceDialog *caseSpaceDialog = new CaseSpaceDialog(this);
	caseSpaceDialog->setQueryCaseText(queryCaseNameLabel->text());
	//caseSpaceDialog->setDukeOverlapDataPath(dukeOverlapDataPath);
	caseSpaceDialog->show();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::selectDukeDirectory()
{
	QFileDialog *dlg = new QFileDialog();
	dlg->setMode(QFileDialog::Directory);
	dlg->setOption(QFileDialog::ShowDirsOnly);

	dukeDir = dlg->getExistingDirectory(0,
		tr("Select directory for Duke case data"), ".");

	if (!dukeDir.isEmpty())
	{
		loadDukeLineEdit->setText(dukeDir);
		dukeQueryCaseComboBox->setDisabled(false);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::selectPoconoDirectory()
{
	QFileDialog *dlg = new QFileDialog();
	dlg->setMode(QFileDialog::Directory);
	dlg->setOption(QFileDialog::ShowDirsOnly);

	poconoDir = dlg->getExistingDirectory(0,
		tr("Select directory for Pocono case data"), ".");

	if (!poconoDir.isEmpty())
	{
		loadPoconoLineEdit->setText(poconoDir);
		poconoQueryCaseComboBox->setDisabled(false);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::selectHighPointDirectory()
{
	QFileDialog *dlg = new QFileDialog();
	dlg->setMode(QFileDialog::Directory);
	dlg->setOption(QFileDialog::ShowDirsOnly);

	highPointDir = dlg->getExistingDirectory(0,
		tr("Select directory for High Point case data"), ".");

	if (!highPointDir.isEmpty())
	{
		loadHighPointLineEdit->setText(highPointDir);
		highPointQueryCaseComboBox->setDisabled(false);
	}
}

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate responses to user manipulation of the GUI controls.
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::createActions()
{
	connect(viewCaseSpaceButton, SIGNAL(clicked()), this,
		SLOT(openCaseSpaceDialog()));

	connect(actionSelect_Duke_Directory, SIGNAL(triggered()), this,
		SLOT(selectDukeDirectory()));
	connect(actionSelect_Pocono_Directory, SIGNAL(triggered()), this,
		SLOT(selectPoconoDirectory()));
	connect(actionSelect_High_Point_Directory, SIGNAL(triggered()), this,
		SLOT(selectHighPointDirectory()));

	connect(loadDukePushButton, SIGNAL(clicked()), this,
		SLOT(selectDukeDirectory()));
	connect(loadPoconoPushButton, SIGNAL(clicked()), this,
		SLOT(selectPoconoDirectory()));
	connect(loadHighPointPushButton, SIGNAL(clicked()), this,
		SLOT(selectHighPointDirectory()));

	connect(dukeQueryCaseComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(selectDukeQueryCase(int)));
	connect(poconoQueryCaseComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(selectPoconoQueryCase(int)));
	connect(highPointQueryCaseComboBox, SIGNAL(currentIndexChanged(int)), this,
		SLOT(selectHighPointQueryCase(int)));

	actionExit->setShortcut(tr("Ctrl+Q"));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

////////////////////////////////////////////////////////////////////////////////
//
// NOTE: relative path from master Duke directory to data currently hardwired.
//
////////////////////////////////////////////////////////////////////////////////
bool MainWindow::setupDukeSelectQueryCaseComboBox()
{
	selectDukeQueryCaseAction = new QAction(dukeQueryCaseComboBox);
	dukeQueryCaseComboBox->addAction(selectDukeQueryCaseAction); 
	dukeQueryCaseComboBox->addItem(QString("-"));

	// for each Duke patient for which we have overlap data, create a combo box
	// item:
	dukeOverlapDataPath = dukeDir + "/overlap/Duke_struct_overlap180.txt";
	QFile file(dukeOverlapDataPath);

	if (!file.open(QIODevice::ReadOnly))
	{
		QString warn = "Failed to open \"" + dukeOverlapDataPath + "\"";
		QMessageBox::warning(this, tr("File Open Failed"), warn);
		return false;
	}

	QTextStream in(&file);

	
	in.readLine();		// Skip over the first line (column headers):

	int index;

	while (!in.atEnd())
	{
		in >> index;
		in.readLine();	// for now, throw the rest of the line away
		dukeQueryCaseComboBox->addItem(QString::number(index)); 
	}

	in.flush();
	file.close();

	return true;
/*
	for (int i = 10; i < 19; i++)
	{
		dukeQueryCaseComboBox->addItem(QString::number(i)); 
	}
*/
}

////////////////////////////////////////////////////////////////////////////////
// 
// 2do:
// When we have real data, there will need to be error checking as we add 
// items to the combo boxes: we'll need to try to open the essential data files
// for each insitution and patient number as it comes up, and add items to the
// combo box only for those patients where the open attempts were successful.
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::setupSelectQueryCaseComboBoxes()
{
	setupDukeSelectQueryCaseComboBox();
/**/
	// Dummy values
	selectPoconoQueryCaseAction = new QAction(poconoQueryCaseComboBox);
	poconoQueryCaseComboBox->addAction(selectPoconoQueryCaseAction); 
	poconoQueryCaseComboBox->addItem(QString("-")); 

	for (int i = 1; i < 30; i++)
	{
		poconoQueryCaseComboBox->addItem(QString::number(i)); 
	}

	selectHighPointQueryCaseAction = new QAction(highPointQueryCaseComboBox);
	highPointQueryCaseComboBox->addAction(selectHighPointQueryCaseAction); 
	highPointQueryCaseComboBox->addItem(QString("-")); 

	for (int i = 1; i < 200; i++)
	{
		highPointQueryCaseComboBox->addItem(QString::number(i)); 
	}
/**/
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void MainWindow::selectDukeQueryCase(int index)
{	
	if (index == 0)
	{
		queryCaseNameLabel->setText("<not selected>");
		//return;
	}

	if (queryCaseSourceInstitution != kDuke)
	{
		poconoQueryCaseComboBox->setCurrentIndex(0);
		highPointQueryCaseComboBox->setCurrentIndex(0);
	}

	QString patientNumAsText = dukeQueryCaseComboBox->itemText(index);
	queryCasePatientNumber = patientNumAsText.toInt();
	QString queryCasePatientDescriptor = "Duke patient #" + patientNumAsText;
	queryCaseNameLabel->setText(queryCasePatientDescriptor);

	queryCaseSourceInstitution = kDuke;

	setViewCaseSpacePushButtonEnabling();
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void MainWindow::selectPoconoQueryCase(int index)
{
	if (index == 0)
	{
		queryCaseNameLabel->setText("<not selected>");
	}

	if (queryCaseSourceInstitution != kPocono)
	{
		dukeQueryCaseComboBox->setCurrentIndex(0);
		highPointQueryCaseComboBox->setCurrentIndex(0);
	}

	QString patientNumAsText = poconoQueryCaseComboBox->itemText(index);
	queryCasePatientNumber = patientNumAsText.toInt();
	QString queryCasePatientDescriptor = "Pocono patient #" + patientNumAsText;
	queryCaseNameLabel->setText(queryCasePatientDescriptor);

	queryCaseSourceInstitution = kPocono;

	setViewCaseSpacePushButtonEnabling();
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void MainWindow::selectHighPointQueryCase(int index)
{
	if (index == 0)
	{
		queryCaseNameLabel->setText("<not selected>");
	}

	if (queryCaseSourceInstitution != kHighPoint)
	{
		dukeQueryCaseComboBox->setCurrentIndex(0);
		poconoQueryCaseComboBox->setCurrentIndex(0);
	}

	QString patientNumAsText = highPointQueryCaseComboBox->itemText(index);
	queryCasePatientNumber = patientNumAsText.toInt();
	QString queryCasePatientDescriptor = "High Point patient #" + patientNumAsText;
	queryCaseNameLabel->setText(queryCasePatientDescriptor);

	queryCaseSourceInstitution = kHighPoint;

	setViewCaseSpacePushButtonEnabling();
}

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////
void MainWindow::setViewCaseSpacePushButtonEnabling()
{
	viewCaseSpaceButton->setDisabled(
		(dukeQueryCaseComboBox->currentIndex() == 0) &&
		(poconoQueryCaseComboBox->currentIndex() == 0) &&
		(highPointQueryCaseComboBox->currentIndex() == 0));
}