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
SET( look_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ LOOK:       Build library from sources: ${look_dir}" )
#
SET( tpc_dir    ${CMAKE_CURRENT_SOURCE_DIR}/../tpcsim )
file( GLOB look_files ${look_dir}/look/*.F )
#
add_library( looklib STATIC ${look_files} )
add_library( Aleph::looklib ALIAS looklib )
target_include_directories( looklib PUBLIC ${look_dir}/inc )
target_link_libraries( looklib
     PRIVATE Aleph::alephlib
             Aleph::alephio
             Aleph::alephlib
             Aleph::bos77
             ${ALEPH_CERN_LIBRARIES}
)
#
add_executable( look ${look_dir}/main/lkmain.F )
target_include_directories( look PUBLIC ${look_dir}/inc )
target_link_libraries( look
     PRIVATE Aleph::looklib
             Aleph::alephlib
             Aleph::alephio
             Aleph::alephlib
             Aleph::bos77
             ${ALEPH_CERN_LIBRARIES}
)
#
install(TARGETS look EXPORT Aleph
  RUNTIME DESTINATION bin
)
