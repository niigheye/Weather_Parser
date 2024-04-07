cmake_minimum_required(VERSION 3.28.0)

project(PKSH_BitBot_2.0  VERSION 0.1.0 LANGUAGES C CXX)
set(project PKSH_BitBot_2.0)
project(${project})

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
find_package(nlohmann_json REQUIRED)
 
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
pkg_check_modules(CURL REQUIRED)
pkg_check_modules(nlohmann_json REQUIRED)
 
include_directories(${GTK4_INCLUDE_DIRS} ${CURL_INCLUDE_DIR} mvc mvc/controller mvc/view mvc/model)
link_directories(${GTK4_LIBRARY_DIRS} ${CURL_LIBRARY})
add_definitions(${GTK4_CFLAGS_OTHER})

# target_include_directories(${project} PRIVATE  mvc)

file(GLOB MVC_HEADERS "mvc/*.h")
file(GLOB MVC_CPPS "mvc/*.cpp")

set(${project}_SOURCES
main.cpp ${MVC_CPPS})

set(${project}_HEADERS
main.h ${MVC_HEADERS} mvc/observer.h)

set(${project}_SOURCE_LIST
        ${${project}_SOURCES}
        ${${project}_HEADERS}
        )

add_executable(${project} ${${project}_SOURCE_LIST})

target_link_libraries(${project} PRIVATE PkgConfig::GTK4 CURL::libcurl)

 