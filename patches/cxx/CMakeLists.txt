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
SET( user_dir ${CMAKE_CURRENT_SOURCE_DIR} )
#
#
find_package(EDM4HEP REQUIRED)
if (NOT EDM4HEP_FOUND)
    message(STATUS "EDM4HEP not found, Building will fail")
else()
  message(STATUS "EDM4HEP found. EDM4HEP_INCLUDE_DIR: ${EDM4HEP_INCLUDE_DIR}")
endif()

find_package(podio REQUIRED)
if(NOT podio_FOUND)
  message(ERROR "podio not found, Building will fail")
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
file( GLOB userlib_files     ${user_dir}/src/*.F
			     ${user_dir}/src/*.cpp	
			     ${user_dir}/tests/*.F
			     ${user_dir}/tests/*.cpp )
add_library( userlib ${ALEPH_LIBRARY_TYPE} ${userlib_files} )
add_library( Aleph::userlib ALIAS userlib )
#
target_include_directories( userlib
   PUBLIC  ${user_dir}/include
           ${user_dir}
	   ${user_dir}/../aleph_headers
)
target_link_libraries(      userlib
   PUBLIC  Aleph::alphalib.${ALPHA_VERSION}
	   Aleph::julialib.${JULIA_VERSION}
           Aleph::alephlib.${ALEPHLIB_VERSION}
	   Aleph::alephio.${ALEPHIO_VERSION}
	   Aleph::bos77
	   Aleph::mathlib
	   Aleph::packlib
           EDM4HEP::edm4hep
           EDM4HEP::edm4hepDict
           podio::podio
           podio::podioDict
           podio::podioIO
           ROOT::Core
           ROOT::Rint
           ROOT::Tree
           ROOT::Geom
)
#
add_executable( alpha_user ${alpha_dir}/pack/qmain.F ${user_dir}/src/quuser.cpp )
target_link_libraries( alpha_user
   PRIVATE Aleph::userlib
           Aleph::alphalib.${ALPHA_VERSION}
           Aleph::alephlib.${ALEPHLIB_VERSION}
           Aleph::julialib.${JULIA_VERSION}
           Aleph::alephio.${ALEPHIO_VERSION}
           Aleph::alephlib.${ALEPHLIB_VERSION}
	   Aleph::packlib
 	   Aleph::bos77
 	   Aleph::hbook
 	   Aleph::jetset
	   Aleph::mathlib
	   Aleph::packlib
	   -lc -lm
	  )
#
#
install(DIRECTORY  include/alpha  DESTINATION include)
install(DIRECTORY  include/bos    DESTINATION include)
install(TARGETS    userlib alpha_user EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
