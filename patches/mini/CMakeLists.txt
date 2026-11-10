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
find_package( X11 REQUIRED )
SET( mini_dir  ${CMAKE_CURRENT_SOURCE_DIR} )
SET( alpha_dir ${CMAKE_CURRENT_SOURCE_DIR}/../alpha )
message(STATUS "+++ MINI:       Build library from sources: ${mini_dir}" )
#
file( GLOB mini_files ${mini_dir}/mdst/*.F )
#
add_library( mini ${mini_files} )
target_include_directories( mini PUBLIC ${mini_dir}/inc )
target_link_libraries( mini
     PRIVATE Aleph::alephlib
             Aleph::alephio
             Aleph::alephlib
             Aleph::alphalib
             ${ALEPH_CERN_LIBRARIES}
)
#
install(TARGETS mini EXPORT Aleph
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
