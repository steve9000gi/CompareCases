////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.cpp: Qt-based GUI for prostate cancer radiation therapy  
// planning tool. This window displays ...
//
// author:  Steve Chall, RENCI
// primary collaborator: Vorakarn Chanyanavich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#include <QtGui>
/*
#include "vtkImageViewer.h"
#include "vtkImageViewer2.h"
#include "vtkRenderer.h"
#include "vtkPNGReader.h"
#include "vtkDICOMImageReader.h"
#include "vtkImageFlip.h"
#include "vtkChartXY.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"
*/

#include "CaseSpaceDialog.h"
#include "CompareDialog.h"

#include <stack>
#include <hash_map>

using namespace std;

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CaseSpaceDialog::CaseSpaceDialog()
{
	this->setupUi(this);
	this->createActions();
}

CaseSpaceDialog::~CaseSpaceDialog()
{
}

////////////////////////////////////////////////////////////////////////////////
// 
// Associate the appropriate responses to user manipulation of the GUI controls.
//
////////////////////////////////////////////////////////////////////////////////
void CaseSpaceDialog::createActions()
{
	connect(compareCasesPushButton, SIGNAL(clicked()), this, SLOT(compareCases()));
	connect(hashTestPushButton, SIGNAL(clicked()), this, SLOT(hashTest()));

	//connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	//connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}


void CaseSpaceDialog::compareCases()
{
	CompareDialog *compareDialog = new CompareDialog;
	compareDialog->show();
}

void CaseSpaceDialog::hashTest()
{
	//QMessageBox::information(this, "hashTest", "hashTest");
	stack<int> intStack;

	for (int i = 0; i < 123; i++)
	{
		intStack.push(i);
	}

	for (int i = 0; i < 123; i++)
	{
		/*int j = */ intStack.pop(); // ????
	}


	hash_map<int, QString> stringHashMap;

	bool succeeded;

	QString s = "QWERTY12345";

	//stringHashMap.insert(s, 

	hash_map<int, int> intHashMap;
	//intHashMap.insert(7, 3);

}