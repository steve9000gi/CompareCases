////////////////////////////////////////////////////////////////////////////////
//
// Patient.h: Header file for class that holds a collection of values specific
// to determining how to display data for an individual prostate cancer patient.
//
// author: Steve Chall, RENCI
// chief collaborator: Vorakarn Chanyanavich, Duke Medical Center
//
////////////////////////////////////////////////////////////////////////////////

#ifndef PATIENT_H
#define PATIENT_H


#include <QString>


class Patient
{

public:
	Patient();
	Patient(int n);
	~Patient();

	enum
	{
		defaultPatientNumber = 12,	// Arbitrary mid value for prototype
		defaultGantryAngle = 180	// Vorakarn's recommendation
	};

	enum SliceOrientation
	{
		X,	// Sagittal
		Y,	// Coronal
		Z	// Axial
	};

	int getNumber() { return number; };
	void setNumber(int n) { number = n; };

	int getSliceMin() { return sliceMin; };
	void setSliceMin(int min) { sliceMin = min; };

	int getSliceMax() { return sliceMax; };
	void setSliceMax(int max) { sliceMax = max; };

	SliceOrientation getSliceOrientation() { return sliceOrientation; };
	void setSliceOrientation(SliceOrientation orientation) { sliceOrientation = orientation; };

	bool getIsShowingFemoralHeads() { return isShowingFemoralHeads; };
	void setIsShowingFemoralHeads(bool showFemoralHeads);

	QString &getDataDir() { return this->dataDir; };
	void setDataDir(const QString &dir) { this->dataDir = dir; };

	QString getPathToProjectionImage(int angle);
	void setPathToProjectionImage(const QString &path);

	QString getPathToCTData();
	void setPathToCTData(const QString &path);

	QString getPathToDVHData();
	void setPathToDVHData(const QString &path);
	
protected:
	int number;
	int sliceMin;
	int sliceMax;
	SliceOrientation sliceOrientation;

	QString dataDir;
	QString projectionImagePath;
	QString CTDataPath;
	QString DVHDataPath;

	bool isShowingFemoralHeads;

private:
	Patient(const Patient&);			// Not implemented.
	void operator=(const Patient&);		// Not implemented.
};

#endif