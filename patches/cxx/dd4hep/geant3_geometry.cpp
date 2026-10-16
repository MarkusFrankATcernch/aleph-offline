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

/// DD4hep include files
#include <DD4hep/Printout.h>

/// ROOT include files
#include <TROOT.h>
#include <TRint.h>
#include <TClass.h>
#include <TGeoVolume.h>

/// C/C++ include files
#include <iomanip>
#include <memory>

namespace  {
  static UInt_t unique_mat_id = 0xBFFEFEED;
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
Material geant3_geometry::get_medium(const std::string& name)  const  {
  auto iv = this->material_aliases.find(name);
  if( iv != this->material_aliases.end() )  {
    auto mat = this->description.material(iv->second);
    if( mat.isValid() )  {
      return mat;
    }
    printout(ALWAYS,"geant3_geometry",
             "+++ Undefined dd4hep medium: %s", name.c_str());
  }
  return this->description.material(name);
}

/// Check if material is supposed to be aliased
bool geant3_geometry::have_material_alias(const std::string& name)  const  {
  auto iv = this->material_aliases.find(name);
  return (iv != this->material_aliases.end());
}

/// Add rotation matrix to build placement transformations
void geant3_geometry::add_rotation( int32_t index, const Rotation3D& rot )  {
  if( this->rotations.find(index) != this->rotations.end() )  {
    except("geant3_geometry", "Rotation with index %d is already registered.", index);
  }
  this->rotations[index] = rot;
}

/// Add material to geometry inventory
void geant3_geometry::add_material( int32_t            index,
                                    const std::string& name,
                                    double             Z,
                                    double             A,
                                    double             density,
                                    double             intlen,
                                    double             radlen)
{
  if( !have_material_alias(name) )  {
    TGeoManager&      mgr = description.manager();
    TGeoElementTable* tab = mgr.GetElementTable();
    TGeoMaterial*     mat = mgr.GetMaterial(name.c_str());
    if( nullptr == mat )  {
      auto* elt = tab->FindElement(name.c_str());
      printout(this->debug_materials ? ALWAYS : DEBUG, "geant3_geometry",
               "add_material: Create material (pure) with name %-24s and index %3d: Z=%5.1f A=%5.1f dens:%7.5f g/cm3",
               name.c_str(), index, Z, A, density);
      if( nullptr == elt )  {
        printout(WARNING, "geant3_geometry",
                 "add_material: Warning: Element with name %s and index %d is UNKNOWN.",
                 name.c_str(), index);
      }
      mat = new TGeoMaterial(name.c_str(), A, Z, density, radlen, intlen);
    }
  }
  this->materials[index] = name;
}

/// Add material to geometry inventory
void geant3_geometry::add_mixture( int32_t            index,
                                   const std::string& name,
                                   double             Z,
                                   double             A,
                                   double             density,
                                   double             intlen,
                                   double             radlen,
                                   const std::vector<mixture_entry>& mixture)
{
  if( !have_material_alias(name) )  {
    TGeoManager&      mgr = description.manager();
    TGeoElementTable* tab = mgr.GetElementTable();
    TGeoMaterial*     mat = mgr.GetMaterial(name.c_str());
    if( nullptr == mat )  {
      auto* elt = tab->FindElement(name.c_str());
      auto* mix = new TGeoMixture(name.c_str(), 1, density);
      printout(this->debug_materials ? ALWAYS : DEBUG, "geant3_geometry",
               "add_material: Create material (mixture) with name %s and index %d: Z=%5.1f A=%5.1f dens:%7.5f g/cm3 mix:%ld",
               name.c_str(), index, Z, A, density, mixture.size());
      for( std::size_t i=0; i < mixture.size(); ++i )  {
        const auto& m = mixture[i];
        printout(this->debug_materials ? ALWAYS : DEBUG, "geant3_geometry",
                 "add_material:                 %2d %-24s Z=%5.1f A=%5.1f fraction:%8.4f",
                 i, "", m.Z, m.A, m.fraction);
      }
      mix->SetRadLen(radlen, intlen);
      if( elt ) mix->AddElement(elt, 1.0);
    }
    else  {
      printout(this->debug_materials ? ALWAYS : DEBUG, "geant3_geometry",
               "add_material: Using material (mixture) with name %s and index %d: Z=%5.1f A=%5.1f dens:%7.5f g/cm3 mix:%d",
               mat->GetName(), index, mat->GetZ(), mat->GetA(), mat->GetDensity(), mat->GetNelements() );
    }
  }
  // this->materials[index] = name;
}

/// Add volume medium to geometry inventory
void geant3_geometry::add_medium( int32_t            index,
                                  const std::string& name,
                                  const std::string& material)  {
  TGeoManager& mgr    = description.manager();
  TGeoMedium*  medium = mgr.GetMedium(name.c_str());

  if( !have_material_alias(name) )  {
    if( nullptr == medium )  {
      TGeoMaterial* mat = mgr.GetMaterial(material.c_str());
      if( !mat )  {
        auto m = this->get_medium(material);
        if( m.isValid() ) mat = m->GetMaterial();
      }
      --unique_mat_id;
      medium = new TGeoMedium(name.c_str(), unique_mat_id, mat);
      medium->SetTitle("material");
      medium->SetUniqueID(unique_mat_id);
      printout(this->debug_materials ? ALWAYS : DEBUG, "geant3_geometry",
               "add_material: Created medium with name: %s index %d material: %s [%s].",
               name.c_str(), index, material.c_str(), mat->GetName() );
    }
  }
  else  {
    medium = this->get_medium(name).ptr();
  }
  this->media[index] = medium;
}

/// Add volume to geometry inventory
void geant3_geometry::add_volume(    int32_t            index,
                                     const std::string& name,
                                     const std::string& medium,
                                     Solid              solid,
                                     const std::string& option)
{
  if( this->divisions.find(name) != this->divisions.end() )  {
    return;
  }
  if( this->volumes.find(name) != this->volumes.end() )  {
    except("geant3_geometry", "+++ Volume %s with index %d is already registered.",
           name.c_str(), index);
  }
  VisAttr  vis = this->description.visAttributes("Vis_"+name);
  Material mat = this->get_medium(medium);
  Volume   vol(name, solid, mat);

  if( !option.empty() )  {
  }
  
  if( have_debug(name[0]) || this->debug_volumes )  {
    std::string vis_name = vis.isValid() ? vis.name() : "????";
    std::string med_name = (medium != mat.name()) ? "[alias: "+std::string(mat.name())+"]" : std::string("");
    std::string mat_name = mat->GetMaterial() ? mat->GetMaterial()->GetName() : "??";
    std::string sol_isa  = solid->IsA()->GetName();
    printout(ALWAYS, "geant3_geometry",
             "+++ Volume: %-10s vis: %-12s medium: %-16s material: %-16s %s [%s]",
             name.c_str(), vis_name.c_str(), medium.c_str(), mat_name.c_str(),
             med_name.c_str(), sol_isa.c_str() );
  }
  if( vis.isValid() )  {
    vol.setVisAttributes(vis);
  }
  this->volumes[name] = vol;
}

/// Add volume division to geometry inventory
void geant3_geometry::add_division( const std::string& name,
                                    const std::string& volume,
                                    int32_t            axis,
                                    int32_t            ndiv,
                                    double             start,
                                    double             step,
                                    int32_t            idx_medium,
                                    const char*        opt)
{
  bool dbg = have_debug( name[0] ) || have_debug( volume[0] ) || this->debug_divisions;
  if( this->processing_step < PROCESSING_DIVISIONS )  {
    this->divisions[name] = nullptr;
    return;
  }
  auto iv = this->volumes.find(name);
  if( iv != this->volumes.end() )  {
    except( "geant3_geometry","add_division: daughter volume "+name+" ALREADY present." );
  }
  iv = this->volumes.find(volume);
  if( iv == this->volumes.end() )  {
    except( "geant3_geometry","add_division: volume "+volume+" NOT present." );
  }
  Volume vol(iv->second);
  if( idx_medium != 0 )  {
    auto ip = this->media.find( idx_medium );
    if( ip != this->media.end() )  {
      idx_medium = ip->second->GetId();
    }
  }
  auto deg = units::degree;
  VisAttr vis = this->description.visAttributes( "Vis_"+name );
  Volume  div = vol.divide( name, axis, ndiv, start/deg, step/deg, idx_medium, opt );
  if( vis.isValid() )  {
    div.setVisAttributes( vis );
  }
  printout( dbg ? ALWAYS : DEBUG, "geant3_geometry",
            "+++ add_division: %s --> %d  axis: %d ndiv: %d start:%f step:%f medium:%d opt:%s",
            volume.c_str(), name.c_str(), axis, ndiv, start, step, idx_medium, opt);
  this->volumes[name] = div;
}

/// Add volume medium to geometry inventory
void geant3_geometry::add_placement( int                index,
                                     const std::string& mother,
                                     const std::string& volume,
                                     int                copy_nr,
                                     const Position&    position,
                                     int                index_rotation,
                                     const params_t&    params,
                                     const std::string& option)
{
  Rotation3D  rot;
  std::string volnam = volume;
  auto ip = this->placements.find(index);
  auto im = this->volumes.find(mother);
  auto iv = this->volumes.find(volume);
  bool dbg = have_debug(volume[0]) || have_debug(mother[0]) || this->debug_placements;

  if( ip != this->placements.end() )  {
    except("geant3_geometry","place_volume: Placed volume %d already registered.", index);
  }
  if( im == this->volumes.end() )  {
    except("geant3_geometry","place_volume: No mother volume "+mother+" present.");
  }
  if( iv == this->volumes.end() )  {
    except("geant3_geometry","place_volume: No daughter volume "+volume+" present.");
  }
  if( index_rotation != 0 )  {
    auto ir = this->rotations.find(index_rotation);
    if( ir == this->rotations.end() )  {
      except("geant3_geometry","place_volume: No rotation for volume "+volume+" im mother "+mother);
    }
    rot = ir->second;
  }
  /// How does one handle boolean solids ?
  if( !option.empty() )  {
    /**
       • UNION: the union of two volumes B and C is obtained positioning two overlapping ‘MANY’ daughters
       B and C inside the same mother A. To identify the result of the union as a single volume is enough to
       create a SET associated to the volumes B and C.
       • SUBTRACTION: the subtraction of a volume B from a volume C is obtained positioning B as ‘ONLY’
       and C as a ‘MANY’ overlapping B. The result of the subtraction is automatically what the tracking
       sees as the C volume, so GSPOS is the only user interface needed. The volume B can have the same
       material as the mother A or not. The normal positioning technique used for ‘ONLYs’ is a particular
       case of boolean subtraction (a volume is contained inside the other).
       • INTERSECTION: the intersection of two volumes B and C is obtained positioning a protuding ‘MANY’
       object C in a ‘MANY’ object B which is normally placed in a ‘ONLY’ mother A. A and B must have the
       same material, while C has the material asked for the intersection. The intersection is given by the
       part of C which is not protuding from B, therefore by what the new tracking sees as the volume C:
       again the only needed user interface is GSPOS. If not interacting with other daughters of A, B could
       also be ‘ONLY’. Without any ambiguity, even further ‘MANY’ daughters of A can overlap the protuding
       part of C outside B, because it is really invisible to the new tracking.
    */
  }
  Volume   vol = iv->second;
  Material mat = vol.material();
  VisAttr  vis = this->description.visAttributes("Vis_"+volume);
  if( !params.empty() )  {
    Solid    new_solid = this->create_solid( vol.solid()->IsA(), params);
    if( !new_solid.isValid() )  {
      printout(WARNING, "geant3_geometry",
               "+++ add_placement: Unhandled shape placement with parameters: %s of type %s",
               volume.c_str(), new_solid->IsA()->GetName() );
    }
    vol = Volume(volnam, new_solid, mat);
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
  this->placements[index] = pv;
  printout(dbg ? ALWAYS : DEBUG, "geant3_geometry",
           "+++ add_placement: %-12s volume: %-10s mother: %-12s material:%-24s %s%s [%s]",
           pv.name(), volnam.c_str(), mother.c_str(), mat.name(),
           vis.isValid() ? "vis: " : "", vis.isValid() ? vis.name() : "",
           vol.solid()->IsA()->GetName() );
}

/// Create copy of an existing solid with new parameters
Solid geant3_geometry::create_solid( const TClass* typ, const params_t& p)  {
  Solid  solid;
  double cm  = units::cm;
  double deg = units::degree;

  if( typ == TGeoTubeSeg::Class() && p.size() == 3 )  {
    solid = Tube( p[0]*cm, p[1]*cm, p[2]*cm );
  }
  else if( typ == TGeoTubeSeg::Class() && p.size() == 5 )  {
    solid = Tube( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*deg, p[4]*deg );
  }
  else if( typ == TGeoCone::Class() )  {
    solid = Cone( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm, p[4]*cm );
  }
  else if( typ == TGeoConeSeg::Class() )  {
    solid = ConeSegment( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm, p[4]*cm, p[5]*deg, p[6]*deg );
  }
  else if( typ == TGeoHype::Class() )  {
    solid = Hyperboloid( p[0]*cm, p[3]*degree, p[1]*cm, p[3]*deg, p[2]*cm );
  }
  else if( typ == TGeoSphere::Class() )  {
    solid = Sphere( p[0]*cm, p[1]*cm, p[2]*deg, p[3]*deg, p[4]*deg, p[5]*deg );
  }                
  else if( typ == TGeoTrd1::Class() )  {
    solid = Trd1( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm );
  }
  else if( typ == TGeoTrd2::Class() )  {
    solid = Trd2( p[0]*cm, p[1]*cm, p[2]*cm, p[3]*cm, p[4]*cm );
  }
  else if( typ == TGeoTrap::Class() )  {
    solid = Trap( p[0]*cm, p[1]*deg, p[2]*deg, p[3]*deg, p[4]*cm, p[5]*cm,
                  p[6]*deg, p[7]*deg, p[8]*cm, p[9]*cm, p[10]*deg );
  }
  else if( typ == TGeoPara::Class() )  {
    solid = Solid(new TGeoPara( "para", p[0]*cm, p[1]*cm, p[2]*cm, p[3], p[4], p[5] ));
  }
  else if( typ == TGeoPcon::Class() )  {
    std::vector<double> dz, rmin, rmax;
    int nz = int(p[2]);
    for( int i=0; i<nz; ++i )  {
      dz.push_back(   p[3+i*3]*cm );
      rmin.push_back( p[4+i*3]*cm );
      rmax.push_back( p[5+i*3]*cm );
    }
    solid = Polycone(p[0]*deg, p[1]*deg, rmin, rmax, dz);
  }
  else if( typ == TGeoPgon::Class() )  {
    std::vector<double> dz, rmin, rmax;
    int nz = int(p[2]);
    for( int i=0; i<nz; ++i )  {
      dz.push_back(   p[3+i*3]*cm );
      rmin.push_back( p[4+i*3]*cm );
      rmax.push_back( p[5+i*3]*cm );
    }
    solid = Polyhedra(nz, p[0]*deg, p[1]*deg, dz, rmin, rmax );
  }
  else if( typ == TGeoBBox::Class() )  {
    solid = Box( p[0]*cm, p[1]*cm, p[2]*cm );
  }
  return solid;
}

/// Get the DetElement parent from the path
DetElement geant3_geometry::get_detelement_parent(const std::string& path)  const  {
  if( path.empty() )   {
    return this->description.world();
  }
  return this->description.world();
}

/// Add DetElement to structure tree
void geant3_geometry::add_detelement(const det_def& def, PlacedVolume pv)  const {
  DetElement parent = this->get_detelement_parent(def.parent);
  DetElement detector( parent, def.name, def.id );
  if( !def.volid.empty() )  {
    detector->volumeID = def.id;
  }
  detector.setPlacement( pv );
  if( !def.volid.empty() )  {
    pv.addPhysVolID( def.volid, def.id );
  }
  SensitiveDetector sd;
  if( !def.sensitive.empty() )  {
    sd = SensitiveDetector( def.name );
    this->description.addSensitiveDetector( sd );
    sd.setType( def.sensitive );
  }
  printout( this->debug_structure ? ALWAYS : DEBUG, "geant3_geometry",
            "+++ Assign DetElement '%s'. sensitive:%s placement:%s",
            detector.path().c_str(), sd.isValid() ? sd.name() : "----",
            detector.placementPath().c_str() );
}

/// Set volume sensitive
void geant3_geometry::set_sensitive( const std::string& sens_det, PlacedVolume pv )  const  {
  Volume vol = pv.volume();
  if( !vol.sensitiveDetector().isValid() )  {
    SensitiveDetector sd = this->description.sensitiveDetector(sens_det);
    if( sd.isValid() )  {
      vol.setSensitiveDetector(sd);
      printout( this->debug_sensitive ? ALWAYS : DEBUG, "geant3_geometry",
                "+++ Assign sensitive detector '%s' to volume %s'",
                sd.name(), vol.name() );
    }
  }
}

/// Enhance PlaceVolume
void geant3_geometry::enhance(const std::string& path, PlacedVolume node)  const {
  Volume      vol  = node.volume();
  std::string vnam = vol.name();

  auto iv = this->detelement_volumes.find(vnam);
  if( iv != this->detelement_volumes.end() )
    this->add_detelement( iv->second, node );
  auto ip = this->detelement_placements.find(path);      
  if( ip != this->detelement_placements.end() )
    this->add_detelement( ip->second, node );
  auto is = this->sensitive_volumes.find(vnam);
  if( is != this->sensitive_volumes.end() )
    this->set_sensitive( is->second, node );
}

/// Initializing constructor
geant3_geometry_imp::geant3_geometry_imp(Detector& desc)
  : geant3_geometry(desc)
{
}

/// Default destructor
geant3_geometry_imp::~geant3_geometry_imp()  {
}

//==========================================================================
namespace alpha  {

  void instantiate_dd4hep_geometry(const std::string& compact)  {
    std::pair<int, char**> arg(0, 0);
    dd4hep::Detector& desc = dd4hep::Detector::getInstance();

    desc.fromXML(compact);
    std::cout << "Create ROOT interpreter." << std::endl;
    gApplication = new TRint("DD4hepRint", &arg.first, arg.second);

    const char* dis_argv[2] = {"-level", "20" };
    desc.apply("DD4hep_GeometryDisplay", sizeof(dis_argv)/sizeof(dis_argv[0]), (char**)dis_argv);
    desc.apply("DD4hep_Rint", 0, nullptr);
  }
}
//==========================================================================
