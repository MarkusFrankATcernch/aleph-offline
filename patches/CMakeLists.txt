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
set(FVERSION "-std=f95")
set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${FVERSION}")
#
SET( PLATFORM_64 ON )
#
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    message(STATUS "Compiling for development...")

    SET(OPTS          "-Wall -fno-automatic -fdollar-ok -fno-backslash -Wno-argument-mismatch -fd-lines-as-comments")
    SET(OPTS "${OPTS}  -Wno-unused -Wno-conversion -Wno-maybe-uninitialized -fno-bounds-check -std=legacy")
    SET(OPTS "${OPTS}  -Wno-unused-dummy-argument -Wno-intrinsic-shadow -fno-range-check")

    # set(GLOBAL_OPTS "-mx32")
    set(GLOBAL_OPTS "-mcmodel=small")
    set(CMAKE_Fortran_FLAGS "${OPTS} ${GLOBAL_OPTS}" )
    set(CMAKE_C_FLAGS   "-Wall -Wno-pointer-to-int-cast -Wno-builtin-declaration-mismatch -Wno-implicit-function-declaration  ${GLOBAL_OPTS}")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra  ${GLOBAL_OPTS}" )
    add_link_options( "${GLOBAL_OPTS}" )

    if( PLATFORM_64 )
      message(STATUS "Compiling for 64 bit platforms...")

      set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fbacktrace -fallow-argument-mismatch -fd-lines-as-comments -fallow-invalid-boz" )
      set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -ffpe-trap=zero -ffpe-summary=zero -O0")
      set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O0")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

    else()

      message(STATUS "Compiling for 32 bit platforms...")
      set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -m32 -O0")
      set(CMAKE_C_FLAGS       "${CMAKE_C_FLAGS}   -m32 -O0")
      set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -m32 -O0")
    endif()

else()
    message(STATUS "Compiling for release...")
    set(CMAKE_Fortran_FLAGS "-O3 -fallow-argument-mismatch -fallow-invalid-boz -fd-lines-as-comments ")
endif()
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

set( ALEPH_PROGRAM_SOURCES "../bos77/src/ibloc.cpp" )
set( ALEPH_CERN_LIBRARIES  "-Wl,-Bstatic -L${CERNLIB_LIBDIR} -lpacklib -lkernlib -lmathlib -llapack" )
set( ALEPH_CERN_SIMLIBRARIES  "-Wl,-Bstatic -L${CERNLIB_LIBDIR} libgeant321.a -ljetset74 -lpdflib804 -lpythia6205 -lpacklib" )
set( ALEPH_CERN_X11LIBRARIES  "-Wl,-Bstatic -L${CERNLIB_LIBDIR} -lgraflib -lgrafX11 -lpawlib  X11 -lXm" )


set( ALEPH_CERN_LIBRARIES  "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libpacklib.a ${CERNLIB_LIBDIR}/libkernlib.a ${CERNLIB_LIBDIR}/libmathlib.a ${CERNLIB_LIBDIR}/liblapack.a -Wl,-Bdynamic" )
set( ALEPH_CERN_SIMLIBRARIES  "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libgeant321.a \
                                            ${CERNLIB_LIBDIR}/libjetset74.a \
                                            ${CERNLIB_LIBDIR}/libpdflib804.a \
                                            ${CERNLIB_LIBDIR}/libpacklib.a ${CERNLIB_LIBDIR}/libkernlib.a -Wl,-Bdynamic"
                                            )
set( ALEPH_CERN_X11LIBRARIES  "-Wl,-Bstatic ${CERNLIB_LIBDIR}/libgraflib.a \
                                            ${CERNLIB_LIBDIR}/libgrafX11.a \
                                            ${CERNLIB_LIBDIR}/libpawlib.a   -Wl,-Bdynamic -lX11 -lXm" )

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
