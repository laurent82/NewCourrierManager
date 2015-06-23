# ------------------------------------------------------------------------------- # 
#
#               ___                  _           ___      _ _       
#              / __|___ _  _ _ _ _ _(_)___ _ _  / __|_  _(_) |_ ___ 
#             | (__/ _ \ || | '_| '_| / -_) '_| \__ \ || | |  _/ -_)
#              \___\___/\_,_|_| |_| |_\___|_|   |___/\_,_|_|\__\___|
#                                                       
#                               Author: Laurent Engels                  	  
#						  
# ------------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 2.8.11)

project(CourrierSuite)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug RelWithDebInfo)

#Output path
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Definition of Qt
FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Network PrintSupport)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_subdirectory(source)
add_subdirectory(plugins)

