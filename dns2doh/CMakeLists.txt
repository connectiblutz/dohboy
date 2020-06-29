cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

add_executable(dohboy)
target_sources(dohboy
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

target_include_directories(dohboy PRIVATE
  include/
)

target_link_libraries(dohboy
  PRIVATE
    bcl
    cJSON
    dnslib
)


add_dependency(TARGET dohboy LIB curl VCPKG_PKG CURL VCPKG_LIB CURL::libcurl)