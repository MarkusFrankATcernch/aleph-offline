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
add_library( alphalib.${ALPHA_VERSION} ${ALEPH_LIBRARY_TYPE} ${alphalib_files} )
add_library( Aleph::alphalib.${ALPHA_VERSION} ALIAS alphalib.${ALPHA_VERSION} )
#
target_include_directories( alphalib.${ALPHA_VERSION} PUBLIC ${alpha_dir}/inc )
target_link_libraries(      alphalib.${ALPHA_VERSION}
     PUBLIC                 Aleph::julialib.${JULIA_VERSION}
                            Aleph::alephlib.${ALEPHLIB_VERSION}
			    Aleph::alephio.${ALEPHIO_VERSION}
			    Aleph::bos77
			    Aleph::mathlib
			    Aleph::packlib
)
#
add_executable( alpha.${ALPHA_VERSION} ${alpha_dir}/pack/qmain.F )
target_link_libraries( alpha.${ALPHA_VERSION}
   PRIVATE Aleph::alphalib.${ALPHA_VERSION}
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
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS    alphalib.${ALPHA_VERSION} alpha.${ALPHA_VERSION} EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
