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

/// dd4hep namespace declaration
namespace dd4hep  {

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
        return { };
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
        return { };
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
          this->placements[ path ] = node;
        for( Int_t idau = 0, ndau = node->GetNdaughters(); idau < ndau; ++idau )
          this->collect( path, node->GetDaughter(idau) );
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
    PrintLevel                print     { INFO };
    void _set_volid( PlacedVolume pv, const std::string& volid, int vid )  {
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
      
      auto pv  = ret.second;
      auto vol = pv.volume();
      auto vis = xd.hasAttr(_U(vis))     ? xd.attr<std::string>(_U(vis))     : std::string();
      auto lim = xd.hasAttr(_U(limits))  ? xd.attr<std::string>(_U(limits))  : std::string();
      auto reg = xd.hasAttr(_U(region))  ? xd.attr<std::string>(_U(region))  : std::string();

      DetElement detector( this->name, this->id );
      detector->volumeID = this->id;
      detector.setPlacement( pv );
      vol.setAttributes( this->description, reg, lim, vis );

      if( xd.hasAttr(_U(sensitive)) )  {
        std::string rdname = "----";
        auto sens = xd.attr<std::string>(_U(sensitive));

        pv.addPhysVolID( "system", this->id );
        this->sensitive.setType( sens );
        vol.setSensitiveDetector( this->sensitive );

        if( xd.hasAttr(_U(readout)) )  {
          rdname = xd.attr<std::string>(_U(readout));
          Readout rdout = this->description.readout( rdname );
          if( !rdout.isValid() )  {
            except("geant3_subdetector", "+++ %s [id:%d] Invalid readout object: %s.",
                   this->name.c_str(), this->id, rdname.c_str() );
          }
          this->sensitive.setReadout( rdout );
        }
        printout( this->print, "geant3_subdetector",
                  "+++ %s [id:%d] Assign readout '%s' to sensitive detector: '%s' [%s]",
                  this->name.c_str(), this->id, rdname.c_str(),
                  this->sensitive.name(), sens.c_str() );
      }
      if( pv.num_daughters() > 0 )  {
        for ( xml_coll_t coll(this->xd, _U(daughter)); coll; ++coll )  {
          this->add_children( detector, coll );
        }
      }
      return detector;
    }

    void add_children(DetElement parent, xml_det_t e)  {
      PlacedVolume pv = parent.placement();
      auto vol = e.attr<std::string>(_U(volume));
      tools::volume_collect helper(vol);

      helper.collect( {}, pv );
      if( helper.placements.empty() )  {
        return;
      }
      Volume volume = (helper.placements.begin()->second).volume();
      if( e.hasAttr(_U(vis)) )  {
        auto vis = xd.attr<std::string>(_U(vis));
        volume.setVisAttributes( this->description, vis );
      }
      if( xd.hasAttr(_U(limits)) )  {
        auto lim = xd.attr<std::string>(_U(limits));
        volume.setLimitSet( this->description, lim );
      }
      if(  xd.hasAttr(_U(region)) )  {
        auto reg = xd.attr<std::string>(_U(region));
        volume.setRegion( this->description, reg );
      }
      if( xd.hasAttr(_U(sensitive)) && xd.attr<bool>(_U(sensitive)) )  {
        volume.setSensitiveDetector( this->sensitive );
      }

      std::string volid = volume.name();
      std::transform( volid.begin(), volid.end(), volid.begin(), ::tolower );
      if( e.hasAttr(_U(physvolid)) )  {
        volid = e.attr<std::string>( _U(physvolid) );
      }

      for( const auto& place : helper.placements )  {
        PlacedVolume pv( place.second );
        int          volume_id = pv.copyNumber();
        VisAttr      vis = pv.volume().visAttributes();
        std::string  nam = e.nameStr() + _toString(volume_id, "_%d");
        DetElement   de( parent, nam, parent.id() );
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
