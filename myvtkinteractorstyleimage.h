#ifndef MYVTKINTERACTORSTYLEIMAGE_H
#define MYVTKINTERACTORSTYLEIMAGE_H

#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// headers needed for this example
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
// needed to easily convert int to std::string
#include <sstream>

//for vector
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkArrowSource.h>
#include <vtkGlyph2D.h>
#include <vtkPolyDataMapper.h>


// helper class to format slice status message
class StatusMessage {
public:
   static std::string Format(int slice, int maxSlice) {
      std::stringstream tmp;
      tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
      return tmp.str();
   }
};


// Define own interaction style
class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
   static myVtkInteractorStyleImage* New();
   vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage)

protected:
   std::vector<std::string> _fileNames;
   vtkSmartPointer<vtkActor> _glyphActor;
   vtkImageViewer2* _ImageViewer;
   //vtkTextMapper* _StatusMapper;
   int _Slice;
   int _MinSlice;
   int _MaxSlice;

public:
   void SetImageViewer(vtkImageViewer2* imageViewer);
   void setVectorFileName(std::vector<std::string> fileNames);

   /*void SetStatusMapper(vtkTextMapper* statusMapper) {
      _StatusMapper = statusMapper;
   }*/


protected:
   void MoveSliceForward(void);
   void MoveSliceBackward(void);
   void setActorVector(void);
   virtual void OnKeyDown(void);


   /*virtual void OnMouseWheelForward() {
      //std::cout << "Scrolled mouse wheel forward." << std::endl;
      MoveSliceForward();
      // don't forward events, otherwise the image will be zoomed
      // in case another interactorstyle is used (e.g. trackballstyle, ...)
      // vtkInteractorStyleImage::OnMouseWheelForward();
   }


   virtual void OnMouseWheelBackward() {
      //std::cout << "Scrolled mouse wheel backward." << std::endl;
      if(_Slice > _MinSlice) {
         MoveSliceBackward();
      }
      // don't forward events, otherwise the image will be zoomed
      // in case another interactorstyle is used (e.g. trackballstyle, ...)
      // vtkInteractorStyleImage::OnMouseWheelBackward();
   }*/
};

#endif // MYVTKINTERACTORSTYLEIMAGE_H
