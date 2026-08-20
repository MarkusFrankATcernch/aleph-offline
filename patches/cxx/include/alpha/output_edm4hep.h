//==========================================================================
//  ALEPH software suite
//--------------------------------------------------------------------------
//  Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
//  All rights reserved.
//
//  For the licensing terms see OnlineSys/LICENSE.
//
//--------------------------------------------------------------------------
//
//  Author     : Markus Frank
//==========================================================================
#ifndef ALPHA_OUTPUT_EDM4HEP_H
#define ALPHA_OUTPUT_EDM4HEP_H 1

/// Framework include files
#include <alpha/defs.h>

/// C/C++ include files
#include <memory>

/// ALPHA namespace declaration
namespace alpha  {

  /// Class to perform data translation from ALEPH BOS to EDM4HEP
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class output_edm4hep  {
  public:
    class io_t;
    class event_t;
    class data_access_t;

  public:
    std::unique_ptr<io_t>          io    { };
    std::unique_ptr<event_t>       event { };
    std::unique_ptr<data_access_t> data  { };

  public:
    /// Default constructor
    output_edm4hep();
    /// Default destructor
    virtual ~output_edm4hep();
    /// Start event saving
    void begin_event();
    /// End event saving
    void end_event();
    /// Convert event data
    void convert_event();
    /// After creation add all links to the MC particles
    void fill_particles_mc();
    /// After creation add all links to the reconstructed tracks
    void fill_tracks();
    /// After creation add all links to the ECAL clusters from PECO
    void fill_hits_ecal();
    /// After creation add all links to the HCAL clusters from PHCO
    void fill_hits_hcal();
  };
}      // End namespace alpha
#endif // ALPHA_OUTPUT_EDM4HEP_H
