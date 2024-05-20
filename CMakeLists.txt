cmake_minimum_required(VERSION 3.28.0)

project(PKSH_BitBot_2.0 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Define paths
set(PKG_CONFIG_EXECUTABLE "C:/msys64/mingw64/bin/pkg-config")
set(CURL_LIBRARY_DIR "C:/msys64/mingw64/lib")
set(CURL_INCLUDE_DIR "C:/msys64/mingw64/include")
set(GTK3_INCLUDE_DIRS "C:/msys64/mingw64/include")
set(GTK3_LIBRARY_DIRS "C:/msys64/mingw64/lib")

# Set PKG_CONFIG_PATH environment variable
set(ENV{PKG_CONFIG_PATH} "C:/msys64/mingw64/lib/pkgconfig")

# Include pkg-config module
include(FindPkgConfig)
find_package(PkgConfig REQUIRED)

# Find GTK and cURL
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
find_package(CURL REQUIRED)

# Include directories and link directories
include_directories(${GTK4_INCLUDE_DIRS} "${CURL_INCLUDE_DIR}")
link_directories(${GTK4_LIBRARY_DIRS} "${CURL_LIBRARY_DIR}")

# Additional definitions
add_definitions(${GTK4_CFLAGS_OTHER})

# Include directories for project files and nlohmann_json
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/mvc/model
    ${CMAKE_CURRENT_SOURCE_DIR}/mvc/view
    ${CMAKE_CURRENT_SOURCE_DIR}/mvc/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/mvc
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/ # Include directory for nlohmann_json
)

# Source and header files
set(CPP_FILES 
    mvc/model/weatherForecastModel.cpp 
    mvc/view/appView.cpp
)
set(H_FILES 
    mvc/observer.h 
    mvc/model/model.h 
    mvc/view/view.h 
    mvc/controller/controller.h 
    mvc/model/weatherForecastModel.h 
    mvc/view/appView.h 
    mvc/controller/appController.h
)

# Combine source and header files
set(SOURCE_FILES ${CPP_FILES} ${H_FILES})

# Create executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PkgConfig::GTK4 
    curl
)
