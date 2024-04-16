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
 
include_directories(${GTK4_INCLUDE_DIRS} ${CURL_INCLUDE_DIR} )
link_directories(${GTK4_LIBRARY_DIRS} ${CURL_LIBRARY} )
add_definitions(${GTK4_CFLAGS_OTHER})

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/mvc/model
        ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view
        ${CMAKE_CURRENT_SOURCE_DIR}/mvc/controller
        ${CMAKE_CURRENT_SOURCE_DIR}/mvc
)

set(CPP_FILES mvc/model/weatherForecastModel.cpp mvc/view/appView.cpp)
set(H_FILES mvc/observer.h mvc/model/model.h mvc/view/view.h mvc/controller/controller.h 
                           mvc/model/weatherForecastModel.h  mvc/view/appView.h mvc/controller/appController.h
             )
message(STATUS "alooo")
message(STATUS ${CPP_FILES})
message(STATUS ${H_FILES})
message(STATUS "alooo")
set (SOURCE_FILES ${CPP_FILES} ${H_FILES})

add_executable(${project} main.cpp ${SOURCE_FILES})

target_link_libraries(${project} PkgConfig::GTK4)
target_link_libraries(${project} CURL::libcurl)

 