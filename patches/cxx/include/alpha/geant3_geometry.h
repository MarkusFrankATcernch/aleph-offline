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
#ifndef ALPHA_GEANT3_GEOMETRY_H
#define ALPHA_GEANT3_GEOMETRY_H 1

/// dd4hep include files
#include <DD4hep/Shapes.h>
#include <DD4hep/Volumes.h>
#include <DD4hep/Objects.h>
#include <DD4hep/DD4hepUnits.h>
#include <DD4hep/Detector.h>

/// dd4hep namespace declaration
namespace dd4hep   {

  class geant3_geometry  {
  public:

    class MaterialWrapper  {
      Material material;
    public:
      MaterialWrapper(Material m) : material(m) {}
      ~MaterialWrapper() = default;
      MaterialWrapper& add_composite(double Z, double A, double weight);
    };
    
  protected:

    double  cnv_len;
    double  cnv_angle;
    double  cnv_density;
    double  cnv_intlen;
    double  cnv_radlen;

    std::map<std::string, Volume>      volumes;
    std::map<int, Rotation3D>  rotations;
    Detector& description;
    
    /// Access material to build volumes
    Material get_material(const std::string& name)  const;
                     
    /// Add volume to geometry inventory
    void add_volume( int index,
                     const std::string& name,
                     const std::string& medium,
                     const Solid& solid );

    /// Add rotation matrix to build placement transformations
    void add_rotation( int index, const Rotation3D& rotation );

    /// Add material to geometry inventory
    MaterialWrapper add_material( int index,
                                  const std::string& name,
                                  double Z, double A,
                                  double density, double intlen, double radlen);

    /// Add material to geometry inventory
    void add_pure_material( int index,
                            const std::string& name,
                            double Z, double A,
                            double density, double intlen, double radlen);

    /// Add volume medium to geometry inventory
    void add_medium(   int index,
                       const std::string& name,
                       const std::string& material);

    /// Add volume medium to geometry inventory
    void place_volume( int index,
                       const std::string& mother,
                       const std::string& volume,
                       int copy_nr,
                       const Position& position,
                       int index_rotation,
                       const std::vector<double>& params);

  public:
    /// Default constructor
    geant3_geometry(Detector& desc);
    
    /// Default destructor
    virtual ~geant3_geometry();

    /// Handle the conversion of the Geant3 materials:
    virtual void handle_materials();

    /// Handle the conversion of the Geant3 media
    virtual void handle_media();

    /// Handle the conversion of the Geant3 volumes
    virtual void handle_volumes();

    /// Handle the conversion of the Geant3 transformation matrices
    virtual void handle_transformations();
    
    /// Handle the conversion of the Geant3 placements
    virtual void handle_placements();
  };
};
#endif // ALPHA_GEANT3_GEOMETRY_H


