////////////////////////////////////////////////////////////////////////////////
//
// Axes.h:  Class for adding the standard 3D axes, labelled, into a 3D scene.  
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
  enum AxisType
  {
    xAxis = 0,
    yAxis,
    zAxis,
    numAxes
  };

  Axes();
  ~Axes();

  vtkFollower *AddFollowingText(AxisType axis, char *text, double x, double y,
    double z, double r, double g, double b, double scale, vtkRenderer *ren);
  vtkAssembly *InsertThis(vtkRenderer *r, double shaftLen = 20.0,
    double x = 0.0, double y = 0.0, double z = 0.0);

  void setAxisLabel(AxisType axis, char *label);
  void setAxisLabelPosition(AxisType axis, double x, double y, double z);
  double getShaftLength() { return shaftLength; };
  vtkAssembly *createGhostAxes(vtkRenderer *r, double shaftLen = 20.0,
    double x = 0.0, double y = 0.0, double z = 0.0);

protected:
  double shaftLength;

  vtkVectorText *vText[numAxes];
  vtkPolyDataMapper *vTextMapper[numAxes];
  vtkFollower *vTextActor[numAxes];

  vtkActor *oActor;
  vtkActor *xConeActor;
  vtkActor *yConeActor;
  vtkActor *zConeActor;
  vtkActor *xShaftActor;
  vtkActor *yShaftActor;
  vtkActor *zShaftActor;

  vtkActor *ghostOActor;
  vtkActor *ghostXConeActor;
  vtkActor *ghostYConeActor;
  vtkActor *ghostXShaftActor;
  vtkActor *ghostYShaftActor;
};

#endif