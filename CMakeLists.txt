cmake_minimum_required (VERSION 2.8)

project (TCFileSync)

#-----------------------------------------------------------------------------
# TCFileSync version number, useful for packaging and doxygen doc:
set (TCSystem_VERSION_MAJOR 2)
set (TCSystem_VERSION_MINOR 0)
set (TCSystem_VERSION_PATCH 1)

SET(TCSystem_VERSION
  "${TCFileSync_VERSION_MAJOR}.${TCFileSync_VERSION_MINOR}.${TCFileSync_VERSION_PATCH}")
SET(PACKAGE_VERSION
  "${TCFileSync_VERSION_MAJOR}.${TCFileSync_VERSION_MINOR}.${TCFileSync_VERSION_PATCH}")
  
# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of openjpeg
SET(TCFileSync_LIBRARY_PROPERTIES
  VERSION   "${TCFileSync_VERSION_MAJOR}.${TCFileSync_VERSION_MINOR}.${TCFileSync_VERSION_PATCH}"
  SOVERSION "${TCFileSync_VERSION_MAJOR}.${TCFileSync_VERSION_MINOR}"
)

SET(CPACK_PACKAGE_VERSION_MAJOR "${TCFileSync_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${TCFileSync_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${TCFileSync_VERSION_PATCH}")
include (CPack)

set (CMAKE_MODULE_PATH ${TCFileSync_SOURCE_DIR}/TCSystem/cmake)
include(TCSystemCompileFlags)

# global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory (TCSystem/Modules/Basic)
add_subdirectory (TCSystem/Modules/Math)
add_subdirectory (TCSystem/Modules/Thread)
add_subdirectory (TCSystem/Modules/GUI)

find_package(FOX)
find_package(Boost)

add_definitions(${FOX_CFLAGS})

include_directories (
   TCSystem/Modules/Basic/inc
   TCSystem/Modules/Math/inc
   TCSystem/Modules/Thread/inc
   TCSystem/Modules/GUI/inc
   ${FOX_INCLUDE_DIRS}
   ${Boost_INCLUDE_DIRS}
)

set (FILES
	src/TCFileSync.cpp
	src/TCFileSyncActionGenerator.cpp
	src/TCFileSyncActionGenerator.h
	src/TCFileSyncActions.cpp
	src/TCFileSyncActions.h
	src/TCFileSyncActionWindow.cpp
	src/TCFileSyncActionWindow.h
	src/TCFileSyncDebugNew.cpp
	src/TCFileSyncFileFinder.cpp
	src/TCFileSyncFileFinder.h
	src/TCFileSyncFileInfo.cpp
	src/TCFileSyncFileInfo.h
	src/TCFileSyncMLS.cpp
	src/TCFileSyncOptionsWindow.cpp
	src/TCFileSyncOptionsWindow.h
	src/TCFileSyncSettings.h
	src/TCFileSyncSyncronizer.cpp
	src/TCFileSyncSyncronizer.h
	src/TCFileSyncVersion.h
	src/TCFileSyncWindow.cpp
	src/TCFileSyncWindow.h
)

add_executable(TCFileSync ${FILES})

target_link_libraries(TCFileSync
    TCBasic
    TCThread
    TCMath
    TCGui
    ${FOX_LIBRARIES}
)

install(TARGETS TCFileSync
   RUNTIME DESTINATION ${TC_SYSTEM_BIN_INSTALL_DIR}
   COMPONENT Apps
)
