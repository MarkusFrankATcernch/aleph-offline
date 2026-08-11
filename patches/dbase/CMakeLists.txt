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
SET( dbase_dir         ${CMAKE_CURRENT_SOURCE_DIR} )
SET( sbank_dir         ${dbase_dir} )
#
file( GLOB sbank_files ${sbank_dir}/subr/*.F
                       ${sbank_dir}/c/*.c
)
#
add_executable( sbank ${sbank_files} )
#
target_include_directories( sbank PUBLIC ${dbase_dir}/../inc )
target_include_directories( sbank PUBLIC ${dbase_dir}/inc )
#
target_link_libraries(      sbank
   PRIVATE                  Aleph::alephio.${ALEPHIO_VERSION}
 	                    Aleph::alephlib.${ALEPHLIB_VERSION}
			    Aleph::alephio.${ALEPHIO_VERSION}
 	                    Aleph::bos77
                            ${ALEPH_CERN_LIBRARIES}
                            -Wl,-Bdynamic -lcurses -lc -lm
)
#
install( TARGETS sbank EXPORT Aleph
  RUNTIME DESTINATION bin
)
