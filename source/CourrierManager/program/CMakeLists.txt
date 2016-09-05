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

add_custom_command(TARGET CourrierManager POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/libs/test.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:MyTest>)                 # <--this is out-file path
# End of CourrierManager
# ----------------------------------------------------------------------------
