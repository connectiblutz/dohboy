cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

add_executable(dns2doh)
target_sources(dns2doh
  PRIVATE
    src/main.cc
    src/worker.cc
    src/doh.cc
    src/settings.cc
  PRIVATE
    include/worker.h
    include/dnspacketdata.h
    include/doh.h
    include/settings.h
)

target_include_directories(dns2doh PRIVATE
  include/
)

target_link_libraries(dns2doh
  PRIVATE
    bcl
    cJSON
    dnslib
)


add_dependency(TARGET dns2doh LIB curl VCPKG_PKG CURL VCPKG_LIB CURL::libcurl)