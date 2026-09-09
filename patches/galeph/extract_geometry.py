# ==========================================================================
#   Software for the ALEPH experiment
# --------------------------------------------------------------------------
#  Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
#  All rights reserved.
# 
#  For the licensing terms see ALSOFTINSTALL/LICENSE.
#  For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
# 
#  Author     : M.Frank
# 
# ==========================================================================

__version__ = "1.0"
__author__  = "Markus Frank <Markus.Frank@cern.ch>"
__usage__   = f"""

python gen_header.py --bank FTFT --output . 
"""

import os
import pdb
import sys
import cmath

# =========================================================================================
def to_len(value):
  return 10.0*value

# =========================================================================================
def to_angle(value):
  v = (value/360.0)*2.0*cmath.pi

# =========================================================================================
def _trim(line):
  return line.lstrip().rstrip()

def to_vector(value_list):
  s = str(value_list).replace("'","").replace('"','').replace('[','{').replace(']','}')
  return s

# =========================================================================================
def _items(line):
  l = _trim(line)
  for i in range(10):
    l = l.replace('  ',' ')
  itm = l.split(' ')
  items = []
  for i in itm:
    if len(i): items.append(i)
  return items


"""
   \author  M.Frank
   \version 1.0
"""
class material:
  # =======================================================================================
  def __init__(self, geo, data):
    self.geometry  = geo
    self.data      = data
    self.index     = data['index']
    self.name      = _trim(data['name'])
    self.A         = data['A']
    self.Z         = data['Z']
    self.density   = data['density']
    self.intlen    = data['intlen']
    self.radlen    = data['radlen']
    self.mix       = data['mix']

"""
   \author  M.Frank
   \version 1.0
"""
class medium:
  # =======================================================================================
  def __init__(self, geo, data):
    self.geometry  = geo
    self.data      = data
    self.index     = data['index']
    self.name      = _trim(data['name'])
    self.idx_mat   = data['material']
    self.isvol     = data['isvol']
    self.ifield    = data['ifield']
    self.magfield  = data['magfield']
    self.tmaxfd    = data['tmaxfd']
    self.stepmax   = data['stepmax']
    self.deemax    = data['deemax']
    self.epsilon   = data['epsilon']
    self.stepmin   = data['stepmin']

  def material(self):
    return self.geometry.material[self.idx_mat]


"""
   \author  M.Frank
   \version 1.0
"""
class matrix:
  # =======================================================================================
  def __init__(self, geo, data):
    self.geometry  = geo
    self.data      = data
    self.index     = data['index']
    self.flag      = data['flag']
    self.matrix    = data['matrix']
    self.phi       = [data['phi1'],data['phi2'],data['phi3']]
    self.theta     = [data['theta1'],data['theta2'],data['theta3']]


"""
   \author  M.Frank
   \version 1.0
"""
class volume:
  # =======================================================================================
  def __init__(self, geo, data):
    self.geometry   = geo
    self.data       = data
    self.index      = data['index']
    self.name       = _trim(data['name'])
    self.shape      = _trim(data['shape'])
    self.idx_medium = data['medium']
    self.params     = data['params']

  def medium(self):
    return self.geometry.medium[self.idx_medium]


"""
   \author  M.Frank
   \version 1.0
"""
class placement:
  # =======================================================================================
  def __init__(self, geo, data):
    self.geometry  = geo
    self.data      = data
    self.index     = data['index']
    

"""
   \author  M.Frank
   \version 1.0
"""
class geant3_geometry:
  # =======================================================================================
  def __init__(self):
    self.material = {}
    self.medium = {}
    self.volumes = {}
    self.matrices = {}
    self.placements = {}
    
  def add_material(self, mat):
    self.material[mat['index']] = material(self, mat)
    
  def add_medium(self, med):
    self.medium[med['index']] = medium(self, med)
    
  def add_placement(self, place):
    self.placements[place['index']] = placement(self, place)

  def add_rotation(self, mat):
    self.matrices[mat['index']] = matrix(self, mat)

  def add_volume(self, vol):
    self.volumes[vol['index']] = volume(self, vol)

# =========================================================================================
class dd4hep_geometry:
  # =======================================================================================
  def __init__(self, geo):
    self.geant3_geo = geo
    self.material = {}
    self.medium = {}
    self.shapes = {}
    self.volumes = {}
    self.matrices = {}
    self.placements = {}
    self.code = \
"""//==========================================================================
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

/// Alpha include files
#include <alpha/geant3_geometry.h>

/// DD4hep namespace declaration
namespace dd4hep  {
   class g3_geometry : public dd4hep::geant3_geometry  {
   public:
         using geant3_geometry::geant3_geometry;
         
      /// Handle the conversion of the Geant3 materials:
      virtual void handle_materials()  override;
      /// Handle the conversion of the Geant3 media
      virtual void handle_media()  override;
      /// Handle the conversion of the Geant3 volumes
      virtual void handle_volumes()  override;
      /// Handle the conversion of the Geant3 transformation matrices
      virtual void handle_transformations()  override;
      /// Handle the conversion of the Geant3 placements
      virtual void handle_placements();
   };
}

namespace units = dd4hep;

"""

  # ========================================================================
  def output(self, text):
    self.code = self.code + text + '\n'

  # =======================================================================================
  def make_shapes(self):
    open_bracket = '{'
    close_bracket = '}'
    for idx, vol in self.geant3_geo.volumes.items():
      shape = 'None'
      typ = vol.shape
      error = None
      if typ == 'BOX':
        shape = f'dd4hep::Box( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len )'
      elif typ == 'TUBE':  #      Geant3 parameters:  rmin, rmax, dz
        shape = f'dd4hep::Tube( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len )'
      elif typ == 'TUBS':  #      Geant3 parameters:  rmin, rmax, dz, start-phi, end-phi
        shape = f'dd4hep::Tube( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len, ' + \
                f'{vol.params[3]}*cnv_angle, {vol.params[4]}*cnv_angle )'
      elif typ == 'CONE':  #      Geant3 parameters:  dz, rmin1, rmax2, rmin2, rmax2
        shape = f'dd4hep::Cone( "{vol.name}_shape", {vol.params[0]}*cnv_len, ' + \
                f'{vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len, ' + \
                f'{vol.params[3]}*cnv_len, {vol.params[4]}*cnv_len )'
      elif typ == 'CONS':  #      Geant3 parameters:  dz, rmin1, rmax2, rmin2, rmax2, start-phi, end-phi
        shape = f'dd4hep::ConeSegment( "{vol.name}_shape", {vol.params[0]}*cnv_len, ' + \
                f'{vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len, ' + \
                f'{vol.params[3]}*cnv_len, {vol.params[4]}*cnv_len, ' + \
                f'{vol.params[5]}*cnv_angle, {vol.params[6]}*cnv_angle )'
      elif typ == 'HYPE':  #      Geant3 parameters:  rmin, rmax, dz, theta
        shape = f'dd4hep::Hyperboloid( "{vol.name}_shape", {vol.params[0]}*cnv_len, ' + \
                f'{vol.params[1]}*cnv_len, 0*cnv_len, {vol.params[2]}*cnv_len, 0*cnv_len, {vol.params[3]}*cnv_angle )'
      elif typ == 'SPHE':  #      Geant3 parameters:  rmin, rmax, start-theta, end-theta, start-phi, end-phi
        shape = f'dd4hep::Sphere( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, ' + \
                f'{vol.params[2]}*cnv_angle, {vol.params[3]}*cnv_angle, ' + \
                f'{vol.params[4]}*cnv_angle, {vol.params[5]}*cnv_angle )'
      elif typ == 'TRD1':  #      Geant3 parameters:  dx1, dx2, dy, dz
        shape = f'dd4hep::Trd1( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len, {vol.params[3]}*cnv_len )'
      elif typ == 'TRD2':  #      Geant3 parameters:  dx1, dx2, dy1, dy2, dz
        shape = f'dd4hep::Trd2( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len, {vol.params[3]}*cnv_len, {vol.params[4]}*cnv_len )'
      elif typ == 'TRAP':  #      Geant3 parameters:  z, theta, phi,  h1, bl1, tl1, alpha1, h2, bl2, tl2, alpha2
        shape = f'dd4hep::Trap( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_angle, {vol.params[2]}*cnv_angle, ' + \
                f'{vol.params[3]}*cnv_angle, {vol.params[4]}*cnv_len, {vol.params[5]}*cnv_len, {vol.params[6]}*cnv_angle, ' + \
                f'{vol.params[7]}*cnv_angle, {vol.params[8]}*cnv_len, {vol.params[9]}*cnv_len, {vol.params[10]}*cnv_angle )'
      elif typ == 'PARA':  #      Geant3 parameters:  dx, dy, dz, alpha, theta, phi
        shape = f'dd4hep::Box( "{vol.name}_shape", 1.0, 1.0, 1.0 )'
        #shape = f'dd4hep::Parallelepiped( "{vol.name}_shape", {vol.params[0]}*cnv_len, {vol.params[1]}*cnv_len, {vol.params[2]}*cnv_len, ' + \
        #        f'{vol.params[3]}*cnv_angle, {vol.params[4]}*cnv_angle, {vol.params[5]}*cnv_angle )'
      elif typ == 'PCON':  #      Geant3 parameters:  phi, dphi, nz, (z, rmin, rmax)
        nz = int(vol.params[2])
        z = []
        rmin = []
        rmax = []
        for i in range(nz):
          z.append(   f'{vol.params[2+i*3]}*cnv_len' )
          rmin.append( f'{vol.params[3+i*3]}*cnv_len' )
          rmax.append( f'{vol.params[4+i*3]}*cnv_len' )
        shape = f'dd4hep::Polycone( "{vol.name}_shape", {vol.params[0]}*cnv_angle, {vol.params[1]}*cnv_angle, ' + \
                f'{to_vector(rmin)}, {to_vector(rmax)}, {to_vector(z)} )'
      elif typ == 'PGON':  #      Geant3 parameters:  phi, dphi, nz, (z, rmin, rmax)
        nz = int(vol.params[2])
        z = []
        rmin = []
        rmax = []
        for i in range(nz):
          z.append(   f'{vol.params[2+i*3]}*cnv_len' )
          rmin.append( f'{vol.params[3+i*3]}*cnv_len' )
          rmax.append( f'{vol.params[4+i*3]}*cnv_len' )
        shape = f'dd4hep::Polyhedra( "{vol.name}_shape", {nz}, {vol.params[0]}*cnv_angle, {vol.params[1]}*cnv_angle, ' + \
                f'{to_vector(z)}, {to_vector(rmin)}, {to_vector(rmax)} )'
                
      else:                # Unknown shape
        error = f'ERROR: Unknown shape {vol.name} {typ} '

      if error:
        print( f' FAILED SHAPE: {error}' )
      else:
        self.shapes[idx] = shape
        medium = vol.medium()
        material = medium.material()
        print( f'---> {vol.name} SHAPE:   {shape}' )
        print( f'          MEDIUM: "{medium.name}"  -> MATERIAL: "{material.name}" ' )
        self.medium[medium.name] = { 'g3': medium, 'code': '' }
        self.material[material.name] = { 'g3': material, 'code': '' }
        self.volumes[idx] = { 'name': vol.name, 'g3': vol, 'shape': shape, 'medium': medium.name }

  # =======================================================================================
  def handle_materials(self):
    self.output( '/// Handle the conversion of the Geant3 materials: \n' + \
           'void dd4hep::g3_geometry::handle_materials()  {          \n' + \
           '  /// First add all pure materials:' )
    for name, mat in self.material.items():
      m = mat['g3']
      if len(m.mix) == 0:
        self.output( f'  this->add_pure_material( {m.index}, "{name}", {m.Z}, {m.A}, {m.density}*cnv_density, {m.intlen}*cnv_intlen, {m.radlen}*cnv_radlen ); ' )
    self.output( f'  /// Now add all the composite materials:' )

    for name, mat in self.material.items():
      m = mat['g3']
      if len(m.mix) > 0:
        # pdb.set_trace()
        self.output( f'  this->add_material( {m.index}, "{name}", {m.Z}, {m.A}, {m.density}*cnv_density, {m.intlen}*cnv_intlen, {m.radlen}*cnv_radlen )' )
        last = ''
        for i in range(len(m.mix)):
          if i == len(m.mix)-1:
            last = ';'
          val = str(m.mix[i]).replace("'A': ",'') \
            .replace("'Z': ",'') \
            .replace("'wmix': ",'') \
            .replace('{','') \
            .replace('}','')
          self.output( f'    .add_composite( {val} ){last}' )
    self.output( '} /// End dd4hep::g3_geometry::handle_materials  \n\n' )

  # =======================================================================================
  def handle_media(self):
    self.output( '/// Handle the conversion of the Geant3 media: \n' + \
           'void dd4hep::g3_geometry::handle_media()  {          ')
    mm = {}
    for idx, vol in self.volumes.items():
      med_name = vol['medium']
      if not mm.get(med_name):
        mm[med_name] = 1
        med = self.medium[med_name]['g3']
        mat_name = med.material().name
        self.output( f'  this->add_medium( {med.index}, "{med_name}", "{mat_name}" );' )
    self.output( '} /// End dd4hep::g3_geometry::handle_media  \n\n' )
  
  # =======================================================================================
  def handle_volumes(self):
    self.output( '/// Handle the conversion of the Geant3 volumes  \n' + \
           'void dd4hep::g3_geometry::handle_volumes()  {        \n')
    for idx, vol in self.volumes.items():
      g3    = vol['g3']
      shape = vol['shape']
      self.output( f'  this->add_volume( {idx}, "{vol['name']}", "{g3.medium().name}", {shape} );' )
    self.output( '} /// End dd4hep::g3_geometry::handle_volumes  \n\n' )

  # =======================================================================================
  def handle_transformations(self):
    open_bracket = '{'
    close_bracket = '}'
    self.output( '/// Handle the conversion of the Geant3 transformation matrices \n' + \
           'void dd4hep::g3_geometry::handle_transformations()  {               \n' + \
           '   // double tmp[9];  \n' )
    """

           -1.0,  0.0,  0.0,
            0.0,  1.0,  0.0,
            0.0,  0.0,  1.0,


    """
    for idx, mat in self.geant3_geo.matrices.items():
      rval = str(mat.data['matrix']).replace('[','{').replace(']','}')
      self.output( f'  /// {str(mat.data)} ' )
      self.output( f'  this->add_rotation( {idx}, ROOT::Math::Rotation3D({rval}));')
    self.output( '} /// End dd4hep::g3_geometry::handle_transformations  \n\n' )

  # =======================================================================================
  def handle_placements(self):
    open_bracket = '{'
    close_bracket = '}'
    self.output( '/// Handle the conversion of the Geant3 placements \n' + \
           'void dd4hep::g3_geometry::handle_placements()  {        \n' + \
           '  Volume       vol, mother;                        \n' + \
           '  PlacedVolume pv;                               \n\n' )
           
    for idx, pv in self.geant3_geo.placements.items():
      data = pv.data;
      pos = f'{open_bracket}{data['x']},{data['y']},{data['z']}{close_bracket}'
      par = str(data['params']).replace('[','{').replace(']','}')
      self.output( f'  /// {str(data)} ' )
      self.output( f'  this->place_volume({idx}, "{data['mother']}", "{data['name']}", {data['copy']}, {pos}, {data['rotation']}, {par});' )
    self.output( '} /// End dd4hep::g3_geometry::handle_placements  \n\n' )
  


geometry_file = "aleph.geometry.txt"
lines = open(geometry_file, 'r').readlines()

aleph_geometry = geant3_geometry()
#pdb.set_trace()
num_err = 0
for line in lines:
  if line.find('aleph_geometry.add') >= 0:
    try:
      eval(line)
    except Exception as X:
      print( f'ERROR: {line} {str(X)}' )
      num_err = num_err + 1

if num_err == 0:
   print( f'+++ Successfully processed Geant3 input file: {geometry_file}')
   print( f'+++ Parsed  {len(aleph_geometry.material)}  material')
   print( f'+++         {len(aleph_geometry.medium)}  media')
   print( f'+++         {len(aleph_geometry.volumes)}  volumes')
   print( f'+++         {len(aleph_geometry.placements)}  placements')
   dd4hep = dd4hep_geometry(aleph_geometry)
   dd4hep.make_shapes()
   
   dd4hep.handle_materials()
   dd4hep.handle_media()
   dd4hep.handle_volumes()
   dd4hep.handle_transformations()
   dd4hep.handle_placements()
   file = open('../cxx/src/geometry.cpp','w')
   file.write( dd4hep.code )
   file.close()
   #   dd4hep.handle_placements()


else:
   print( f'+++ FAILED to process Geant3 input file: {geometry_file}.  {num_err} errors encountered.')
