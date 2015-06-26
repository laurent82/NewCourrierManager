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

set(CORE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/core/include")
set(COMMONPLUGIN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/plugins/common/include")

include_directories(${CORE_INCLUDE})
include_directories(${COMMONPLUGIN_INCLUDE})

add_subdirectory(core)
add_subdirectory(plugins)

message(STATUS "[/PROJECT] End of Courrier Manager")
