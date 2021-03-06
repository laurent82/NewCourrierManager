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

#set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Release Debug RelWithDebInfo)

#Output path
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Definition of Qt
FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Network PrintSupport)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(source)

#Include directories
file(GLOB_RECURSE COURRIER_HEADERS "source/*.h")
set (COURRIER_INCLUDE_DIRS "")

foreach (_headerFile ${COURRIER_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND COURRIER_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES COURRIER_INCLUDE_DIRS)

include_directories(${COURRIER_INCLUDE_DIRS})

# CPack values for the installer
set(CPACK_PACKAGE_NAME "CourrierSuite")
set(CPACK_PACKAGE_VENDOR "Laurent Engels")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/INSTALLER")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CourrierSuite")

# Special values for NSIS
set (CPACK_NSIS_DISPLAY_NAME "Courrier Suite")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/GlobalResources/main_icon.ico")
INCLUDE(CPack)

