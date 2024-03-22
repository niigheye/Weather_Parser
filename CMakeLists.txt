cmake_minimum_required(VERSION 3.28.0)

project(aladin_practise_cur1 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(PKG_CONFIG_EXECUTABLE "C:/msys64/mingw64/bin/pkg-config")

set(CURL_LIBRARY "C:/Program Files (x86)/CURL/lib")
set(CURL_INCLUDE_DIR "C:/Program Files (x86)/CURL/include")

set(GTK3_INCLUDE_DIRS "C:/msys64/mingw64/include")
set(GTK3_LIBRARY_DIRS "C:/msys64/mingw64/lib") 
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconf")

 
include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(CURL REQUIRED)
 
include_directories(${GTK4_INCLUDE_DIRS} ${CURL_INCLUDE_DIR})
link_directories(${GTK4_LIBRARY_DIRS} ${CURL_LIBRARY})
add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(aladin_practise_cur1 main.cpp main.h)
target_link_libraries(aladin_practise_cur1 PRIVATE PkgConfig::GTK4 CURL::libcurl)

 