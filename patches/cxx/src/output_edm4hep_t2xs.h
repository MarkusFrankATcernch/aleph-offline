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
#include <alpha/t2xs.h>
#include <alpha/tpcgeom.h>

namespace {
  edm4hep::Quantity _dedxOv(int tag, float v, float e)  {
    return { tag|alpha::output_edm4hep::event_t::DEDX_TRACK_OVERLAP, v, e };
  }
}

/// Production output Tpc track pad dE/dX (NR=0)
void alpha::output_edm4hep::event_t::process_t2xs()  {
  /**
     +------+                               Subschema: TpcJULBanks                
     | T2XS |  Tpc dE/dX Segment for                                               
     +------+  Overlapping Tracks (NR=0).                                          
               One track pair segment per                                          
               sector crossed                                                      
 
     ..............................................................                
     1          I    Number of words per pair (=7)                             
                     segment                                                   
     2          I    Number of pair segments                                   
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
      6    T1  I    Tracknumber1     [1,*]                                     
                       Pointer to first track entry in TGFT                    
      7    T2  I    Tracknumber2     [1,*]                                     
                       Pointer to second track entry in TGFT                   
     ..............................................................                
   */
  std::stringstream log;
  auto& tpc = *this->exp.tpc;
  auto& dsc = tpc.descriptor;
  bool  dbg = this->data.t2xs.debug;
  auto* tab = this->data.t2xs.load<object_table<class t2xs> >(true);
  
  if( dbg )  {
    log << bos77::to_string(tab) << std::endl;
  }
  if( !tab )  {
    return;
  }
  for( uint32_t i=1; i <= tab->size(); ++i )  {
    auto*    ah      = tab->row(i);
    int32_t  slot    = ah->segmentId();
    int32_t  sector  = tpcgeom.ITPSEC[slot-1];
    uint64_t cell    = tpc.desc_system + dsc.encode(tpc.field_sector, sector);
    track_t  track1  = this->particle_frft(ah->tracknumber1());
    track_t  track2  = this->particle_frft(ah->tracknumber2());

    {
      auto mean    = this->dedx_trunc_mean.create( _dedxOv(DEDX_TRUNCATED_MEAN, ah->truncatedMean(), 0e0));
      auto drift   = this->dedx_avg_drift.create(  _dedxOv(DEDX_AVERAGE_DRIFT, ah->averageDrift(), 0e0));
      auto samples = this->dedx_num_samples.create(_dedxOv(DEDX_NUM_SAMPLES, ah->numberSamples(), 0e0));
      auto length  = this->dedx_tracklen.create(   _dedxOv(DEDX_TRACKLENGTH, ah->trackLength(), 0e0));
      mean.setTrack(track1);
      drift.setTrack(track1);
      samples.setTrack(track1);
      length.setTrack(track1);
    }
    {
      auto mean    = this->dedx_trunc_mean.create( _dedxOv(DEDX_TRUNCATED_MEAN, ah->truncatedMean(), 0e0));
      auto drift   = this->dedx_avg_drift.create(  _dedxOv(DEDX_AVERAGE_DRIFT, ah->averageDrift(), 0e0));
      auto samples = this->dedx_num_samples.create(_dedxOv(DEDX_NUM_SAMPLES, ah->numberSamples(), 0e0));
      auto length  = this->dedx_tracklen.create(   _dedxOv(DEDX_TRACKLENGTH, ah->trackLength(), 0e0));
      mean.setTrack(track2);
      drift.setTrack(track2);
      samples.setTrack(track2);
      length.setTrack(track2);
    }
    if( dbg )  {
      char text[512];
      ::snprintf(text, sizeof(text),
                 "\tT2XS %3d %08lX Cell:%08lX Sector:%2d Mean:%7.3f len:%s samples:%3d drift:%3.0f track:%2d/%2d",
                 i, uint64_t(ah), cell, sector, ah->truncatedMean(), fmt_len(ah->trackLength()).c_str(),
                 ah->numberSamples(), ah->averageDrift(), ah->tracknumber1(), ah->tracknumber2());
      log << text << std::endl;
    }
  }
  if( dbg )  {
    ::printf("%s\n", log.str().c_str());
  }
}
