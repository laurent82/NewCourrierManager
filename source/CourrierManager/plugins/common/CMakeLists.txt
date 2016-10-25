message(STATUS "[PLUGIN] Generation of the core plugin")

file(
        GLOB_RECURSE
        common_sources
        *
        ${GENERATOR_PLUGINLOADER}
)

ADD_LIBRARY(PluginCore STATIC ${common_sources})

target_link_libraries(PluginCore Qt5::Core)
