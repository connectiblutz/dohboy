cmake_minimum_required(VERSION 3.13)
project(apfd VERSION 0.1.0 LANGUAGES CXX C)

add_subdirectory(dns2doh)
add_subdirectory(bcl)
add_subdirectory(third_party/cJSON)
