#Definition of CourrierServer

file(
	GLOB_RECURSE
	server_sources
	*
)

add_executable(
	CourrierServer
	${server_sources}
)

target_link_libraries(CourrierServer Qt5::Core Qt5::Network)

# End of CourrierServer
# ----------------------------------------------------------------------------


