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
add_library( alephio.${ALEPHIO_VERSION} ${ALEPH_LIBRARY_TYPE} ${alephio_files} )
add_library( Aleph::alephio.${ALEPHIO_VERSION} ALIAS alephio.${ALEPHIO_VERSION} )
#
target_compile_definitions( alephio.${ALEPHIO_VERSION} PUBLIC ${ALEPH_DEBUG_DEFS} )
target_include_directories( alephio.${ALEPHIO_VERSION} PUBLIC ${alephio_dir}/../inc )
target_include_directories( alephio.${ALEPHIO_VERSION} PUBLIC ${alephio_dir}/inc )
target_link_libraries(      alephio.${ALEPHIO_VERSION} PUBLIC Aleph::bos77 Aleph::packlib Aleph::mathlib )
target_compile_options(     alephio.${ALEPHIO_VERSION} PUBLIC
                        $<$<COMPILE_LANGUAGE:C>:  -Wno-return-mismatch -Wno-unused -Wno-implicit-int -Wno-return-type>
)
#
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS    alephio.${ALEPHIO_VERSION} EXPORT Aleph
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
