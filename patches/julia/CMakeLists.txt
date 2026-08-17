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
add_library( julialib ${ALEPH_LIBRARY_TYPE} ${julia_files} )
add_library( Aleph::julialib ALIAS julialib )
#
target_include_directories( julialib PUBLIC ${julia_dir}/inc )
#
# Necessary for include files!
target_link_libraries(      julialib
   PUBLIC  Aleph::alephio
 	   Aleph::alephlib
           Aleph::bos77
)
#
#
add_executable(             julia ${julia_dir}/a_/aamain.F )
target_include_directories( julia PUBLIC ${julia_dir}/inc )
target_link_libraries( julia
   PUBLIC  Aleph::alephlib
           Aleph::julialib 
           Aleph::alephio
           Aleph::alephlib
           Aleph::bos77
           ${ALEPH_CERN_SIMLIBRARIES}
)
#
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS    julia julialib EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
