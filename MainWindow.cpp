////////////////////////////////////////////////////////////////////////////////
//
// MainWindow.cpp: Qt-based GUI for prostate cancer radiation therapy treatment 
// tool.  This window displays ...
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyanavich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
/*
#include "vtkImageViewer.h"
#include "vtkImageViewer2.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkPNGReader.h"
#include "vtkTestUtilities.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageClip.h"
#include "vtkExtractVOI.h"
#include "vtkXYPlotActor.h"
#include "vtkDataObjectCollection.h"
#include "vtkFloatArray.h"
#include "vtkFieldData.h"
#include "vtkImageFlip.h"
#include "vtkChartXY.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"
#include "vtkVector.h"

#include "Patient.h"
*/

#include "CaseSpaceDialog.h"
#include "MainWindow.h"

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow()
{
	setupUi(this);
	createActions();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::openCaseSpaceDialog()
{
	CaseSpaceDialog *caseSpaceDialog = new CaseSpaceDialog();
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

	dukeDir = dlg->getExistingDirectory(0, tr("Select directory for Duke case data"), ".");

	if (!dukeDir.isEmpty())
	{
		QMessageBox::information(this, QString("Selected Duke directory"), dukeDir);
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

	poconoDir = dlg->getExistingDirectory(0, tr("Select directory for Pocono case data"), ".");

	if (!poconoDir.isEmpty())
	{
		QMessageBox::information(this, QString("Selected Pocono directory"), poconoDir);
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

	highPointDir = dlg->getExistingDirectory(0, tr("Select directory for High Point case data"), ".");

	if (!highPointDir.isEmpty())
	{
		QMessageBox::information(this, QString("Selected High Point directory"), highPointDir);
	}
}

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate responses to user manipulation of the GUI controls.
//
////////////////////////////////////////////////////////////////////////////////
void MainWindow::createActions()
{
	connect(viewCaseSpaceButton, SIGNAL(clicked()), this, SLOT(openCaseSpaceDialog()));
	connect(actionSelect_Duke_Directory, SIGNAL(triggered()), this, SLOT(selectDukeDirectory()));
	connect(actionSelect_Pocono_Directory, SIGNAL(triggered()), this, SLOT(selectPoconoDirectory()));
	connect(actionSelect_High_Point_Directory, SIGNAL(triggered()), this, SLOT(selectHighPointDirectory()));

	actionExit->setShortcut(tr("Ctrl+Q"));
	connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
}



