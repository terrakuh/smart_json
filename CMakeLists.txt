cmake_minimum_required(VERSION 3.14)

project(
  smart_json
  VERSION 0.1.0
  DESCRIPTION "Header only automatic JSON encoding and decoding with Boost.Describe"
  HOMEPAGE_URL "https://github.com/terrakuh/smart_json"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(default_build_type Release)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type for ${PROJECT_NAME} to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE
      "${default_build_type}"
      CACHE STRING "Build type." FORCE
  )
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(SMART_JSON_BUILD_EXAMPLES "Build the provided examples." OFF)
option(SMART_JSON_FETCH_CONTENT "Automagically fetches the missing and required dependencies from github.com"
       ON
)

if(SMART_JSON_FETCH_CONTENT)
  set(find_package_option QUIET)
else()
  set(find_package_option REQUIRED)
endif()

find_package(Boost 1.78 ${find_package_option})
if(NOT Boost_FOUND AND SMART_JSON_FETCH_CONTENT)
  message(STATUS "Fetching Boost")
  include(FetchContent)
  FetchContent_Declare(
    Boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
    URL_HASH SHA256=94ced8b72956591c4775ae2207a9763d3600b30d9d7446562c552f0a14a63be7
  )
  FetchContent_MakeAvailable(Boost)
  FetchContent_GetProperties(Boost SOURCE_DIR Boost_INCLUDE_DIR)
  find_package(Boost 1.78 REQUIRED)
endif()

# Library
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/include/*.(hpp|inl)")
add_library(smart_json INTERFACE)
add_library(smart_json::smart_json ALIAS smart_json)
target_sources(smart_json INTERFACE ${sources})
target_include_directories(
  smart_json INTERFACE "$<INSTALL_INTERFACE:include>"
                       "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
target_link_libraries(smart_json INTERFACE Boost::boost)

install(TARGETS smart_json EXPORT ${PROJECT_NAME}-targets)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include/)

# Examples
if(SMART_JSON_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Install
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_NAME}-config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "${LIBRARY_INSTALL_DIR}/cmake/${PROJECT_NAME}"
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME}
)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" DESTINATION include/${PROJECT_NAME}/)
install(
  EXPORT ${PROJECT_NAME}-targets
  DESTINATION lib/cmake/${PROJECT_NAME}
  NAMESPACE ${PROJECT_NAME}::
  EXPORT_LINK_INTERFACE_LIBRARIES
)

# CPack
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT "Yunus Ayar")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR TGZ)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
