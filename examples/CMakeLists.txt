file(GLOB examples "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(example ${examples})
	get_filename_component(name "${example}" NAME_WE)

	add_executable(${name} "${example}")
	target_link_libraries(${name} PRIVATE smart_json::smart_json)
endforeach()
