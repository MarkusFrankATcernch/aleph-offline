# ==================================================================================================
#
#   ALEPH library build
#
#   \author  M.Frank
#   \date    09/07/2026
#   \version 1.0
#
# ==================================================================================================
#
SET( alpha_dir ${CMAKE_CURRENT_SOURCE_DIR}/../alpha )
SET( user_dir  ${CMAKE_CURRENT_SOURCE_DIR}  )
#
#
find_package(EDM4HEP REQUIRED)
if (NOT EDM4HEP_FOUND)
    message(STATUS "EDM4HEP not found, Building will fail")
else()
  message(STATUS "EDM4HEP found. EDM4HEP_INCLUDE_DIR: ${EDM4HEP_INCLUDE_DIR}")
endif()
#
#
find_package(podio REQUIRED)
if(NOT podio_FOUND)
  message(ERROR "podio not found, Building will fail")
endif()
#
#
find_package(DD4hep)
if(NOT DD4hep_FOUND)
  message(ERROR "DD4hep not found, Building will fail")
endif()
#
#
find_package(ROOT REQUIRED)
if(NOT ROOT_FOUND)
  message(ERROR "ROOT not found, Building will fail")
else()
  message(STATUS "ROOT found. ROOT_INCLUDE_DIRS: ${ROOT_INCLUDE_DIRS}")
endif()
#
#
#
file( GLOB alpha_userlib_files ${user_dir}/src/*.cpp
                               ${user_dir}/dd4hep/aleph_geometry.cpp
                               ${user_dir}/dd4hep/geant3_dump.cpp
                               ${user_dir}/dd4hep/geant3_geometry.cpp )
add_library( alpha_userlib SHARED  ${alpha_userlib_files} )
add_library( Aleph::alpha_userlib ALIAS alpha_userlib )
#
target_include_directories( alpha_userlib
   PUBLIC  ${user_dir}/include
	   ${user_dir}/../aleph_headers
           ${user_dir}
)
target_link_libraries(      alpha_userlib
   PUBLIC
#           Aleph::alphalib
#	   Aleph::julialib
#           Aleph::alephlib
#	   Aleph::alephio
#	   Aleph::bos77
           DD4hep::DDCore
           EDM4HEP::edm4hep
           EDM4HEP::edm4hepDict
           podio::podio
           podio::podioDict
           podio::podioIO
           ROOT::Core
           ROOT::Rint
           ROOT::Tree
           ROOT::Geom
           ${ALEPH_CERN_LIBRARIES}
)
#
include(DD4hepMacros) 
#
dd4hep_add_plugin(AlephGeometry SOURCES ${user_dir}/dd4hep/*.cpp
  USES DD4hep::DDCore ROOT::Core ROOT::Geom ROOT::GenVector ROOT::MathCore
)
target_include_directories( AlephGeometry PUBLIC  ${user_dir}/include )
#
#
file( GLOB alpha_user_files ${user_dir}/src/*.F ${user_dir}/tests/*.F ${user_dir}/tests/*.cpp )
add_executable( alpha_user  ${alpha_user_files}
                            ${user_dir}/main/functions.cpp
                            ${user_dir}/main/quuser.cpp
                            ${ALEPH_PROGRAM_SOURCES} 
)
target_link_libraries( alpha_user
   PRIVATE Aleph::alpha_userlib
           Aleph::alphalib
           Aleph::alephlib
           Aleph::julialib
           Aleph::alephio
           Aleph::alephlib
           ${ALEPH_CERN_LIBRARIES}
)
target_compile_options(alpha_user PUBLIC "-fpie")
target_link_options(alpha_user PUBLIC "-fpie")
#
add_executable( alpha_test  ${user_dir}/tests/f77_tests.F
                            ${user_dir}/src/quuser.F
                            ${user_dir}/main/qutest.cpp
                            ${user_dir}/main/functions.cpp
                            ${user_dir}/src/functions.cpp
                            ${user_dir}/src/bcs.cpp
                            ${user_dir}/src/bosbank.cpp
                            ${user_dir}/src/tpcgeom.cpp
                            ${ALEPH_PROGRAM_SOURCES} 
)
target_link_libraries( alpha_test
   PRIVATE Aleph::alphalib
           Aleph::alephlib
           Aleph::julialib
           Aleph::alephio
           Aleph::alephlib
 	   Aleph::bos77 # Aleph::alpha_userlib
           ${ALEPH_CERN_LIBRARIES}
)
target_compile_options(alpha_test PUBLIC "-fpie")
target_include_directories(alpha_test PUBLIC  ${user_dir} ${user_dir}/include )
target_link_options(alpha_test PUBLIC "-fpie")
#
dd4hep_install_dir(${user_dir}/compact DESTINATION ${CMAKE_INSTALL_PREFIX} )
install(DIRECTORY  ${user_dir}/include/alpha  DESTINATION include)
install(DIRECTORY  ${user_dir}/include/bos    DESTINATION include)
install(TARGETS    alpha_userlib  alpha_test EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
