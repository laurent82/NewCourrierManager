message (STATUS "[EXECUTABLE] Generation of Manager")

set ( CM_WITH_CONSOLE OFF CACHE BOOL "Add console")

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
set ( stylesheet ${CMAKE_CURRENT_SOURCE_DIR}/res/qss/stylesheet.qss )

if (CM_WITH_OCR)
    #FIXME
    set(OCRPLUGIN_INCLUDE "${CMAKE_SOURCE_DIR}/source/CourrierManager/plugins/Ocr/tesseract/include")
    include_directories(${OCRPLUGIN_INCLUDE})
    link_directories( ${TESSERACT_LIB_DIR} )
endif(CM_WITH_OCR)

add_executable(
	CourrierManager
	${manager_sources}
    ${qt5_manager_sources}
	${UIS_HDRS}
)

if ( NOT CM_WITH_CONSOLE )
    message("Suppression de la console")
	set_property(TARGET CourrierManager PROPERTY WIN32_EXECUTABLE true)
endif ( NOT CM_WITH_CONSOLE )

target_link_libraries(CourrierManager Qt5::Core Qt5::Widgets Qt5::Network Qt5::PrintSupport PluginCore)

if (CM_WITH_OCR)
     target_link_libraries(CourrierManager PluginOCR)
endif(CM_WITH_OCR)


#Installer (incomplet...)
install ( TARGETS CourrierManager DESTINATION bin )
#file ( COPY ${CMAKE_CURRENT_SOURCE_DIR}/res/qss/stylesheet.qss DESTINATION ${EXECUTABLE_OUTPUT_PATH} )
