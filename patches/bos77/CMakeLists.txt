# ==================================================================================================
#
#   ALEPH library build
#
#   \author  M.Frank
#   \version 1.0
#
# ==================================================================================================
#
SET( bos77_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ BOS77:      Build library from sources: ${bos77_dir}" )
#
file( GLOB bos_files  ${bos77_dir}/F77/*.F )
add_library( bos77 ${ALEPH_LIBRARY_TYPE} ${bos_files} )
add_library( Aleph::bos77 ALIAS bos77 )
#
target_compile_definitions( bos77 PUBLIC -DUNIX -DALEPH_LINUX=1 -DALEPH_C=1 )
target_include_directories( bos77 PUBLIC ${bos77_dir}/inc )
target_include_directories( bos77 PUBLIC ${bos77_dir}/../inc )
target_compile_options(     bos77 PRIVATE -fdec )
target_link_libraries(      bos77 PUBLIC Aleph::packlib Aleph::mathlib )
#
#
install(DIRECTORY  inc   DESTINATION include)
install(TARGETS    bos77 EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
