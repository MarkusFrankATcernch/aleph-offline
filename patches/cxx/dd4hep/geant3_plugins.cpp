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
#include <DD4hep/DetFactoryHelper.h>
#include <XML/Conversions.h>

/// C/C++ include files
#include <iomanip>
#include <memory>

//==========================================================================
#include <TClass.h>
typedef TGeoNode                geo_node_t;
typedef TGeoVolume              geo_volume_t;
typedef TGeoVolumeAssembly      geo_assembly_t;

/// dd4hep namespace declaration
namespace dd4hep  {
  
  class Volume__Import   {
  public:
    void setShapeTitle(TGeoVolume* vol)   {
      if ( vol )   {
        TGeoShape* sh = vol->GetShape();
        std::string tag = get_shape_tag(sh);
        sh->SetTitle(tag.c_str());
      }
    }

  public:
    /// Callback for simple imports. Simple add a user extension
    void operator()(PlacedVolume pv)   {
      if( nullptr == pv->GetUserExtension() )  {
        pv->SetUserExtension( new PlacedVolume::Object() );
        std::cout << "2) Set user extension: " << pv->GetName() << std::endl;
      }
      (*this)( pv.volume().ptr() );
    }
    /// Callback for simple imports. Simple add a user extension
    void operator()(TGeoVolume* v)   {
      TClass* c = v->IsA();
      if ( !v->GetUserExtension() )   {
        if ( c == geo_volume_t::Class() )
          v->SetUserExtension(new Volume::Object());
        else if  ( c == geo_assembly_t::Class() )
          v->SetUserExtension(new Assembly::Object());
        else if  ( c == TGeoVolumeMulti::Class() )
          v->SetUserExtension(new VolumeMulti::Object());
        else
          except("dd4hep","VolumeImport: Unknown TGeoVolume sub-class: %s",v->IsA()->GetName());
      }
      if  ( c == TGeoVolumeMulti::Class() )  {
        TGeoVolumeMulti* mv = (TGeoVolumeMulti*)v;
        for( int i=0, n=mv->GetNvolumes(); i<n; ++i )   {
          TGeoVolume* vol = mv->GetVolume(i);
          this->setShapeTitle(vol);
          (*this)(vol);
        }
      }
      for( Int_t i=0; i<v->GetNdaughters(); ++i )  {
        geo_node_t* pv = v->GetNode(i);
        if ( !pv->GetUserExtension() )
          pv->geo_node_t::SetUserExtension(new PlacedVolume::Object());
        (*this)(pv->GetVolume());
      }
    }
    /// Callback for clone imports, where the user extension should be copied
    void operator()(TGeoVolume* new_v, TGeoVolume* old_v, SensitiveDetector sd, int set_bit)   {
      if ( !new_v || !old_v )  {
        except("dd4hep","VolumeImport: ERROR: The refected volume is INVALID!");        
      }
      else if ( !old_v->GetUserExtension() )   {
        except("dd4hep","VolumeImport: ERROR: Reflection of non-dd4hep volume %s",new_v->IsA()->GetName());
      }
      else if ( !new_v->GetUserExtension() )   {
        Volume old_vol(old_v);
        Volume new_vol(new_v);
        if ( sd.isValid() && old_vol.isSensitive() )   {
          new_vol.setSensitiveDetector(sd);
        }
        std::cout << "Should not enter here! " << std::endl;
#if 0
        if ( c == geo_volume_t::Class() )  {
          Volume::Object *new_e, *old_e = (Volume::Object*)_data(old_vol);
          old_e->reflected = new_v;
          new_v->SetUserExtension(new_e = new Volume::Object(*old_e)); 
          new_e->reflected = old_v;
        }
        else if  ( c == geo_assembly_t::Class() )  {
          Assembly::Object *new_e, *old_e = (Assembly::Object*)_data(old_vol);
          old_e->reflected = new_v;
          new_v->SetUserExtension(new_e = new Assembly::Object(*old_e));
          new_e->reflected = old_v;
        }
        else if  ( c == TGeoVolumeMulti::Class() )   {
          VolumeMulti::Object *new_e, *old_e = (VolumeMulti::Object*)_data(old_vol);
          TGeoVolumeMulti* new_mv = (TGeoVolumeMulti*)new_v;
          TGeoVolumeMulti* old_mv = (TGeoVolumeMulti*)old_v;
          new_v->SetUserExtension(new_e = new VolumeMulti::Object(*old_e));
          old_e->reflected = new_v;
          new_e->reflected = old_v;
          for(int i=0, n=new_mv->GetNvolumes(); i<n; ++i)  {
            TGeoVolume* vol = new_mv->GetVolume(i);
            this->setShapeTitle(vol);
            (*this)(vol, old_mv->GetVolume(i), sd, set_bit);
          }
        }
        else
          except("dd4hep","VolumeImport: Unknown TGeoVolume sub-class: %s",new_v->IsA()->GetName());

        new_vol.setSensitiveDetector(old_vol.sensitiveDetector());
        new_vol.setVisAttributes(old_vol.visAttributes());
        new_vol.setLimitSet(old_vol.limitSet());
        new_vol.setRegion(old_vol.region());

        if ( set_bit >= 0 ) new_vol.setFlagBit(set_bit);
        for(Int_t i=0; i<new_v->GetNdaughters(); ++i)  {
          geo_node_t* pv = new_v->GetNode(i);
          geo_node_t* ov = old_v->GetNode(i);
          if ( !pv->GetUserExtension() )   {
            auto* e = (PlacedVolume::Object*)ov->geo_node_t::GetUserExtension();
            pv->geo_node_t::SetUserExtension(new PlacedVolume::Object(*e));
          }
          (*this)(pv->GetVolume(), ov->GetVolume(), sd, set_bit);
        }
#endif
      }
    }
  };


  /// tool sub-namespace declaration
  namespace tools  {
    
    struct placement_scan  {
      const std::string& _place;
      /// Default constructor
      placement_scan(const std::string& pl) : _place(pl)  {
      }
      std::pair<std::string, PlacedVolume>
      scan( std::string p, PlacedVolume node )  const {
        std::string path = std::move(p) + "/" + node.name();
        if( path == this->_place ) return { path, node };
        for( Int_t idau = 0, ndau = node->GetNdaughters(); idau < ndau; ++idau )  {
          auto ret = this->scan( path, node->GetDaughter(idau) );
          if( ret.second.isValid() ) return ret;
        }
        return { {}, {} };
      }
    };
    struct volume_scan  {
      const std::string& _volume;
      /// Default constructor
      volume_scan(const std::string& v) : _volume(v)  { }
      std::pair<std::string, PlacedVolume>
      scan( std::string p, PlacedVolume node )  const {
        std::string path = std::move(p) + "/" + node.name();
        if( node.volume().name() == this->_volume ) return { path, node };
        for( Int_t idau = 0, ndau = node->GetNdaughters(); idau < ndau; ++idau )  {
          auto ret = this->scan( path, node->GetDaughter(idau) );
          if( ret.second.isValid() ) return ret;
        }
        return { {}, {} };
      }
    };
    struct volume_collect  {
      const std::string& _volume;
      std::map<std::string, PlacedVolume> placements;
      /// Default constructor
      volume_collect(const std::string& v) : _volume(v)  { }
      void collect( std::string p, PlacedVolume node ) {
        std::string path = std::move(p) + "/" + node.name();
        if( node.volume().name() == this->_volume )
          this->placements.insert( { path, node } );
        for( Int_t idau = 0, ndau = node->GetNdaughters(); idau < ndau; ++idau )
          this->collect( path, { node->GetDaughter(idau) } );
      }
    };
  }
}

//==========================================================================
namespace  {
  dd4hep::geant3_geometry& geometry( dd4hep::Detector& dsc )  {
    dd4hep::geant3_geometry* ext = dsc.extension<dd4hep::geant3_geometry>(false);
    if( nullptr == ext )  {
      auto* e = new dd4hep::geant3_geometry_imp(dsc);
      dsc.addExtension<dd4hep::geant3_geometry>(e);
      ext = e;
    }
    return *ext;
  }
}

//==========================================================================
static dd4hep::Ref_t
geant3_detector_create( dd4hep::Detector& description, xml_h e, dd4hep::Ref_t /* sens */ )  {
  using namespace dd4hep;
  geant3_geometry& geo = geometry( description );
  xml_det_t      x_det = e;
  std::string   volume = x_det.attr<std::string>(_U(volume));

  geo.handle_divisions();
  geo.processing_step = geant3_geometry::PROCESSING_MATERIALS;
  geo.resolve_materials = 0;
  geo.handle_materials();
  geo.resolve_materials = 1;
  geo.handle_materials();
  geo.resolve_materials = 2;
  geo.handle_materials();
  geo.handle_media();
  geo.processing_step = geant3_geometry::PROCESSING_MATRICES;
  geo.handle_rotations();
  geo.processing_step = geant3_geometry::PROCESSING_VOLUMES;
  geo.handle_volumes();
  geo.processing_step = geant3_geometry::PROCESSING_DIVISIONS;
  geo.handle_divisions();
  geo.processing_step = geant3_geometry::PROCESSING_PLACEMENTS;
  geo.handle_placements();
  geo.processing_step = geant3_geometry::PROCESSING_FINISHED;
  
  auto im = geo.volumes.find( volume );
  if( im != geo.volumes.end() )  {
    Volume     vol = im->second;
    DetElement det( x_det.nameStr(), x_det.id() );
    Volume     mother = description.pickMotherVolume( det );

    //geo.description.manager().SetTopVolume( vol.ptr() );
    //geo.description.init();
    //geo.description.endDocument( "close" );
    PlacedVolume phv = mother.placeVolume( vol, 1 );
    vol.setVisAttributes( description, x_det.visStr() );
    vol.setLimitSet( description, x_det.limitsStr() );
    vol.setRegion( description, x_det.regionStr() );
    det.setPlacement( phv );
    return det;
  }
  except( "geant3_geometry",
          "Invalid world volume for this detector: %s",
          volume.c_str() );
  return {};
}

// first argument is the type from the xml file
DECLARE_DETELEMENT(DD4hep_Geant3Detector,geant3_detector_create)

//==========================================================================
/// dd4hep namespace declaration
namespace dd4hep {

  class geant3_subdetector  {
  private:
    dd4hep::Detector&         description;
    xml_det_t                 xd;
    dd4hep::SensitiveDetector sensitive {      };
    std::string               name      {      };
    int                       id        {   -1 };
    PrintLevel                print     { ALWAYS };
    void _set_volid( PlacedVolume pv, const std::string& volid, int vid )  {
      if( nullptr == pv->GetUserExtension() )   {
        Volume__Import imp;
        imp(pv);
      }
      const auto& ids = pv.volIDs();
      for( const auto& id : ids )  {
        if( id.first == volid )  {
          if( vid == id.second )  {
            return;
          }
          printout(ERROR, "geant3_subdetector",
                   "+++ Invalid VOLID: %s %d <> %d", volid.c_str(), id.second, vid);
        }
      }
      pv.addPhysVolID( volid, vid );
    }
    
  public:
    /// Default constructor
    geant3_subdetector( Detector& dsc, xml_h e, dd4hep::SensitiveDetector sd )
      : description(dsc), xd(e), sensitive(sd)      {
      if( xd.hasAttr(_U(debug)) )  {
        int value = xd.attr<int>(_U(debug));
        this->print = PrintLevel(value);
      }
    }
    /// Set properties for all volumes
    void set_attr(Volume volume, xml_det_t e)  {
      if( e.hasAttr(_U(vis)) )  {
        auto vis = e.attr<std::string>(_U(vis));
        volume.setVisAttributes( this->description, vis );
      }
      if( e.hasAttr(_U(limits)) )  {
        auto lim = e.attr<std::string>(_U(limits));
        volume.setLimitSet( this->description, lim );
      }
      if(  e.hasAttr(_U(region)) )  {
        auto reg = e.attr<std::string>(_U(region));
        volume.setRegion( this->description, reg );
      }
      if( this->sensitive.isValid() && xd.ptr() != e.ptr() && e.hasAttr(_U(sensitive)) )  {
        volume.setSensitiveDetector( this->sensitive );
      }
    }
    /// Construct subdetector from Geant3 geometry
    DetElement construct()  {
      std::pair<std::string, PlacedVolume> ret;
      
      this->id   = this->xd.id();
      this->name = this->xd.nameStr();
      if( this->xd.hasAttr(_U(placement)) )  {
        auto place = this->xd.attr<std::string>(_U(placement));
        ret = tools::placement_scan(place).scan( {}, this->description.world().placement() );
        if( !ret.second.isValid() )  {
          except("geant3_subdetector", "+++ %s [id:%d]: Failed to find top level placement: '%s'",
                 this->name.c_str(), this->id, place.c_str() );
        }
      }
      else if( this->xd.hasAttr(_U(volume)) )  {
        auto vol = this->xd.attr<std::string>(_U(volume));
        ret = tools::volume_scan(vol).scan( {}, this->description.world().placement() );
        if( !ret.second.isValid() )  {
          except("geant3_subdetector", "+++ %s [id:%d]: Failed to find top level volume: '%s'",
                 this->name.c_str(), this->id, vol.c_str() );
        }
      }
      else  {
        except("geant3_subdetector", "+++ %s [id:%d] Unknown DetElement selector.",
               this->name.c_str(), this->id );
      }

      if( xd.hasAttr(_U(sensitive)) )  {
        auto sens = xd.attr<std::string>(_U(sensitive));
        this->sensitive.setType( sens );
      }

      DetElement detector( this->name, this->id );
      auto pv  = ret.second;
      auto vol = pv.volume();
#if 0
    Volume     mother = description.pickMotherVolume( det );

    //geo.description.manager().SetTopVolume( vol.ptr() );
    //geo.description.init();
    //geo.description.endDocument( "close" );
    PlacedVolume phv = mother.placeVolume( vol, 1 );
    vol.setVisAttributes( description, x_det.visStr() );
    vol.setLimitSet( description, x_det.limitsStr() );
    vol.setRegion( description, x_det.regionStr() );
    det.setPlacement( phv );

#endif
      
      detector->volumeID = this->id;
      detector.setPlacement( pv );
      this->set_attr( vol, xd );

      printout(this->print, "geant3_subdetector",
               "+++ Creating detector element %-32s id:%d path:%s",
               detector.path().c_str(), detector.id(), detector.placementPath().c_str());
      if( xd.hasAttr(_U(sensitive)) )  {
        std::string readout_name = "----";

        pv.addPhysVolID( "system", this->id );

        if( xd.hasAttr(_U(readout)) )  {
          readout_name = xd.attr<std::string>(_U(readout));
          Readout rdout = this->description.readout( readout_name );
          if( !rdout.isValid() )  {
            except("geant3_subdetector", "+++ %s [id:%d] Invalid readout object: %s.",
                   this->name.c_str(), this->id, readout_name.c_str() );
          }
          this->sensitive.setReadout( rdout );
        }
        printout( this->print, "geant3_subdetector",
                  "+++ %s [id:%d] Assign readout '%s' to sensitive detector: '%s' [%s]",
                  this->name.c_str(), this->id, readout_name.c_str(),
                  this->sensitive.name(), this->sensitive.type().c_str() );
      }
      if( pv.num_daughters() > 0 )  {
        for ( xml_coll_t coll(this->xd, _U(daughter)); coll; ++coll )  {
          this->add_children( detector, coll );
        }
      }
      return detector;
    }
    /// Add sub-branch of DetElements to subdetector
    void add_children(DetElement parent, xml_det_t e)  {
      auto pv  = parent.placement();
      auto vol = e.attr<std::string>(_U(volume));
      tools::volume_collect helper(vol);

      helper.collect( {}, pv );
      if( helper.placements.empty() )  {
        return;
      }
      Volume volume = (helper.placements.begin()->second).volume();
      this->set_attr(volume, e);
      if( !this->sensitive.isValid() )  {
        if( e.hasAttr(_U(sensitive)) )  {
          volume.setSensitiveDetector( this->sensitive );
        }
      }
      std::string volid = volume.name();
      if( e.hasAttr(_U(physvolid)) )
        volid = e.attr<std::string>( _U(physvolid) );
      else
        std::transform( volid.begin(), volid.end(), volid.begin(), ::tolower );

      for( auto& place : helper.placements )  {
        PlacedVolume& pv = place.second;
        int           volume_id = pv.copyNumber();
        VisAttr       vis = pv.volume().visAttributes();
        std::string   nam = e.nameStr() + _toString(volume_id, "_%d");
        DetElement    de( parent, nam, parent.id() );
        this->_set_volid( pv, volid, volume_id );
        de->volumeID = volume_id;
        de.setPlacement( pv );

        printout( this->print, "geant3_subdetector",
                  "+++ %-36s [id:%d] sub-element %-8s [id:%d] vis:%-10s volid:%-8s %3d --> place %s",
                  de.path().c_str(), parent.id(), de.name(), de.id(),
                  vis.isValid() ? vis.name() : "---",
                  volid.c_str(), pv.copyNumber(), place.first.c_str() );
        if( pv.num_daughters() > 0 )  {
          for ( xml_coll_t coll(e, _U(daughter)); coll; ++coll )  {
            this->add_children( de, coll );
          }
        }
      }
    }
  };
}

static dd4hep::Ref_t
geant3_subdetector_create( dd4hep::Detector& description, xml_h e, dd4hep::Ref_t sens )  {
  dd4hep::geant3_subdetector sub_detector( description, e, sens );
  auto sdet = sub_detector.construct( );
  return sdet;
}

// first argument is the type from the xml file
DECLARE_DETELEMENT(DD4hep_Geant3SubDetector,geant3_subdetector_create)
//==========================================================================

namespace {
  struct sensitives;
  struct properties;
  struct material;
  struct detector;
  struct debug;
}
namespace dd4hep {
  template <> void Converter<sensitives>::operator()( xml_h e) const;
  template <> void Converter<properties>::operator()( xml_h e) const;
  template <> void Converter<material>::operator()( xml_h e) const;
  template <> void Converter<detector>::operator()( xml_h e) const;
  template <> void Converter<debug>::operator()( xml_h e) const;
}

/// Converter for material aliases
template <> void dd4hep::Converter<material>::operator()( xml_h element ) const {
  xml_comp_t c = element;
  auto& geo = this->_object<geant3_geometry>();
  geo.material_aliases.emplace( c.nameStr(), c.valueStr() );
  printout( geo.debug_materials ? ALWAYS : DEBUG, "geant3_geometry",
            "+++ add_material_alias: %s --> %s",
            c.nameStr().c_str(), c.valueStr().c_str() );
}

/// Converter to declare sensitive elements
template <> void dd4hep::Converter<sensitives>::operator()( xml_h element ) const  {
  xml_comp_t  c = element;
  std::string nam = c.nameStr();
  std::string det = c.attr<std::string>(_U(sensitive_detector));
  auto& geo = this->_object<geant3_geometry>();
  geo.sensitive_volumes.emplace( nam, det );
  printout( geo.debug_sensitive ? ALWAYS : DEBUG, "geant3_geometry",
            "+++ add_sensitive_volume:  volume: %s --> sd: %s", nam.c_str(), det.c_str() );
}

/// Converter for DetElement pathes/volumes
template <> void dd4hep::Converter<detector>::operator()( xml_h element ) const {
  xml_comp_t c = element;
  auto& geo = this->_object<geant3_geometry>();
  auto par  = c.hasAttr(_U(parent))    ? c.attr<std::string>(_U(parent))    : std::string();
  auto sens = c.hasAttr(_U(sensitive)) ? c.attr<std::string>(_U(sensitive)) : std::string();
  auto ro   = c.hasAttr(_U(readout))   ? c.attr<std::string>(_U(readout))   : std::string();
  auto vid  = c.hasAttr(_U(physvolid)) ? c.attr<std::string>(_U(physvolid)) : 0;
  auto nam  = c.nameStr();
  int  id   = c.id();

  if( c.hasAttr(_U(placement)) )  {
    auto place = c.attr<std::string>(_U(placement));
    geo.detelement_placements[place] = { nam, par, sens, place, vid, ro, id };
    printout( geo.debug_enhancements ? ALWAYS : DEBUG, "geant3_geometry",
              "+++ add_detelement_place:  detelement:%16s.%-8s id:%2d vid:%-8s --> place:%s",
              par.c_str(), nam.c_str(), id, vid.c_str(), place.c_str() );
  }
  else if( c.hasAttr(_U(volume)) )  {
    auto vol = c.attr<std::string>(_U(volume));
    geo.detelement_volumes[vol] = { nam, par, sens, vol, vid, ro, id };
    printout( geo.debug_enhancements ? ALWAYS : DEBUG, "geant3_geometry",
              "+++ add_detelement_volume: detelement:%16s.%-8s id:%2d vid:%-8s --> volume:%s",
              par.c_str(), nam.c_str(), id, vid.c_str(), vol.c_str() );
  }
  else  {
    except("Converter<detector>", "+++ Unknown DetElement selector.");
  }
}

/// Converter to setup debugging and printouts
template <> void dd4hep::Converter<debug>::operator()( xml_h element ) const {
  auto geo = this->_param<geant3_geometry>();
  for (xml_coll_t coll(element, _U(star)); coll; ++coll) {
    xml_comp_t  c = coll;
    std::string tag = coll.tag();
    
    if( tag == "detector" )  {
      auto dets = c.attr<std::string>(_U(name));
      if( dets.length() > 0 )  {
        geo->debug_detector += char(::toupper(dets[0]));
      }
    }
    else if( tag == "materials" )  {
      geo->debug_materials    = c.attr<bool>(_U(value));
    }
    else if( tag == "medium" )  {
      geo->debug_media        = c.attr<bool>(_U(value));
    }
    else if( tag == "volume" )  {
      geo->debug_volumes      = c.attr<bool>(_U(value));
    }
    else if( tag == "placement" )  {
      geo->debug_placements   = c.attr<bool>(_U(value));
    }
    else if( tag == "division" )  {
      geo->debug_divisions    = c.attr<bool>(_U(value));
    }
    else if( tag == "structure" )  {
      geo->debug_structure    = c.attr<bool>(_U(value));
    }
    else if( tag == "sensitive" )  {
      geo->debug_sensitive   = c.attr<bool>(_U(value));
    }
    else if( tag == "enhancements" )  {
      geo->debug_enhancements = c.attr<bool>(_U(value));
    }
    else  {
      except("Converter<detector>", "+++ Unknown debug information selector: "+tag);
    }
  }
}

/// Properties converter
template <> void dd4hep::Converter<properties>::operator()( xml_h element ) const {
  xml_elt_t compact(element);
  auto geo = this->_param<geant3_geometry>();
  geo->debug_detector = "";
  xml_coll_t( compact, _U(debug) ).for_each( Converter<debug>(description, geo) );
  xml_coll_t( compact, _U(materials) ).for_each( _U(material), Converter<material>(description, geo) );
  xml_coll_t( compact, _U(detectors) ).for_each( _U(detector), Converter<detector>(description, geo) );
  xml_coll_t( compact, _U(sensitives) ).for_each( _U(volume), Converter<sensitives>(description, geo) );
}

static long geant3_geo_enhancements(dd4hep::Detector& description, const xml_h& element) {
  auto& geo = geometry(description);
  (dd4hep::Converter <properties> ( description, &geo ))(element);
  return 1;
}
DECLARE_XML_DOC_READER(geant3_geometry_enhancements, geant3_geo_enhancements)

//==========================================================================
static long geant3_proc_enhancements(dd4hep::Detector& description, int /* argc */, char** /* argv */) {
  using namespace dd4hep;
  struct Actor {
  public:
    Detector&        description;
    geant3_geometry& geometry;
  public:
    Actor(Detector& desc, geant3_geometry& geo) : description(desc), geometry(geo)  { }
    void dump(std::string prefix, PlacedVolume node)  const {
      std::string path = std::move(prefix) + "/" + node.name();
      this->geometry.enhance(path, node);
      for (Int_t idau = 0, ndau = node->GetNdaughters(); idau < ndau; ++idau)
        dump(path, node->GetDaughter(idau));
    }
  };

  Actor(description, geometry(description)).dump("", description.world().placement());
  return 1;
}
DECLARE_APPLY(geant3_process_enhancements,geant3_proc_enhancements)
//==========================================================================
