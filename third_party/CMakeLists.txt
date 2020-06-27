cmake_minimum_required(VERSION 3.13)
project(apfd VERSION 0.1.0 LANGUAGES CXX C)

add_subdirectory(cJSON)
add_subdirectory(dnslib/src)

target_include_directories(dnslib PUBLIC
  dnslib/src
)