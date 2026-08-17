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
SET( alpha_dir ${CMAKE_CURRENT_SOURCE_DIR} )
#
file( GLOB alphalib_files    ${alpha_dir}/enfl/*.F
			     ${alpha_dir}/mini/*.F
			     ${alpha_dir}/nano/*.F
			     ${alpha_dir}/pack/*.F
			     ${alpha_dir}/qfn/*.F
			     ${alpha_dir}/qlep/*.F
			     ${alpha_dir}/qpi0/*.F
			     ${alpha_dir}/qvs/*.F
			     ${alpha_dir}/util/*.F )
add_library( alphalib ${ALEPH_LIBRARY_TYPE} ${alphalib_files} )
add_library( Aleph::alphalib ALIAS alphalib )
#
target_include_directories( alphalib PUBLIC ${alpha_dir}/inc )
target_link_libraries(      alphalib
     PUBLIC                 Aleph::julialib
                            Aleph::alephlib
			    Aleph::alephio
			    Aleph::bos77
			    ${ALEPH_CERN_LIBRARIES}
)
#
add_executable( alpha ${alpha_dir}/pack/qmain.F
                                       ${alpha_dir}/pack/quibos.F
                                       ${alpha_dir}/pack/quihis.F
                                       ${ALEPH_PROGRAM_SOURCES} )
target_link_libraries( alpha
   PRIVATE Aleph::alphalib
           Aleph::alephlib
           Aleph::julialib
           Aleph::alephio
 	   Aleph::bos77
           ${ALEPH_CERN_LIBRARIES}
	  )
#
#
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS    alphalib alpha EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
