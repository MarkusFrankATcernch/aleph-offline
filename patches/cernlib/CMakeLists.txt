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
SET( cernlib_dir ${CMAKE_CURRENT_SOURCE_DIR}/src )
SET( mathlib_dir ${cernlib_dir}/mathlib )
SET( packlib_dir ${cernlib_dir}/packlib )
SET( kernlib_dir ${packlib_dir}/kernlib )


file( GLOB epio_files     ${packlib_dir}/epio/code/*.F
                          ${packlib_dir}/epio/util/cfribmsv.F
                          ${packlib_dir}/epio/util/ctoibstv.F
                          ${packlib_dir}/epio/util/btswpstv.F
                          ${packlib_dir}/epio/util/blo8stv.F
                          ${packlib_dir}/epio/util/blo16stv.F
                          ${packlib_dir}/epio/util/blo32wsv.F
                          ${packlib_dir}/epio/util/bun8stv.F
                          ${packlib_dir}/epio/util/bun16stv.F
                          ${packlib_dir}/epio/util/bun32wsv.F
                          ${packlib_dir}/epio/util/iw16stv.F
                          ${packlib_dir}/epio/util/w16mvstv.F
                          ${packlib_dir}/epio/util/iwd16stv.F
                          ${packlib_dir}/epio/util/sfrascap.F
                          ${packlib_dir}/epio/util/sfrascvx.F
                          ${packlib_dir}/epio/util/stoascap.F
                          ${packlib_dir}/epio/util/stoascvx.F
)                         
file( GLOB kernlib_files
                          ${kernlib_dir}/kernbit/i303/*.F
                          ${kernlib_dir}/kernbit/j530/*.F
                          ${kernlib_dir}/kernbit/m429/*.F
                          ${kernlib_dir}/kernbit/m433/*.F
                          ${kernlib_dir}/kernbit/m437/*.F
                          ${kernlib_dir}/kernbit/m439/*.F
                          ${kernlib_dir}/kernbit/m442/*.F
                          ${kernlib_dir}/kernbit/m443/*.F
                          ${kernlib_dir}/kernbit/t001/*.F
                          ${kernlib_dir}/kernbit/t002/*.F
                          ${kernlib_dir}/kernbit/z009/*.F
# VAX                     ${kernlib_dir}/kernbit/z037/*.F
                          ${kernlib_dir}/kernbit/z264/*.F
                          ${kernlib_dir}/kernbit/z265/*.F
                          ${kernlib_dir}/kernbit/z268/*.F
# VAX                     ${kernlib_dir}/kernbit/z313/*.F


                          ${kernlib_dir}/kerngen/ccgen/*.c
                          ${kernlib_dir}/kerngen/ccgen/*.F
                          ${kernlib_dir}/kerngen/ccgencf/*.c
                          ${kernlib_dir}/kerngen/ccgencf/*.F
                          ${kernlib_dir}/kerngen/ccgenci/*.c
                          ${kernlib_dir}/kerngen/ccgenci/*.F
                          ${kernlib_dir}/kerngen/ccgenu/*.c
			  ${kernlib_dir}/kerngen/tcgen/*.F
			  ${kernlib_dir}/kerngen/tcgen/*.c
#                          ${kernlib_dir}/kerngen/tcgenc/*.F
                          ${kernlib_dir}/kerngen/tcgenc/*.c
			  ${kernlib_dir}/kerngen/tcgens/*.F
                          ${kernlib_dir}/kerngen/tcgens/*.c
                          ${kernlib_dir}/kerngen/xvect/*.F
                          ${kernlib_dir}/umon/umonftn/*.F
                          ${kernlib_dir}/kerngen/unix/lnxgs/*.c
                          ${kernlib_dir}/kerngen/unix/lnxgs/*.F

                          ${kernlib_dir}/kernnum/c204fort/*.F
                          ${kernlib_dir}/kernnum/c205fort/*.F
                          ${kernlib_dir}/kernnum/c308fort/*.F
                          ${kernlib_dir}/kernnum/c312fort/*.F
                          ${kernlib_dir}/kernnum/c313fort/*.F
                          ${kernlib_dir}/kernnum/c336fort/*.F
                          ${kernlib_dir}/kernnum/c337fort/*.F
                          ${kernlib_dir}/kernnum/c341fort/*.F
                          ${kernlib_dir}/kernnum/d106fort/*.F
                          ${kernlib_dir}/kernnum/d209fort/*.F
                          ${kernlib_dir}/kernnum/d509fort/*.F
                          ${kernlib_dir}/kernnum/d703fort/*.F
                          ${kernlib_dir}/kernnum/d704fort/*.F
                          ${kernlib_dir}/kernnum/e100fort/*.F
                          ${kernlib_dir}/kernnum/e104fort/*.F
                          ${kernlib_dir}/kernnum/e105fort/*.F
                          ${kernlib_dir}/kernnum/e106fort/*.F
                          ${kernlib_dir}/kernnum/e208fort/*.F
                          ${kernlib_dir}/kernnum/f002fort/*.F
                          ${kernlib_dir}/kernnum/f003fort/*.F
                          ${kernlib_dir}/kernnum/f004fort/*.F
                          ${kernlib_dir}/kernnum/f010fort/*.F
                          ${kernlib_dir}/kernnum/f011fort/*.F
                          ${kernlib_dir}/kernnum/f012fort/*.F
                          ${kernlib_dir}/kernnum/f406fort/*.F
                          ${kernlib_dir}/kernnum/g900fort/*.F
                          ${kernlib_dir}/kernnum/g901fort/*.F
                          ${kernlib_dir}/kernnum/n001fort/*.F
                          ${kernlib_dir}/kernnum/kernnum/*.F
                          ${kernlib_dir}/kernnum/obsolete/d103fort/*.F
			  ${kernlib_dir}/kernnum/e105fort/*.F
)
file( GLOB packlib_files  ${packlib_dir}/cspack/cz/*.F
                          ${packlib_dir}/cspack/fz/*.F
                          ${packlib_dir}/cspack/sz/*.F
                          ${packlib_dir}/cspack/xz/*.F
                          ${packlib_dir}/cspack/tcpaw/*.c
                          ${packlib_dir}/ffread/sffread/*.F
                          ${epio_files}
                          ${kernlib_files}
)
file( GLOB mathlib_files  ${mathlib_dir}/gen/a/*.F
			  ${mathlib_dir}/gen/b/*.F
			  ${mathlib_dir}/gen/c/*.F
			  ${mathlib_dir}/gen/d/*.F
			  ${mathlib_dir}/gen/e/*.F
			  ${mathlib_dir}/gen/f/*.F
			  ${mathlib_dir}/gen/g/*.F
			  ${mathlib_dir}/gen/h/*.F
			  ${mathlib_dir}/gen/j/*.F
			  ${mathlib_dir}/gen/m/*.F
			  ${mathlib_dir}/gen/n/*.F
			  ${mathlib_dir}/gen/s/*.F
			  ${mathlib_dir}/gen/u/*.F
			  ${mathlib_dir}/gen/v/*.F
			  ${mathlib_dir}/gen/x/*.F
)

message(STATUS  ${ccgencf_files} )
# ==================================================================================================
add_library( packlib ${ALEPH_LIBRARY_TYPE} ${packlib_files} )
add_library( Aleph::packlib ALIAS packlib )
target_compile_definitions( packlib  PUBLIC -DCERNLIB_UNIX -DCERNLIB_LINUX=1)

SET( packlib_includes ${cernlib_dir}
                      ${packlib_dir}/zebra
                      ${packlib_dir}/cspack
                      ${packlib_dir}/epio
                      ${packlib_dir}/ffread
                      ${kernlib_dir}/kernnum
                      ${kernlib_dir}/kerngen
                      ${kernlib_dir}/kernbit )
target_include_directories( packlib  PRIVATE ${packlib_includes} )
target_compile_options( packlib PRIVATE -Wno-uninitialized
                        $<$<COMPILE_LANGUAGE:C>:  -Wno-int-conversion>
                        $<$<COMPILE_LANGUAGE:Fortran>:  -Wno-tabs -Wno-character-truncation>
)
#
# ==================================================================================================
add_library( mathlib ${ALEPH_LIBRARY_TYPE} ${mathlib_files} )
add_library( Aleph::mathlib ALIAS mathlib )
target_include_directories( mathlib  PRIVATE ${mathlib_dir}/gen )
target_compile_options( packlib PRIVATE -Wno-uninitialized
            $<$<COMPILE_LANGUAGE:Fortran>:  -Wno-tabs -Wno-character-truncation>
)
target_link_libraries( mathlib PUBLIC packlib )
#
install(TARGETS  packlib mathlib EXPORT Aleph
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
#
#
# ==================================================================================================
#if(NEVER)
  file( GLOB hbook_files  ${packlib_dir}/hbook/code/*.F
                          ${packlib_dir}/hbook/fpclassc/*.c
                          ${packlib_dir}/hbook/hdiff/*.F
                          ${packlib_dir}/hbook/hglobal/*.F
  #                       ${packlib_dir}/hbook/hmcstat/*.F
                          ${packlib_dir}/hbook/hmerge/*.F
                          ${packlib_dir}/hbook/chbook/halloc.c
                          ${packlib_dir}/hbook/chbook/*.c
                          ${packlib_dir}/hbook/hmmap/*.F
                          ${packlib_dir}/hbook/hntup/*.F
                          ${packlib_dir}/hbook/hquad/*.F
                          ${packlib_dir}/hbook/hrz/*.F
                          ${packlib_dir}/hbook/hrz/*.F

                          ${packlib_dir}/comis/code/*.F
#                          ${packlib_dir}/kuip/code_kuip/*.c
#                          ${packlib_dir}/kuip/code_kuip/*.F

                          ${packlib_dir}/zbook/code/*.F
                          ${packlib_dir}/zbook/code/*.c

                          ${packlib_dir}/zebra/fq/*.F
                          ${packlib_dir}/zebra/rz/*.F
                          ${packlib_dir}/zebra/mq/*.F
                          ${packlib_dir}/zebra/mqg/*.F
                          ${packlib_dir}/zebra/mqs/*.F
                          ${packlib_dir}/zebra/mqv/*.F
                          ${packlib_dir}/zebra/qend/*.F
                          ${packlib_dir}/zebra/qutil/*.F
                          ${packlib_dir}/zebra/zebra/*.F
                          ${packlib_dir}/zebra/dzebra/*.F
                          ${packlib_dir}/minuit/code/*.F
  )
  # ==================================================================================================
  add_library( hbook ${ALEPH_LIBRARY_TYPE} ${hbook_files} )
  add_library( Aleph::hbook ALIAS hbook )
  target_include_directories( hbook
  PRIVATE ${cernlib_dir}
          ${packlib_dir}
          ${packlib_dir}/hbook
          ${packlib_dir}/kuip
          ${packlib_dir}/minuit
          ${packlib_dir}/zbook
          ${packlib_dir}/zebra
          ${packlib_dir}/minuit
  )
  #
  target_compile_definitions( hbook PRIVATE -DCERNLIB_UNIX -Dlynx)
  target_compile_options( hbook PRIVATE -Wno-return-type -Wno-uninitialized
                          $<$<COMPILE_LANGUAGE:Fortran>:  -Wno-tabs -Wno-character-truncation>
  )
  target_link_libraries( hbook PUBLIC Aleph::packlib Aleph::mathlib -lcrypt -ldl -lc )
  #
  # ==================================================================================================
  file( GLOB jetset_files   ${cernlib_dir}/mclibs/jetset/jetset/*.F
                            ${cernlib_dir}/mclibs/pythia/code/*.F
  )
  add_library( jetset ${ALEPH_LIBRARY_TYPE} ${jetset_files} )
  add_library( Aleph::jetset ALIAS jetset )
  target_include_directories( jetset PRIVATE ${packlib_includes} )
  target_compile_options( jetset PRIVATE -Wno-uninitialized
            $<$<COMPILE_LANGUAGE:Fortran>:  -Wno-tabs -Wno-character-truncation>
  )
  target_link_libraries( jetset PUBLIC Aleph::packlib Aleph::mathlib )
  #
  # ==================================================================================================
  SET( geant_dir  ${cernlib_dir}/geant321 )
  file( GLOB geant_files  ${geant_dir}/cdf/*.F
                          ${geant_dir}/erdecks/*.F
                          ${geant_dir}/gcons/*.F
                          ${geant_dir}/ghits/*.F
                          ${geant_dir}/peanut/*.F
                          ${geant_dir}/ghutils/*.F
                          ${geant_dir}/miguti/*.F
                          ${geant_dir}/giopa/*.F
                          ${geant_dir}/matx55/*.F
                          ${geant_dir}/gparal/*.F
                          ${geant_dir}/gscan/*.F
                          ${geant_dir}/gtrak/*.F
                          ${geant_dir}/gxint/*.F
                          ${geant_dir}/ggeom/*.F
                          ${geant_dir}/block/*.F
                          ${geant_dir}/cgpack/*.F
                          ${geant_dir}/erpremc/*.F
                          ${geant_dir}/fiface/*.F
                          ${geant_dir}/gbase/*.F
                          ${geant_dir}/gdraw/*.F
                          ${geant_dir}/geocad/*.F
                          ${geant_dir}/gheisha/*.F
                          ${geant_dir}/ghrout/*.F
                          ${geant_dir}/giface/*.F
                          ${geant_dir}/gkine/*.F
                          ${geant_dir}/gphys/*.F
                          ${geant_dir}/gstrag/*.F
                          ${geant_dir}/guser/*.F
                          ${geant_dir}/gxuser/*.F
                          ${geant_dir}/miface/*.F
                          ${geant_dir}/neutro/*.F
  )
  # ==================================================================================================
  add_library( geant3 ${ALEPH_LIBRARY_TYPE} ${geant_files} )
  add_library( Aleph::geant3 ALIAS geant3 )
  target_include_directories( geant3 PRIVATE ${geant_dir}  )
  target_compile_options( geant3 PRIVATE -Wno-return-type -Wno-uninitialized
            $<$<COMPILE_LANGUAGE:Fortran>:  -Wno-tabs -Wno-character-truncation>
  )
  target_link_libraries( geant3 PUBLIC Aleph::mathlib Aleph::packlib )
  # ==================================================================================================
  #
  install(TARGETS    hbook jetset geant3 EXPORT Aleph
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
  )
#endif()
#
# ==================================================================================================
