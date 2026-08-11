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
#
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
add_library( galephlib.${GALEPH_VERSION} ${ALEPH_LIBRARY_TYPE} ${galeph_files} )
add_library( Aleph::galephlib.${GALEPH_VERSION} ALIAS galephlib.${GALEPH_VERSION} )
#
target_include_directories( galephlib.${GALEPH_VERSION} PUBLIC ${galeph_dir}/inc ${tpc_dir}/inc )
target_link_libraries(      galephlib.${GALEPH_VERSION}
   PUBLIC Aleph::alephlib.${ALEPHLIB_VERSION}
          Aleph::alephio.${ALEPHIO_VERSION}
          ${ALEPH_CERN_LIBRARIES} ${ALEPH_CERN_SIMLIBRARIES} ${ALEPH_CERN_X11LIBRARIES}
)
target_link_options( galephlib.${GALEPH_VERSION}  PRIVATE "-Wl,-Bstatic" )
#
add_executable( galeph${GALEPH_VERSION} ${galeph_dir}/gal/galeph.F )
target_link_libraries( galeph${GALEPH_VERSION}
     PRIVATE Aleph::galephlib.${GALEPH_VERSION} 
             Aleph::alephlib.${ALEPHLIB_VERSION}
             Aleph::alephio.${ALEPHIO_VERSION}
             Aleph::alephlib.${ALEPHLIB_VERSION}
             ${ALEPH_CERN_LIBRARIES} ${ALEPH_CERN_SIMLIBRARIES} ${ALEPH_CERN_X11LIBRARIES}
)
install(TARGETS  galeph${GALEPH_VERSION} EXPORT Aleph
  RUNTIME DESTINATION bin
)
#
install(DIRECTORY  inc  DESTINATION include)
install(TARGETS  galephlib.${GALEPH_VERSION} EXPORT Aleph
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
