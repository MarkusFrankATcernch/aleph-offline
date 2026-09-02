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
#include <alpha/texs.h>
#include <alpha/tpcgeom.h>

namespace {
  edm4hep::Quantity _dedx(int tag, float v, float e)  {
    return { tag, v, e };
  }
}

/// Production output Tpc track pad dE/dX (NR=0)
void alpha::edm4hep_output::event_t::process_texs()  {
  /**
     +------+                               Subschema: TpcJULBanks                
     | TEXS |  Tpc dE/dX Segment (NR=0) One                                        
     +------+  track segment per sector                                            
               crossed                                                             
     ..............................................................                
     1          I    Number of words per segment (=7)                          
     2          I    Number of segments                                        
     ..............................................................                
      1    SI  I    SegmentId        [1,*]                                     
                       Sector slot number                                      
      2    TM  F    TruncatedMean    [0.0,*]                                   
                       Truncated mean of dE/dx measurements                    
      3    TL  F    TrackLength      [0.0,500.0]                               
                       Useful length of track for dE/dx                        
      4    NS  I    NumberSamples    [1,500]                                   
                       Number of samples used for dE/dx                        
      5    AD  F    AverageDrift     [0.0,220.0]                               
                       Average drift length of samples                         
      6    TN  I    TrackNumber      [1,*]                                     
                       Pointer to track entry in TGFT                          
      7    SF  I    SatFlag          [0,1]                                     
                       1= >40% saturated hits.                                 
                       0= <40% saturated hits                                  
   */
  std::stringstream log;
  auto& tpc = *this->exp.tpc;
  auto& dsc = tpc.descriptor;
  bool  dbg = this->data.texs.debug;
  auto* tab = this->data.texs.load<object_table<class texs> >(true);

  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    auto*    ah      = tab->row(i);
    int32_t  slot    = ah->segmentId();
    int32_t  sector  = tpcgeom.ITPSEC[slot-1];
    uint64_t cell    = tpc.desc_system + dsc.encode(tpc.field_sector, sector);
    auto     mean    = this->dedx_trunc_mean.create( _dedx(DEDX_TRUNCATED_MEAN, ah->truncatedMean(), 0e0));
    auto     drift   = this->dedx_avg_drift.create(  _dedx(DEDX_AVERAGE_DRIFT, ah->averageDrift(), 0e0));
    auto     samples = this->dedx_num_samples.create(_dedx(DEDX_NUM_SAMPLES, ah->numberSamples(), 0e0));
    auto     length  = this->dedx_tracklen.create(   _dedx(DEDX_TRACKLENGTH, ah->trackLength(), 0e0));
    track_t  track   = this->particle_frft(ah->trackNumber());

    mean.setTrack(track);
    drift.setTrack(track);
    samples.setTrack(track);
    length.setTrack(track);
    if( dbg )  {
      char text[512];
      ::snprintf(text, sizeof(text),
                 "\tTEXS %3d %08lX Cell:%08lX Sector:%2d Mean:%7.3f len:%s samples:%3d drift:%3.0f track:%3d flag:%1d",
                 i, uint64_t(ah), cell, sector, ah->truncatedMean(), fmt_len(ah->trackLength()).c_str(),
                 ah->numberSamples(), ah->averageDrift(), ah->trackNumber(), ah->satFlag());
      log << text << std::endl;
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
