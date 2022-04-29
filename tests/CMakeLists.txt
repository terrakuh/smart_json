find_package(Catch2 2 QUIET ${find_package_option})
if(NOT Catch2_FOUND AND SMART_JSON_FETCH_CONTENT)
  message(STATUS "Fetching Catch2")
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.9.tar.gz
    URL_HASH SHA256=06dbc7620e3b96c2b69d57bf337028bf245a211b3cddb843835bfe258f427a52
  )
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
else()
  message(
    FATAL_ERROR "Catch2 not found. If you don't want to build tests, disable them with BUILD_TESTING=OFF"
  )
endif()

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.[ch]pp")
add_executable(tests ${sources})
target_link_libraries(tests PRIVATE Catch2::Catch2 smart_json::smart_json)

include(Catch)
catch_discover_tests(tests)
