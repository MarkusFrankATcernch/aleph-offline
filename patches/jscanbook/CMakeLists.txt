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
SET( scanbook_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message(STATUS "+++ SCANBOOK:       Build library from sources: ${scanbook_dir}" )
#
file( GLOB scanbook_files ${scanbook_dir}/fortran/*.f )
#
add_executable( scanbook ${scanbook_files} )
target_include_directories( scanbook PUBLIC ${scanbook_dir}/inc )
target_link_libraries( scanbook
     PRIVATE Aleph::alephlib
             Aleph::alephio
             Aleph::alephlib
             ${ALEPH_CERN_LIBRARIES}
)
#
install(TARGETS scanbook EXPORT Aleph
  RUNTIME DESTINATION bin
)
