////////////////////////////////////////////////////////////////////////////////
//
// CaseSpaceDialog.cpp: Qt-based GUI for prostate cancer radiation therapy 
// planning tool. This window displays ...
//
// author: Steve Chall, RENCI
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
#include "vtkDoubleArray.h"
#include "vtkStringArray.h"
*/

#include "vtkRenderWindow.h"
#include "vtkChartXY.h"
#include "vtkContextView.h"
#include "vtkContextScene.h"
#include "vtkPlot.h"
#include "vtkTable.h"
#include "vtkAxis.h"
#include "vtkFloatArray.h"

#include "CaseSpaceDialog.h"
#include "CompareDialog.h"

#include <stack>
#include <hash_map>

using namespace std;

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
CaseSpaceDialog::CaseSpaceDialog()
	:	caseSpaceChart(NULL),
		caseSpaceView(NULL)
{
	this->setupUi(this);
	this->setupVTKUI();
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
	connect(testPushButton, SIGNAL(clicked()), this, SLOT(testTemplate()));

	//connect(okPushButton, SIGNAL(clicked()), this, SLOT(accept()));
	//connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
}


void CaseSpaceDialog::compareCases()
{
	CompareDialog *compareDialog = new CompareDialog;
	compareDialog->show();
}

void CaseSpaceDialog::testTemplate()
{
	//QMessageBox::information(this, "testTemplate", "testTemplate");
	stack<int> intStack;

	for (int i = 0; i < 123; i++)
	{
		intStack.push(i);
	}

	for (int i = 0; i < 125; i++)
	{
		if (!intStack.empty())
		{
			int j = intStack.top();
			intStack.pop();
		}
	}

	hash_map<int, QString> stringHashMap;

	//bool succeeded;
	//QString s = "QWERTY12345";
	//stringHashMap.insert(s, 

	hash_map<int, int> intHashMap;
	//intHashMap.insert(7, 3);

}

// http://en.wikipedia.org/wiki/Park%E2%80%93Miller_random_number_generator
unsigned long lcg_rand(int a)
{
    return ((unsigned long)a * 279470273) % 4294967291;
}


void CaseSpaceDialog::setupVTKUI()
{
	if (caseSpaceChart) caseSpaceChart->Delete();
	caseSpaceChart = vtkChartXY::New();
	//setDVHYAxisTicks(caseSpaceChart);

	if (caseSpaceView) caseSpaceView->Delete();
	caseSpaceView = vtkContextView::New();

	caseSpaceView->GetScene()->AddItem(caseSpaceChart);
	caseSpaceView->SetInteractor(caseSpaceViewWidget->GetInteractor());
	caseSpaceViewWidget->SetRenderWindow(caseSpaceView->GetRenderWindow());
	caseSpaceView->GetRenderWindow()->SetSize(920, 569);

	caseSpaceChart->SetTitle("");
	caseSpaceChart->GetAxis(vtkAxis::LEFT)->SetTitle("PTV + bladder overlap");
	caseSpaceChart->GetAxis(vtkAxis::BOTTOM)->SetTitle("PTV + rectum overlap");
	caseSpaceChart->SetShowLegend(true);

	vtkFloatArray *dukeXCoords = vtkFloatArray::New();
	const int numCases = 200;
	vtkFloatArray *dukeYCoords = vtkFloatArray::New();
	vtkTable *table = vtkTable::New();
	vtkPlot *line;
	float dukeX[numCases] = {
	10097, 32533, 76520, 13586, 34673, 54876, 80959, 9117, 39292, 74945,
	37542, 4805, 64894, 74296, 24805, 24037, 20636, 10402, 822, 91665,
	8422, 68953, 19645, 9303, 23209, 2560, 15953, 34764, 35080, 33606,
	99019, 2529, 9376, 70715, 38311, 31165, 88676, 74397, 4436, 27659,
	12807, 99970, 80157, 36147, 64032, 36653, 98951, 16877, 12171, 76833,
	66065, 74717, 34072, 76850, 36697, 36170, 65813, 39885, 11199, 29170,
	31060, 10805, 45571, 82406, 35303, 42614, 86799, 7439, 23403, 9732,
	85269, 77602, 2051, 65692, 68665, 74818, 73053, 85247, 18623, 88579,
	63573, 32135, 5325, 47048, 90553, 57548, 28468, 28709, 83491, 25624,
	73796, 45753, 3529, 64778, 35808, 34282, 60935, 20344, 35273, 88435,
	98520, 17767, 14905, 68607, 22109, 40558, 60970, 93433, 50500, 73998,
	11805, 5431, 39808, 27732, 50725, 68248, 29405, 24201, 52775, 67851,
	83452, 99634, 6288, 98083, 13746, 70078, 18475, 40610, 68711, 77817,
	88685, 40200, 86507, 58401, 36766, 67951, 90364, 76493, 29609, 11062,
	99594, 67348, 87517, 64969, 91826, 8928, 93785, 61368, 23478, 34113,
	65481, 17674, 17468, 50950, 58047, 76974, 73039, 57186, 40218, 16544,
	80124, 35635, 17727, 8015, 45318, 22374, 21115, 78253, 14385, 53763,
	74350, 99817, 77402, 77214, 43236, 210, 45521, 64237, 96286, 2655,
	69916, 26803, 66252, 29148, 36936, 87203, 76621, 13990, 94400, 56418,
	9893, 20505, 14225, 68514, 46427, 56788, 96297, 78822, 54382, 14598,
	};

	float dukeY[numCases] = {
	80508, 76285, 17630, 9429, 30293, 16391, 87516, 20628, 53159, 80261,
	12043, 94593, 2328, 43332, 83707, 12201, 23088, 39829, 76777, 55495,
	41717, 72807, 33686, 73225, 30173, 5410, 91541, 45387, 48084, 21855,
	54866, 57899, 13389, 68475, 77825, 1301, 74831, 15970, 68803, 14519,
	12030, 92278, 86864, 4430, 50868, 4949, 8820, 98949, 33713, 87279,
	71744, 72285, 82724, 45846, 69682, 89838, 70910, 26386, 16527, 21698,
	7607, 46148, 29548, 8230, 93459, 69788, 43771, 50812, 60337, 40035,
	25584, 34039, 92437, 61873, 7874, 43107, 56212, 48897, 48008, 83125,
	64572, 2625, 39993, 32573, 88828, 19036, 19394, 51921, 68629, 84838,
	22089, 96239, 65157, 3977, 92561, 41314, 80082, 60159, 74429, 34535,
	58590, 90320, 67095, 28958, 62803, 5097, 8269, 63296, 92249, 80332,
	21640, 45655, 94143, 89051, 22782, 29086, 38014, 11641, 54398, 85092,
	81936, 35183, 97146, 90677, 41012, 62425, 19569, 40059, 32565, 23037,
	34506, 67652, 56534, 21287, 58697, 36165, 43304, 52134, 22272, 75345,
	64575, 80559, 38389, 21713, 36749, 30055, 39889, 4287, 21294, 77790,
	28400, 71414, 73453, 62631, 85191, 18446, 81309, 33305, 67816, 56922,
	797, 10584, 63075, 31922, 48847, 34738, 32528, 1884, 71241, 34618,
	39544, 26038, 86456, 29624, 76562, 21853, 31395, 81509, 72150, 35599,
	94881, 65970, 22406, 21125, 41074, 63283, 61007, 22211, 21082, 73175,
	74166, 39761, 35695, 43436, 38419, 937, 68925, 63631, 90667, 15306,
	};


	for (int i = 0; i < numCases; i++)
	{
		int randX = lcg_rand(i) % 2053;
		int randY = lcg_rand(i * 7) * 1733;

		dukeX[i] = ((int)dukeX[i] % 100) * ((randY % 2) ? 1.0 : -1.0);
		dukeY[i] = ((int)dukeY[i] % 100) * ((randX % 2) ? -1.0 : 1.0);

//		dukeX[i] = (lcg_rand(randX) % 100) * ((randY % 2) ? 1.0 : -1.0);
//		dukeY[i] = (lcg_rand(randY) % 100) * ((randX % 2) ? -1.0 : 1.0);
	}

	dukeXCoords->SetArray(dukeX, numCases, 1);
	dukeXCoords->SetName("PTV + bladder overlap");
	table->AddColumn(dukeXCoords);
	table->SetNumberOfRows(numCases);

	dukeYCoords->SetArray(dukeY, numCases, 1);
	dukeYCoords->SetName("Duke");
	table->AddColumn(dukeYCoords);
	table->Update();
	line = caseSpaceChart->AddPlot(vtkChart::POINTS);
	line->SetInput(table, 0, 1);
	line->SetColor(200, 200, 255, 255);


	float queryX[numCases]; 
	float queryY[numCases];

	for (int i = 0; i < numCases; i++) 
	{
		queryX[i] = dukeX[i] / 50.0;
		queryY[i] = dukeY[i] / 50.0 * 1.618; // golden ratio to compensate for aspect ratio
	}

	vtkFloatArray *queryXCoords = vtkFloatArray::New();
	vtkFloatArray *queryYCoords = vtkFloatArray::New();
	vtkPlot *queryPoint = caseSpaceChart->AddPlot(vtkChart::POINTS);;
	queryXCoords->SetArray(queryX, numCases, 1);
	queryXCoords->SetName("qcx");
	queryYCoords->SetArray(queryY, numCases, 1);
	queryYCoords->SetName("Query case");

	table->AddColumn(queryXCoords);
	table->AddColumn(queryYCoords);

	queryPoint->SetInput(table, 2, 3);
	queryPoint->SetColor(255, 148, 128, 255);

	caseSpaceChart->SetDrawAxesAtOrigin(true);
	caseSpaceChart->Update();
	caseSpaceChart->RecalculateBounds();

	dukeXCoords->Delete();
	dukeYCoords->Delete();
	queryXCoords->Delete();
	queryYCoords->Delete();
}
