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

/// Framework include files
#include <alpha/alpha.h>
#include <alpha/qcde.h>
#include <alpha/qvec.h>
#include <alpha/qdet.h>
#include <alpha/qvrt.h>
#include <alpha/frft.h>
#include <alpha/frtl.h>
#include <alpha/frid.h>
#include <alpha/texs.h>
#include <alpha/eidt.h>
#include <alpha/hmad.h>
#include <alpha/mcad.h>
#include <alpha/muid.h>
#include <alpha/peco.h>
#include <alpha/phco.h>
#include <alpha/pgac.h>
#include <alpha/pcqa.h>
#include <alpha/pmlt.h>
#include <alpha/pdlt.h>


/// C/C++ include files
#include <cstring>

extern "C" void get_particle_name_(const int32_t& pcode, int32_t& len, char* text, int32_t siz);

/// Track's particle name (see ALPHA user's manual for details)
std::string alpha::qvec::cqtpn() const   {
  int32_t len = 0;
  char text[128];
  get_particle_name_(this->particle_code, len, text, int(sizeof(text)));
  text[len] = 0;
  return text;
}

/// Particle class (see ALPHA user's manual for details)
int32_t alpha::qvec::kclass()  const    {
  return qcde.KCLACO[this->track_class-1];
}

/// Direct access to the charged daughter track
const class alpha::qvec* alpha::qvec::daughter(uint32_t i)  const {
  return params.qvec_table->row(this->kdau(i));
}

/// Direct access to the charged mother track
const class alpha::qvec* alpha::qvec::mother(uint32_t i)  const {
  return params.qvec_table->row(this->kmoth(i));
}

/// Error squared on total momentum from particle fit
float alpha::qvec::qsigp2()   const   {
  return
    (this->qx2()*this->qsmat(0) + this->qy2()*this->qsmat(2) + this->qz2()*this->qsmat(5) +
     2e0*(this->px*(this->py*this->qsmat(1) + this->pz*qsmat(3)) + this->py*this->pz*this->qsmat(4))) / this->qp2();
}

/// Error squared on mass from particle fit
float alpha::qvec::qsigm2()   const   {
  /*
    QSIGMM(KI)=
    QM(KI) * (QE(KI)**2*QSMAT(KI,9)+QX(KI)**2*QSMAT(KI,0)+ QY(KI)**2*QSMAT(KI,2)+QZ(KI)**2*QSMAT(KI,5) +
    2.*(QX(KI)* (QY(KI)*QSMAT(KI,1) + QZ(KI)*QSMAT(KI,3)) +
    QY(KI)*QZ(KI)*QSMAT(KI,4) -
    QE(KI)* (QX(KI)*QSMAT(KI,6) + QY(KI)*QSMAT(KI,7) + QZ(KI)*QSMAT(KI,8))))
    / AMAX1(QM(KI)**3,1.E-16)
  */
  float m = this->mass;
  float sig =
    m * (this->qe2()*this->qsmat(9) + this->qx2()*this->qsmat(0) +
         this->qy2()*this->qsmat(2) + this->qz2()*this->qsmat(5) +
         2e0 * (this->px*(this->py*this->qsmat(1) + this->pz*this->qsmat(3)) +
                this->py*this->pz*this->qsmat(4) -
                this->energy*(this->px*this->qsmat(6) + this->py*this->qsmat(7) + this->pz*this->qsmat(8))))
    / std::max(m*m*m, float(1e-16));
  return std::abs(sig);
}

/// Get string representation of this track
std::string alpha::qvec::to_string(uint32_t /* flags */)  const {
  const auto* table = alpha::get_qvec();
  int32_t which = table->index(this)+1;
  const auto* track = this;
  if( this != table->row(which) )  {
    throw std::runtime_error("Bad index calculation!!!!");
  }
  char text[4096];
  std::size_t siz = sizeof(text);
  std::size_t len = ::snprintf(text, siz, "qvec[%3d] ", which);
  len += ::snprintf(text+len, siz-len, "ktn:%2d ", track->ktn());
  len += ::snprintf(text+len, siz-len, "%12s ", track->cqtpn().c_str());
  len += ::snprintf(text+len, siz-len, "%s", fmt_ene(track->px).c_str());
  len += ::snprintf(text+len, siz-len, "%s", fmt_ene(track->py).c_str());
  len += ::snprintf(text+len, siz-len, "%s", fmt_ene(track->pz).c_str());
  len += ::snprintf(text+len, siz-len, "%s", fmt_ene(track->mass).c_str());
  len += ::snprintf(text+len, siz-len, "e:%7.2f #m:%2d #d:%2d ",
                    track->energy, track->knmoth(), track->kndau() );

  if( which >= qcde.KFMCT && which <= qcde.KLMCT )  {
    len += ::snprintf(text+len, siz-len, "[MC] FKIN:%3d ", track->ktn());
    const auto* org = track->origin_vtx();
    const auto* end = track->end_vtx();
    if( org )  {
      len += ::snprintf(text+len, siz-len, "Org:(");
      len += ::snprintf(text+len, siz-len, "%s", fmt_len(org->x).c_str());
      len += ::snprintf(text+len, siz-len, "%s", fmt_len(org->y).c_str());
      len += ::snprintf(text+len, siz-len, "%s", fmt_len(org->z).c_str());
      len += ::snprintf(text+len, siz-len, ") typ:%2d in:%3d #out:%2d #:%3d ",
                        org->kvtype(), org->kvincp(), org->kvndau(), org->kvn());
    }
    if( end )  {
      len += ::snprintf(text+len, siz-len, "End:(");
      len += ::snprintf(text+len, siz-len, "%s", fmt_len(end->x).c_str());
      len += ::snprintf(text+len, siz-len, "%s", fmt_len(end->y).c_str());
      len += ::snprintf(text+len, siz-len, "%s", fmt_len(end->z).c_str());
      len += ::snprintf(text+len, siz-len, ") typ:%2d in:%3d #out:%2d #:%3d ",
                        end->kvtype(), end->kvincp(), end->kvndau(), end->kvn());
    }
    len += ::snprintf(text+len, siz-len, "Dau: ");
    for( uint32_t id=0; id < track->kndau(); ++id )  {
      len += ::snprintf(text+len, siz-len, "%d ", track->kdau(id));
    }
  }
  const class qdet* pdet = track->qdet();
  if( pdet )  {
    if( which >= qcde.KFIST && which <= qcde.KLAST )  {
      const char* tag = (which >= qcde.KFIST && which <= qcde.KLIST)
        ? "[CALO non-assoc] " : "[CALO associated]";
      len += ::snprintf(text+len, siz-len, "%s ", tag);
      len += ::snprintf(text+len, siz-len, "NFRFT:%2d ", track->knchgd());
      for(uint32_t ich=0; ich < track->knchgd(); ++ich )  {
        const auto* trk = track->charged_track(ich);
        auto        row = track->charged_track_rownum(ich);
        len += ::snprintf(text+len, siz-len, "%2d/%2d ", row, trk->ktn());
      }
      len += ::snprintf(text+len, siz-len, "KNEC:%2d ", track->knecal());
      for(uint32_t iec=0; iec < track->knecal(); ++iec )  {
        const auto* ec_clu = track->peco(iec);
        auto        ec_row = track->peco_rownum(iec);
        len += ::snprintf(text+len, siz-len, "%2d/%.2f ", ec_row, ec_clu->ecorr());
      }
      len += ::snprintf(text+len, siz-len, "KNHC:%2d ", track->knhcal());
      for(uint32_t ihc=0; ihc < track->knhcal(); ++ihc )  {
        const auto* ec_clu = track->phco(ihc);
        auto        ec_row = track->phco_rownum(ihc);
        len += ::snprintf(text+len, siz-len, "%2d/%.2f ", ec_row, ec_clu->ecorr());
      }
    }
    else if( which >= qcde.KFCHT && which <= qcde.KLCHT )  {
      len += ::snprintf(text+len, siz-len, "[CH] sigm2:%6.2f sige:%6.2f sigp:%6.2f ",
                        track->qsigm2(), track->qsige(), track->qsigp());
    }
    else  {
      len += ::snprintf(text+len, siz-len, "[QVEC] ");
    }

    if( track->xfrft() )  {
      const class frft* pfrft = pdet->frft();
      len += ::snprintf(text+len, siz-len, "FRFT:%08lX ",uint64_t(pfrft));
      len += ::snprintf(text+len, siz-len, "ktn:%2d d0:%7.2f z0:%7.2f ",
                        track->ktn(), pfrft->d0(), pfrft->z0());
    }
    if( track->xfrtl() )  {
      const class frtl* pfrtl = pdet->frtl();
      len += snprintf(text+len, sizeof(text)-len, "FRTL:%08lX ",uint64_t(pfrtl));
      len += snprintf(text+len, sizeof(text)-len, "%2d %1d %2d ",
                      pfrtl->narcV(), pfrtl->narcI(), pfrtl->narcT());
    }
    if( track->xfrid() )  {
      const class frid* pfrid = pdet->frid();
      len += snprintf(text+len, sizeof(text)-len, "FRID:%08lX ",uint64_t(pfrid));
      len += snprintf(text+len, sizeof(text)-len, "e-:%4.3f pi:%4.3f ",
                      pfrid->probElec(), pfrid->probpIon());
    }
    if( track->xeidt() )  {
      const auto* eidt = track->eidt();
      len += ::snprintf(text+len, sizeof(text)-len, "EIDT:%08lX ", uint64_t(eidt));
      len += ::snprintf(text+len, sizeof(text)-len, "t:%2d ec:%2d ", eidt->frFT(), eidt->peCO());
    }
    if( track->xhmad() )  {
      const auto* hmad = track->hmad();
      len += ::snprintf(text+len, sizeof(text)-len, "HMAD:%08lX ", uint64_t(hmad));
      len += ::snprintf(text+len, sizeof(text)-len, "t:%2d pl:%2d ",
                        hmad->trackNo(), hmad->nplaFired());
    }
    if( track->xmcad() )  {
      const auto* mcad = track->mcad();
      len += ::snprintf(text+len, sizeof(text)-len, "MCAD:%08lX ", uint64_t(mcad));
      len += ::snprintf(text+len, sizeof(text)-len, "t:%2d nh:%2d ",
                        mcad->trackNo(), mcad->nassHit()[0]);
    }
    if( track->xmuid() )  {
      const auto* muid = track->muid();
      len += ::snprintf(text+len, sizeof(text)-len, "MUID:%08lX ", uint64_t(muid));
      len += ::snprintf(text+len, sizeof(text)-len, "t:%2d id:%2d ",
                        muid->trackNumber(), muid->idFlag());
    }

    if( pdet->xtexs() )  {
      len += ::snprintf(text+len, sizeof(text)-len, "TEXS:%2d ", pdet->kntexs() );
      for(uint32_t i=0; i<pdet->kntexs(); ++i)  {
        const class texs* ptexs = pdet->texs(i);
        len += ::snprintf(text+len, siz-len, "%d: SEG:%2d #S:%2d T:%2d ",
                          i, ptexs->segmentId(), ptexs->numberSamples(), ptexs->trackNumber());
      }
    }
    //
    if( pdet->xpgac() )  {
      const auto* pgac = pdet->pgac();
      float ene = pgac->energyCorrected();
      len += ::snprintf(text+len, siz-len, "PGAC:%08lX e:", uint64_t(pgac));
      len += ::snprintf(text+len, siz-len, "%s", fmt_ene(ene).c_str());
    }
    if( pdet->xpdlt() )  {
      const auto* pdlt = pdet->pdlt();
      len += ::snprintf(text+len, siz-len, "PDLT:%08lX typ:", uint64_t(pdlt));
      len += ::snprintf(text+len, siz-len, "%2d ", pdlt->particletype());
    }
    if( pdet->xpmlt() )  {
      const auto* pmlt = pdet->pmlt();
      len += ::snprintf(text+len, siz-len, "PMLT:%08lX fkin:", uint64_t(pmlt));
      len += ::snprintf(text+len, siz-len, "%2d ", pmlt->kineTrack());
    }
  }
  return { text };
}
