IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
pkg_check_modules (GLIB2 glib-2.0)
set(icu_libraries icudata icui18n icuio icule iculx icutest icutu icuuc)
set(linux_libraries ${icu_libraries} pthread uuid)
ENDIF()

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/../src/include
    ${GLIB2_INCLUDE_DIRS}
)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/ctemplate-osx/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/uchardet-osx/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/libetpan-osx/lib
)

IF(APPLE)
    find_library(FOUNDATIONFRAMEWORK NAMES Foundation)
    find_library(SECURITYFRAMEWORK NAMES Security)
    find_library(CORESERVICESFRAMEWORK NAMES CoreServices)
    set(test_mac_files test-all-mac.mm)
    set(mac_libraries iconv)
ENDIF()

add_executable (tests main.cpp test-all.cpp ${test_mac_files})
target_link_libraries (
    tests MailCore z etpan xml2 uchardet sasl2 tidy ctemplate ssl crypto
    ${mac_libraries} ${linux_libraries}
    ${GLIB2_LIBRARIES} ${FOUNDATIONFRAMEWORK} ${SECURITYFRAMEWORK} ${CORESERVICESFRAMEWORK})

