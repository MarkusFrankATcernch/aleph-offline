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
#include <alpha/pcrl.h>
#include <alpha/ppob.h>
#include <alpha/htub.h>
#include <alpha/hwdi.h>

/// Create ECAL wire data from PEWI data
void alpha::edm4hep_output::event_t::process_htub()  {
  /**
 +------+                              Subschema: HcalJULBanks                
 | HTUB |  Hcal TUBes data (from Julia                                         
 +------+  prepare data)                                                       
 ..............................................................                
     1          I    Number of words/tube (=7)                                 
     2          I    Number of fired tubes                                     
 ..............................................................                
      1    SN  I    SubcompNumbe     [1,3]                                     
                       subcomponent number                                     
      2    MN  I    ModuleNumber     [1,24]                                    
                       module number (1-6 endcaps, 1-24 barrel)                
      3    LN  I    LayerNumber      [1,23]                                    
                       layer number (1-23)                                     
      4    LC  F    LocalCoordinate  [0.0,400.]                                
                       local coordinate of center of cluster                   
      5    CW  F    ClusterWidth     [0.0,20.]                                 
                       cluster width ( cm )                                    
      6    PR  I    PointerRawdata   [0,*]                                     
                       Pointer to raw data bank HWDI                           
      7    PP  I    PointerPattern   [0,*]                                     
                       Pointer to Pattern bank HPCO/PPOB                       
 ..............................................................                

 +------+                              Subschema: HcalRAWBanks                
 | HWDI |  Hcal Wire (tube)                                                    
 +------+  DIgits(RawData)                                                     
  ..............................................................                
     1          I    Nb of words/wirecluster (=1)                              
     2          I    Number of clusters                                        
 ..............................................................                
      1    CA  I    ClusterAddr      [*,*]                                     
                       Cluster address and width                               
                       bits: 0- 7 : address of last tube                       
                       in cluster [1,255]                                      
                       8-15 : cluster width [1,15]                             
                       16-23 : layer number [0,22]                             
                       24-31 : electronic module number                        
                       [1,36]                                                  
                       1-12 end-cap A                                          
                       13-24 barrel                                            
                       25-36 end-cap B                                         
                       attention ! the electronic                              
                       module number is different                              
                       for data, look HPRDIG in Julia                          
 ..............................................................
 
-- requires process_phco run before!
   */
  std::map<int32_t, std::set<int32_t> > ppob2htub;
  std::map<int32_t, std::set<int32_t> > htub2hwdi;
  
  auto* tab_htub = this->data.htub.load<object_table<class htub> >( false );
  auto* tab_hwdi = this->data.hwdi.load<object_table<class hwdi> >( false );
  if( !tab_htub || !tab_hwdi )  {
    return;
  }

  // First store indices of HCAL cluster data to patterns and wire data to tube data
  for( uint32_t ihtub=1, siz=tab_htub->size(); ihtub <= siz; ++ihtub )  {
    const auto* htub = tab_htub->row(ihtub);
    int32_t ippob = htub->pointerPattern();
    int32_t ihwdi = htub->pointerRawdata();
    if( ippob > 0 ) ppob2htub[ippob].insert(ihtub);
    if( ihwdi > 0 ) htub2hwdi[ihtub].insert(ihwdi);
  }
  ++this->data.htub.num_process;

  // Store HCAL pad clusters
  for( const auto& lnk_ppob : this->hcal_hit2pattern )  {
    edm4hep::MutableCluster hit = hcal_hits.at(lnk_ppob.first-1);
    for( int32_t ind_ppob : lnk_ppob.second )  {
      auto ippob = ppob2htub.find(ind_ppob);
      if( ippob != ppob2htub.end() )  {
        const auto& tubes = ippob->second;
        hit.addToShapeParameters( tubes.size() );
        for( int32_t ihtube : tubes )  {
          auto*   htub   = tab_htub->row(ihtube);
          int32_t barrel = htub->subcompNumbe();
          int32_t module = htub->moduleNumber();
          int32_t layer  = htub->layerNumber();
          int32_t cell   = ( barrel & 0x3 ) +
            ( (module & 0x1F) << 2 ) +
            ( (layer  & 0x1F) << 7 );
          float   coord  = htub->localCoordinate();
          float   width  = htub->clusterWidth();
          hit.addToShapeParameters( cell  );
          hit.addToShapeParameters( coord );
          hit.addToShapeParameters( width );
        }
      }
    }
  }

  // Store HCAL wire digits
  for( const auto& lnk_ppob : this->hcal_hit2pattern )  {
    edm4hep::MutableCluster hit = hcal_hits.at(lnk_ppob.first-1);
    for( int32_t ind_ppob : lnk_ppob.second )  {
      auto ippob = ppob2htub.find(ind_ppob);
      if( ippob != ppob2htub.end() )  {
        for( int32_t ihtube : ippob->second )  {
          auto ihwdi = htub2hwdi.find(ihtube);
          if( ihwdi != htub2hwdi.end() )  {
            for( int32_t ind_ihwdi : ihwdi->second )  {
              auto*   hwdi = tab_hwdi->row(ind_ihwdi);
              int32_t addr = hwdi->clusterAddr();
              hit.addToShapeParameters(addr);
            }
          }
        }
      }
    }
  }
  ++this->data.hwdi.num_process;
}
