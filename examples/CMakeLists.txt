file(GLOB examples "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(example ${examples})
  get_filename_component(name "${example}" NAME_WE)
  set(libraries smart_json::smart_json)

  if(name STREQUAL "nlohmann")
    require_optional_dependencies()
    if(TARGET nlohmann_json::nlohmann_json)
      list(APPEND libraries nlohmann_json::nlohmann_json)
    else()
      message(STATUS "Skipping nlohmann/json example")
      continue()
    endif()
  endif()

  add_executable(${name} "${example}")
  target_link_libraries(${name} PRIVATE ${libraries})
endforeach()
