message(STATUS "[PLUGIN] Generation of the core plugin")

file(
        GLOB_RECURSE
        common_sources
        *
        ${GENERATOR_PLUGINLOADER}
)

ADD_LIBRARY(PluginCore STATIC ${common_sources})

target_link_libraries(PluginCore Qt5::Core)

if (CM_WITH_HELLO)
    set(HELLOPLUGIN_INCLUDE "${CMAKE_SOURCE_DIR}/source/CourrierManager/plugins/Hello/include")
    include_directories(${HELLOPLUGIN_INCLUDE})
    target_link_libraries(PluginCore plugin-hello)
endif(CM_WITH_HELLO)
