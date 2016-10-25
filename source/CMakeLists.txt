set(Build_Courrier_Manager ON CACHE BOOL "Build Courrier Manager")
if(Build_Courrier_Manager)
	add_subdirectory(CourrierManager)
endif()

set(Build_Courrier_Server OFF CACHE BOOL "Build Courrier Server")
if(Build_Courrier_Server)
	add_subdirectory(CourrierServer)
endif()

