cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

#find_package(CURL CONFIG REQUIRED)

add_executable(dns2doh)
target_sources(dns2doh
  PRIVATE
    src/main.cc
    src/worker.cc
    src/doh.cc
  PRIVATE
    include/worker.h
    include/dnspacketdata.h
    include/doh.h
)

target_include_directories(dns2doh PRIVATE
  include/
)

target_link_libraries(dns2doh
  bcl
  cJSON
  dnslib
  curl
  # CURL::libcurl
)
