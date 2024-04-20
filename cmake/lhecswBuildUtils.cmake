macro(lhecsw_add_directory dir_name)
  set(options)
  set(one_vals)
  set(multi_vals LIBS)
  cmake_parse_arguments(ARG "${options}" "${one_vals}" "${multi_vals}" ${ARGN})
  message(STATUS "Building ${dir_name}...")
  file(GLOB sources ${dir_name}/src/*.cc)

  add_library(${dir_name} SHARED ${sources})
  target_link_libraries(${dir_name} PRIVATE DD4hep::DDCore DD4hep::DDRec)
  if(ARG_LIBS)
    message(STATUS "... external libs: ${ARG_LIBS}")
    target_link_libraries(${dir_name} PRIVATE ${ARG_LIBS})
  endif()
  install(TARGETS ${dir_name} LIBRARY DESTINATION lib)
  list(APPEND lhecswlibs ${dir_name})
  set(lhecswlibs ${lhecswlibs} PARENT_SCOPE)
endmacro()
