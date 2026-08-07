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
SET( julia_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ JULIA:      Build library from sources: ${julia_dir}" )
#
file( GLOB julia_files ${julia_dir}/a_/*.F
			     ${julia_dir}/b_/*.F
			     ${julia_dir}/c_/*.F
			     ${julia_dir}/e_/*.F
			     ${julia_dir}/f_/*.F
			     ${julia_dir}/g_/*.F
			     ${julia_dir}/h_/*.F
			     ${julia_dir}/i_/*.F
			     ${julia_dir}/l_/*.F
			     ${julia_dir}/m_/*.F
			     ${julia_dir}/o_/*.F
			     ${julia_dir}/p_/*.F
			     ${julia_dir}/q_/*.F
			     ${julia_dir}/r_/*.F
			     ${julia_dir}/s_/*.F
			     ${julia_dir}/t_/*.F
			     ${julia_dir}/u_/*.F
			     ${julia_dir}/v_/*.F
			     ${julia_dir}/w_/*.F
			     ${julia_dir}/x_/*.F
			     ${julia_dir}/y_/*.F
			     ${julia_dir}/vglob/*.F
)
add_library( julialib.${JULIA_VERSION} ${ALEPH_LIBRARY_TYPE} ${julia_files} )
add_library( Aleph::julialib.${JULIA_VERSION} ALIAS julialib.${JULIA_VERSION} )
#
target_include_directories( julialib.${JULIA_VERSION} PUBLIC ${julia_dir}/inc )
#
# Necessary for include files!
target_link_libraries(      julialib.${JULIA_VERSION}
   PUBLIC  Aleph::alephio.${ALEPHIO_VERSION}
 	   Aleph::alephlib.${ALEPHLIB_VERSION}
 	   Aleph::hbook
 	   Aleph::jetset
	   Aleph::mathlib
           Aleph::bos77
 	   Aleph::mathlib
 	   Aleph::packlib c m )
#
#
add_executable(             julia.${JULIA_VERSION} ${julia_dir}/a_/aamain.F )
target_include_directories( julia.${JULIA_VERSION} PUBLIC ${julia_dir}/inc )
target_link_libraries( julia.${JULIA_VERSION}
   PUBLIC  Aleph::alephlib.${ALEPHLIB_VERSION}
           Aleph::julialib.${JULIA_VERSION} 
           Aleph::alephio.${ALEPHIO_VERSION}
           Aleph::alephlib.${ALEPHLIB_VERSION}
	   c m
	  )
#
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS    julia.${JULIA_VERSION} julialib.${JULIA_VERSION} EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
