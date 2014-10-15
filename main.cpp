//
// This example demonstrates how to read a series of dicom images
// and how to scroll with the mousewheel or the up/down keys
// through all slices
//
// some standard vtk headers
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
// headers needed for this example
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
// needed to easily convert int to std::string
#include <sstream>

//for vector
//#include <vtkFieldData.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkGlyph2D.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkCamera.h>

#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>



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
   vtkImageViewer2* _ImageViewer;
   vtkTextMapper* _StatusMapper;
   int _Slice;
   int _MinSlice;
   int _MaxSlice;

public:
   void SetImageViewer(vtkImageViewer2* imageViewer) {
      _ImageViewer = imageViewer;
      _MinSlice = imageViewer->GetSliceMin();
      _MaxSlice = imageViewer->GetSliceMax();
      _Slice = _MinSlice;
      cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
   }

   void SetStatusMapper(vtkTextMapper* statusMapper) {
      _StatusMapper = statusMapper;
   }


protected:
   void MoveSliceForward() {
      if(_Slice < _MaxSlice) {
         _Slice += 1;
         cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
         _ImageViewer->SetSlice(_Slice);
         std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
         _StatusMapper->SetInput(msg.c_str());
         _ImageViewer->Render();
      }
   }

   void MoveSliceBackward() {
      if(_Slice > _MinSlice) {
         _Slice -= 1;
         cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
         _ImageViewer->SetSlice(_Slice);
         std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
         _StatusMapper->SetInput(msg.c_str());
         _ImageViewer->Render();
      }
   }


   virtual void OnKeyDown() {
      std::string key = this->GetInteractor()->GetKeySym();
      if(key.compare("Up") == 0) {
         //cout << "Up arrow key was pressed." << endl;
         MoveSliceForward();
      }
      else if(key.compare("Down") == 0) {
         //cout << "Down arrow key was pressed." << endl;
         MoveSliceBackward();
      }
      // forward event
      vtkInteractorStyleImage::OnKeyDown();
   }


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

vtkStandardNewMacro(myVtkInteractorStyleImage)


int main(int argc, char* argv[])
{
    // Verify input arguments
    if ( argc != 2 )
    {
        std::cout << "Usage: " << argv[0] << " VectorFile.vtu (it is assumed that all dicom files are in the same folder dicom)" << std::endl;
        return EXIT_FAILURE;
    }

    ///load vectors and create an actor
    // Create the reader for the data.
    // This is the data that will be volume rendered.
    std::string filename = argv[1];
    std::cout << "Loading " << filename.c_str() << std::endl;
    vtkSmartPointer<vtkUnstructuredGridReader> readerVector = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    readerVector->SetFileName(filename.c_str());
    readerVector->Update();

    //set places and orientations of all glyphs
    vtkSmartPointer<vtkArrowSource> glyphForm = vtkSmartPointer<vtkArrowSource>::New();
    vtkSmartPointer<vtkGlyph3D> glyphs = vtkSmartPointer<vtkGlyph3D>::New();
    glyphs->SetSourceConnection(glyphForm->GetOutputPort()); //set type of glyph
    glyphs->SetInputData(readerVector->GetOutput()/*myUnstructuredGrid*/); //set points and directions
    glyphs->ScalingOn();
    glyphs->SetVectorModeToUseVector();
    glyphs->SetScaleModeToScaleByVector();
    glyphs->SetScaleFactor(0.20);
    glyphs->Update();




    //usual stuffs for ploting
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyphMapper->SetInputConnection(glyphs->GetOutputPort());
    glyphMapper->ScalarVisibilityOff();


    vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
    glyphActor->SetMapper(glyphMapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(glyphActor);
    renderer->SetBackground(0.328125, 0.347656, 0.425781);







    ///load all dicom and create actor
    std::string folder = "dicom";//argv[1];

    // Read all the DICOM files in the specified directory.
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    ///reader->SetDirectoryName(folder.c_str());
    reader->SetFileName("dicom/OZON_MATTHEW.MR.ESSAIS_KM.0007.0001.2014.01.20.21.11.15.125000.4868800.IMA");
    reader->Update();

    vtkSmartPointer<vtkImageData> originalImage = vtkSmartPointer<vtkImageData>::New();
    originalImage = reader->GetOutput();
    ///vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    ///imageViewer->SetInputConnection(reader->GetOutputPort());

    // (xmin, ymin, xmax, ymax)
    double originalViewport[4] = {0.0, 0.0, 1.0, 1.0};

    vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
    originalActor->GetMapper()->SetInputData(originalImage);
    originalActor->InterpolateOff();

    //vtkSmartPointer<vtkRenderer> originalRenderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetViewport(originalViewport);
    renderer->SetBackground(.2, .3, .6);
    renderer->AddActor(originalActor);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderer->SetViewport(originalViewport);
    renderWindow->AddRenderer(renderer);
    //renderWindow->AddRenderer(originalRenderer);




    // Render and interact
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    //vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
    ///myInteractorStyle->SetImageViewer(imageViewer);

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetInteractorStyle( style );//myInteractorStyle
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindow->Render();
    renderWindowInteractor->Start();
}
