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

/// Alpha include files
#include <alpha/icco.h>

/// Create VDET 3D hit from VDCO row
void alpha::edm4hep_output::event_t::process_icco()  {
  /**
   *  +------+                             Subschema: ItcJuliaBanks
   *  | ICCO |  Itc Corrected COordinates for
   *  +------+  the final track fit. This
   *            bank is parallel to ITCO. Use
   *            track trajectory to correct
   *            used coords. (for Z, TOF,
   *            signal time and entrance
   *            angle thru cell) Used coords.
   *            correspond to lists in IGCL
   *            or FICL. Unused coords.
   *            cannot be corrected and have
   *            null entries. (JUL)
   * 
   *  ..............................................................
   *      1          I    Number of words per (=5)
   *                      coordinate
   *      2          I    Number of coordinates.
   *  ..............................................................
   *       1    RV  F    RValue           [15.000,27.300]
   *                        radius of coord.(cm.)
   *       2    PH  F    PHi              [0.0,6.30000]
   *                        Phi of coord.
   *       3    ZV  F    ZValue           [-150.0,150.0]
   *                        Z of coord (cm.)
   *       4    SR  F    SigmaRphi        [0.0,1.000000]
   *                        Sigma(r-phi)**2 (cm**2)
   *       5    SZ  F    SigmaZ           [0.0,*]
   *                        Sigma(z)**2 (cm**2)
   */
  bool  dbg = this->data.icco.debug;
  auto* tab = this->data.icco.load<object_table<class icco> >(true);

  printout( dbg ? ALWAYS : DEBUG, __FUNCTION__,"+++ %s", bos77::to_string(tab).c_str() );
  for( uint32_t itk=1; itk <= tab->size(); ++itk )  {
    const icco*  ah       = tab->row(itk);
    trackerhit_t hit      = this->hits_icco.create();
    double       r        = _LEN( ah->rvalue() );
    double       z        = _LEN( ah->zvalue() );
    double       phi      = ah->phi();
    double       sinPhi   = std::sin(phi);
    double       cosPhi   = std::cos(phi);
    double       sigrphi2 = _LEN2( ah->sigmaRphi() );   //
    double       sigz2    = _LEN2( ah->sigmaZ() );      //
    /// ITCO is parallel to ICCO. Hence just copy the cell ID
    auto         itco     = this->hitplanes_itco1.at( itk-1 );

    hit.setCellID( itco.getCellID() );
    hit.setQuality(0);
    hit.setType(ITC_COORDINATE);
    hit.setEDep(      _ENE(0e0) );       // Not available
    hit.setEDepError( _ENE(0e0) );       // Not available
    hit.setTime(      itco.getTime() );
    hit.setPosition( { r * cosPhi, r * sinPhi, z } );
    hit.setCovMatrix( sigrphi2 * sinPhi * sinPhi, edm4hep::Cartesian::x, edm4hep::Cartesian::x);
    hit.setCovMatrix(-sigrphi2 * sinPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::x);
    hit.setCovMatrix( sigrphi2 * cosPhi * cosPhi, edm4hep::Cartesian::y, edm4hep::Cartesian::y);
    hit.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::x);
    hit.setCovMatrix( 0.0f,  edm4hep::Cartesian::z, edm4hep::Cartesian::y);
    hit.setCovMatrix( sigz2, edm4hep::Cartesian::z, edm4hep::Cartesian::z);
  }
}
