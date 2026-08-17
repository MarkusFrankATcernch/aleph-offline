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
find_package( X11 REQUIRED )
SET( galeph_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ GALEPH:     Build library from sources: ${galeph_dir}" )
#
SET( tpc_dir    ${CMAKE_CURRENT_SOURCE_DIR}/../tpcsim )
file( GLOB galeph_files ${galeph_dir}/a_/*.F
			     ${galeph_dir}/calo/*.F
			     ${galeph_dir}/draw/*.F
			     ${galeph_dir}/ecal/*.F
			     ${galeph_dir}/gal/*.F
			     ${galeph_dir}/geom/*.F
			     ${galeph_dir}/guse/*.F
			     ${galeph_dir}/hcal/*.F
			     ${galeph_dir}/inc/*.F
			     ${galeph_dir}/itc/*.F
			     ${galeph_dir}/lcal/*.F
			     ${galeph_dir}/muon/*.F
			     ${galeph_dir}/satr/*.F
			     ${galeph_dir}/scal/*.F
			     ${galeph_dir}/skel/*.F
			     ${galeph_dir}/tpc/*.F
			     ${galeph_dir}/vdet/*.F
			     ${tpc_dir}/F77/*.F
)
#
add_executable( galeph ${galeph_files} )
target_include_directories( galeph PUBLIC ${galeph_dir}/inc ${tpc_dir}/inc )
target_link_libraries( galeph
     PRIVATE Aleph::alephlib
             Aleph::alephio
             Aleph::alephlib
             ${ALEPH_CERN_SIMLIBRARIES} ${ALEPH_CERN_X11LIBRARIES}
)
#
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS  galeph EXPORT Aleph
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
