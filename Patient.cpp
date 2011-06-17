////////////////////////////////////////////////////////////////////////////////
//
// Patient.cpp: Implementation file for class that holds a collection of 
// values specific to determining how to display data for an individual prostate
// cancer patient.
//
// author: Steve Chall, RENCI
// chief collaborator: Vorakarn Chanyavanich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////


#include "Patient.h"


///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Patient::Patient()
	:	dataDir("."),
		number(defaultPatientNumber),
		sliceMin(-1),
		sliceMax(-1),
		sliceOrientation(Z)
{
}

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Patient::Patient(int n)
	:	dataDir("."),
		number(n),
		sliceMin(-1),
		sliceMax(-1),
		sliceOrientation(Z)
{
}

///dtor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Patient::~Patient()
{
}

////////////////////////////////////////////////////////////////////////////////
//
// Set the flag that's used to determine whether or not to show the femoral
// heads for this patient.
//
////////////////////////////////////////////////////////////////////////////////
void Patient::setIsShowingFemoralHeads(bool showFemoralHeads)
{
	this->isShowingFemoralHeads = showFemoralHeads;
}
	
////////////////////////////////////////////////////////////////////////////////
// 
// Currently hard-wired to a specific absolute path. 
//
// 2do: Add path select dialog (but probably not here).
//
// The Carl Zhang/Vorakarn Chanyavanich convention for naming structure 
// geometry input files:
//
//     <inFileType>_<patient number>_<structure>.out
// where <inFileType> = [ "faces" | "vertices" ],
//       <patient number> is a 3-digit integer (front-padded with 0's),
// and   <structure>  = [ "bladder" | "LtFem" | "PTV" | "rectum" | "RtFem" ].
//
////////////////////////////////////////////////////////////////////////////////
QString Patient::getPathToProjectionImage(int angle)
{
	QString headsPath = 
		dataDir + "/projections/patient";
	QString noHeadsPath =
		dataDir + "/projections_noFemHeads/patient";
	projectionImagePath  = isShowingFemoralHeads ? headsPath : noHeadsPath;

	QString numberAsString = QString(("%1")).arg(this->number, 3, 10, QLatin1Char('0'));
	projectionImagePath.append(numberAsString);

	projectionImagePath.append("_");

	QString angleAsString = QString(("%1")).arg(angle, 3, 10, QLatin1Char('0'));
	projectionImagePath.append(angleAsString);

	projectionImagePath.append(".png");

	return projectionImagePath;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Patient::setPathToProjectionImage(const QString &path)
{
	projectionImagePath = path;
}

////////////////////////////////////////////////////////////////////////////////
// 
// Currently hard-wired to a specific absolute path. 
//
// 2do: Add path select dialog (but probably not here).
//
////////////////////////////////////////////////////////////////////////////////
QString Patient::getPathToCTData()
{
	CTDataPath = dataDir + "/CT/Patient";

	QString numberAsString = QString(("%1")).arg(this->number, 3, 10, QLatin1Char('0'));
	CTDataPath.append(numberAsString);

	CTDataPath.append("/Primary");

	return CTDataPath;
}

////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
void Patient::setPathToCTData(const QString &path)
{
	CTDataPath = path;
}
	
////////////////////////////////////////////////////////////////////////////////
// 
// Currently hard-wired to a specific absolute path. 
//
// 2do: Add path select dialog (but probably not here).
//
////////////////////////////////////////////////////////////////////////////////
QString Patient::getPathToDVHData()
{
	DVHDataPath = dataDir + "/DVHdata/DVH";

	QString numberAsString = QString(("%1")).arg(this->number, 3, 10, QLatin1Char('0'));
	DVHDataPath.append(numberAsString);

	DVHDataPath.append(".txt");

	return DVHDataPath;

}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Patient::setPathToDVHData(const QString &path)
{
	DVHDataPath = path;
}
	
