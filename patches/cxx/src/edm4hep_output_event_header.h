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
#include <alpha/eveh.h>
#include <alpha/kevh.h>
#include <alpha/revh.h>

/// Create HCAL cluster relations from PHCO to FRFT tracks: bank PFHR
void alpha::edm4hep_output::event_t::process_event_header()  {
  /**
 +------+                            Subschema: StatusRAWBanks
 | EVEH |  EVEnt Header (NR = 0)
 +------+
 ..............................................................
      1    EN  I    ExpNumber
                       Experiment Number
                       Online = [0,1000]
                       MonteCarlo =1001
      2    RN  I    RunNumber        [1,*]
                       Unique run number
      3    RT  I    RunType
                       Monte Carlo run type
                       KINE "LUND" : [1000,1999]
                       "SJET" : [2000,2999]
                       "PART" : [3000,3999]
                       "USER" : [10000,99999]
                       undefined : 100000
      4    DA  I    DAte
                       event date : yymmdd
      5    TI  I    TIme
                       event time : hhmmss
      6    EV  I    EVentnumber
                       event number
      7    M1  I    Mask1
                       Trigger mask 1
      8    M2  I    Mask2
                       Trigger mask 2
      9    M3  I    Mask3
                       Trigger mask 3
      10   M4  I    Mask4
                       Trigger mask 4
      11   TY  I    TYpe
                       Event type
                       Real event = 1
      12   ES  I    ErrorStatus
                       Error status (OK = 1)
      13   TE  I    TotalEnergy
                       2*Beam energy (keV)

     +------+                            Subschema: StatusJULBanks
     | REVH |  Reconstructio EVent Header
     +------+

     ..............................................................
     1          I    Number of Words (=10)
     2          I    Number of Headers(=1)
     ..............................................................
      1    DS  I    DetectorStatus   [*,*]
                       Detector status bits
    2-3    FE  I    FatalErr         [*,*]
                       Fatal error bits
    4-5    NE  I    NonfatalErr      [*,*]
                       Non-fatal error bits
      6    SB  I    SelectionBits    [*,*]
                       Selection bits
      7    TI  F    TIme             [0.0,*]
                       Time for reconstruction
    8-9    RB  I    ReadoutBits      [*,*]
                       Readout Bits, shows existence
                       of detector banks in raw data
                       bit 0 - VDET
                       bit 1 - ITC
                       bit 2 - TPC pad
                       bit 3 - TPC wire
                       bit 4 - ECAL pad
                       bit 5 - ECAL wire
                       bit 6 - HCAL pad
                       bit 7 - HCAL wire
                       bit 8 - MUON
                       bit 9 - SATR
                       bit 10- LCAL pad
                       bit 11- LCAL wire
                       bit 12- BCAL
                       bit 13- Trigger
                       bit 14- BOM
                       bit 15- SICAL
      10   EC  I    EdirClass        [0,*]
                       EDIR class word


 +------+                          Subschema: EventKINGALBanks
 | KEVH |  Event generator status(NR=0)
 +------+

 ..............................................................
     1          I    Number of words/event (=7)
     2          I    Number of event (=1)
 ..............................................................
      1    RN  I    firstRandomNu
                       1st random number
      2    NT  I    NumberofTrac
                       # of tracks generated
      3    NV  I    NumberofVert
                       # of vertices generated
      4    PI  I    ProcessId
                       process identification
      5    WT  F    WeighT
                       weight
      6    SR  I    SeconRandomnu
                       2nd random number
      7    TR  I    ThirdRandomnu
                       3rd random number
  */
  edm4hep::MutableEventHeader eh = this->event_headers.create();
  auto* eveh = this->data.eveh.load<class eveh>(true);
  auto* revh = this->data.eveh.load<object_table<class revh> >(true)->at(0);
  auto* kevh = this->data.eveh.load<object_table<class kevh> >(true)->at(0);
  eh.setRunNumber(eveh->runNumber());
  eh.setEventNumber(eveh->eventnumber());
  eh.setWeight(kevh->weighT());

  this->eveh_properties["experiment"]       = eveh->expNumber();
  this->eveh_properties["runtype"]          = eveh->runType();
  this->eveh_properties["event_date"]       = eveh->date();
  this->eveh_properties["time"]             = eveh->time();
  this->eveh_properties["trmask1"]          = eveh->mask1();
  this->eveh_properties["trmask2"]          = eveh->mask2();
  this->eveh_properties["trmask3"]          = eveh->mask3();
  this->eveh_properties["trmask4"]          = eveh->mask4();
  this->eveh_properties["event_type"]       = eveh->type();
  this->eveh_properties["event_error"]      = eveh->errorStatus();
  this->eveh_properties["total_energy"]     = eveh->totalEnergy();

  this->revh_properties["detector_status"]  = revh->detectorStatus();
  this->revh_properties["selection_bits"]   = revh->selectionBits();
  this->revh_properties["fatal_error_0"]    = revh->fatalErr()[0];
  this->revh_properties["fatal_error_1"]    = revh->fatalErr()[1];
  this->revh_properties["nonfatal_error_0"] = revh->nonfatalErr()[0];
  this->revh_properties["nonfatal_error_1"] = revh->nonfatalErr()[1];
  this->revh_properties["readout_bits_0"]   = revh->readoutBits()[0];
  this->revh_properties["readout_bits_1"]   = revh->readoutBits()[1];
  this->revh_properties["edir_class"]       = revh->readoutBits()[1];

  this->gene_properties["1rst_random"]      = kevh->firstRandomNu();
  this->gene_properties["2nd_random"]       = kevh->seconRandomnu();
  this->gene_properties["3rd_random"]       = kevh->thirdRandomnu();
  this->gene_properties["number_of_tracks"] = kevh->numberofTrac();
  this->gene_properties["number_of_vertices"] = kevh->numberofVert();
}
