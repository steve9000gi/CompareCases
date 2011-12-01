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

const double coneRadiusRatio = 0.015; // 0.025; // 0.0375;
const double coneHeightRatio = 0.045; // 0.067; // 0.1;
const double shaftRadiusRatio = 0.003; // 0.004; // 0.005;


///ctor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Axes::Axes()
:	//vText(NULL),
	//vTextMapper(NULL),
	//vTextActor(NULL),
	oActor(NULL),
	xConeActor(NULL),
	yConeActor(NULL),
	zConeActor(NULL),
	xShaftActor(NULL),
	yShaftActor(NULL),
	zShaftActor(NULL),
	ghostOActor(NULL),
	ghostXConeActor(NULL),
	ghostYConeActor(NULL),
	ghostXShaftActor(NULL),
	ghostYShaftActor(NULL)
{}

///dtor/////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Axes::~Axes()
{
	//if (vText) vText->Delete();
	//if (vTextMapper) vTextMapper->Delete();
	//if (vTextActor) vTextActor->Delete();
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
vtkFollower *Axes::AddFollowingText(AxisType axis, char *text, double x, 
	double y, double z, double r, double g, double b, double scale,
	vtkRenderer *ren)
{
	vText[axis] = vtkVectorText::New();
	vTextMapper[axis] = vtkPolyDataMapper::New();
	vTextActor[axis] = vtkFollower::New();

	vText[axis]->SetText(text);
	vTextMapper[axis]->SetInputConnection(vText[axis]->GetOutputPort());
	vTextActor[axis]->SetMapper(vTextMapper[axis]);
	vTextActor[axis]->SetScale(scale, scale, scale);
	vTextActor[axis]->AddPosition(x, y, z);
	vTextActor[axis]->GetProperty()->SetColor(r, g, b);
	vTextActor[axis]->SetCamera(ren->GetActiveCamera());

	return vTextActor[axis];
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
vtkAssembly *Axes::InsertThis(vtkRenderer *r, double shaftLen /*= 20.0 */,
		double x /*= 0.0 */, double y /* = 0.0 */, double z /*= 0.0 */)
{
	this->shaftLength = shaftLen;
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
void Axes::setAxisLabel(AxisType axis, char *label)
{
	vText[axis]->SetText(label);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void Axes::setAxisLabelPosition(AxisType axis, double x, double y, double z)
{
	vTextActor[axis]->SetPosition(x, y, z);
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
vtkAssembly *Axes::createGhostAxes(vtkRenderer *r, double shaftLen /*= 20.0 */,
		double x /*= 0.0 */, double y /* = 0.0 */, double z /*= 0.0 */)
{
	const double shaftPosition = shaftLen / 2.0;
	const double opacity = 0.25;

	vtkConeSource *ghostXCone = vtkConeSource::New();
	vtkPolyDataMapper *ghostXConeMapper = vtkPolyDataMapper::New();
	ghostXConeActor = vtkActor::New();
	ghostXConeActor->PickableOff();
	ghostXCone->SetResolution(40);
	ghostXCone->SetHeight(shaftLength * coneHeightRatio);
	ghostXCone->SetRadius(shaftLength * coneRadiusRatio);
	ghostXCone->SetDirection(1, 0, 0);
	ghostXConeMapper->SetInputConnection(ghostXCone->GetOutputPort());
	ghostXConeActor->SetPosition(shaftLength, 0, 0);
	ghostXConeMapper->ScalarVisibilityOff();
	ghostXConeActor->SetMapper(ghostXConeMapper);
	ghostXConeActor->GetProperty()->SetColor(1, 0, 0);
	ghostXConeActor->GetProperty()->SetOpacity(opacity);
  
	vtkCylinderSource *ghostXShaft = vtkCylinderSource::New();
	vtkPolyDataMapper *ghostXShaftMapper = vtkPolyDataMapper::New();
	ghostXShaftActor = vtkActor::New();
	ghostXShaftActor->PickableOff();
	ghostXShaft->SetResolution(40);
	ghostXShaft->SetHeight(shaftLength);
	ghostXShaft->SetRadius(shaftLength * shaftRadiusRatio);
	ghostXShaftMapper->SetInputConnection(ghostXShaft->GetOutputPort());
	ghostXShaftMapper->ScalarVisibilityOff();
	ghostXShaftActor->RotateZ(90);
	ghostXShaftActor->SetPosition(shaftPosition, 0, 0);
	ghostXShaftActor->SetMapper(ghostXShaftMapper);
	ghostXShaftActor->GetProperty()->SetColor(1, 0, 0);
	ghostXShaftActor->GetProperty()->SetOpacity(opacity);

	vtkConeSource *ghostYCone = vtkConeSource::New();
	vtkPolyDataMapper *ghostYConeMapper = vtkPolyDataMapper::New();
	ghostYConeActor = vtkActor::New();
	ghostYConeActor->PickableOff();
	ghostYCone->SetResolution(40);
	ghostYCone->SetHeight(shaftLength * coneHeightRatio);
	ghostYCone->SetRadius(shaftLength * coneRadiusRatio);
	ghostYCone->SetDirection(0, 1, 0);
	ghostYConeMapper->SetInputConnection(ghostYCone->GetOutputPort());
	ghostYConeActor->SetPosition(0, shaftLength, 0);
	ghostYConeMapper->ScalarVisibilityOff();
	ghostYConeActor->SetMapper(ghostYConeMapper);
	ghostYConeActor->GetProperty()->SetColor(0, 1, 0);
	ghostYConeActor->GetProperty()->SetOpacity(opacity);
  
	vtkCylinderSource *ghostYShaft = vtkCylinderSource::New();
	vtkPolyDataMapper *ghostYShaftMapper = vtkPolyDataMapper::New();
	ghostYShaftActor = vtkActor::New();
	ghostYShaftActor->PickableOff();
	ghostYShaft->SetResolution(40);
	ghostYShaft->SetHeight(shaftLength);
	ghostYShaft->SetRadius(shaftLength * shaftRadiusRatio);
	ghostYShaftMapper->SetInputConnection(ghostYShaft->GetOutputPort());
	ghostYShaftMapper->ScalarVisibilityOff();
	ghostYShaftActor->SetPosition(0, shaftPosition, 0);
	ghostYShaftActor->SetMapper(ghostYShaftMapper);
	ghostYShaftActor->GetProperty()->SetColor(0, 1, 0);
	ghostYShaftActor->GetProperty()->SetOpacity(opacity);

	vtkCubeSource *ghostOrigin = vtkCubeSource::New();
	vtkPolyDataMapper *ghostOMapper = vtkPolyDataMapper::New();
	ghostOActor = vtkActor::New();
	ghostOActor->PickableOff();
	ghostOrigin->SetXLength(shaftLength * 0.025);
	ghostOrigin->SetYLength(shaftLength * 0.025);
	ghostOrigin->SetZLength(shaftLength * 0.025);
	ghostOMapper->SetInputConnection(ghostOrigin->GetOutputPort());
	ghostOMapper->ScalarVisibilityOff();
	ghostOActor->SetMapper(ghostOMapper);
	ghostOActor->GetProperty()->SetColor(1, 1, 1);
	ghostOActor->GetProperty()->SetOpacity(opacity);

 	vtkAssembly *assembly = vtkAssembly::New();
	assembly->AddPart(ghostXConeActor);
	assembly->AddPart(ghostYConeActor);
	assembly->AddPart(ghostXShaftActor);
	assembly->AddPart(ghostYShaftActor);
	assembly->AddPart(ghostOActor);

	//r->AddActor(assembly);

	return assembly;
}





