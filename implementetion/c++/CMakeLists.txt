
cmake_minimum_required (VERSION 2.6)

project (Visual_Odometry)

if(${CMAKE_VERSION} VERSION_LESS "3.1")
    add_compile_options(-std=c++14)
else()
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package( OpenCV REQUIRED )

include_directories (${OpenCV_INCLUDE_DIRS} 
                     "${CMAKE_CURRENT_SOURCE_DIR}/Camera"
                     "${CMAKE_CURRENT_SOURCE_DIR}/EsstialMatrix")

add_subdirectory (Camera)
add_subdirectory (EssentialMatrix)

set (ESSENTIAL_LIB ${ESSENTIAL_LIB} EssentialMatrix)
set (CAMERA_LIB ${CAMERA_LIB} Camera)

# add the executable
# add_executable (Feat_Detect Features_Detect.cpp)
# add_executable (Feat_Match Features_Match.cpp)
add_executable (Matching Matching.cpp)
add_executable (EssentialMatrix_Calculation Rt_Calculation.cpp)
add_executable (PointPosition PointPosition.cpp)

# target_link_libraries (Feat_Detect ${OpenCV_LIBS})
# target_link_libraries (Feat_Match ${OpenCV_LIBS})
target_link_libraries (Matching ${OpenCV_LIBS})
target_link_libraries (EssentialMatrix_Calculation ${ESSENTIAL_LIB} ${CAMERA_LIB})
target_link_libraries (PointPosition ${CAMERA_LIB})
