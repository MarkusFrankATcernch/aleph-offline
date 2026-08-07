//==========================================================================
//  Software for the ALEPH experiment
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see ALSOFTINSTALL/LICENSE.
// For the list of contributors see ALSOFTINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================
#ifndef ALPHA_TESTS_H
#define ALPHA_TESTS_H

/// C/C++ include files
#include <cstdint>
#include <alpha/track.h>

namespace alpha  {

  namespace tests  {
    
    void print_charged_track_relations( bool print, int32_t first, int32_t last );
    /// Verify track-mother relationships
    void print_charged_track_mothers(int32_t itk);
    /// Verify track-daughter relationships
    void print_charged_track_daughters(int32_t itk);
    /// Check links from QVEC to other banks (1)
    void print_charged_track_opts1(int32_t itk);
    /// Check links from QVEC track fit parameters
    void print_charged_track_trackfit(int32_t itk);
    /// Check links from QVEC to ECAL/HCAL banks
    void print_charged_track_calo(int32_t itk);

    /// Print attributes for a range of vertices
    void print_vertex_relations( bool print, int32_t first, int32_t last );
    /// Print vertex attributes
    void print_vertex( int32_t ivx );

    
    void print_mc_tracks(bool print);
    void print_mc_track(int32_t itk);
    
    void print_charged_tracks( bool print, int32_t first, int32_t last );
    void check_qcde_alignment( bool print );
    void access_bank_lists(bool print);
    void print_bank_lists(bool print);

    /// Loop and print PECO objects
    void print_peco(bool print);
    /// Loop and print PHCO objects
    void print_phco(bool print);

    void process_event();
    void initialize();
    void finalize();
  }
}

extern "C"  {
  void f77_print_qcde_();
  void f77_print_tracks_(const uint32_t& first, const uint32_t& last );
  void f77_print_track_(const uint32_t& which );
  void f77_print_peco_(const uint32_t& which );
  void f77_print_phco_(const uint32_t& which );
  void f77_print_vertex_1_(const uint32_t& ivx);
  void f77_print_vertex_2_(const uint32_t& ivx);

  void f77_print_track_mothers_(const uint32_t& which );
  void f77_print_track_daughters_(const uint32_t& which );
  void f77_print_loop_vars_(const char*, const char*, const uint32_t&, const uint32_t&, const uint32_t&);

  void f77_print_track_opts1_(const uint32_t& itk);
  void f77_print_track_trackfit_(const uint32_t& itk);
  void f77_print_track_ecal_(const uint32_t& itk);
  void f77_print_track_hcal_(const uint32_t& itk);
}

#endif // ALPHA_TESTS_H
