#include <myvtkinteractorstyleimage.h>

void myVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer)
{
   _ImageViewer = imageViewer;
   _MinSlice = imageViewer->GetSliceMin();
   _MaxSlice = imageViewer->GetSliceMax();
   _Slice = _MinSlice;
   //cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
}


void myVtkInteractorStyleImage::setVectorFileName(std::vector<std::string> fileNames)
{
    _fileNames = fileNames;
    vtkSmartPointer<vtkUnstructuredGridReader> readerVector = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    readerVector->SetFileName(_fileNames.at(0).data());
    readerVector->Update();

    //set places and orientations of all glyphs
    vtkSmartPointer<vtkArrowSource> glyphForm = vtkSmartPointer<vtkArrowSource>::New();
    vtkSmartPointer<vtkGlyph3D> glyphs = vtkSmartPointer<vtkGlyph3D>::New();
    glyphs->SetSourceConnection(glyphForm->GetOutputPort()); //set type of glyph
    #if VTK_MAJOR_VERSION <= 5
    glyphs->SetInput(readerVector->GetOutput()); //set points and directions
    #else
    glyphs->SetInputData(readerVector->GetOutput()); //set points and directions ///myUnstructuredGrid
    #endif
    glyphs->ScalingOn();
    glyphs->SetVectorModeToUseVector();
    glyphs->SetScaleModeToScaleByVector();
    glyphs->SetScaleFactor(0.20);
    glyphs->Update();

    //usual stuffs for ploting
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyphMapper->SetInputConnection(glyphs->GetOutputPort());
    glyphMapper->ScalarVisibilityOff();
    _glyphActor = vtkSmartPointer<vtkActor>::New();
    _glyphActor->SetMapper(glyphMapper);
    _ImageViewer->GetRenderer()->AddActor2D(_glyphActor);
}


void myVtkInteractorStyleImage::MoveSliceForward(void)
{
   if(_Slice < _MaxSlice) {
      _Slice += 1;
      //cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
      _ImageViewer->SetSlice(_Slice);
      //std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
      //_StatusMapper->SetInput(msg.c_str());
      if(_Slice<_MaxSlice) setActorVector();
      _ImageViewer->Render();
   }
}

void myVtkInteractorStyleImage::MoveSliceBackward(void)
{
   if(_Slice > _MinSlice) {
      _Slice -= 1;
      //cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
      _ImageViewer->SetSlice(_Slice);
      //std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
      //_StatusMapper->SetInput(msg.c_str());
      if(_Slice<_MaxSlice) setActorVector();
      _ImageViewer->Render();
   }
}


void myVtkInteractorStyleImage::setActorVector(void)
{
    vtkSmartPointer<vtkUnstructuredGridReader> readerVector = vtkSmartPointer<vtkUnstructuredGridReader>::New();
    readerVector->SetFileName(_fileNames.at(_Slice).data());
    readerVector->Update();

    //set places and orientations of all glyphs
    vtkSmartPointer<vtkArrowSource> glyphForm = vtkSmartPointer<vtkArrowSource>::New();
    vtkSmartPointer<vtkGlyph3D> glyphs = vtkSmartPointer<vtkGlyph3D>::New();
    glyphs->SetSourceConnection(glyphForm->GetOutputPort()); //set type of glyph
    #if VTK_MAJOR_VERSION <= 5
    glyphs->SetInput(readerVector->GetOutput()); //set points and directions
    #else
    glyphs->SetInputData(readerVector->GetOutput()); //set points and directions ///myUnstructuredGrid
    #endif
    glyphs->ScalingOn();
    glyphs->SetVectorModeToUseVector();
    glyphs->SetScaleModeToScaleByVector();
    glyphs->SetScaleFactor(0.20+0.2*((float) _Slice));//
    glyphs->Update();

    //usual stuffs for ploting
    vtkSmartPointer<vtkPolyDataMapper> glyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyphMapper->SetInputConnection(glyphs->GetOutputPort());
    glyphMapper->ScalarVisibilityOff();
    //vtkSmartPointer<vtkActor> glyphActor = vtkSmartPointer<vtkActor>::New();
    _glyphActor->SetMapper(glyphMapper);
    _ImageViewer->UpdateDisplayExtent();
    //_ImageViewer->GetRenderer()->AddActor2D(glyphActor);
}

void myVtkInteractorStyleImage::OnKeyDown(void)
{
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
