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
    /// Decode Itc DIGitisings
    /**
     *  +------+                            Subschema: ItcEventRecord                
     *  | IDIG |  Itc DIGitisings (ordered by 
     *  +------+  increasing wire number)  (RAW)
     *  ..............................................................                
     *      1          I    Number of words/digit. (=1)                               
     *      2          I    Number of digits                                          
     *  ..............................................................                
     *       1    PD  I    PackedDigit      [*,*]                                     
     *                        ITC digits. info. packed into                           
     *                        a 32 bit word.                                          
     *                        10 bits wire number - bits 0 - 9,                       
     *                        9 bits r-phi TDC - bits 10 - 18,                        
     *                        9 bits Z TDC - bits 19 - 27,                            
     *                        2 bits quality flag- bits 28 - 29,                      
     *                        2 bits spare - bits 30 - 31. (least                     
     *                        signif. bit = bit 0).                                   
     *  ..............................................................
     *       
     *  See: alephlib/ides/igeomw.F
     *       alephlib/itc/icrcoo.F
     *       alephlib/itc/itswco.F
     *       
     *    \author  M.Frank
     *    \date    01/08/2026
     */
    class DigitDecoder  {
    public:
      int32_t _address;

      // alephlib/ides/igeomw.F:
      static constexpr int32_t NWIR[8] = { 96,    96,    96,    96,   144,   144,   144,   144    };
      static constexpr int32_t IWIR[8] = {  0,    96,   192,   288,   384,   528,   672,   816    };
      static constexpr float   RWIR[8] = { 16.11, 17.20, 18.70, 19.79, 21.68, 23.01, 24.69, 26.02 };
      static constexpr float   PHWR[8] = {  0.,    1.,    0.,    1.,    0.,    1.,    0.,    1.   };
      static constexpr float   SGMXIT  =    0.010;
      static constexpr float   WZMXIT  =  100.;
      static constexpr float   twopi   =    2.0e0 * M_PI;
      //
      // IWIRIT(I) = IWIR(I)
      // NWIRIT(I) = NWIR(I)
      // RWIRIT(I) = RWIR(I)
      // CELWIT(I) = TWOPI*RWIR(I)/FLOAT(NWIR(I))
      // PHWRIT(I) = PI*PHWR(I)/FLOAT(NWIR(I))
      //
    public:
      DigitDecoder(int32_t addr) : _address(addr)  {      }
      int32_t digit()  const     {  return this->_address;                 }
      int32_t wire()  const      {  return (this->_address)     & 0x3FF;   }
      int32_t tdc_rphi()  const  {  return (this->_address>>10) & 0x1FF;   }
      int32_t tdc_z()  const     {  return (this->_address>>18) & 0x1FF;   }
      int32_t quality()  const   {  return (this->_address>>27) & 0x3;     }
      int32_t _spare()  const    {  return (this->_address>>29) & 0x3;     }
      
      /// Layer number starting from 'offset'
      int32_t layer(int32_t offset=0)  const     {
        auto w = this->wire();
        if( w < IWIR[4] ) return (w-1)/NWIR[0] + offset;
        return ((w-IWIR[4]-1)/NWIR[4]) + 4 + offset;
      }
      /// 
      float phi()  const         {  // See: igeomw.F, itswco.F
        int32_t wir  = this->wire();
        int32_t lay  = this->layer();
        int32_t ind  = IWIR[lay];
        float   j    = float(wir - ind - 1);
        float   phsw = M_PI * ( j*2e0 + PHWR[lay] ) / float(NWIR[lay]);
        return (phsw < 0e0) ? phsw + twopi : ((phsw > twopi) ? phsw - twopi : phsw);
      }
#if 0
      float r()  const           { // itswco.F
        float tdiff = ;
        float sag   = SGMXIT * (1.0 - ()*() );
        float value = RWIR[this->layer(0)];
        return value;
      }
      float z0()  const  {         // itswco.F
        //int32_t tdiff =
        return 0x0;
      }
      float sigmaZ0()  const  {
        return 0x0;
      }
#endif
    };
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

    /// Decode HCAL wire/cluster address
    /**
     * See: HWDI:   Hcal Wire (tube) DIgits(RawData)
     *      HLWD:   Hcal Longitudinal Wire Digit        
     * ..............................................................                
     *     1          I    Nb of words/wirecluster (=1)                              
     *     2          I    Number of clusters                                        
     * ..............................................................                
     *      1    CA  I    ClusterAddr      [*,*]                                     
     *                       Cluster address and width                               
     *                       bits: 0- 7 : address of last tube                       
     *                       in cluster [1,255]                                      
     *                       8-15 : cluster width [1,15]                             
     *                       16-23 : layer number [0,22]                             
     *                       24-31 : electronic module number                        
     *                       [1,36]                                                  
     *                       1-12 end-cap A                                          
     *                       13-24 barrel                                            
     *                       25-36 end-cap B                                         
     *    \author  M.Frank
     *    \date    01/08/2026
     */
    class WireDecoder  {
    public:
      int32_t _address;
    public:
      WireDecoder(int32_t addr) : _address(addr)  {                         }
      int32_t address()  const   {  return this->_address;                  }
      int32_t tube()  const      {  return (this->_address)     & 0xFF;     }
      int32_t width()  const     {  return (this->_address>>8)  & 0x1F;     }
      int32_t layer()  const     {  return (this->_address>>16) & 0x1F;     }
      int32_t module()  const    {  return (this->_address>>24) & 0xFF;     }
      bool    is_endcapA() const {  return this->module() < 13;             }
      bool    is_endcapB() const {  return this->module() > 24;             }
      bool    is_barrel() const  {  return !(is_endcapA() || is_endcapB()); }
    };
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
