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
SET( dali_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ DALI:       Build library from sources: ${dali_dir}" )
#
file( GLOB dali_files ${dali_dir}/*.F ${dali_dir}/*.c )
#
add_executable( dali ${dali_files} )
target_include_directories( dali PUBLIC ${dali_dir} )
target_link_libraries( dali
     PRIVATE Aleph::alephlib
             Aleph::alephio
             Aleph::alphalib
             Aleph::looklib
             Aleph::bos77
             Aleph::alephlib
             ${ALEPH_CERN_LIBRARIES} -Wl,-Bdynamic -lX11 -lXm -lXt
)
#
install(TARGETS dali EXPORT Aleph
  RUNTIME DESTINATION bin
)
