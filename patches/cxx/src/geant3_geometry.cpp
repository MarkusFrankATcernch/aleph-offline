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
#include <alpha/geant3_geometry.h>

#include <DD4hep/Detector.h>
#include <TGeoSystemOfUnits.h>
#include <TGeant4SystemOfUnits.h>

namespace  {
  static UInt_t unique_mat_id = 0xBFFEFEED;
}

/// Default constructor
dd4hep::geant3_geometry::geant3_geometry(Detector& desc)
  : description(desc)
{
  this->cnv_len     = TGeoUnit::cm / TGeant4Unit::cm;
  this->cnv_intlen  = TGeoUnit::cm / TGeant4Unit::cm;
  this->cnv_radlen  = TGeoUnit::cm / TGeant4Unit::cm;
  this->cnv_angle   = TGeoUnit::degree / TGeant4Unit::rad;
  this->cnv_density = (TGeoUnit::gram  / TGeoUnit::cm3)
    / (TGeant4Unit::gram/TGeant4Unit::cm3);
}

/// Default destructor
dd4hep::geant3_geometry::~geant3_geometry()
{
}

/// Access material to build volumes
dd4hep::Material dd4hep::geant3_geometry::get_material(const std::string& name)  const  {
  return this->description.material(name);
}

/// Add volume to geometry inventory
void dd4hep::geant3_geometry::add_volume( int index,
                                          const std::string& name,
                                          const std::string& medium,
                                          const Solid& solid )
{
  if( this->volumes.find(name) != this->volumes.end() )  {
    throw std::runtime_error("Volume "+name+" with index "+
                             std::to_string(index)+" is already registered.");
  }
  Material mat = description.material(medium);
  Volume   vol(name, solid, mat);
  this->volumes[name] = vol;
}

/// Add rotation matrix to build placement transformations
void dd4hep::geant3_geometry::add_rotation( int index, const Rotation3D& rot )  {
  if( this->rotations.find(index) != this->rotations.end() )  {
    throw std::runtime_error("Rotation with index "+
                             std::to_string(index)+" is already registered.");
  }
  this->rotations[index] = rot;
}

/// Add material to geometry inventory
void dd4hep::geant3_geometry::add_pure_material( int index,
                                                 const std::string& name,
                                                 double /* Z */, double /* A */,
                                                 double density, double intlen, double radlen)  {
  TGeoManager&      mgr = description.manager();
  TGeoElementTable* tab = mgr.GetElementTable();
  TGeoMaterial*     mat = mgr.GetMaterial(name.c_str());
  if( nullptr == mat )  {
    auto* elt = tab->FindElement(name.c_str());
    if( nullptr == elt )  {
      throw std::runtime_error("add_material: Element with name "+name+" and index "+
                               std::to_string(index)+" is UNKNOWN.");
    }
    auto* mix = new TGeoMixture(name.c_str(), 1, density);
    mix->SetRadLen(radlen, intlen);
    mix->AddElement(elt, 1.0);
    std::cout << "add_material: Created pure material with name "<< name
              << " and index " << std::to_string(index) << "." << std::endl;
  }
}

/// Add material to geometry inventory
dd4hep::geant3_geometry::MaterialWrapper
dd4hep::geant3_geometry::add_material( int index, const std::string& name,
                                       double /* Z */, double /* A */,
                                       double density, double intlen, double radlen)  {
  TGeoManager&      mgr = description.manager();
  TGeoElementTable* tab = mgr.GetElementTable();
  TGeoMaterial*     mat = mgr.GetMaterial(name.c_str());
  if( nullptr == mat )  {
    auto* elt = tab->FindElement(name.c_str());
    auto* mix = new TGeoMixture(name.c_str(), 1, density);
    mix->SetRadLen(radlen, intlen);
    if ( elt ) mix->AddElement(elt, 1.0);
    std::cout << "add_material: Created material with name "<< name
              << " and index " << std::to_string(index) << "." << std::endl;
    return { Material(mix) };
  }
  return { {} };
}

/// Add volume medium to geometry inventory
void dd4hep::geant3_geometry::add_medium(   int index,
                                            const std::string& name,
                                            const std::string& material)  {
  auto mat = this->description.material(name);
  if( mat.isValid() )  {
    throw std::runtime_error("Medium with name "+name+" and index "+
                             std::to_string(index)+" is already registered.");
  }
  TGeoManager&  mgr = description.manager();
  TGeoMedium*   med = mgr.GetMedium(name.c_str());
  if ( nullptr == med ) {
    TGeoMaterial* mat = mgr.GetMaterial(material.c_str());
    --unique_mat_id;
    med = new TGeoMedium(name.c_str(), unique_mat_id, mat);
  }
}

/// Add volume medium to geometry inventory
void dd4hep::geant3_geometry::place_volume( int /* index */,
                                            const std::string& mother,
                                            const std::string& volume,
                                            int copy_nr,
                                            const Position& position,
                                            int index_rotation,
                                            const std::vector<double>& params)  {
  Rotation3D rot;
  auto im = this->volumes.find(mother);
  auto iv = this->volumes.find(volume);
  if( im == this->volumes.end() )  {
    throw std::runtime_error("place_volume: No mother volume "+mother+" present.");
  }
  if( iv == this->volumes.end() )  {
    throw std::runtime_error("place_volume: No daughter volume "+volume+" present.");
  }
  if( index_rotation != 0 )  {
    auto ir = this->rotations.find(index_rotation);
    if( ir == this->rotations.end() )  {
      throw std::runtime_error("place_volume: No daughter volume "+volume+" present.");
    }
    rot = ir->second;
  }
  if( params.empty() )  {
  }
  else  {
  }
  Transform3D trafo( rot, position );
  im->second.placeVolume( iv->second, copy_nr, trafo );
}

/// Handle the conversion of the Geant3 materials:
void dd4hep::geant3_geometry::handle_materials()  {
}

/// Handle the conversion of the Geant3 media
void dd4hep::geant3_geometry::handle_media()  {
}

/// Handle the conversion of the Geant3 volumes
void dd4hep::geant3_geometry::handle_volumes()  {
}

/// Handle the conversion of the Geant3 transformation matrices
void dd4hep::geant3_geometry::handle_transformations()  {
}  

/// Handle the conversion of the Geant3 placements
void dd4hep::geant3_geometry::handle_placements()  {
}

