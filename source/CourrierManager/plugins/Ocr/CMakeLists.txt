SET(CM_WITH_OCR OFF CACHE BOOL "Build ocr plugin")

if (CM_WITH_OCR)

message(STATUS "[PLUGIN] Generation of the OCR plugin")

set (TESSERACT_INCLUDE_DIR "default" CACHE FILEPATH "description")
set (TESSERACT_LIB_DIR "default" CACHE FILEPATH "description")

file(
       GLOB_RECURSE
       ocr_sources
       *
       ${GENERATOR_PLUGINLOADER}
)


file(GLOB_RECURSE TESSERACT_HEADERS ${TESSERACT_INCLUDE_DIR}/*.h )
set (TESSERACT_INCLUDE "")
foreach (_headerFile ${TESSERACT_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND TESSERACT_INCLUDE ${_dir})
endforeach()
list(REMOVE_DUPLICATES TESSERACT_INCLUDE)
include_directories( ${TESSERACT_INCLUDE} )
link_directories( ${TESSERACT_LIB_DIR} )

set(OCRPLUGIN_INCLUDE "${CMAKE_SOURCE_DIR}/source/CourrierManager/plugins/Ocr/tesseract/include")
include_directories(${OCRPLUGIN_INCLUDE})

add_library(PluginOCR STATIC ${ocr_sources})
target_link_libraries(PluginOCR PluginCore Qt5::Core Qt5::Widgets tesseract )

endif(CM_WITH_OCR)


