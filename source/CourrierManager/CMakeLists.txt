# ------------------------------------------------------------------------------- #
#
#         ___                  _           __  __
#        / __|___ _  _ _ _ _ _(_)___ _ _  |  \/  |__ _ _ _  __ _ __ _ ___ _ _
#       | (__/ _ \ || | '_| '_| / -_) '_| | |\/| / _` | ' \/ _` / _` / -_) '_|
#        \___\___/\_,_|_| |_| |_\___|_|   |_|  |_\__,_|_||_\__,_\__, \___|_|
#                                                               |___/
#
#                               Author: Laurent Engels
#
# ------------------------------------------------------------------------------- #

message(STATUS "[PROJECT] Generation of Courrier Manager")

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/target")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CORE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/core/include")
set(COMMONPLUGIN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/plugins/common/include")

message(STATUS "[DYNAMIC] Generate dynamic files")
SET(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")

configure_file( dynamic/plugins_config.h.in
                ${GENERATED_DIR}/plugins_config.h )


include_directories(${CORE_INCLUDE})
include_directories(${COMMONPLUGIN_INCLUDE})
include_directories(${GENERATED_DIR})

add_subdirectory(core)
add_subdirectory(plugins)

message(STATUS "[/PROJECT] End of Courrier Manager")

