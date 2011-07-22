////////////////////////////////////////////////////////////////////////////////
// Axes.h:  
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AXES_H
#define AXES_H

#include <qstring.h>

// Forward declarations:
class vtkAssembly;
class vtkRenderer;
class vtkFollower;
class vtkVectorText;
class vtkPolyDataMapper;
class vtkFollower;

class Axes
{
public:
	Axes();
	~Axes();

	vtkFollower *AddFollowingText(char *text, double x, double y, double z,
		double r, double g, double b, vtkRenderer *ren);
	vtkAssembly *InsertThis(vtkRenderer *r, double shaftLen = 20.0,
		double x = 0.0, double y = 0.0, double z = 0.0);
	void Insert(vtkRenderer *r, double shaftLen = 20.0,
		double x = 0.0, double y = 0.0, double z = 0.0);

	void setXAxislabel(char *label) { xAxisLabel = label; };
	void setYAxislabel(char *label) { yAxisLabel = label; };
	void setZAxislabel(char *label) { zAxisLabel = label; };

protected:
	double shaftLength;
	QString xAxisLabel;
	QString yAxisLabel;
	QString zAxisLabel;

	vtkVectorText *vText;
	vtkPolyDataMapper *vTextMapper;
	vtkFollower *vTextActor;

	vtkActor *oActor;
	vtkActor *xConeActor;
	vtkActor *yConeActor;
	vtkActor *zConeActor;
	vtkActor *xShaftActor;
	vtkActor *yShaftActor;
	vtkActor *zShaftActor;

};

#endif