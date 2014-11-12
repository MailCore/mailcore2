pkg_check_modules (GLIB2   glib-2.0)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/include ${GLIB2_INCLUDE_DIRS})
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/ctemplate-osx/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/uchardet-osx/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/libetpan-osx/lib
)

#find_library(FOUNDATIONFRAMEWORK NAMES Foundation)
#find_library(SECURITYFRAMEWORK NAMES Security)
#find_library(CORESERVICESFRAMEWORK NAMES CoreServices)

add_executable (tests main.cpp) 
target_link_libraries (tests MailCore z etpan xml2 uchardet sasl2 tidy ctemplate ssl crypto pthread icudata icui18n icuio icule iculx icutest icutu icuuc uuid ${GLIB2_LIBRARIES})


