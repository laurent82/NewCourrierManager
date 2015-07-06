
SET(CM_WITH_HELLO OFF CACHE BOOL "Build Hello plugin")
SET(CM_WITH_CLEANLINE OFF CACHE BOOL "Build clean line plugin")
SET(CM_WITH_OCR OFF CACHE BOOL "Build ocr plugin")

if (CM_WITH_HELLO)
    set(HELLOPLUGIN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/plugins/hello/include")
    include_directories(${HELLOPLUGIN_INCLUDE})
    add_subdirectory(Hello)
endif(CM_WITH_HELLO)

if (CM_WITH_CLEANLINE)
    set(CLEANLINEPLUGIN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/plugins/cleanline/include")
    include_directories(${CLEANLINEPLUGIN_INCLUDE})
    add_subdirectory(CleanLine)
endif(CM_WITH_CLEANLINE)

if (CM_WITH_OCR)
    set(OCR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/plugins/ocr/include")
    include_directories(${OCR_INCLUDE})
    add_subdirectory(Ocr)
endif(CM_WITH_OCR)

add_subdirectory(common)
