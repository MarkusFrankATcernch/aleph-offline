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

/// Framework include files
#include <alpha/frtl.h>
#include <alpha/frid.h>
#include <alpha/eidt.h>
#include <alpha/hmad.h>
#include <alpha/mcad.h>
#include <alpha/muid.h>

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

    /** Add here all auxiliary track information if present.
     *  Each section is seperated by a marker indicating the data type
     */
    auto* frtl = track->frtl(); // Present for all charged tracks
    /// See 'sbank FRTL' for details
    trk.addToSubdetectorHitNumbers(charged_track_items::FRTL_DATA);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->narcV()                : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->narcI()                : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->nrEsti()               : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->narcT()                : 0);
    trk.addToSubdetectorHitNumbers(frtl ? frtl->nrestt()               : 0);
    trk.addToSubdetectorHitNumbers(0);

    auto* frid = track->frid(); // Present for all charged tracks
    /// See 'sbank FRID' for details
    trk.addToSubdetectorHitNumbers(charged_track_items::FRID_DATA);
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
    if( track->xeidt() )  {
      /// See 'sbank EIDT' for details
      const auto* eidt = track->eidt();
      trk.addToSubdetectorHitNumbers(charged_track_items::EIDT_DATA);
      trk.addToSubdetectorHitNumbers(eidt->iflag());
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r1esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r2esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r3esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r4esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r5esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r6esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(int(1000e0*eidt->r7esti())); // Do not know if a factor 1000e0 is good
      trk.addToSubdetectorHitNumbers(1000e0*eidt->ecorr());
      trk.addToSubdetectorHitNumbers(eidt->ippoth());
      trk.addToSubdetectorHitNumbers(1000e0*eidt->e1());
      trk.addToSubdetectorHitNumbers(1000e0*eidt->e2());
      trk.addToSubdetectorHitNumbers(1000e0*eidt->e3());
    }
    if( track->xhmad() )  {
      /// See 'sbank HMAD' for details
      const auto* hmad = track->hmad();
      trk.addToSubdetectorHitNumbers(charged_track_items::HMAD_DATA);
      trk.addToSubdetectorHitNumbers(hmad->nplaFired());
      trk.addToSubdetectorHitNumbers(hmad->nplaExp());
      trk.addToSubdetectorHitNumbers(hmad->nplaLast());
      trk.addToSubdetectorHitNumbers(hmad->multHits());
      trk.addToSubdetectorHitNumbers(hmad->igeomFlag());
      trk.addToSubdetectorHitNumbers(int(1000e0*hmad->enerDep()));
      trk.addToSubdetectorHitNumbers(int(1000e0*hmad->ChiSquare()));
      trk.addToSubdetectorHitNumbers(hmad->numbDeg());
      trk.addToSubdetectorHitNumbers(hmad->iexpbmap());
      trk.addToSubdetectorHitNumbers(hmad->itruebmap());
      trk.addToSubdetectorHitNumbers(hmad->idenFlag());
    }
    if( track->xmcad() )  {
      /// See 'sbank MCAD' for details
      const auto* mcad = track->mcad();
      trk.addToSubdetectorHitNumbers(charged_track_items::MCAD_DATA);
      trk.addToSubdetectorHitNumbers(mcad->nasshit());
      trk.addToSubdetectorHitNumbers(int(1000e0*mcad->distHit()));
      trk.addToSubdetectorHitNumbers(int(1000e0*mcad->distCut()));
      trk.addToSubdetectorHitNumbers(int(1000e0*mcad->angMin()));
      trk.addToSubdetectorHitNumbers(int(1000e0*mcad->angCut()));
    }
    if( track->xmuid() )  {
      /// See 'sbank MUID' for details
      const auto* muid = track->muid();
      trk.addToSubdetectorHitNumbers(charged_track_items::MUID_DATA);
      trk.addToSubdetectorHitNumbers(muid->idFlag());
      trk.addToSubdetectorHitNumbers(int(1000e0*muid->sumResid()));
      trk.addToSubdetectorHitNumbers(int(1000e0*muid->distMuch()));
      trk.addToSubdetectorHitNumbers(int(1000e0*muid->shadTrack()));
    }
  }
}

