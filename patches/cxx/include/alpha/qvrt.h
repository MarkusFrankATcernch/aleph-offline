//==========================================================================
//  Software for the ALEPH experiment
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see ALSOFTINSTALL/LICENSE.
// For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
#ifndef ALPHA_BOS_QVRT_H
#define ALPHA_BOS_QVRT_H

/// C/C++ include files

/// Framework include files
#include <alpha/alpha.h>

/// ALPHA namespace declaration
namespace alpha  {

  /// Forward declarations
  class qvrt;
  class qvec;
  class qdet;
  class fver;
  class yv0v;

  /// Track data from bank QVRT
  /** HAC parameters and description from ALPHA manual Appendix B:
   *
   *       PARAMETER ( JQVRVX=1,JQVRVY=2,JQVRVZ=3,JQVRVN=4,JQVRTY=5,
   *     1   JQVRIP=6,JQVRND=7,JQVRDL=8,JQVRAY=9,JQVRAF=10,JQVREM=11,
   *     2   JQVRCF=17,JQVRET=18)
   *
   *  *------*
   *  | QVRT |          VERTICES
   *  *------*
   *
   *    1     number of words / vertex
   *    2     maximum allowed number of vertices
   *
   *    1     VX    F   XPosition
   *    2     VY    F   YPosition
   *    3     VZ    F   ZPosition
   *    4     VN    I   JULIA /GALEPH Vertex number
   *    5     TY    B   vertex TYpe
   *    6     IP    I   track number of Incoming Particle
   *    7     ND    I   Number of Decay particles
   *    8     DL    I   offset for decay particle list
   *    9     AY    I   offset for corresponding row in YV0V
   *   10     AF    I   offset for corresponding row in FVER
   *   11-16  EM    F   triangular error matrix
   *   17     CF    F   Fchi**2 for vertex fit -- filled by KVFITN, KVFITV
   *   18-25  ET    F   track-vertex error matrix
   *   26-30        F   User space
   *
   *
   *   \author M.Frank
   *   \version 1.0
   */
  class qvrt  {
  public:
    float    x                    { 0e0 };              // JQVRVX=0
    float    y                    { 0e0 };              // JQVRVY=1
    float    z                    { 0e0 };              // JQVRVZ=2
    int32_t  kv_number            {   0 };              // JQVRVN=3
    int32_t  kv_type              {   0 };              // JQVRTY=4
    int32_t  particle_incoming    {   0 };              // JQVRIP=5
    int32_t  num_outgoing         {   0 };              // JQVRND=6
    int32_t  first_outgoing       {   0 };              // JQVRDL=7
    int32_t  offset_yv0v          {   0 };              // JQVRAY=8
    int32_t  offset_fver          {   0 };              // JQVRAF=9
    float    error_matrix[5];                           // JQVREM=10
    float    chi_square           { 0e0 };              // JQVRCF=16
    float    track_vtx_error_matrix[7];                 // JQVRET=17
    float    user_space[1];
  public:
    /** HAC parameters                           */
    constexpr static const int32_t JQVRVX = 0;
    constexpr static const int32_t JQVRVY = 1;
    constexpr static const int32_t JQVRVZ = 2;
    constexpr static const int32_t JQVRVN = 3;
    constexpr static const int32_t JQVRTY = 4;
    constexpr static const int32_t JQVRIP = 5;
    constexpr static const int32_t JQVRND = 6;
    constexpr static const int32_t JQVRDL = 7;
    constexpr static const int32_t JQVRAY = 8;
    constexpr static const int32_t JQVRAF = 9;
    constexpr static const int32_t JQVREM = 10;
    constexpr static const int32_t JQVRCF = 16;
    constexpr static const int32_t JQVRET = 17;

  protected:
    /// Default constructor inhibited: object lives in the BOS common block
    qvrt() = delete;
    /// Default destructor inhibited: object lives in the BOS common block
    ~qvrt() = delete;

  public:
    /// Ease switching from single to double precision
    using real_t = float;

    /** Alpha like access functions   */
    /// x momentum component
    real_t  qvx()      const   {  return this->x;                                }
    /// y momentum component
    real_t  qvy()      const   {  return this->y;                                }
    /// z momentum component
    real_t  qvz()      const   {  return this->z;                                }

    /// JULIA/GALEPH vertex number
    int32_t kvn()      const   {  return this->kv_number;                        }
    /// Vertex type  (See ALPHA manual for details)
    int32_t kvtype()   const   {  return this->kv_type;                          }
    /// particle incoming to vertex IVX
    int32_t kvincp()   const   {  return this->particle_incoming;                }
    /// number of outgoing tracks
    int32_t kvndau()   const   {  return this->num_outgoing;                     }

    /// Chisquare of the vertex fit (See ALPHA manual for details)
    real_t  qvchif()   const   {  return this->chi_square;                       }
    /// element (N,M) of the covariance matrix N,M = 0,1,2
    real_t  qvem(int n, int m)  const  {
      int32_t element = params.kmatix[n][m];
      return this->error_matrix[element];
    }
    /// Access pointer to YV0V
    const class yv0v* yv0v()  const  {  return bcs_offset<class yv0v>(this->offset_yv0v);  }
    /// Access pointer to FVER
    const class fver* fver()  const  {  return bcs_offset<class fver>(this->offset_fver);  }

  };
}      // End namespace alpha
#include <alpha/qvec.h>

/// ALPHA namespace declaration
namespace alpha  {


}      // End namespace alpha
#endif // ALPHA_BOS_QVRT_H
