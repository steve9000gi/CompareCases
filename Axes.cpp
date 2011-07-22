////////////////////////////////////////////////////////////////////////////////
// Axes.h:  
//
////////////////////////////////////////////////////////////////////////////////

#include "windows.h" 

#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"
#include "vtkVectorText.h"
#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkCylinderSource.h"
#include "vtkFollower.h"
#include "vtkTextActor.h"
#include "vtkCommand.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "vtkCellArray.h"
#include "vtkDecimatePro.h"
#include "vtkPolyDataNormals.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkAxes.h"
#include "vtkWindowToImageFilter.h"
#include "vtkAssembly.h"

#include "Axes.h"

using namespace std;

///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Axes::Axes()
:	vText(NULL),
	vTextMapper(NULL),
	vTextActor(NULL),
	oActor(NULL),
	xConeActor(NULL),
	yConeActor(NULL),
	zConeActor(NULL),
	xShaftActor(NULL),
	yShaftActor(NULL),
	zShaftActor(NULL)
{}

///dtor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Axes::~Axes()
{
	if (vText) vText->Delete();
	if (vTextMapper) vTextMapper->Delete();
	if (vTextActor) vTextActor->Delete();
	if (oActor) oActor->Delete();
	if (xConeActor) xConeActor->Delete();
	if (yConeActor) yConeActor->Delete();
	if (zConeActor) zConeActor->Delete();
	if (xShaftActor) xShaftActor->Delete();
	if (yShaftActor) yShaftActor->Delete();
	if (zShaftActor) zShaftActor->Delete();
}

///AddFollowingText/////////////////////////////////////////////////////////////
// 
// Specify some text, where you want it to go in 3-space, a color, and a
// renderer, and it will always face the active camera associated with that
// renderer.
//
////////////////////////////////////////////////////////////////////////////////
vtkFollower *Axes::AddFollowingText(char *text, double x, double y, double z,
	double r, double g, double b, vtkRenderer *ren)
{
	vText = vtkVectorText::New();;
	vTextMapper = vtkPolyDataMapper::New();
	vTextActor = vtkFollower::New();

	vText->SetText(text);
	vTextMapper->SetInputConnection(vText->GetOutputPort());
	vTextActor->SetMapper(vTextMapper);
	//vTextActor->SetScale(2, 2, 2);
	vTextActor->AddPosition(x, y, z);
	vTextActor->GetProperty()->SetColor(r, g, b);
	vTextActor->SetCamera(ren->GetActiveCamera());

	return vTextActor;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
vtkAssembly *Axes::InsertThis(vtkRenderer *r, double shaftLen /*= 20.0 */,
		double x /*= 0.0 */, double y /* = 0.0 */, double z /*= 0.0 */)
{
	this->shaftLength = shaftLen;

	const double coneRadiusRatio = 0.015; // 0.025; // 0.0375;
	const double coneHeightRatio = 0.045; // 0.067; // 0.1;
	const double shaftRadiusRatio = 0.003; // 0.004; // 0.005;
	const double shaftPosition = shaftLength / 2.0;

	vtkConeSource *xCone = vtkConeSource::New();
	vtkPolyDataMapper *xConeMapper = vtkPolyDataMapper::New();
	xConeActor = vtkActor::New();
	xConeActor->PickableOff();
	xCone->SetResolution(40);
	xCone->SetHeight(shaftLength * coneHeightRatio);
	xCone->SetRadius(shaftLength * coneRadiusRatio);
	xCone->SetDirection(1, 0, 0);
	xConeMapper->SetInputConnection(xCone->GetOutputPort());
	xConeActor->SetPosition(shaftLength, 0, 0);
	xConeMapper->ScalarVisibilityOff();
	xConeActor->SetMapper(xConeMapper);
	xConeActor->GetProperty()->SetColor(1, 0, 0);
  
	vtkCylinderSource *xShaft = vtkCylinderSource::New();
	vtkPolyDataMapper *xShaftMapper = vtkPolyDataMapper::New();
	xShaftActor = vtkActor::New();
	xShaftActor->PickableOff();
	xShaft->SetResolution(40);
	xShaft->SetHeight(shaftLength);
	xShaft->SetRadius(shaftLength * shaftRadiusRatio);
	xShaftMapper->SetInputConnection(xShaft->GetOutputPort());
	xShaftMapper->ScalarVisibilityOff();
	xShaftActor->RotateZ(90);
	xShaftActor->SetPosition(shaftPosition, 0, 0);
	xShaftActor->SetMapper(xShaftMapper);
	xShaftActor->GetProperty()->SetColor(1, 0, 0);

	vtkConeSource *yCone = vtkConeSource::New();
	vtkPolyDataMapper *yConeMapper = vtkPolyDataMapper::New();
	yConeActor = vtkActor::New();
	yConeActor->PickableOff();
	yCone->SetResolution(40);
	yCone->SetHeight(shaftLength * coneHeightRatio);
	yCone->SetRadius(shaftLength * coneRadiusRatio);
	yCone->SetDirection(0, 1, 0);
	yConeMapper->SetInputConnection(yCone->GetOutputPort());
	yConeActor->SetPosition(0, shaftLength, 0);
	yConeMapper->ScalarVisibilityOff();
	yConeActor->SetMapper(yConeMapper);
	yConeActor->GetProperty()->SetColor(0, 1, 0);
  
	vtkCylinderSource *yShaft = vtkCylinderSource::New();
	vtkPolyDataMapper *yShaftMapper = vtkPolyDataMapper::New();
	yShaftActor = vtkActor::New();
	yShaftActor->PickableOff();
	yShaft->SetResolution(40);
	yShaft->SetHeight(shaftLength);
	yShaft->SetRadius(shaftLength * shaftRadiusRatio);
	yShaftMapper->SetInputConnection(yShaft->GetOutputPort());
	yShaftMapper->ScalarVisibilityOff();
	yShaftActor->SetPosition(0, shaftPosition, 0);
	yShaftActor->SetMapper(yShaftMapper);
	yShaftActor->GetProperty()->SetColor(0, 1, 0);

	vtkConeSource *zCone = vtkConeSource::New();
	vtkPolyDataMapper *zConeMapper = vtkPolyDataMapper::New();
	zConeActor = vtkActor::New();
	zConeActor->PickableOff();
	zCone->SetResolution(40);
	zCone->SetHeight(shaftLength * coneHeightRatio);
	zCone->SetRadius(shaftLength * coneRadiusRatio);
	zCone->SetDirection(0, 0, 1);
	zConeMapper->SetInputConnection(zCone->GetOutputPort());
	zConeActor->SetPosition(0, 0, shaftLength);
	zConeMapper->ScalarVisibilityOff();
	zConeActor->SetMapper(zConeMapper);
	zConeActor->GetProperty()->SetColor(0, 0, 1);
  
	vtkCylinderSource *zShaft = vtkCylinderSource::New();
	vtkPolyDataMapper *zShaftMapper = vtkPolyDataMapper::New();
	zShaftActor = vtkActor::New();
	zShaftActor->PickableOff();
	zShaft->SetResolution(40);
	zShaft->SetHeight(shaftLength);
	zShaft->SetRadius(shaftLength * shaftRadiusRatio);
	zShaftMapper->SetInputConnection(zShaft->GetOutputPort());
	zShaftMapper->ScalarVisibilityOff();
	zShaftActor->RotateX(90);
	zShaftActor->SetPosition(0, 0, shaftPosition);
	zShaftActor->SetMapper(zShaftMapper);
	zShaftActor->GetProperty()->SetColor(0, 0, 1);
  
	vtkCubeSource *origin = vtkCubeSource::New();
	vtkPolyDataMapper *oMapper = vtkPolyDataMapper::New();
	oActor = vtkActor::New();
	oActor->PickableOff();
	origin->SetXLength(shaftLength * 0.025);
	origin->SetYLength(shaftLength * 0.025);
	origin->SetZLength(shaftLength * 0.025);
	oMapper->SetInputConnection(origin->GetOutputPort());
	oMapper->ScalarVisibilityOff();
	oActor->SetMapper(oMapper);
	oActor->GetProperty()->SetColor(1, 1, 1);
 	vtkAssembly *assembly = vtkAssembly::New();
	assembly->AddPart(xConeActor);
	assembly->AddPart(yConeActor);
	assembly->AddPart(zConeActor);
	assembly->AddPart(xShaftActor);
	assembly->AddPart(yShaftActor);
	assembly->AddPart(zShaftActor);
	assembly->AddPart(oActor);

	r->AddActor(assembly);

	return assembly;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Axes::Insert(vtkRenderer *r, double shaftLen, double x, double y, double z)
{
/*  this->shaftLength = shaftLen;

  vtkActor *oActor = NULL;
  vtkActor *xConeActor = NULL;
  vtkActor *yConeActor = NULL;
  vtkActor *zConeActor = NULL;
  vtkActor *xShaftActor = NULL;
  vtkActor *yShaftActor = NULL;
  vtkActor *zShaftActor = NULL;

  vtkConeSource *xCone = vtkConeSource::New();
  vtkPolyDataMapper *xConeMapper = vtkPolyDataMapper::New();
  xConeActor = vtkActor::New();
  xCone->SetResolution(40);
  xCone->SetHeight(shaftLength / 10.0);
  xCone->SetRadius(shaftLength * coneRadiusRatio);
  xCone->SetDirection(1, 0, 0);
  xConeMapper->SetInputConnection(xCone->GetOutputPort());
  xConeActor->SetPosition(shaftLength, y, z);
  xConeMapper->ScalarVisibilityOff();
  xConeActor->SetMapper(xConeMapper);
  xConeActor->GetProperty()->SetColor(1, 0, 0);
  
  vtkCylinderSource *xShaft = vtkCylinderSource::New();
  vtkPolyDataMapper *xShaftMapper = vtkPolyDataMapper::New();
  xShaftActor = vtkActor::New();
  xShaft->SetResolution(40);
  xShaft->SetHeight(shaftLength);
  xShaft->SetRadius(shaftLength / 200.0);
  xShaftMapper->SetInputConnection(xShaft->GetOutputPort());
  xShaftMapper->ScalarVisibilityOff();
  xShaftActor->RotateZ(90);
  xShaftActor->SetPosition(shaftLength / 2.0, y, z);
  xShaftActor->SetMapper(xShaftMapper);
  xShaftActor->GetProperty()->SetColor(1, 0, 0);

  vtkConeSource *yCone = vtkConeSource::New();
  vtkPolyDataMapper *yConeMapper = vtkPolyDataMapper::New();
  yConeActor = vtkActor::New();
  yCone->SetResolution(40);
  yCone->SetHeight(shaftLength / 10.0);
  yCone->SetRadius(shaftLength * coneRadiusRatio);
  yCone->SetDirection(0, 1, 0);
  yConeMapper->SetInputConnection(yCone->GetOutputPort());
  yConeActor->SetPosition(x, shaftLength, z);
  yConeMapper->ScalarVisibilityOff();
  yConeActor->SetMapper(yConeMapper);
  yConeActor->GetProperty()->SetColor(0, 1, 0);
  
  vtkCylinderSource *yShaft = vtkCylinderSource::New();
  vtkPolyDataMapper *yShaftMapper = vtkPolyDataMapper::New();
  yShaftActor = vtkActor::New();
  yShaft->SetResolution(40);
  yShaft->SetHeight(shaftLength);
  yShaft->SetRadius(shaftLength / 200.0);
  yShaftMapper->SetInputConnection(yShaft->GetOutputPort());
  yShaftMapper->ScalarVisibilityOff();
  yShaftActor->SetPosition(x, shaftLength / 2.0, z);
  yShaftActor->SetMapper(yShaftMapper);
  yShaftActor->GetProperty()->SetColor(0, 1, 0);

  vtkConeSource *zCone = vtkConeSource::New();
  vtkPolyDataMapper *zConeMapper = vtkPolyDataMapper::New();
  zConeActor = vtkActor::New();
  zCone->SetResolution(40);
  zCone->SetHeight(shaftLength / 10.0);
  zCone->SetRadius(shaftLength * coneRadiusRatio);
  zCone->SetDirection(0, 0, 1);
  zConeMapper->SetInputConnection(zCone->GetOutputPort());
  zConeActor->SetPosition(x, y, shaftLength);
  zConeMapper->ScalarVisibilityOff();
  zConeActor->SetMapper(zConeMapper);
  zConeActor->GetProperty()->SetColor(0, 0, 1);
  
  vtkCylinderSource *zShaft = vtkCylinderSource::New();
  vtkPolyDataMapper *zShaftMapper = vtkPolyDataMapper::New();
  zShaftActor = vtkActor::New();
  zShaft->SetResolution(40);
  zShaft->SetHeight(shaftLength);
  zShaft->SetRadius(shaftLength / 200.0);
  zShaftMapper->SetInputConnection(zShaft->GetOutputPort());
  zShaftMapper->ScalarVisibilityOff();
  zShaftActor->RotateX(90);
  zShaftActor->SetPosition(x, y, shaftLength / 2.0);
  zShaftActor->SetMapper(zShaftMapper);
  zShaftActor->GetProperty()->SetColor(0, 0, 1);
  
  vtkCubeSource *origin = vtkCubeSource::New();
  vtkPolyDataMapper *oMapper = vtkPolyDataMapper::New();
  oActor = vtkActor::New();
  origin->SetXLength(shaftLength * 0.025);
  origin->SetYLength(shaftLength * 0.025);
  origin->SetZLength(shaftLength * 0.025);
  oMapper->SetInputConnection(origin->GetOutputPort());
  oMapper->ScalarVisibilityOff();
  oActor->SetMapper(oMapper);
  oActor->GetProperty()->SetColor(1, 1, 1);
  
  char xLabel[] = "+x";
  char yLabel[] = "+y";
  char zLabel[] = "+z";

  r->AddActor(xConeActor);
  r->AddActor(yConeActor);
  r->AddActor(zConeActor);
  r->AddActor(xShaftActor);
  r->AddActor(yShaftActor);
  r->AddActor(zShaftActor);
  r->AddActor(oActor);

  AddFollowingText(xLabel, 23, -1, 0, 1, 0, 0, r);
  AddFollowingText(yLabel, -1, 23, 0, 0, 1, 0, r);
  AddFollowingText(zLabel, 0, -1, 23, 0, 0, 1, r);
*/
}