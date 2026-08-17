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
SET( kingal_dir ${CMAKE_CURRENT_SOURCE_DIR} )
message( STATUS "+++ KINGAL:      Build executables from sources: ${kingal_dir}" )
#
#
function(aleph_kin_generator name)
  cmake_parse_arguments( ARG "" "" "SOURCES;INCLUDES" ${ARGN} )
  add_executable( ${name} ${kingal_dir}/kingal.f ${ARG_SOURCES} )
  #
  foreach( _f ${ARG_SOURCES} )
     set_source_files_properties( ${_f} PROPERTIES COMPILE_DEFINITIONS -DKINGAL=1 -DPROGRAM=SUBROUTINE -Dprogram=SUBROUTINE )
  endforeach()
  #
  target_include_directories( ${name} PRIVATE ${kingal_dir} ${kingal_dir}/interfaces ${ARG_INCLUDES} )
  target_compile_options(     ${name} PRIVATE -Wno-tabs -Wno-line-truncation )
  target_link_libraries(      ${name}
    PRIVATE kingal -Wl,-Bstatic ${CERNLIB_LIBDIR}/libpythia.a
                                ${CERNLIB_LIBDIR}/libphotos202.a
                                ${CERNLIB_LIBDIR}/libpacklib.a
  )
  target_link_options(        ${name} PRIVATE -uamarset_ -ualseed_ )
  #
  #
  install( TARGETS            ${name} EXPORT  Aleph RUNTIME DESTINATION bin )
endfunction(aleph_kin_generator)
#
#
file( GLOB kingal_files ${kingal_dir}/lutaud.f
                        ${kingal_dir}/uskrin.f
                        ${kingal_dir}/xksect.f
                        ${kingal_dir}/kinmar.f
                        ${kingal_dir}/rinjob.F
)
#
add_library( kingal ${ALEPH_LIBRARY_TYPE} ${kingal_files} )
add_library( Aleph::kingal ALIAS kingal )
#
target_include_directories( kingal PUBLIC ${kingal_dir}/interfaces )
target_link_libraries( kingal
  PUBLIC Aleph::alephio
         Aleph::alephlib
         Aleph::julialib
         Aleph::bos77
         ${ALEPH_CERN_LIBRARIES}
)
#
#
set( kin_org   ${kingal_dir}/origine )
set( kin_dummy ${kingal_dir}/dummies )
set( kin_iface ${kingal_dir}/interfaces )
set( ugtsec    ${kin_dummy}/ugtsec.f )
set( dymu02_files ${kin_org}/dymu02_origine.f ${kin_dummy}/hstore.f ${kin_dummy}/dymu02.f )
#
aleph_kin_generator(   aria02   SOURCES  ${kin_iface}/aria02.f  ${kin_org}/aria02_origine.f ${ugtsec} )
aleph_kin_generator(   aria04   SOURCES  ${kin_iface}/aria04.f  ${kin_org}/aria04_origine.f ${ugtsec} )
aleph_kin_generator(   bhab01   SOURCES  ${kin_iface}/bhab01.f  ${kin_org}/bhab01_origine.f ${ugtsec} )
aleph_kin_generator(   bhab02   SOURCES  ${kin_iface}/bhab02.f  ${kin_org}/bhab02_origine.f ${ugtsec} )
aleph_kin_generator(   bhlu02   SOURCES  ${kin_iface}/bhlu02.f  ${kin_org}/bhlu02_origine.f ${ugtsec} )
aleph_kin_generator(   bhlu04   SOURCES  ${kin_iface}/bhlu04.f  ${kin_org}/bhlu04_origine.f )
aleph_kin_generator(   bhwi01   SOURCES  ${kin_iface}/bhwi01.f  ${kin_org}/bhwi01_origine.f )
aleph_kin_generator(   bmph01   SOURCES  ${kin_iface}/bmph01.f  ${ugtsec} )
aleph_kin_generator(   cha001   SOURCES  ${kin_iface}/cha001.f  ${ugtsec} )
aleph_kin_generator(   digz01   SOURCES  ${kin_iface}/digz01.f  ${kin_org}/digz01_origine.f ${ugtsec} )
aleph_kin_generator(   dymu02   SOURCES  ${kin_iface}/dymu02.f  ${dymu02_files} ${ugtsec} )
aleph_kin_generator(   eegg01   SOURCES  ${kin_iface}/eegg01.f  ${kin_org}/eegg01_origine.f ${ugtsec} )
aleph_kin_generator(   ferm01   SOURCES  ${kin_iface}/ferm01.f  ${ugtsec} )
aleph_kin_generator(   fgam01   SOURCES  ${kin_iface}/fgam01.f  ${kin_org}/fgam01_origine.f ${ugtsec} )
aleph_kin_generator(   gggb01   SOURCES  ${kin_iface}/gggb01.f  ${kin_org}/gggb01_origine.f ${kin_dummy}/hstore.f )
aleph_kin_generator(   hrwg10   SOURCES  ${kin_iface}/hrwg10.f  ${kin_org}/hrwg10_origine.f ${dymu02_files} )
aleph_kin_generator(   hrwg12   SOURCES  ${kin_iface}/hrwg12.f  ${kin_org}/hrwg12_origine.f ${dymu02_files} )
aleph_kin_generator(   hzha01   SOURCES  ${kin_iface}/hzha01.f  ${ugtsec} )
aleph_kin_generator(   hzha03   SOURCES  ${kin_iface}/hzha03.f )
aleph_kin_generator(   lpww02   SOURCES  ${kingal_dir}/lpww02.f ${kin_org}/lpww02_origine.f ${ugtsec} )
aleph_kin_generator(   lund04   SOURCES  ${kingal_dir}/lund04.f ${ugtsec} )
aleph_kin_generator(   nngg03   SOURCES  ${kin_iface}/nngg03.f  ${kin_org}/nngg03_origine.f ${ugtsec} )
aleph_kin_generator(   nngp01   SOURCES  ${kin_iface}/nngp01.f  ${kin_org}/nngp01_origine.f
                               INCLUDES  ${kingal_dir}/utils )
aleph_kin_generator(   phoj01   SOURCES  ${kin_iface}/phoj01.f  ${kin_org}/phoj01_origine.f )
aleph_kin_generator(   phot02   SOURCES  ${kin_iface}/phot02.f  ${kin_org}/phot02_origine.f )
aleph_kin_generator(   pyth05   SOURCES  ${kin_iface}/pyth05.f )
aleph_kin_generator(   ubab01   SOURCES  ${kin_iface}/ubab01.f  ${kin_org}/ubab01_origine.f ${ugtsec} )
aleph_kin_generator(   ubab02   SOURCES  ${kin_iface}/ubab02.f  ${kin_org}/ubab02_origine.f ${ugtsec} )
#
#
# Don't know how to link this stuff:
# aleph_kin_generator(   brem03   SOURCES ${kin_iface}/brem03.f  ${kin_org}/brem03_origine.f ${ugtsec} )
# aleph_kin_generator(   elw201   SOURCES ${kin_iface}/elw201.f  ${kin_org}/elw201_origine.f ${dymu02_files} )
# aleph_kin_generator(   hrwg09   SOURCES ${kin_iface}/hrwg09.f  ${kin_org}/hrwg09_origine.f ${dymu02_files} )
# aleph_kin_generator(   hzha02   SOURCES ${kin_iface}/hzha02.f )
# aleph_kin_generator(   lutaud   SOURCES ${kingal_dir}/lutaud.f ${kingal_dir}/tauola25_2.f ${ugtsec} )
# aleph_kin_generator(   korl07   SOURCES ${kin_iface}/korl07.f  ${kin_org}/korl07_origine.f )
# aleph_kin_generator(   krlw04   SOURCES ${kin_iface}/krlw04.f  ${kin_org}/krlw04_origine.f
#                                INCLUDES ${kin_iface}/krlw04_inc )
# aleph_kin_generator(   tauola25 SOURCES ${kingal_dir}/tauola25_2.f ${ugtsec} )
#
#
# Not compiling:
# aleph_kin_generator(   mssm01   SOURCES ${kin_iface}/mssm01.f  ${kin_org}/mssm01_origine.f ${ugtsec} )
# aleph_kin_generator(   mssm02   SOURCES ${kin_iface}/mssm02.f  ${kin_org}/mssm02_origine.f )
# aleph_kin_generator(   kk2f02   SOURCES  ${kin_iface}/kk2f02.f  ${kin_org}/kk2f02_origine.f ${dymu02_files} )
#                                 INCLUDES ${kin_iface}/kk2f02_inc )
# aleph_kin_generator(   kk2f03   SOURCES  ${kin_iface}/kk2f03.f  ${kin_org}/kk2f03_origine.f ${dymu02_files}
#                                 INCLUDES ${kin_iface}/kk2f03_inc )
# aleph_kin_generator(   hvfl04   SOURCES  ${kin_iface}/hvfl04.f  ${kin_org}/hvfl04_origine.f ${dymu02_files}
#                                 INCLUDES  ${kin_iface}/hvfl04_inc )
# aleph_kin_generator(   hvfl05   SOURCES  ${kin_iface}/hvfl05.f  ${kin_org}/hvfl05_origine.f ${dymu02_files}
#                                 INCLUDES  ${kin_iface}/hvfl05_inc )

