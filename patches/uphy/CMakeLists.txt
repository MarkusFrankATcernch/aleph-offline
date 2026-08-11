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
SET( uphy_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ UPHY:       Build library from sources: ${uphy_dir}" )
#
file( GLOB uphy_files    ${uphy_dir}/bcalinfo/*.F
			     ${uphy_dir}/bhasel/*.F
			     ${uphy_dir}/elcor/*.F
			     ${uphy_dir}/jnucle/*.F
			     ${uphy_dir}/lcalid/*.F
			     ${uphy_dir}/matrix/*.F
			     ${uphy_dir}/qbmtag/*.F
# 			     ${uphy_dir}/qfitw2/*.F
			     ${uphy_dir}/qfitww/*.F
			     ${uphy_dir}/qfndip/*.F
			     ${uphy_dir}/qjcone/*.F
			     ${uphy_dir}/qnnbtag/*.F
			     ${uphy_dir}/qnnwwtag/*.F
			     ${uphy_dir}/qpcorr/*.F
#			     ${uphy_dir}/qufsim/*.F
			     ${uphy_dir}/setshad/*.F
			     ${uphy_dir}/smearfit/*.F
			     ${uphy_dir}/wmuid/*.F
)
add_library( uphy.${UPHY_VERSION} ${ALEPH_LIBRARY_TYPE} ${uphy_files} )
add_library( Aleph::uphy.${UPHY_VERSION} ALIAS uphy.${UPHY_VERSION} )
target_compile_options(uphy.${UPHY_VERSION} PRIVATE -Wno-line-truncation -Wno-tabs -fdec )
target_include_directories(uphy.${UPHY_VERSION} PRIVATE ${uphy_dir}/qufsim )
#
target_link_libraries(      uphy.${UPHY_VERSION}
     PUBLIC                 Aleph::julialib.${JULIA_VERSION}
                            Aleph::alphalib.${ALPHA_VERSION}
                            Aleph::alephlib.${ALEPHLIB_VERSION}
			    Aleph::alephio.${ALEPHIO_VERSION}
 			    Aleph::bos77
                            ${CERNLIB_PACKLIB}
                            ${CERNLIB_KERNLIB}
                            ${CERNLIB_MATHLIB}
)
#
install(TARGETS    alphalib.${ALPHA_VERSION} alpha.${ALPHA_VERSION} EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
