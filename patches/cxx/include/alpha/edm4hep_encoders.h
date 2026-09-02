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
#ifndef ALPHA_EDM4HEP_ENCODERS_H
#define ALPHA_EDM4HEP_ENCODERS_H

/// Include files
#include <DD4hep/IDDescriptor.h>

/// ALPHA namespace declaration
namespace alpha  {

  /// Helper class to hold subdetector properties
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_t  {
  public:
    using Field = dd4hep::BitFieldElement;
    dd4hep::IDDescriptor descriptor;
    const Field*         field_system = nullptr;
    dd4hep::VolumeID     desc_system  = 0UL;
  public:
    /// Default constructor
    detector_t(const char* id);
    /// Default destructor
    virtual ~detector_t();
  };
  
  /// Helper class
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_vdet_t : public detector_t  {
  public:
    const Field* field_layer = nullptr;
    const Field* field_z     = nullptr;
    const Field* field_phi   = nullptr;
    const Field* field_view  = nullptr;
    const Field* field_strip = nullptr;
  public:
    /// Default constructor
    detector_vdet_t();
  };

  /// Helper class
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_itc_t : public detector_t  {
  public:
    const Field* field_layer = nullptr;
    const Field* field_wire  = nullptr;
    /// Default constructor
    detector_itc_t();
  };

  /// Helper class
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_tpc_t : public detector_t  {
  public:
    const Field* field_sector = nullptr;
    const Field* field_padrow = nullptr;
    const Field* field_pad    = nullptr;
  public:
    /// Default constructor
    detector_tpc_t();
  };

  /// Helper class
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_ecal_t : public detector_t  {
  public:
    const Field* field_barrel = nullptr;
    const Field* field_layer  = nullptr;
  public:
    /// Default constructor
    detector_ecal_t();
  };

  /// Helper class
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_hcal_t : public detector_t  {
  public:
    const Field* field_barrel = nullptr;
    const Field* field_layer  = nullptr;
  public:
    /// Default constructor
    detector_hcal_t();
  };

  /// Helper class
  /**
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class output_edm4hep::output_edm4hep::experiment_t  {
  public:
    std::unique_ptr<detector_vdet_t> vdet  { };
    std::unique_ptr<detector_itc_t>  itc   { };
    std::unique_ptr<detector_tpc_t>  tpc   { };
    std::unique_ptr<detector_ecal_t> ecal  { };
    std::unique_ptr<detector_hcal_t> hcal  { };
    std::unique_ptr<detector_t>      muon  { };

  public:
    /// Default constructor
    experiment_t();
    /// Default destructor
    ~experiment_t();
  };
}      // End namespace alpha
#endif // ALPHA_EDM4HEP_ENCODERS_H
