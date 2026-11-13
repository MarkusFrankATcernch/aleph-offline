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
#include <alpha/alpha.h>
#include <DD4hep/IDDescriptor.h>

/// C/C++ include files
#include <memory>
#include <cmath>


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
    std::string          detector_name;
    int                  detector_id;
  public:
    /// Default constructor
    detector_t(const char* det, const char* id, int det_id);
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
    /// https://periodictableguide.com/ionization-energy-chart-of-all-elements
    static constexpr float const_ionization_energy = 8.15 * TGeant4Unit::electronvolt;
    const Field* field_layer = nullptr;
    const Field* field_z     = nullptr;
    const Field* field_phi   = nullptr;
    const Field* field_view  = nullptr;
    const Field* field_strip = nullptr;
  public:
    /// Default constructor
    detector_vdet_t();

    /// Energy deposit from number of electrons of pulseheight
    float energy_deposit(int num_electrons)  const  {
      return num_electrons * const_ionization_energy;
    }
    /// Energy deposit error from number of electrons of pulseheight
    float energy_deposit_error(int num_electrons)  const  {
      // Assume statistical error on the number of electrons produces
      return std::sqrt(num_electrons) * const_ionization_energy;
    }
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
   *    ECAL energy resolution
   *    dE/E = (16.98 +- 0.44 %)/sqrt(E) + 0.45 % (wires)
   *    dE/E = (19.23 +- 0.49 %)/sqrt(E) + 0.33 % (pads)
   *
   *    Spacial resolution of cluster in tower units
   *    dphi = dtheta = a + b/sqrt(E)
   *    a = (1 += 0.2) * 0.01
   *    b = (17.6 += 0.9) * 0.01 * sqrt(GeV)
   *
   *    \author  M.Frank
   *    \date    01/08/2026
   */
  class detector_ecal_t : public detector_t  {
  public:
    const Field* field_barrel = nullptr;
    const Field* field_layer  = nullptr;
    class tower  {
    public:
      std::size_t num_corners { 0UL };
      Position    corners[8];
      Position    center      {     };
      int32_t     module      {  -1 };
    };
    struct tower_id  {
    public:
      int32_t theta = 0, phi = 0, stack = 0;
      bool operator<(const tower_id& c) const  {
        if( theta < c.theta )
          return true;
        else if( phi < c.phi )
          return true;
        else if( stack < c.stack )
          return true;
        return false;
      }
    };

    /// Tower geometry
    class _tower_geometry  {
    protected:
      /// map with ecal tower geometries 
      mutable std::map<tower_id, class tower> _towers;
      
      /// Select tower from identifier. If not present try to load from ECAL geometry package
      const class tower* _get_tower( const tower_id& id )  const;
      
    public:
      /// Default constructor
      _tower_geometry() = default;
      
      /// Access ECAL tower center by id
      Position tower_center(const tower_id& id)  {
        if( const auto* p = this->_get_tower(id) )
          return p->center;
        return { };
      }
      /// Access ECAL tower corners by id
      std::pair<std::size_t, const Position*> tower_corners(const tower_id& id)  {
        if( const auto* p = this->_get_tower(id) )
          return { p->num_corners, p->corners };
        return { 0, nullptr };
      }
      /// Access ECAL tower center by id triplet
      Position tower_center(int itheta, int iphi, int kstack)  {
        return this->tower_center( {itheta, iphi, kstack} );
      }
      /// Access ECAL tower corners by id triplet
      std::pair<std::size_t, const Position*> tower_corners(int itheta, int iphi, int kstack)  {
        return this->tower_corners( {itheta, iphi, kstack} );
      }
    } tower_geometry;


  public:
    /// Default constructor
    detector_ecal_t();
    /// Wire energy resolution of clusters
    double wire_energy_error(double energy_in_gev)  const  {
      double sq_e = std::sqrt(energy_in_gev);
      double de = 0.1698 * sq_e + 0.0045*energy_in_gev;
      return de;
    }
    /// Pad energy resolution of clusters
    double pad_energy_error(double energy_in_gev)  const  {
      double sq_e = std::sqrt(energy_in_gev);
      double de = 0.1923 * sq_e + 0.0033*energy_in_gev;
      return de;
    }
    /// Spacial resolution of cluster in tower units
    double cluster_space_error(double energy_in_gev)  const  {
      double dsp = 1e-2 + 17.9e-2/std::sqrt(energy_in_gev);
      return dsp;
    }
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
  class experiment_t  {
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
