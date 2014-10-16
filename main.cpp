//example inpired by vtk wiki:
//http://www.cmake.org/Wiki/VTK/Examples/Cxx/Images/Gradient
//http://www.vtk.org/Wiki/VTK/Examples/Cxx/IO/ReadDICOMSeries
//http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/HedgeHog

/// This example demonstrates how to read a series of dicom images
/// stored in a folder (however, there is no sorting options). You
/// can go through slices/time using up/down keys which not only
/// change slices but associated vetor field. You can use the mouse
/// to control brightness, contrast and zoom in/out. Controls are
/// defined by the class myVtkInteractorStyleImage OnKey for specific
/// actions (slice/time and vector). Other actions are inherited
/// from vtkInteractorStyleImage.

/// the main function: load a series of vector fields stored in vtk
/// files containing unstructured data (.vtu), load dicom files
/// store in a directory "dicom", show images and vectors.


#include <vtkDICOMImageReader.h> /*load the dicom files stored in a directory*/
#include <vtkImageViewer2.h> /*handle visualization from mapper to render*/
#include <vtkRenderWindowInteractor.h> /*in order to define an interactor with data*/
#include <myvtkinteractorstyleimage.h> /*define the interactions (inherites from the usual interaction style)*/
#include <vtkObjectFactory.h>

vtkStandardNewMacro(myVtkInteractorStyleImage)

int main(int argc, char* argv[])
{
    std::vector<std::string> fileNames;
    fileNames.push_back("test2D.vtu");
    fileNames.push_back("test2D.vtu");
    fileNames.push_back("test2D.vtu");

    // Read all the DICOM files in the specified directory.
    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName("dicom");
    reader->Update();


    // Visualize
    vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    imageViewer->SetInputConnection(reader->GetOutputPort()); //set the data to visualize

    // create an interactor with our own style (inherit from vtkInteractorStyleImage)
    // in order to catch mousewheel and key events
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
    myInteractorStyle->SetImageViewer(imageViewer);
    myInteractorStyle->setVectorFileName(fileNames); //set vector files
    imageViewer->SetupInteractor(renderWindowInteractor);
    // make the interactor use our own interactorstyle
    renderWindowInteractor->SetInteractorStyle(myInteractorStyle);

    // initialize rendering and interaction
    imageViewer->Render();
    imageViewer->GetRenderer()->ResetCamera();
    imageViewer->Render();
    renderWindowInteractor->Start();
    return EXIT_SUCCESS;

}
