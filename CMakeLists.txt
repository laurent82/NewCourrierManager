# ------------------------------------------------------------------------------- #
#										  #
#      ___                  _           __  __                             	  #
#     / __|___ _  _ _ _ _ _(_)___ _ _  |  \/  |__ _ _ _  __ _ __ _ ___ _ _ 	  #
#    | (__/ _ \ || | '_| '_| / -_) '_| | |\/| / _` | ' \/ _` / _` / -_) '_| 	  #
#     \___\___/\_,_|_| |_| |_\___|_|   |_|  |_\__,_|_||_\__,_\__, \___|_|  	  #
#                                                        |___/         		  #
#										  #
#             			Author: Laurent Engels                  	  #
#  										  #
# ------------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 2.8.11)

project(CourrierSuite)

#Output path
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Definition of Qt
FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Network PrintSupport)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# ----------------------------------------------------------------------------
# Definition of CourrierManager

QT5_WRAP_UI(UIS_HDRS
    source/CourrierManager/ui_forms/courriermanager.ui
)

file(
	GLOB_RECURSE
	manager_sources
	source/CourrierManager/*
)

include_directories( ${CMAKE_SOURCE_DIR}/source/CourrierManager/include)

add_executable(
	CourrierManager
	${manager_sources}
	${UIS_HDRS}
)
target_link_libraries(CourrierManager Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport)

# End of CourrierManager
# ----------------------------------------------------------------------------
#Definition of CourrierServer

file(
	GLOB_RECURSE
	server_sources
	source/CourrierServer/*
)

add_executable(
	CourrierServer
	${server_sources}
)

target_link_libraries(CourrierServer Qt5::Core Qt5::Network)

# End of CourrierServer
# ----------------------------------------------------------------------------


