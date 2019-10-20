project (Camera)

set(SOURCES "Camera.cpp")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(Camera SHARED ${SOURCES})
target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})