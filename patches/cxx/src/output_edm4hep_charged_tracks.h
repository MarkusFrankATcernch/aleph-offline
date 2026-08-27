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

/// Convert charged tracks. They enter the catalog with their FRFT row number
void alpha::output_edm4hep::event_t::process_charged_tracks()  {
  this->alpha2edm4hep_charged.clear();
  if( this->data.debug_charged_tracks )  {
  std::cout << "+++ Charged Tracks: KFCHT=" << qcde.KFCHT << " KLCHT:" << qcde.KLCHT << std::endl;
  }
  for( int itk = qcde.KFCHT; itk <= qcde.KLCHT; ++itk )  {
    std::size_t key   = this->particles_reco.size();
    auto*       track = this->data.qvec.row<class qvec>(itk);
    auto*       frft  = track->frft();
    if( !frft )  {
      throw std::runtime_error("Error: No FRFT bank present");
    }
    int32_t frft_track = track->ktn();
    auto    trk = this->particles_reco.create();

    this->alpha2edm4hep_charged[frft_track] = key;
    trk.setType(track->kclass());
    trk.setNholes(0);                // ???
    trk.setChi2(frft->chis2());      // Should go to track state ??
    trk.setNdf(frft->numDegFree());  // Should go to track state ??

    if( this->data.debug_charged_tracks )  {
      std::cout << track->to_string() << std::endl;
      // std::cout << ">>> FRFT track:" << frft_track << " Key:" << key << std::endl;
    }
    edm4hep::TrackState state;
    state.location       = edm4hep::TrackState::AtIP;
    state.time           = 0e0;
    state.D0             = frft->d0();
    state.Z0             = frft->z0();
    state.phi            = frft->phi0();
    state.omega          = frft->inverseRadi();
    state.tanLambda      = frft->tanLambda();
    state.referencePoint = { 0e0, 0e0, 0e0 };
    for( uint32_t i=0; i<21; ++i )
      state.covMatrix.values[i] = frft->ecovarM()[i];
    trk.addToTrackStates(state);
  
    auto* frtl = track->frtl();
    trk.addToSubdetectorHitNumbers(frtl ? frtl->narcV()                : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->narcI()                : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->nrEsti()               : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->narcT()                : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->nrestt()               : 0);
    trk.addToSubdetectorHitNumbers(0);

    auto* frid = track->frid();
    trk.addToSubdetectorHitNumbers(frid ? frid->bitPat()               : 0);
    trk.addToSubdetectorHitNumbers(frid ? frid->deadZone()             : 0);
    trk.addToSubdetectorHitNumbers(frid ? frid->bitpatC()              : 0);
    trk.addToSubdetectorHitNumbers(frid ? frid->deadzoneC()            : 0);
    trk.addToSubdetectorHitNumbers(frid ? int(frid->probElec()*1000)   : 0);
    trk.addToSubdetectorHitNumbers(frid ? int(frid->probMuon()*1000)   : 0);
    trk.addToSubdetectorHitNumbers(frid ? int(frid->probpIon()*1000)   : 0);
    trk.addToSubdetectorHitNumbers(frid ? int(frid->probKaon()*1000)   : 0);
    trk.addToSubdetectorHitNumbers(frid ? int(frid->probProton()*1000) : 0);
    trk.addToSubdetectorHitNumbers(frid ? int(frid->noKinkprob()*1000) : 0);
    trk.addToSubdetectorHitNumbers(frid ? frid->qualityFlag()          : 0);
  }
}

