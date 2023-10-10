
file(GLOB_RECURSE SOURCEFILES "*.cpp")
foreach(_source IN LISTS SOURCEFILES)
    get_filename_component(_basename ${_source} NAME_WE)
    add_executable(${_basename} ${_source})
    target_include_directories(${_basename} PRIVATE ${CppConcurrency_SOURCE_DIR}/includes)
    target_link_libraries(${_basename} PRIVATE Boost::thread CommonUtils)
endforeach(_source IN LISTS SOURCEFILES)