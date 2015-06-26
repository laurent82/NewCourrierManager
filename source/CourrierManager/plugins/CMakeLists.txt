add_subdirectory(common)

SET(CM_WITH_HELLO OFF CACHE BOOL "Build Hello plugin")
SET(CM_WITH_CLEANLINE OFF CACHE BOOL "Build clean line plugin")
SET(CM_WITH_OCR OFF CACHE BOOL "Build ocr plugin")

if (CM_WITH_HELLO)
	add_subdirectory(Hello)
endif(CM_WITH_HELLO)

if (CM_WITH_CLEANLINE)
	add_subdirectory(CleanLine)
endif(CM_WITH_CLEANLINE)

if (CM_WITH_OCR)
	add_subdirectory(Ocr)
endif(CM_WITH_OCR)
