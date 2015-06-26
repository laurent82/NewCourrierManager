message(STATUS "[PLUGIN] Generation of the common plugin")

file(
        GLOB_RECURSE
        common_sources
        *
)


#ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_PLUGIN)
#ADD_DEFINITIONS(-DQT_SHARED)

ADD_LIBRARY(PluginCore STATIC ${common_sources})


target_link_libraries(PluginCore Qt5::Core)
