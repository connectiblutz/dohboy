cmake_minimum_required(VERSION 3.13)

add_subdirectory(cJSON)
add_subdirectory(dnslib/src)

target_include_directories(dnslib PUBLIC
  dnslib/src
)
set_target_properties(dnslib PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")