message (STATUS "[EXECUTABLE] Generation of Manager")

# Definition of CourrierManager
QT5_WRAP_UI(UIS_HDRS
    ui_forms/courriermanager.ui
)

file(
	GLOB_RECURSE
	manager_sources
	*.cpp
    *.h
    *.rc
)

file(
	GLOB_RECURSE
	manager_resources
	*.qrc
)

QT5_ADD_RESOURCES (qt5_manager_sources ${manager_resources})

add_executable(
	CourrierManager
	${manager_sources}
    ${qt5_manager_sources}
	${UIS_HDRS}
)
set ( stylesheet ${CMAKE_CURRENT_SOURCE_DIR}/res/qss/stylesheet.qss )
target_link_libraries(CourrierManager Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport PluginCore)
install ( TARGETS CourrierManager DESTINATION bin )
#install ( TARGETS ${stylesheet} DESTINATION bin )
#file ( COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/qss/stylesheet.qss DESTINATION ${EXECUTABLE_OUTPUT_PATH} )
