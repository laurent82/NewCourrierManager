file(
        GLOB_RECURSE
        hello_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*
)

include_directories( include )

add_library(Hello ${hello_sources})

target_link_libraries(Hello Qt5::Core)

target_include_directories (Hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
