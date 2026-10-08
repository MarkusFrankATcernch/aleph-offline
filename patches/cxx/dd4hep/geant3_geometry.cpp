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
#include <dd4hep/geant3_geometry.h>

#include <DD4hep/Objects.h>
#include <DD4hep/Detector.h>
#include <DD4hep/Printout.h>

#include <TROOT.h>
#include <TRint.h>
#include <TClass.h>
#include <TGeoSystemOfUnits.h>
#include <TGeant4SystemOfUnits.h>

#include <iomanip>

namespace  {
  static UInt_t unique_mat_id = 0xBFFEFEED;
  static char s_debug_detector = 'L';
}

using namespace dd4hep;

/// Default constructor
geant3_geometry::geant3_geometry(Detector& desc)
  : description(desc)
{
}

/// Default destructor
geant3_geometry::~geant3_geometry()
{
}

/// Access material to build volumes
Material geant3_geometry::get_material(const std::string& name)  const  {
  return this->description.material(name);
}

/// Add rotation matrix to build placement transformations
void geant3_geometry::add_rotation( int index, const Rotation3D& rot )  {
  if( this->rotations.find(index) != this->rotations.end() )  {
    except("geant3_geometry", "Rotation with index %d is already registered.", index);
  }
  this->rotations[index] = rot;
}

/// Add material to geometry inventory
void
geant3_geometry::add_material( int index,
                                                 const std::string& name,
                                                 double Z, double A,
                                                 double density, double intlen, double radlen)
{
  TGeoManager&      mgr = description.manager();
  TGeoElementTable* tab = mgr.GetElementTable();
  TGeoMaterial*     mat = mgr.GetMaterial(name.c_str());
  if( nullptr == mat )  {
    auto* elt = tab->FindElement(name.c_str());
    if( this->debug_materials )  {
      printout(ALWAYS, "geant3_geometry",
               "add_material: Create material (pure) with name %d and index %d.",
               name.c_str(), index);
    }
    if( nullptr == elt )  {
      printout(WARNING, "geant3_geometry",
               "add_material: Warning: Element with name %d and index %d is UNKNOWN.",
               name.c_str(), index);
    }
    mat = new TGeoMaterial(name.c_str(), A, Z, density, radlen, intlen);
  }
}

/// Add material to geometry inventory
void
geant3_geometry::add_mixture( int index, const std::string& name,
                                                double /* Z */, double /* A */,
                                                double density, double intlen, double radlen,
                                                const std::vector<mixture_entry>& /* mix */)
{
  TGeoManager&      mgr = description.manager();
  TGeoElementTable* tab = mgr.GetElementTable();
  TGeoMaterial*     mat = mgr.GetMaterial(name.c_str());
  if( nullptr == mat )  {
    auto* elt = tab->FindElement(name.c_str());
    auto* mix = new TGeoMixture(name.c_str(), 1, density);
    if( this->debug_materials )  {
      printout(ALWAYS, "geant3_geometry",
               "add_material: Create material (mixture) with name %d and index %d.",
               name.c_str(), index);
    }
    mix->SetRadLen(radlen, intlen);
    if ( elt ) mix->AddElement(elt, 1.0);
  }
}

/// Add volume medium to geometry inventory
Material
geant3_geometry::add_medium( int index,
                                               const std::string& name,
                                               const std::string& material)  {
  TGeoManager& mgr    = description.manager();
  TGeoMedium*  medium = mgr.GetMedium(name.c_str());
  if ( nullptr == medium ) {
    TGeoMaterial* mat = mgr.GetMaterial(material.c_str());
    --unique_mat_id;
    medium = new TGeoMedium(name.c_str(), unique_mat_id, mat);
    medium->SetTitle("material");
    medium->SetUniqueID(unique_mat_id);
    if( this->debug_materials )  {
      printout(ALWAYS, "geant3_geometry",
               "add_material: Created medium with name: %s and index %d.",
               name.c_str(), index);
    }
  }
  return { medium };
}

/// Add volume to geometry inventory
Volume
geant3_geometry::add_volume( int index,
                                               const std::string& name,
                                               const std::string& medium,
                                               Solid solid )
{
  if( this->volumes.find(name) != this->volumes.end() )  {
    except("geant3_geometry", "+++ Volume %s with index %d is already registered.",
           name.c_str(), index);
  }
  VisAttr  vis = this->description.visAttributes("Vis_"+name);
  Material mat = this->description.material(medium);
  Volume   vol(name, solid, mat);

  if( name[0] == s_debug_detector )  {
    std::string vis_name = vis.isValid() ? vis.name() : "????";
    printout(ALWAYS, "geant3_geometry",
             "+++ Volume: %-10s medium: %-16s material: %-16s vis: %-12s  [%s]",
             name.c_str(), medium.c_str(), mat->GetMaterial()->GetName(), vis_name.c_str(),
             vol.solid()->IsA()->GetName() );
  }
  if( vis.isValid() )  {
    vol.setVisAttributes(vis);
  }
  this->volumes[name] = vol;
  return vol;
}

/// Add volume medium to geometry inventory
PlacedVolume
geant3_geometry::add_placement( int /* index */,
                                                  const std::string& mother,
                                                  const std::string& volume,
                                                  int copy_nr,
                                                  const Position& position,
                                                  int index_rotation,
                                                  const std::vector<double>& params)
{
  Rotation3D rot;
  std::string volnam = volume;
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
  Volume vol = iv->second;
  if( !params.empty() )  {
    Solid   new_solid;
    double  cm = units::cm;
    double  deg = units::degree;
    const auto& p = params;
    std::vector<double> new_par;
    Solid    solid = vol.solid();
    Material material = vol.material();

    if( solid->IsA() == TGeoTubeSeg::Class() && p.size() == 3 )  {
      new_solid = Tube( p[0]*cm, p[1]*cm, p[2]*cm );
    }
    else if( solid->IsA() == TGeoTubeSeg::Class() && p.size() == 5 )  {
      new_solid = Tube( params[0]*cm, p[1]*cm, p[2]*cm, p[3]*deg, p[4]*deg );
    }
    else if( solid->IsA() == TGeoCone::Class() )  {
      new_solid = Cone( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm, p[4]*cm );
    }
    else if( solid->IsA() == TGeoConeSeg::Class() )  {
      new_solid = ConeSegment( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm, p[4]*cm, p[5]*deg, p[6]*deg );
    }
    else if( solid->IsA() == TGeoHype::Class() )  {
      new_solid = Hyperboloid( p[0]*cm, p[3]*degree, p[1]*cm, p[3]*deg, p[2]*cm );
    }
    else if( solid->IsA() == TGeoSphere::Class() )  {
      new_solid = Sphere( p[0]*cm, p[1]*cm, p[2]*deg, p[3]*deg, p[4]*deg, p[5]*deg );
    }                
    else if( solid->IsA() == TGeoTrd1::Class() )  {
      new_solid = Trd1( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm );
    }
    else if( solid->IsA() == TGeoTrd2::Class() )  {
      new_solid = Trd2( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm, p[4]*cm );
    }
    else if( solid->IsA() == TGeoTrap::Class() )  {
      new_solid = Trap( p[0]*cm, p[1]*deg, p[2]*deg, p[3]*deg, p[4]*cm, p[5]*cm,
                        p[6]*deg, p[7]*deg, p[8]*cm, p[9]*cm, p[10]*deg );
    }
    else if( solid->IsA() == TGeoPara::Class() )  {
      new_solid = Solid(new TGeoPara( "para", p[0]*cm, p[1]*cm, p[2]*cm, p[3], p[4], p[5] ));
    }
    else if( solid->IsA() == TGeoPcon::Class() )  {
      std::vector<double> dz, rmin, rmax;
      int nz = int(p[2]);
      for( int i=0; i<nz; ++i )  {
        dz.push_back(    p[3+i*3]*cm );
        rmin.push_back( p[4+i*3]*cm );
        rmax.push_back( p[5+i*3]*cm );
      }
      new_solid = Polycone(p[0]*deg, p[1]*deg, rmin, rmax, dz);
    }
    else if( solid->IsA() == TGeoPgon::Class() )  {
      std::vector<double> dz, rmin, rmax;
      int nz = int(p[2]);
      for( int i=0; i<nz; ++i )  {
        dz.push_back(    p[3+i*3]*cm );
        rmin.push_back( p[4+i*3]*cm );
        rmax.push_back( p[5+i*3]*cm );
      }
      new_solid = Polyhedra(nz, p[0]*deg, p[1]*deg, dz, rmin, rmax );
    }
    else if( solid->IsA() == TGeoBBox::Class() )  {
      new_solid = Box( p[0]*cm, p[1]*cm, p[2]*cm );
    }
    else  {
      printout(WARNING, "geant3_geometry",
               "+++ add_placement: Unhandled shape placement with parameters: %s of type %s",
               volume.c_str(), solid->IsA()->GetName() );
      return {};
    }
    vol = Volume(volnam, new_solid, material);
    VisAttr vis = this->description.visAttributes("Vis_"+volume);
    if( vis.isValid() )  {
      vol.setVisAttributes(vis);
    }
    volnam = volume+" ("+std::to_string(copy_nr)+")";
  }
#if defined(USE_TGEOHMATRIX)
  TGeoHMatrix *trafo = new TGeoHMatrix();
  double rot_data[9];
  rot.GetComponents(rot_data, rot_data+9);
  trafo->SetDx(position.x());
  trafo->SetDy(position.y());
  trafo->SetDz(position.z());
  trafo->SetRotation(rot_data);
#else
  Transform3D trafo( rot, position );
#endif
  PlacedVolume pv = im->second.placeVolume( vol, copy_nr, trafo );
  if( this->debug_placements || volume[0] == s_debug_detector )  {
    std::cout << "+++ add_placement:"
              << " place: "  << std::setw(24) << std::left << pv.name()
              << " volume: " << std::setw(10) << std::left << volnam
              << " mother: " << std::setw(24) << std::left << mother
              << " [" << vol.solid()->IsA()->GetName() << "]"
              << std::endl;
  }
  return pv;
}

geant3_geometry_imp::geant3_geometry_imp(Detector& desc)
  : geant3_geometry(desc)
{
}

geant3_geometry_imp::~geant3_geometry_imp()  {
}

namespace alpha  {
  void instantiate_dd4hep_geometry(const std::string& compact)  {
    std::pair<int, char**> arg(0, 0);
    Detector& desc = Detector::getInstance();

    desc.fromXML(compact);
    std::cout << "Create ROOT interpreter." << std::endl;
    gApplication = new TRint("DD4hepRint", &arg.first, arg.second);

    const char* dis_argv[2] = {"-level", "20" };
    desc.apply("DD4hep_GeometryDisplay", sizeof(dis_argv)/sizeof(dis_argv[0]), (char**)dis_argv);
    desc.apply("DD4hep_Rint", 0, nullptr);
  }
}
#include "DD4hep/DetFactoryHelper.h"

static Ref_t create_element(Detector& description, xml_h e, Ref_t /* sens */)  {
  geant3_geometry_imp geo(description);
  xml_det_t  x_det = e;
  xml_comp_t volume (x_det.child(_U(volume)));

  geo.debug_materials = true;
  geo.handle_materials();
  geo.handle_media();
  geo.handle_rotations();
  geo.handle_volumes();
  geo.handle_placements();
  auto im = geo.volumes.find(volume.nameStr());
  if( im != geo.volumes.end() )  {
    Volume     vol = im->second;
    DetElement det( x_det.nameStr(), x_det.id() );
    Volume     mother = description.pickMotherVolume(det);

    geo.description.manager().SetTopVolume(vol.ptr());
    geo.description.init();
    geo.description.endDocument("close");
    PlacedVolume phv = mother.placeVolume(vol);
    vol.setVisAttributes(description, x_det.visStr());
    vol.setLimitSet(description, x_det.limitsStr());
    vol.setRegion(description, x_det.regionStr());
    phv.addPhysVolID("system", x_det.id());
    det.setPlacement(phv);
    return det;
  }
  except("geant3_geometry",
         "Invalid world volume for this detector: %s",
         volume.nameStr().c_str());
  return {};
}

// first argument is the type from the xml file
DECLARE_DETELEMENT(DD4hep_Geant3Detector,create_element)
