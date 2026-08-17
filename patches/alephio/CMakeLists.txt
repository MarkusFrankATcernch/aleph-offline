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
SET( alephio_dir ${CMAKE_CURRENT_SOURCE_DIR} )
#
message(STATUS "+++ ALEPHIO:    Build library from sources: ${alephio_dir}" )
file( GLOB alephio_files ${alephio_dir}/alph/*.F
                         ${alephio_dir}/ard/*.F
                         ${alephio_dir}/c/*.c
                         ${alephio_dir}/dbas/*.F
                         ${alephio_dir}/epio/*.F
                         ${alephio_dir}/io/*.F
                         ${alephio_dir}/pres/*.F
)
#
add_library( alephio ${ALEPH_LIBRARY_TYPE} ${alephio_files} )
add_library( Aleph::alephio ALIAS alephio )
#
target_compile_definitions( alephio PUBLIC ${ALEPH_DEBUG_DEFS} )
target_include_directories( alephio PUBLIC ${alephio_dir}/../inc )
target_include_directories( alephio PUBLIC ${alephio_dir}/inc )
target_compile_options(     alephio PUBLIC
  $<$<COMPILE_LANGUAGE:C>:  -Wno-return-mismatch -Wno-unused -Wno-implicit-int -Wno-return-type>
)
target_link_libraries(      alephio PUBLIC Aleph::bos77 ${ALEPH_CERN_LIBRARIES} )
#
install(DIRECTORY  inc     DESTINATION include)
install(TARGETS    alephio EXPORT Aleph
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
