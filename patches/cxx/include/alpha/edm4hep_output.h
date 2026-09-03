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
#ifndef ALPHA_EDM4HEP_OUTPUT_H
#define ALPHA_EDM4HEP_OUTPUT_H 1

/// Framework include files
#include <alpha/defs.h>
#include <alpha/edm4hep_flags.h>
#include <alpha/edm4hep_encoders.h>

/// C/C++ include files
#include <memory>

/// ALPHA namespace declaration
namespace alpha  {

  class detector_t;
  class constants_t;
  
  /// Class to perform data translation from ALEPH BOS to EDM4HEP
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class edm4hep_output  {
  public:
    class io_t;
    class event_t;
    class data_access_t;

  public:
    std::unique_ptr<io_t>            io    { };
    std::unique_ptr<event_t>         event { };
    std::unique_ptr<data_access_t>   data  { };
    std::unique_ptr<experiment_t>    exp   { };

    std::string output_file          {       };
    std::string debug_banks          {       };

    bool        debug_io             { false };
    
    bool        save_all             { false };
    
    bool        save_particles       { false };
    bool        save_mc_particles    { false };
    bool        save_charged_tracks  { false };

    bool        save_tracker         { false };
    bool        save_vdet_sim        { false };
    bool        save_vdet_reco       { false };
    bool        save_itc_sim         { false };
    bool        save_itc_reco        { false };
    bool        save_tpc_sim         { false };
    bool        save_tpc_reco        { false };

    bool        save_calorimeter     { false };
    bool        save_ecal_reco       { false };
    bool        save_hcal_reco       { false };
    
  public:
    /// Default constructor
    edm4hep_output();
    /// Default destructor
    virtual ~edm4hep_output();

    /// Set option value
    int set_option(const char* name, const char* value);

    /// Initialize conversion. Open file
    bool initialize();
    /// Finalize conversion. Close file
    bool finalize();
    /// Framework event callback
    bool handle_event(constants_t& par);

    /// Start event saving
    std::unique_ptr<event_t> begin_event();
    /// End event saving
    void end_event(event_t& event);
    /// Convert event data
    void convert_event(event_t& event);
 };
}      // End namespace alpha
#endif // ALPHA_EDM4HEP_OUTPUT_H
