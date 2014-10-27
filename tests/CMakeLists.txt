include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src/include)
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/ctemplate-osx/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/uchardet-osx/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../Externals/libetpan-osx/lib
)

find_library(FOUNDATIONFRAMEWORK NAMES Foundation)
find_library(SECURITYFRAMEWORK NAMES Security)
find_library(CORESERVICESFRAMEWORK NAMES CoreServices)

add_executable (tests main.mm test-all.mm) 
target_link_libraries (tests MailCore z etpan xml2 uchardet sasl2 iconv tidy ctemplate ssl crypto
    ${FOUNDATIONFRAMEWORK} ${SECURITYFRAMEWORK} ${CORESERVICESFRAMEWORK})
