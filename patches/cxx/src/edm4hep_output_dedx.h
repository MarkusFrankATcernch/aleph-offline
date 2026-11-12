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

extern "C"  {
  void qdedx_(int32_t& itk, int32_t& nhyp, float* mass, float* charge,
              float& trunc_mean, int32_t& nsamples, float& useful_length,
              float* expected_ionization,
              float* err_ionization,
              int32_t* return_code);
  void qpadx_(int32_t& itk, int32_t& nhyp, float* mass, float* charge,
              float& ionization, int32_t& npads, float& useful_length,
              float* expected_ionization,
              float* err_ionization,
              int32_t* return_code);
}

/// Production output Tpc track wire dE/dX from QDEDX
void alpha::edm4hep_output::event_t::process_dedx_wires()  {
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    auto*        track = this->data.qvec.row<class qvec>(itk);
    int32_t frft_track = track->ktn();
    int32_t nhyp                {   1 };
    float   mass[1]           = { 0e0 };
    float   charge[1]         = { 0e0 };
    float   ionization[1]     = { 0e0 };
    float   err_ionization[1] = { 0e0 };
    float   truncated_mean    =   0e0;
    float   useful_length     =   0e0;
    int32_t num_samples       =     0;
    int32_t return_code       =     0;
    ::qdedx_( itk, nhyp, mass, charge,
              truncated_mean, num_samples, useful_length,
              ionization, err_ionization, &return_code );
    auto reco_track = this->particle_frft(frft_track);
    auto sample = _dedx(return_code + (DEDX_NUM_SAMPLES>>4),    num_samples, 0e0);
    _add_dedx(this->dedx_wire_num_samples, reco_track, sample);
    auto length = _dedx(return_code + (DEDX_TRACKLENGTH>>4),    useful_length, 0e0);
    _add_dedx(this->dedx_wire_tracklen,    reco_track, length);
    auto mean   = _dedx(return_code + (DEDX_TRUNCATED_MEAN>>4), truncated_mean, 0e0);
    _add_dedx(this->dedx_wire_trunc_mean,    reco_track, mean);
    auto ionize = _dedx(return_code + (DEDX_IONIZATION>>4),     ionization[0], err_ionization[0]);
    _add_dedx(this->dedx_wire_ionization,    reco_track, ionize);
  }
}

/// Production output Tpc track wire dE/dX from QPADX
void alpha::edm4hep_output::event_t::process_dedx_pads()  {
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    auto*        track = this->data.qvec.row<class qvec>(itk);
    int32_t frft_track = track->ktn();
    int32_t nhyp                {   1 };
    float   mass[1]           = { 0e0 };
    float   charge[1]         = { 0e0 };
    float   ionization[1]     = { 0e0 };
    float   err_ionization[1] = { 0e0 };
    float   truncated_mean    =   0e0;
    float   useful_length     =   0e0;
    int32_t num_pads          =     0;
    int32_t return_code       =     0;
    ::qpadx_( itk, nhyp, mass, charge,
              truncated_mean, num_pads, useful_length,
              ionization, err_ionization, &return_code );
    auto reco_track = this->particle_frft(frft_track);
    auto sample = _dedx(return_code + (DEDX_NUM_SAMPLES>>4),    num_pads, 0e0);
    _add_dedx(this->dedx_pads_num_pads, reco_track, sample);
    auto length = _dedx(return_code + (DEDX_TRACKLENGTH>>4),    useful_length, 0e0);
    _add_dedx(this->dedx_pads_tracklen,    reco_track, length);
    auto mean   = _dedx(return_code + (DEDX_TRUNCATED_MEAN>>4), truncated_mean, 0e0);
    _add_dedx(this->dedx_pads_trunc_mean,    reco_track, mean);
    auto ionize = _dedx(return_code + (DEDX_IONIZATION>>4),     ionization[0], err_ionization[0]);
    _add_dedx(this->dedx_pads_ionization,    reco_track, ionize);
  }
}
