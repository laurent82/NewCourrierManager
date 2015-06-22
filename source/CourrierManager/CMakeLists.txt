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

message (STATUS "Generation of Courrier Manager")

# Definition of CourrierManager
QT5_WRAP_UI(UIS_HDRS
    ui_forms/courriermanager.ui
)

file(
	GLOB_RECURSE
	manager_sources
	*
)

include_directories( include )

add_executable(
	CourrierManager
	${manager_sources}
	${UIS_HDRS}
)
target_link_libraries(CourrierManager Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport)

# End of CourrierManager
# ----------------------------------------------------------------------------
