# ==================================================================================================
#
#   ALEPH library build
#
#   \author  M.Frank
#   \date    09/07/2026
#   \version 1.0
#
# ==================================================================================================
cmake_minimum_required (VERSION 3.10)
#
project (Aleph Fortran C CXX)
#
SET( CMAKE_BUILD_TYPE Debug )
SET( CMAKE_FC_COMPILER f77 )
#
# ==================================================================================================
option( PLATFORM_64    "Build for 64 bit platform                       "   ON  )
option( CERNLIB_SHARED "Use shared libraries from CERNLIB (experimental)"   OFF )
# ==================================================================================================
#
set(FVERSION "-std=f95")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${FVERSION}")
#
function(clean_cmake_flags val)
  string( REPLACE "\n" " " ${val} "${${val}}" )
  string( REPLACE "     " " " ${val} "${${val}}" )
  string( REPLACE "    " " " ${val} "${${val}}" )
  string( REPLACE "   " " " ${val} "${${val}}" )
  string( REPLACE "   " " " ${val} "${${val}}" )
  string( REPLACE "  " " " ${val} "${${val}}" )
  set(ARG_LIST ${${val}})
  separate_arguments(ARG_LIST)
  list(REMOVE_DUPLICATES ARG_LIST)
  string ( REGEX REPLACE "([^\\]|^);" "\\1 " _TMP_STR "${ARG_LIST}" )
  string ( REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}" )
  set ( ${val} "${_TMP_STR}" PARENT_SCOPE )
endfunction(clean_cmake_flags)
#
# ==================================================================================================
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    message(STATUS "Compiling for DEBUG development...")
    set(GLOBAL_OPTS -O0 -g -no-pie)
    # set(GLOBAL_OPTS "-mx32")
    # set(GLOBAL_OPTS "-mcmodel=small")
else()
    set(GLOBAL_OPTS -Og -no-pie)
    message(STATUS "Compiling for RELEASE...")
endif()
#
# ==================================================================================================
set(CMAKE_Fortran_FLAGS "-fPIC -std=legacy -mfpmath=387
                         -Df2cFortran                
                         -Wall                       
                         -Wno-argument-mismatch      
                         -Wno-unused                 
                         -Wno-conversion             
                         -Wno-maybe-uninitialized    
                         -Wno-unused-dummy-argument  
                         -Wno-intrinsic-shadow"
)
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS}
                         -fno-f2c
                         -fno-align-commons
                         -fno-automatic
                         -fno-range-check
                         -fno-backslash
                         -fno-bounds-check
                         -fno-aggressive-loop-optimizations
                         -fdollar-ok
                         -fbacktrace
                         -fallow-argument-mismatch
                         -fd-lines-as-comments
                         -fcommon
                         -fstack-protector
                         -fallow-invalid-boz
                         -ffpe-trap=zero
                         -ffpe-summary=zero
                         ${GLOBAL_OPTS}"
)
#
set(CMAKE_C_FLAGS       "-fPIC -mfpmath=387 -Wall
                         -Wno-pointer-to-int-cast
                         -Wno-builtin-declaration-mismatch
                         -Wno-implicit-function-declaration
                         ${GLOBAL_OPTS}"
)
#
set(CMAKE_CXX_FLAGS "-fPIC -mfpmath=387 -Wall -Wextra  ${GLOBAL_OPTS}" )
add_link_options( ${GLOBAL_OPTS} )
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${GLOBAL_OPTS}")
#
#
if( PLATFORM_64 )
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS}")
  set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}")
  set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}")
  message( STATUS "+++ Compiling for 64 bit platforms..." )
else()
  message(STATUS "Compiling for 32 bit platforms...")
  set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -m32")
  set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}       -m32")
  set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS}     -m32")
endif()
#
#
message( STATUS "+++ CMAKE_Fortran_FLAGS:  ${CMAKE_Fortran_FLAGS}" )
message( STATUS "+++ CMAKE_C_FLAGS:        ${CMAKE_C_FLAGS}" )
message( STATUS "+++ CMAKE_CXX_FLAGS:      ${CMAKE_CXX_FLAGS}" )

clean_cmake_flags (CMAKE_Fortran_FLAGS)
clean_cmake_flags (CMAKE_C_FLAGS)
clean_cmake_flags (CMAKE_CXX_FLAGS)
#
message( STATUS "+++ CMAKE_Fortran_FLAGS:  ${CMAKE_Fortran_FLAGS}" )
message( STATUS "+++ CMAKE_C_FLAGS:        ${CMAKE_C_FLAGS}" )
message( STATUS "+++ CMAKE_CXX_FLAGS:      ${CMAKE_CXX_FLAGS}" )
message( STATUS "+++ CMAKE_CXX_FLAGS:      ${CMAKE_CXX_FLAGS}" )
message( STATUS "+++ CMAKE_CXX_LINK_FLAGS: ${CMAKE_CXX_LINK_FLAGS}" )
#
#  SET( ALEPH_DEBUG_DEBUG_DEFS -DARDEB=1 )
#
SET( ALEPH_LIBRARY_TYPE STATIC )
#
SET( ALEPHLIB_VERSION 314 )
SET( ALEPHIO_VERSION 314 )
SET( GALEPH_VERSION 308 )
SET( JULIA_VERSION 310 )
SET( ALPHA_VERSION 124 )
SET( KIN_VERSION 001 )
SET( GEN_VERSION 001 )
SET( UPHY_VERSION 001 )
#
find_package( CERNLIB REQUIRED )

get_filename_component(CERNLIB_LIBDIR "${CERNLIB_MATHLIB}" DIRECTORY CACHE)

set( ALEPH_PROGRAM_SOURCES ../cxx/opt/chkloc.c )

# ==================================================================================================
# Settings to use CERNLIB SHARED
# ==================================================================================================
set( ALEPH_LIBRARY_LINKOPTS   "-Wl,-Bdynamic" )
set( ALEPH_CERN_LIBRARIES     "-Wl,-Bdynamic -L${CERNLIB_LIBDIR} -lkernlib -lmathlib -llapack -Wl,-Bstatic ${CERNLIB_LIBDIR}/libpacklib.a -Wl,-Bdynamic" )
set( ALEPH_CERN_SIMLIBRARIES  "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libgeant321.a -Wl,-Bdynamic -L${CERNLIB_LIBDIR} -ljetset74 -lpdflib804 -lpythia6205 -Wl,-Bstatic ${CERNLIB_LIBDIR}/libpacklib.a -Wl,-Bdynamic" )
set( ALEPH_CERN_X11LIBRARIES  "-Wl,-Bdynamic -L${CERNLIB_LIBDIR} -lgraflib -lgrafX11 -lpawlib -lX11 -lXm" )

# ==================================================================================================
# Settings to use CERNLIB STATIC
# ==================================================================================================
set( ALEPH_LIBRARY_LINKOPTS   "-Wl,-Bstatic" )
set( ALEPH_CERN_LIBRARIES     "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libpacklib.a   \
                                            ${CERNLIB_LIBDIR}/libmathlib.a   \
                                            ${CERNLIB_LIBDIR}/libkernlib.a   \
                                            -Wl,-Bdynamic" )
set( ALEPH_CERN_SIMLIBRARIES  "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libgeant321.a  \
                                            ${CERNLIB_LIBDIR}/libjetset74.a  \
                                            ${CERNLIB_LIBDIR}/libpdflib804.a \
                                            ${CERNLIB_LIBDIR}/liblapack.a    \
                                            ${CERNLIB_LIBDIR}/libblas.a      \
                                            ${CERNLIB_LIBDIR}/libpacklib.a   \
                                            ${CERNLIB_LIBDIR}/libmathlib.a   \
                                            ${CERNLIB_LIBDIR}/libkernlib.a   \
                                            -Wl,-Bdynamic" )
set( ALEPH_CERN_X11LIBRARIES  "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libgraflib.a   \
                                            ${CERNLIB_LIBDIR}/libgrafX11.a   \
                                            ${CERNLIB_LIBDIR}/libpawlib.a    \
                                            -Wl,-Bdynamic -lX11 -lXm" )
#
add_subdirectory(   bos77 )
add_subdirectory(   alephio )
add_subdirectory(   alephlib )
add_subdirectory(   dbase )
add_subdirectory(   julia )
add_subdirectory(   alpha )
add_subdirectory(   galeph )
add_subdirectory(   uphy )
add_subdirectory(   cxx )
#add_subdirectory(  kin.${KIN_VERSION} )
#add_subdirectory(   gen.${GEN_VERSION} )
#
