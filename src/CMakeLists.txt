set(CMAKE_INCLUDE_CURRENT_DIR ON)
IF(APPLE)
  set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
ELSE()
  set(CMAKE_CXX_FLAGS "-std=gnu++0x")
ENDIF()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(core)
include(async)
include(objc)

file(GLOB_RECURSE
  source_files
  *.h
  *.m
  *.c
)

SET_SOURCE_FILES_PROPERTIES(
  ${source_files}
  PROPERTIES LANGUAGE C
)

file(COPY
  MailCore.h
  ${public_headers_objc}
  ${public_headers_core}
  ${public_headers_async}

  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}/include/MailCore
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include
  ${async_includes}
  ${core_includes}
  ${objc_includes}
  ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/ctemplate/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/icu4c/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/libetpan/include
  /usr/include/tidy
  /usr/include/libxml2/
)

add_library(MailCore
  ${core_files}
  ${async_files}
  ${objc_files}
)
