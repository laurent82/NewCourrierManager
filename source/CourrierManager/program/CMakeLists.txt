message (STATUS "[EXECUTABLE] Generation of Manager")

# Definition of CourrierManager
QT5_WRAP_UI(UIS_HDRS
    ui_forms/courriermanager.ui
)

file(
	GLOB_RECURSE
	manager_sources
	*
)

add_executable(
	CourrierManager
	${manager_sources}
	${UIS_HDRS}
)

target_link_libraries(CourrierManager Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport PluginCore)
install ( TARGETS CourrierManager DESTINATION bin )

