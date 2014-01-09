include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/include)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/ctemplate/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/icu4c/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/libetpan/lib
)

find_library(FOUNDATIONFRAMEWORK NAMES Foundation)
find_library(SECURITYFRAMEWORK NAMES Security)
find_library(CORESERVICESFRAMEWORK NAMES CoreServices)

add_executable (tests main.mm test-all.mm) 
if(APPLE)
  set(ICU_LIBRARIES icucore)
else()
  set(ICU_LIBRARIES icudata icui18n icuuc)
endif()
target_link_libraries (tests MailCore z etpan xml2 sasl2 iconv tidy ctemplate ssl crypto ${ICU_LIBRARIES}
    ${FOUNDATIONFRAMEWORK} ${SECURITYFRAMEWORK} ${CORESERVICESFRAMEWORK})
