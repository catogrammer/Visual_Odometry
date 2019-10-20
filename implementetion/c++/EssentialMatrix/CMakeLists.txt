project (EssentialMatrix)

set(SOURCES "EssentialMatrix.cpp")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(EssentialMatrix SHARED ${SOURCES})
target_include_directories(EssentialMatrix PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})