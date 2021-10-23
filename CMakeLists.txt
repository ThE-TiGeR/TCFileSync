cmake_minimum_required (VERSION 2.8)

project (TCFileSync)

#-----------------------------------------------------------------------------
# TCFileSync version number, useful for packaging and doxygen doc:
set (TCFileSync_VERSION_MAJOR 4)
set (TCFileSync_VERSION_MINOR 0)
set (TCFileSync_VERSION_PATCH 0)

set(TCFileSync_PROGRAM_NAME "TCFileSync")
set(TCFileSync_VERSION "${TCFileSync_VERSION_MAJOR}.${TCFileSync_VERSION_MINOR}.${TCFileSync_VERSION_PATCH}")
set(TCFileSync_AUTHOR "Thomas Goessler")
set(TCFileSync_COMPANY "TCSystem")
set(TCFileSync_COPYRIGHT "Copyright (c) 2003 - 2021")
  
configure_file (
  ${PROJECT_SOURCE_DIR}/src/TCFileSyncVersion.h.in
  ${PROJECT_BINARY_DIR}/TCFileSyncVersion.h
  )


set(PACKAGE_VERSION    "${TCFileSync_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${TCFileSync_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${TCFileSync_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${TCFileSync_VERSION_PATCH}")
include (CPack)

set (CMAKE_MODULE_PATH ${TCFileSync_SOURCE_DIR}/TCSystem/cmake)
include(TCSystemCompileFlags)

# global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory (TCSystem/Modules/Basic)
add_subdirectory (TCSystem/Modules/Math)

include_directories (
   TCSystem/Modules/Basic/inc
   TCSystem/Modules/Math/inc
   ${Boost_INCLUDE_DIRS}
   ${PROJECT_BINARY_DIR}
)

set (FILES
	src/TCFileSync.cpp
	src/TCFileSyncActionGenerator.cpp
	src/TCFileSyncActionGenerator.h
	src/TCFileSyncActions.cpp
	src/TCFileSyncActions.h
	src/TCFileSyncDebugNew.cpp
	src/TCFileSyncFileFinder.cpp
	src/TCFileSyncFileFinder.h
	src/TCFileSyncFileInfo.cpp
	src/TCFileSyncFileInfo.h
	src/TCFileSyncMLS.cpp
	src/TCFileSyncSettings.h
	src/TCFileSyncSyncronizer.cpp
	src/TCFileSyncSyncronizer.h
	src/TCFileSyncVersion.h.in
    ${PROJECT_BINARY_DIR}/TCFileSyncVersion.h
	.github/workflows/build.yml
)

add_executable(TCFileSync ${FILES})

target_link_libraries(TCFileSync
    TCBasic
    TCMath
)

install(TARGETS TCFileSync
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   COMPONENT Apps
)
