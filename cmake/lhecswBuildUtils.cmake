macro(lhecsw_build lib_name)
  set(options)
  set(one_vals)
  set(multi_vals SOURCES LIBRARIES HEADERS PROPERTY)
  cmake_parse_arguments(ARG "${options}" "${one_vals}" "${multi_vals}" ${ARGN})
  if(ARG_SOURCES)
    set(sources)
    foreach(_s ${ARG_SOURCES})
      file(GLOB s "${_s}")
      list(APPEND sources ${s})
    endforeach()
    message(STATUS "Building ${lib_name}...")
    add_library(${lib_name} SHARED ${sources})
    if(ARG_LIBRARIES)
      message(STATUS "... external libs: ${ARG_LIBRARIES}")
      target_link_libraries(${lib_name} PUBLIC ${ARG_LIBRARIES})
    endif()
    if(ARG_HEADERS)
      message(STATUS "... external headers: ${ARG_HEADERS}")
      target_include_directories(${lib_name} PRIVATE ${ARG_HEADERS})
    endif()
    if(ARG_PROPERTY)
      message(STATUS "... additional properties: ${ARG_PROPERTY}")
      set_target_properties(${lib_name} PROPERTIES ${ARG_PROPERTY})
    endif()
  else()
    message(STATUS "Building headers-only ${lib_name}...")
    add_library(${lib_name} INTERFACE)
    target_include_directories(${lib_name} INTERFACE include)
  endif()
  install(TARGETS ${lib_name} LIBRARY DESTINATION lib)
  list(APPEND lhecswlibs ${lib_name})
  set(lhecswlibs ${lhecswlibs} PARENT_SCOPE)
endmacro()
