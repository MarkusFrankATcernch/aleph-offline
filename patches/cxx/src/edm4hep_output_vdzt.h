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
#include <alpha/vdzt.h>

/// Create VDET 3D hit from VDCO row
void alpha::edm4hep_output::event_t::process_vdzt()  {
  /*
                                        Subschema: VdetPOTBanks
     +------+                                                                       
     | VDZT |  MVD hits in z wafer. (POT)                                           
     +------+  NR=ILAYER*10000+IZED*1000+IPH-
               I*10+(IVIEW=1) Bank number is
               packed/unpaked by
               VAENWA,VADEWA computed with
               values starting from 0
     ..............................................................
     1          I    Number of words/hit (=10)
     2          I    Number of hits per wafer
     ..............................................................
      1    ZC  F    ZCor             [-99.0000,99.0000]
                       z coordinate of hit
      2    WC  F    WCor             [-5.0000,5.0000]
                       local wafer coordinate
      3    SZ  F    SigmaZ           [0.0,5.00000]
                       sigma on x coord
      4    SW  F    SigmaW           [0.0,5.00000]
                       sigma on w coord
      5    PH  F    PulseHeight      [0.0,9999.0000]
                       pulseheight
      6    QF  I    QualityFlag      [0,9999999]
                       quality flag
      7    NA  I    NAss             [0,1000000]
                       number of ass. tracks
      8    IP  I    IPoint           [0,1000000]
                       pointer to first associated track in
                       VLST
      9    IW  I    IWaf             [0,1000000]
                       VFHL bank number
      10   IH  I    IHit             [0,1000000]
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

  for( this->data.vdzt.load(false); this->data.vdzt.data; this->data.vdzt.knext() )  {
    auto* table = this->data.vdzt.table<class vdzt>();
    uint32_t row   = table->bank_header::row();
    uint64_t ilay  = ((row/10000));
    uint64_t iphi  = ((row/10)%100);
    uint64_t iz    = ((row/1000)%10);
    uint64_t iview = (row%10);
    uint64_t cell  = vdet.desc_system +
      desc.encode(vdet.field_layer,  ilay) +
      desc.encode(vdet.field_z,      iz)    +
      desc.encode(vdet.field_phi,    iphi)  +
      desc.encode(vdet.field_view,   iview);

    if( this->data.vdzt.debug )  {
      log << bos77::to_string(table) << " Wafers hits: " << std::endl;
    }
    for( uint32_t ihit=1; ihit <= table->size(); ++ihit )  {
      float       energy = 0;
      float       err_energy = 0;
      uint64_t    hit_cell = cell;
      int32_t     addr = 0, strip = 0;
      auto*       ah  = table->row(ihit);
      std::size_t key = this->hits_vdzt.size();
      auto        hit = this->hits_vdzt.create();
      Position    pos(_LEN(ah->zcor()), _LEN(ah->wcor()), 0e0);
      Position    err(_LEN(ah->sigmaZ()), _LEN(ah->sigmaW()), 0e0);
      int32_t     vfhl = ah->iwaf();

      if( vfhl_nr != vfhl )  {
        vfhl_nr = vfhl;
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
      hit.setType( VDET_HIT_Z );
      
      this->alpha2vdzt[(row << 16) + ihit] = ah;
      this->alpha2edm4hep_vdzt[(row << 16) + ihit] = key;

      if( this->data.vdzt.debug )  {
        log << "\t [vfhl:" << vfhl
            << "." << std::setw(3) << ah->ihit() << "-" << iview
            << " lay:" << std::setw(1) << ilay
            << " phi:" << std::setw(2) << iphi
            << " z:"   << std::setw(1) << iz << " "
            << " add:" << std::setw(8) << std::hex << addr << std::dec << " ";
        // VFHL encoding:
        // addr  = Nstrip<<18 + layer<<17 + wafer<<15 + view<<10 + strip;
        // strip = (addr&0x3FF);
        // view  = (addr&0x7FFF)>>10;
        // wafer = (addr&0x1FFFF)>>15;
        // Nstrip= (addr>>18);
        //
        // Absolute unclear how VFHL hit address relates to VDZT row number
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
            << " pos: " << to_string(pos.x(),"%7.4f")
            << "," << to_string(pos.y(),"%7.4f")
            << "," << to_string(pos.z(),"%7.4f")
            << " pulse:" << to_string(ah->pulseHeight(),"%6.0f")
            << std::endl;
      }
    }
  }
  if( this->data.vdzt.debug )  {
    ::printf("%s", log.str().c_str());
  }
}
