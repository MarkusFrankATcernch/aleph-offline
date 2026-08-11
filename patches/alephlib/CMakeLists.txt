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
find_package( CERNLIB REQUIRED )
#
SET( aleph_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ ALEPHLIB:   Build library from sources: ${aleph_dir}" )
#
#
file( GLOB alephlib_files    ${aleph_dir}/alef/*.F
			     ${aleph_dir}/c/*.c
                             ${aleph_dir}/c/*.cpp
                             ${aleph_dir}/comp/*.F
			     ${aleph_dir}/dedx/*.F
			     ${aleph_dir}/ecal/*.F
			     ${aleph_dir}/edes/*.F
			     ${aleph_dir}/edir/*.F
                             ${aleph_dir}/fyxx/*.F
			     ${aleph_dir}/fmtr/*.F
			     ${aleph_dir}/gam/*.F
			     ${aleph_dir}/hdes/*.F
			     ${aleph_dir}/inc/*.F
			     ${aleph_dir}/kalm/*.F
			     ${aleph_dir}/ldes/*.F
			     ${aleph_dir}/muca/*.F
			     ${aleph_dir}/news/*.F
			     ${aleph_dir}/phy/*.F
			     ${aleph_dir}/ptoj/*.F
			     ${aleph_dir}/sdes/*.F
			     ${aleph_dir}/tdes/*.F
			     ${aleph_dir}/trig/*.F
			     ${aleph_dir}/vdet/*.F
			     ${aleph_dir}/ysv/*.F
			     ${aleph_dir}/yv0/*.F
			     ${aleph_dir}/ides/*.F
			     ${aleph_dir}/itc/*.F
			     ${aleph_dir}/kine/*.F
			     ${aleph_dir}/mdes/*.F
			     ${aleph_dir}/muid/*.F
			     ${aleph_dir}/phot/*.F
			     ${aleph_dir}/prin/*.F
			     ${aleph_dir}/satr/*.F
			     ${aleph_dir}/spy/*.F
			     ${aleph_dir}/tpc/*.F
			     ${aleph_dir}/vdes/*.F
			     ${aleph_dir}/xlum/*.F
			     ${aleph_dir}/ytop/*.F
)
#
add_library( alephlib.${ALEPHLIB_VERSION} ${ALEPH_LIBRARY_TYPE} ${alephlib_files} )
add_library( Aleph::alephlib.${ALEPHLIB_VERSION} ALIAS alephlib.${ALEPHLIB_VERSION} )
#
target_include_directories( alephlib.${ALEPHLIB_VERSION} PUBLIC ${aleph_dir}/../inc )
target_include_directories( alephlib.${ALEPHLIB_VERSION} PUBLIC ${aleph_dir}/inc )
target_compile_options(     alephlib.${ALEPHLIB_VERSION} PUBLIC  -fPIC )
target_link_libraries(      alephlib.${ALEPHLIB_VERSION}
   PUBLIC        Aleph::alephio.${ALEPHIO_VERSION}
                 Aleph::bos77
		 ${ALEPH_CERN_LIBRARIES} ${ALEPH_CERN_SIMLIBRARIES}
)
#
#
install(DIRECTORY inc  DESTINATION include)
install(TARGETS   alephlib.${ALEPHLIB_VERSION} EXPORT Aleph
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
