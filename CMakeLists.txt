cmake_minimum_required (VERSION 2.6)
project (mailcore2) 

IF(APPLE)
  set(CMAKE_CXX__FLAGS "-std=c++11 -stdlib=libc++")
  set(CMAKE_EXE_LINKER_FLAGS "-lc++ -stdlib=libc++")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  find_package(PkgConfig)
ENDIF()

add_subdirectory (src)
add_subdirectory (tests)

