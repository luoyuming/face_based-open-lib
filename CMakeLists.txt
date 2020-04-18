cmake_minimum_required(VERSION 3.5)
project(face_project)



AUX_SOURCE_DIRECTORY(. DIR_SRCS)

set(PYBIND11_CPP_STANDARD -std=c++11)

# find required opencv
find_package(OpenCV REQUIRED)
MESSAGE( " *** OpenCV_INCLUDE_DIRS : " ${OpenCV_INCLUDE_DIRS} )
MESSAGE( " *** OpenCV_LIBRARY_DIRS : " ${OpenCV_LIBRARY_DIRS} )

find_package(PythonLibs 3 REQUIRED)
MESSAGE( " *** PYTHON_INCLUDE_DIRS : " ${PYTHON_INCLUDE_DIRS} )
MESSAGE( " *** PYTHON_LIBRARIES : " ${PYTHON_LIBRARIES} )


# include directories
INCLUDE_DIRECTORIES(
  ${OpenCV_INCLUDE_DIRS} 
  ./pybind11/include
  ./include  
  ./
)


# lib directories
LINK_DIRECTORIES(  
    ${OpenCV_LIBRARY_DIRS}
	${PYTHON_LIBRARIES}
    /home/lym/arcface/lib/linux_x64
	/home/tarantu/luo/lib/linux_x64
)

list(APPEND BASE_SDK -larcsoft_face -larcsoft_face_engine)
list(APPEND OpenCV_LIBS -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_video -lopencv_videoio)

add_subdirectory(pybind11)
#pybind11_add_module(mlvideo  ${DIR_SRCS})
add_library(tface MODULE ${DIR_SRCS})
target_link_libraries(tface PRIVATE pybind11::module ${PYTHON_LIBRARIES} ${BASE_SDK} ${OpenCV_LIBS})
#target_link_libraries(tface PRIVATE pybind11::module)
set_target_properties(tface PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                         SUFFIX "${PYTHON_MODULE_EXTENSION}")
