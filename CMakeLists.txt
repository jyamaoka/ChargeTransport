cmake_minimum_required(VERSION 3.10)
project(ChargeTransport)

#----------------------------------------------------------------------------
# G4CMP — must be found before Geant4 so its include dirs come first
#
set(G4CMPINSTALL "${CMAKE_CURRENT_SOURCE_DIR}/../install")
set(CMAKE_PREFIX_PATH "${G4CMPINSTALL}/cmake" ${CMAKE_PREFIX_PATH})
find_package(G4CMP REQUIRED)

#----------------------------------------------------------------------------
# Geant4
#
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# The conda Geant4 package was built with MT support but doesn't export the
# G4MULTITHREADED define through CMake — add it explicitly.
#
add_compile_definitions(G4MULTITHREADED)

#----------------------------------------------------------------------------
# Include dirs and compile flags
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${G4CMP_INCLUDE_DIRS})
include(${G4CMP_USE_FILE})

#----------------------------------------------------------------------------
# RPATH
#
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
     "${CMAKE_INSTALL_PREFIX}/lib" is_SYSTEM_DIR)
if("${is_SYSTEM_DIR}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif()

#----------------------------------------------------------------------------
# Sources
#
set(SOURCES
    src/ChargeTransportActionInitialization.cc
    src/ChargeTransportDetectorConstruction.cc
    src/ChargeTransportDetectorMessenger.cc
    src/ChargeTransportPhysicsList.cc
    src/ChargeTransportPrimaryGeneratorAction.cc
    src/ChargeTransportRunAction.cc
    src/ChargeTransportSensitiveDetector.cc
)

if(USE_GEANT4_STATIC_LIBS)
  add_library(chargeTransportLib STATIC ${SOURCES})
else()
  add_library(chargeTransportLib SHARED ${SOURCES})
endif()
set_target_properties(chargeTransportLib PROPERTIES OUTPUT_NAME ChargeTransport)
target_link_libraries(chargeTransportLib ${G4CMP_LIBRARIES} ${Geant4_LIBRARIES})

add_executable(charge_transport charge_transport.cc)
target_link_libraries(charge_transport chargeTransportLib)

#----------------------------------------------------------------------------
# Copy macros to build directory
#
configure_file(run.mac ${CMAKE_CURRENT_BINARY_DIR}/run.mac COPYONLY)

install(TARGETS chargeTransportLib DESTINATION lib)
install(TARGETS charge_transport  DESTINATION bin)
