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

/// Framework include files
#include <alpha/edm4hep_flags.h>
#include <alpha/edm4hep_encoders.h>

/// Default constructor
alpha::detector_t::detector_t(const char* det, const char* id, int det_id)
  : descriptor(det, id), detector_name(det), detector_id(det_id)
{
  this->field_system = this->descriptor.field("system");
  this->desc_system  = this->descriptor.encode(this->field_system, det_id);
}

/// Default destructor
alpha::detector_t::~detector_t()  {
}

/// Default constructor
alpha::detector_vdet_t::detector_vdet_t()
  : detector_t("VDET", "system:8,layer:10,iz:4,iphi:7,view:2,strip:15", detectorid::VDET)  // See vadewa.F
{
  this->field_layer  = this->descriptor.field("layer");
  this->field_z      = this->descriptor.field("iz");
  this->field_phi    = this->descriptor.field("iphi");
  this->field_view   = this->descriptor.field("view");
  this->field_strip  = this->descriptor.field("strip");
}

/// Default constructor
alpha::detector_itc_t::detector_itc_t()
  : detector_t("ITC", "system:8,layer:10,wire:10", detectorid::ITC)
{
  this->field_system  = this->descriptor.field("system");
  this->field_layer   = this->descriptor.field("layer");
  this->field_wire    = this->descriptor.field("wire");
}

/// Default constructor
alpha::detector_tpc_t::detector_tpc_t()
  : detector_t("TPC", "system:8,endcap:2,sector:4,padrow:5,pad:8", detectorid::TPC)
{
  this->field_system  = this->descriptor.field("system");
  this->field_sector  = this->descriptor.field("sector");
  this->field_padrow  = this->descriptor.field("padrow");
  this->field_pad     = this->descriptor.field("pad");
}

/// Default constructor
alpha::detector_ecal_t::detector_ecal_t()
  : detector_t("ECAL", "system:8,barrel:2,layer:10,wire:10", detectorid::ECAL)
{
  this->field_system = this->descriptor.field("system");
  this->field_barrel = this->descriptor.field("barrel");
  this->field_layer  = this->descriptor.field("layer");
}

/// Default constructor
alpha::detector_hcal_t::detector_hcal_t()
  : detector_t("HCAL", "system:8,barrel:2,layer:10,wire:10", detectorid::HCAL)
{
  this->field_system = this->descriptor.field("system");
  this->field_barrel = this->descriptor.field("barrel");
  this->field_layer  = this->descriptor.field("layer");
}

 
/// Default constructor
alpha::experiment_t::experiment_t()   {
  const char* muon_id = "system:8,endcap:1";
  this->vdet = std::make_unique<detector_vdet_t>();
  this->itc  = std::make_unique<detector_itc_t>();
  this->tpc  = std::make_unique<detector_tpc_t>();
  this->ecal = std::make_unique<detector_ecal_t>();
  this->hcal = std::make_unique<detector_hcal_t>();
  this->muon = std::make_unique<detector_t>("MUON",muon_id,detectorid::MUON);
}

/// Default destructor
alpha::experiment_t::~experiment_t()   {
}
