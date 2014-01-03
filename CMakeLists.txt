cmake_minimum_required (VERSION 2.6)
project (mailcore2) 

IF(APPLE)
  set(CMAKE_CXX__FLAGS "-std=c++11 -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "-lc++ -stdlib=libc++")
ENDIF()

add_subdirectory (src)
IF(APPLE)
  add_subdirectory (tests)
ENDIF()
