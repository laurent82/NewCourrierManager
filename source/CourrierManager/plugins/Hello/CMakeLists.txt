message(STATUS "[PLUGIN] Generation of Hello plugin")

file(
        GLOB_RECURSE
        hello_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/*
)


ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(Hello SHARED ${hello_sources})

include_directories(include)

target_link_libraries(Hello Qt5::Core PluginCore)
