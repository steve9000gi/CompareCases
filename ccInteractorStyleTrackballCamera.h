//  Program:   CompareCases
//  Module:    ccInteractorStyleTrackballCamera.h


#ifndef __ccInteractorStyleTrackballCamera_h
#define __ccInteractorStyleTrackballCamera_h

#include "vtkInteractorStyleTrackballCamera.h"

class VTK_RENDERING_EXPORT ccInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera
{
public:
  static ccInteractorStyleTrackballCamera *New();
  vtkTypeMacro(ccInteractorStyleTrackballCamera,vtkInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Event bindings controlling the effects of pressing mouse buttons
  // or moving the mouse.
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnMouseWheelForward();
  virtual void OnMouseWheelBackward();

  // SAC modified from vtkInteractorStyle:
  virtual void OnChar();

  // These methods for the different interactions in different modes
  // are overridden in subclasses to perform the correct motion. Since
  // they are called by OnTimer, they do not have mouse coord parameters
  // (use interactor's GetEventPosition and GetLastEventPosition)
  virtual void Rotate();
  virtual void Spin();
  virtual void Pan();
  virtual void Dolly();

  // Description:
  // Set the apparent sensitivity of the interactor style to mouse motion.
  vtkSetMacro(MotionFactor,double);
  vtkGetMacro(MotionFactor,double);
  
protected:
  ccInteractorStyleTrackballCamera();
  ~ccInteractorStyleTrackballCamera();

  double MotionFactor;

  virtual void Dolly(double factor);

private:
  ccInteractorStyleTrackballCamera(const ccInteractorStyleTrackballCamera&);  // Not implemented.
  void operator=(const ccInteractorStyleTrackballCamera&);  // Not implemented.
};

#endif
