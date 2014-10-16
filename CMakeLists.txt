cmake_minimum_required(VERSION 2.8)

PROJECT(displayDicomAndVector)

include_directories(.)
link_directories(.)


find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#Configuration de l'exécutable
file(
        GLOB
        source_files
        *.cpp *.h
)


add_executable(
        displayDicomAndVector
        ${source_files}
)

if(VTK_LIBRARIES)
  target_link_libraries(displayDicomAndVector ${VTK_LIBRARIES})
else()
  target_link_libraries(displayDicomAndVector vtkHybrid vtkWidgets)
endif()
