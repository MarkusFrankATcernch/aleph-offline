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
#include <alpha/vfhl.h>
#include <alpha/vfph.h>
#include <alpha/vdxy.h>

/// Create VDET 3D hit from VDXY row
void alpha::edm4hep_output::event_t::process_vdxy()  {
  /**
    +------+  Subschema: VdetPOTBanks
    | VDXY |  MVD hits in r-phi wafer.
    +------+  (POT)
           NR=ILAYER*10000+IZED*1000+IPH-I*10+(IVIEW=1) Bank number is
           packed/unpaked by VAENWA,VADEWA computed with values starting
           from 0
     ..............................................................
     1          I    Number of words/hit (=12)
     2          I    Number of hits per wafer
     ..............................................................
      1    XC  F    XCor             [-99.0000,99.0000]
                       x coordinate of hit
      2    YC  F    YCor             [-99.0000,99.0000]
                       y coordinate of hit
      3    UC  F    UCor             [-5.00000,5.00000]
                       local wafer coordinate
      4    SX  F    SigmaX           [0.0,5.00000]
                       sigma on x coord
      5    SY  F    SigmaY           [0.0,5.00000]
                       sigma on y coord
      6    SU  F    SigmaU           [0.0,5.00000]
                       sigma on u coord
      7    PH  F    PulseHeight      [0.0,*]
                       pulseheight
      8    QF  I    QualityFlag      [0,9999999]
                       quality flag
      9    NA  I    NAss             [0,1000000]
                       number of ass. tracks
      10   IP  I    IPoint           [0,1000000]
                       pointer to first associated track in
                       VLST
      11   IW  I    IWaf             [0,1000000]
                       VFHL bank number
      12   IH  I    IHit             [0,1000000]
                       Hit number in VFHL bank

     +------+                                                                       
     | VFHL |  VDET final hit list bank,                                            
     +------+  NR=VHLS number (POT) Hit
               address is computed as in
               VHLS packed/unpacked by
               VADDPK/VADDUN This bank is
               needed to unpack the
               VFPH,VFLG banks
     ..............................................................
     1          I    number of words/hit (=1)
     2          I    number of final hits
     ..............................................................
      1    HA  I    HitAdd           [0,*]
                       Hit address =Nstrip*2**18 +
                       Ilayer*2**17 + Iwafer*2**15 +
                       Iview*2**10 + Istrip

     +------+                              Subschema: VdetPOTBanks                
     | VFPH |  VDET final pulse height bank,                                       
     +------+  NR=VHLS number (POT) This                                           
               bank links up the VFHL bank                                         
               Pulseheight is stored in 250                                        
               e units                                                             
 
     ..............................................................                
     1          I    number of words/strip (=1)                                
     2          I    number of strips                                          
     ..............................................................                
      1    PH  I    PuHeight         [0,*]                                     
                       Pulseheight
  */
  std::stringstream log;
  int32_t vfhl_nr = -1;
  auto& vdet = *this->exp.vdet;
  auto& desc = vdet.descriptor;
  int32_t vfhl_nami = this->data.vfhl.nami;
  int32_t vfph_nami = this->data.vfph.nami;
  object_table<class vfhl>* vfhl_bank = nullptr;
  object_table<class vfph>* vfph_bank = nullptr;
 
  for( this->data.vdxy.load(false); this->data.vdxy.data; this->data.vdxy.knext() )  {
    auto* table = this->data.vdxy.table<class vdxy>();
    uint32_t row   = table->bank_header::row();
    uint64_t ilay  = ((row/10000));
    uint64_t iz    = ((row/1000)%10);
    uint64_t iphi  = ((row/10)%100);
    uint64_t iview = (row%10);
    uint64_t cell  = vdet.desc_system      +
      desc.encode(vdet.field_layer, ilay)  +
      desc.encode(vdet.field_z,     iz)    +
      desc.encode(vdet.field_phi,   iphi)  +
      desc.encode(vdet.field_view,  iview);

    if( this->data.vdxy.debug )  {
      log << bos77::to_string(table) << " Wafer hits: " << std::endl;
    }

    for( uint32_t ihit=1; ihit <= table->size(); ++ihit )  {
      float       energy = 0;
      float       err_energy = 0;
      uint64_t    hit_cell = cell;
      int32_t     addr = 0, strip = 0;
      auto*       ah   = table->row(ihit);
      std::size_t key  = this->hits_vdxy.size();
      auto        hit  = this->hits_vdxy.create();
      int32_t     indx_vfhl = ah->iwaf();
      // These are local coordinates with respect to the wafer frame
      Position    pos(_LEN(ah->xcor()), _LEN(ah->ycor()), ah->ucor());
      // This cannot really be correct, because x,y,u are local wafer coordinates,
      // but how to do it propery
      Position    err(_LEN(ah->sigmaX()), _LEN(ah->sigmaY()), _LEN(ah->sigmaU()));

      if( vfhl_nr != indx_vfhl )  {
        vfhl_nr = indx_vfhl;
        vfhl_bank  = (object_table<class vfhl>*)bos77::get_bank(vfhl_nami, vfhl_nr);
        vfph_bank  = (object_table<class vfph>*)bos77::get_bank(vfph_nami, vfhl_nr);
      }
      if( vfhl_bank )  {
        addr = vfhl_bank->row(ah->ihit())->hitAdd();
        strip = (addr&0x3FF);
        hit_cell = cell + desc.encode(vdet.field_strip, strip);
      }
      if( vfph_bank )  {
        /// Pulseheight is in units of 250 electrons
        const auto* vfph = vfph_bank->row(ah->ihit());
        int32_t num_electrons = vfph->puHeight() * 250;
        energy     = vdet.energy_deposit(num_electrons);
        err_energy = vdet.energy_deposit_error(num_electrons);
      }
      
      hit.setCellID( hit_cell );
      hit.setTime( _TIM(0e0) );
      hit.setEDep( energy );
      hit.setEDepError( err_energy );
      hit.setPosition( { float(pos.x()), float(pos.y()), float(pos.z()) } );
      hit.setCovMatrix( {
          err.x()*err.x(),
          0e0,             err.y()*err.y(),
          0e0,             0e0,             err.z()*err.z() } );
      hit.setQuality(ah->qualityFlag());
      hit.setType( VDET_HIT_RPHI );

      this->alpha2edm4hep_vdxy[(row<<16) + ihit] = key;
      
      if( this->data.vdxy.debug )  {
        log << "\t [vfhl:" << indx_vfhl
            << "." << std::setw(3) << ah->ihit() << "-" << iview
            << " lay:" << std::setw(1) << ilay
            << " phi:" << std::setw(2) << iphi
            << " z:"   << std::setw(1) << iz << " "
            << " add:" << std::setw(8) << std::hex << addr << std::dec << " ";
        // VFHL encoding:
        // addr  = Nstrip<<18 + ilay<<17 + wafer<<15 + view<<10 + strip;
        // strip = (addr&0x3FF);
        // view  = (addr&0x7FFF)>>10;
        // wafer = (addr&0x1FFFF)>>15;
        // Nstrip= (addr>>18);
        //
        // Absolute unclear how VFHL hit address relates to VDXY row number
        if( vfhl_bank )  {
          int32_t view  = (addr&0x7FFF)>>10;
          int32_t wafer = (addr&0x1FFFF)>>15;
          int32_t layer = (addr&0x3FFFF)>>17;
          int32_t Nstrip= (addr>>18);
          log << " strip:"  << std::setw(4) << strip
              << " view:"   << std::setw(2) << view
              << " waf:"    << std::setw(2) << wafer
              << " lay:"    << std::setw(1) << layer
              << " NStrip:" << std::setw(2) << Nstrip;
          int32_t check = (Nstrip<<18) + (layer<<17) + (wafer<<15) + (view<<10) + strip;
          if( check != addr )  {
            log << std::endl << "+++ !!!!!! ADDRESS CALCULATION WRONG:"
                << std::hex << addr << " <> " << check << std::dec << std::endl;
          }
        }
        log << "] "
            << " pos: "  << to_string(pos.x(),"%7.4f")
            << ","       << to_string(pos.y(),"%7.4f")
            << ","       << to_string(pos.z(),"%7.4f")
            << " pulse:" << to_string(ah->pulseHeight(),"%6.0f")
            << std::endl;
      }
    }
  }
  if( this->data.vdxy.debug )  {
    ::printf("%s\n", log.str().c_str());
  }
}
