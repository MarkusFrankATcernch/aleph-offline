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
#include <tests/tests.h>

#include <alpha/defs.h>
#include <alpha/qcde.h>
#include <bos/bosbank.h>

#include <alpha/qvec.h>
#include <alpha/eidt.h>
#include <alpha/hmad.h>
#include <alpha/mcad.h>
#include <alpha/muid.h>
#include <alpha/peco.h>
#include <alpha/phco.h>
#include <alpha/pgac.h>
#include <alpha/pcqa.h>

#include <alpha/track.h>
#include <alpha/calo_object.h>


/// C/C++ include files
#include <cstdio>
#include <set>

using bos77::bcs;

namespace {
  std::string hex_addr(const void* addr)  {
    char text[32];
    if( addr )
      ::snprintf(text, sizeof(text), "%16p", addr);
    else
      ::snprintf(text, sizeof(text), "%-6s", "-----");
    return { text };
  }
  void print_track(int which, const char* prefix = nullptr)  {
    const auto* track  = alpha::params.qvec_table->at(which);
    const auto* pgac  = track->xpgac() ? track->pgac() : nullptr;
    const auto* pdlt  = track->xpdlt() ? track->pdlt() : nullptr;
    const auto* pmlt  = track->xpmlt() ? track->pmlt() : nullptr;
    if( prefix ) ::printf("%s", prefix);
    ::printf("[%3d] %12s px:%7.2f py:%7.2f pz:%7.2f m:%7.1f e:%7.2f #m:%2d #d:%2d ",
             which, track->cqtpn().c_str(), track->px, track->py, track->pz,
             track->mass*1000e0, track->energy,
             track->knmoth(), track->kndau() );
    // Only valid in GAT section: KFGAT, KLGAT, KNGAT
    ::printf("PGAC:%c/%s ", true_false(track->xpgac()), hex_addr(pgac).c_str());
    ::printf("PDLT:%c/%s ", true_false(track->xpdlt()), hex_addr(pdlt).c_str());
    ::printf("PMLT:%c/%s ", true_false(track->xpmlt()), hex_addr(pmlt).c_str());
  }
}

void alpha::tests::initialize()  {
  ::printf("+++++++  Calling %s\n!", __FUNCTION__);
}

void alpha::tests::finalize()  {
  ::printf("+++++++  Calling %s\n!", __FUNCTION__);
}

void alpha::tests::check_qcde_alignment( bool print )  {
  using namespace alpha;
  if( print )  {
    printf("++++ Check loop variables (QCDE struct/common alignment)\n");
    f77_print_qcde_();
    f77_print_loop_vars_("C++", "CHT", qcde.KNCHT, qcde.KFCHT, qcde.KLCHT);
    f77_print_loop_vars_("C++", "JET", qcde.KNJET, qcde.KFJET, qcde.KLJET);
    f77_print_loop_vars_("C++", "FRT",         -1, qcde.KFFRT, qcde.KLFRT);
    f77_print_loop_vars_("C++", "EHT", qcde.KNEHT, qcde.KFEHT, qcde.KLEHT);
    f77_print_loop_vars_("C++", "IST", qcde.KNIST, qcde.KFIST, qcde.KLIST);
    f77_print_loop_vars_("C++", "AST", qcde.KNAST, qcde.KFAST, qcde.KLAST);
    f77_print_loop_vars_("C++", "V0T", qcde.KNV0T, qcde.KFV0T, qcde.KLV0T);
    f77_print_loop_vars_("C++", "NET", qcde.KNNET, qcde.KFNET, qcde.KLNET);
    f77_print_loop_vars_("C++", "OVT", qcde.KNOVT, qcde.KFOVT, qcde.KLOVT);
    f77_print_loop_vars_("C++", "REV", qcde.KNREV, qcde.KFREV, qcde.KLREV);
    f77_print_loop_vars_("C++", "MCT", qcde.KNMCT, qcde.KFMCT, qcde.KLMCT);
    f77_print_loop_vars_("C++", "MCV", qcde.KNMCV, qcde.KFMCV, qcde.KLMCV);
    f77_print_loop_vars_("C++", "GAT", qcde.KNGAT, qcde.KFGAT, qcde.KLGAT);
    f77_print_loop_vars_("C++", "EFT", qcde.KNEFT, qcde.KFEFT, qcde.KLEFT);
  }
}

void alpha::tests::print_bank_lists(bool print)  {
  if( print )  {
    bos77::print_bank_lists(bos77::bos_bank_lists);
    ::printf("\n");
  }
}

void alpha::tests::access_bank_lists(bool print)  {
  if( print )  {
    for( int h=0; h<5; ++h )  {
      char bank_list = bos77::bos_bank_lists[h];
      std::string names;
      std::size_t total_mem = 0;

      names.reserve(255);
      ::printf(" +++ %s: BANKLIST: %c\n", __FUNCTION__, bank_list);
      for( int i = 1; ; ++i )  {
        std::string bnam = bos77::nlistb(i, bank_list);
        if( bnam.empty() )  {
          break;
        }
        for( int j=1; ; ++j )  {
          auto* bank = bos77::get_bank(bnam, j);
          if( !bank )  {
            break;
          }
          total_mem += bank->total_num_words();
          names += bnam;
          names += " ";
          ::printf("%-8ld %p  %s\n",
                   ((uint8_t*)bank) - (uint8_t*)bos77::bcs.iw,
                   (void*)bank, bank->to_string().c_str());
        }
      }
      ::printf(" +++ %s: BANKLIST: %c  %ld words\n\n", __FUNCTION__, bank_list, total_mem);
    }
  }
}

void alpha::tests::print_charged_tracks( bool print, int32_t first, int32_t last )  {
  if( print )  {
    auto*   native = params.qvec_table;
    auto&   table  = object_table<qvec, alphatrack>::get(params.naqvec);
    int32_t kqvec  = bcs.iw[params.naqvec];
    int32_t kcqvec = bcs.iw[kqvec]; // Indices in C start with 0: F77: IW(KOQVEC+1)
    int32_t koqvec = kqvec + bos77::subheader_words - kcqvec;

    ::printf("Check charged track container: first %d last %d count %d\n", first, last, last-first+1 );
    ::printf("C++ Tracks       "
             "BCS  px      py        pz   ||    "
             "object_table<qvec,track>     ||    "
             "object_table<qvec>            ||    "
             "Fortran tracks \n");
    for(int32_t i=first; i <= last; ++i )  {
      int   id = koqvec + i*kcqvec;
      auto  t  = table.at(i);
      auto  p  = native->at(i);
      float qx = bcs.rw[id + qvec::JQVEQX];
      float qy = bcs.rw[id + qvec::JQVEQY];
      float qz = bcs.rw[id + qvec::JQVEQZ];
      ::fprintf(stdout,
                "C++ Track[%3d] %9.3f %9.3f %9.3f || %9.3f  %9.3f  %9.3f || %9.3f  %9.3f  %9.3f  ||    ",
                i, qx, qy, qz, t.px(), t.py(), t.pz(), p->px, p->py, p->pz );
      ::fflush(stdout);
      f77_print_track_(i);
    }
  }
}

namespace alpha  {
  /// Access PECO objects as calo_object entities
  using calo_peco = object_table<peco, calo_object<peco> >;
  /// Access PECO objects as calo_object entities
  using calo_ecal = object_table<peco, ecal_object<peco> >;
  
  /// Access PHCO objects as calo_object entities
  using calo_phco = object_table<phco, calo_object<phco> >;
  /// Access PHCO objects as calo_object entities
  using calo_hcal = object_table<phco, hcal_object<phco> >;
}

/// Loop and print PECO objects
void alpha::tests::print_peco(bool print)  {
  if( print )  {
    auto& peco = alpha::calo_peco::get("PECO");
    auto& ecal = alpha::calo_ecal::get("PECO");
    for(uint32_t i=1; i <= peco.size(); ++i )   {
      auto hit = peco.at(i);
      auto ec  = ecal.at(i);
      ::printf("+++ PECO %-3d energy:%7.3f corr:%7.3f theta: %4.2f phi: %4.2f"
               " region:%1d corr:%4d rel:%02X id: %6d stack1:%7.3f stack2:%7.3f ",
               i, hit.raw(), hit.energy(), hit.theta(), hit.phi(),
               hit.region_code(), hit.correction_code(), hit.relations(),
               hit.object_id(), ec.stack1(), ec.stack2());
      f77_print_peco_(i);
    }
  }
}

/// Loop and print PHCO objects
void alpha::tests::print_phco(bool print)  {
  if( print )  {
    auto& phco = alpha::calo_phco::get("PHCO");
    auto& hcal = alpha::calo_hcal::get("PHCO");
    for(uint32_t i=1; i <= phco.size(); ++i )   {
      auto hit = phco.at(i);
      auto hc  = hcal.at(i);
      ::printf("+++ PHCO %-3d energy:%7.3f corr:%7.3f theta: %4.2f phi: %4.2f"
               " region:%1d corr:%4d rel:%02X id: %6d noise:%02X\n",
               i, hit.raw(), hit.energy(), hit.theta(), hit.phi(),
               hit.region_code(), hit.correction_code(), hit.relations(),
               hit.object_id(), hc.noise_flag());
      f77_print_phco_(i);
    }
  }
}

/// Verify track-mother relationships
void alpha::tests::print_charged_track_mothers(int32_t itk)  {
  const auto* track = params.qvec_table->at(itk);
  f77_print_track_mothers_(itk);
  if( track->knmoth() > 0 )  {
    /// Mothers ?? Only for MC tracks
    // KNMOTH(KI)    = IW(KOQVEC+KI*KCQVEC+JQVENO);  --> qvec::number_mothers
    //                 IW(KOQVEC+KI*KCQVEC+JQVEOL);  --> qvec::offset first mother
    // KMOTH(KI,KI1) = IW(KOQLIN+KI1 + IW(KOQVEC+KI*KCQVEC+JQVEOL));
    print_track(itk, "C++ Track ");
    if( track->knmoth() > 0 ) ::printf(" mothers: ");
    for(uint32_t im=0; im < track->knmoth(); ++im )  {
      auto* m = track->mother(im);
      ::printf(" #mot:%1d  trk:%3d %7.3f %7.3f %7.3f ",
               track->knmoth(), track->kmoth(im), m->px, m->py, m->pz);
    }
    ::printf("\n");
  }
}

/// Verify track-daughter relationships
void alpha::tests::print_charged_track_daughters(int32_t itk)  {
  const auto* track = params.qvec_table->at(itk);
  f77_print_track_daughters_(itk);
  if( track->kndau() > 0 )  {
    printf("C++ Track[%3d] daughters %6.3f %7.3f %7.3f #ndau:%1d ",
           itk, track->px, track->py, track->pz, track->kndau());
    for(uint32_t idau=0; idau < track->kndau(); ++idau )  {
      auto* dau = track->daughter(idau);
      ::printf(" trk:%3d %7.3f %7.3f %7.3f ", track->kdau(idau), dau->px, dau->py, dau->pz);
    }
    ::printf("\n");
  }
}

#include <alpha/qdet.h>
#include <alpha/frft.h>
#include <alpha/frtl.h>
#include <alpha/frid.h>
#include <alpha/texs.h>

/// Check links from QVEC to other banks (1)
void alpha::tests::print_charged_track_opts1(int32_t itk)  {
  const auto* track = params.qvec_table->at(itk);
  // KTN(KI)=IW(KOQVEC+KI*KCQVEC+JQVETN)
  //              v         v        =  8    --> qvec::frft_row
  //
  const class qdet* pqdet = track->qdet();
  //
  const class frtl* pfrtl = pqdet->frtl();
  // Coordinates: 
  //     KFRTNV(KI)=IW(IW(KJQDET(KI)+JQDEAL)+JFRTNV)
  //     KFRTNI(KI)=IW(IW(KJQDET(KI)+JQDEAL)+JFRTNI)
  //     KFRTNE(KI)=IW(IW(KJQDET(KI)+JQDEAL)+JFRTNE)
  //     KFRTNT(KI)=IW(IW(KJQDET(KI)+JQDEAL)+JFRTNT)
  //     KFRTNR(KI)=IW(IW(KJQDET(KI)+JQDEAL)+JFRTNR)
  const class frft* pfrft = pqdet->frft();
  //     QFRIPE(KI)=RW(IW(KJQDET(KI)+JQDEFI)+JFRIPE)
  //     QFRIPM(KI)=RW(IW(KJQDET(KI)+JQDEFI)+JFRIPM)
  //     QFRIPI(KI)=RW(IW(KJQDET(KI)+JQDEFI)+JFRIPI)
  //     QFRIPK(KI)=RW(IW(KJQDET(KI)+JQDEFI)+JFRIPK)
  //     QFRIPP(KI)=RW(IW(KJQDET(KI)+JQDEFI)+JFRIPP)
  //     QFRINK(KI)=RW(IW(KJQDET(KI)+JQDEFI)+JFRINK)
  const class frid* pfrid = pqdet->frid();
  //     XTEX(KI)=IW(KJQDET(KI)+JQDENT).NE.0
  //     KNTEX(KI)=IW(KJQDET(KI)+JQDENT)
  //     KTEXSI(KI,KI1)=IW(IW(KJQDET(KI)+KI1+JQDENT)+JTEXSI)
  //     QTEXTM(KI,KI1)=RW(IW(KJQDET(KI)+KI1+JQDENT)+JTEXTM)
  //     QTEXTL(KI,KI1)=RW(IW(KJQDET(KI)+KI1+JQDENT)+JTEXTL)
  //     KTEXNS(KI,KI1)=IW(IW(KJQDET(KI)+KI1+JQDENT)+JTEXNS)
  //     QTEXAD(KI,KI1)=RW(IW(KJQDET(KI)+KI1+JQDENT)+JTEXAD)
  if( pqdet->kntexs() < 0xff )  {
    printf("C++ Track[%3d] "
           "FRFT:%2d d0:%7.2f z0:%7.2f "
           "FRTL: %1d %1d %2d "
           "FRID:%p e-:%4.3f pi:%4.3f "
           "TEXS:%c %2d ",
           itk, track->ktn(), pfrft->d0(), pfrft->z0(),
           pfrtl->narcV(), pfrtl->narcI(), pfrtl->narcT(),
           (void*)pfrid, pfrid->probElec(), pfrid->probpIon(),
           true_false(pqdet->xtexs()), pqdet->kntexs() );
  }
  //
  const class texs* ptexs;
  if( pqdet->kntexs() >= 1 )  {
    ptexs = pqdet->texs(0);
    ::printf("0: seg:%2d #s:%2d ", ptexs->segmentId(), ptexs->numberSamples());
  }
  if( pqdet->kntexs() >= 2 )  {
    ptexs = pqdet->texs(1);
    ::printf("0: seg:%2d #s:%2d ", ptexs->segmentId(), ptexs->numberSamples());
  }
  if( pqdet->kntexs() >= 3 )  {
    ptexs = pqdet->texs(2);
    ::printf("0: seg:%2d #s:%2d ", ptexs->segmentId(), ptexs->numberSamples());
  }
  ::printf("\n");
  f77_print_track_opts1_(itk);
}

/// Check links from QVEC to other banks (2)
void alpha::tests::print_charged_track_opts2(int32_t itk)  {
  const auto* track = params.qvec_table->at(itk);
  const auto* eidt  = track->xeidt() ? track->eidt() : nullptr;
  const auto* hmad  = track->xhmad() ? track->hmad() : nullptr;
  const auto* mcad  = track->xmcad() ? track->mcad() : nullptr;
  const auto* muid  = track->xmuid() ? track->muid() : nullptr;
  
  /// Electron identification: Bank EIDT
  /// XEID(KI)=IW(KJQDET(KI)+JQDEAE).NE.KQZER
  /// QEIDEC(KI)=RW(IW(KJQDET(KI)+JQDEAE)+JEIDEC)

  //if( XEID(itk) == true )  {  }
      
  /// Muon − HCAL association: Bank HMAD
  /// XHMA(KI)=IW(KJQDET(KI)+JQDEAH).NE.KQZER
  //if( XHMA(itk) == true )  {  }
      
  /// Muon chamber data: Bank MCAD
  /// XMCA(KI)=IW(KJQDET(KI)+JQDEAM).NE.KQZER
  //if( XMCA(itk) == true )  {  }

  /// QMUIDO Muon Identification: Bank MUID
  /// XMUI(KI)=IW(KJQDET(KI)+JQDEMU).NE.KQZER
  //if( XMUI(itk) == true )  {  }

  ::printf("C++ Track[%3d] FRFT:%2d ", itk, track->ktn());

  ::printf("EIDT:%c/%s ", true_false(track->xeidt()), hex_addr(eidt).c_str());
  if( eidt ) ::printf("t:%2d ec:%2d ", eidt->frFT(), eidt->peCO());
  ::printf("HMAD:%c/%s ", true_false(track->xhmad()), hex_addr(hmad).c_str());
  if( hmad ) ::printf("t:%2d pl:%2d ", hmad->trackNo(), hmad->nplaFired());
  ::printf("MCAD:%c/%s ",true_false(track->xmcad()), hex_addr(mcad).c_str());
  if( mcad ) ::printf("t:%2d nh:%2d ", mcad->trackNo(), mcad->nassHit()[0]);
  ::printf("MUID:%c/%s ", true_false(track->xmuid()), hex_addr(muid).c_str());
  if( muid ) :: printf("t:%2d id:%1X ", muid->trackNumber(), muid->idFlag());
  // Consistency checks:
  if( eidt && eidt->frFT() != track->ktn() )  {
    ::printf(" !!!!! EIDT: ERROR !!!!!");
  }
  if( hmad && hmad->trackNo() != track->ktn() )  {
    ::printf(" !!!!! HMAD: ERROR !!!!!");
  }
  if( mcad && mcad->trackNo() != track->ktn() )  {
    ::printf(" !!!!! MCAD: ERROR !!!!!");
  }
  if( muid && muid->trackNumber() != track->ktn() )  {
    ::printf(" !!!!! MUID: ERROR !!!!!");
  }
  ::printf("\n");
  f77_print_track_opts2_(itk);
}

/// Check links from QVEC track fit parameters
void alpha::tests::print_charged_track_trackfit(int32_t itk)  {
  const auto* track = params.qvec_table->at(itk);
  f77_print_track_trackfit_(itk);
  if( track->kndau() > 0 )  {
    printf("C++ Track[%3d] FRFT:%2d %s sigm:%6.2f sige:%6.2f sigp:%6.2f kndau:%2d",
           itk, track->ktn(), track->cqtpn().c_str(),
           track->qsigm2(), track->qsige(), track->qsigp(), track->kndau() );
    for(uint32_t i=0; i < track->kndau(); ++i)  {
      int idau = track->kdau(i);
      ::printf(" %2d", idau);
    }
    ::printf(" Cov:");
    for(int i=0; i < 10; ++i)  {
      ::printf("%7.2f ", track->cov[i]);
    }
    ::printf("\n");
    track = nullptr;
  }
}

/// Check links from QVEC to ECAL banks
void alpha::tests::print_charged_track_ecal(int32_t itk)  {
  //
  // KTN(KI)=IW(KOQVEC+KI*KCQVEC+JQVETN)
  //
  // KJQDET(KI)   = IW(KOQVEC+KI*KCQVEC+JQVEQD)          --> qvec::offset_qdet
  // KNECAL(KI)   = IW(KJQDET(KI)+JQDENE)                --> qdet::num_associated_ecal
  // KECAL(KI,KI1)= IW(KOQLIN+KI1+IW(KJQDET(KI)+JQDEEL)) --> 
  //                                            = 20
  const auto* track = params.qvec_table->at(itk);
  const auto* qdet  = track->qdet();
  if( qdet->knecal() > 0 )  {
    printf("C++ Track[%3d] ecal objs #nec:%1d ", itk, track->knecal());
    for(uint32_t iec=0; iec < track->knecal(); ++iec )  {
      const class peco* ec_clu = track->peco(iec);
      auto        ec_row       = qdet->peco_rownum(iec);
      const auto* ec_track     = track->ecal_track(iec);
      auto        ec_track_row = qdet->ecal_track_rownum(iec);
      ::printf("ECOBJ:%3d PECO:%3d E:%7.3f %7.3f ",
               ec_track_row, ec_row, ec_clu->ecorr(), ec_track->energy);
    }
  }
  f77_print_track_ecal_(itk);
}

/// Check links from QVEC to HCAL banks
void alpha::tests::print_charged_track_hcal(int32_t itk)  {
  //
  // KTN(KI)=IW(KOQVEC+KI*KCQVEC+JQVETN)
  //
  // KJQDET(KI)   = IW(KOQVEC+KI*KCQVEC+JQVEQD)          --> qvec::qdet_offset
  // KNHCAL(KI)   = IW(KJQDET(KI)+JQDENH)                --> qdet::num_associated_hcal
  // KHCAL(KI,KI1)= IW(KOQLIN+KI1+IW(KJQDET(KI)+JQDEHL)) --> 
  //                                            = 20
  const auto* track = params.qvec_table->at(itk);
  const auto* qdet  = track->qdet();
  if( qdet->knhcal() > 0 )  {
    printf("C++ Track[%3d] hcal objs #nhc:%1d ", itk, track->knhcal());
    for(uint32_t ihc=0; ihc < track->knhcal(); ++ihc )  {
      const auto* hc_clu       = track->phco(ihc);
      auto        hc_row       = qdet->phco_rownum(ihc);
      const auto* hc_track     = track->hcal_track(ihc);
      auto        hc_track_row = qdet->hcal_track_rownum(ihc);
      ::printf("HCOBJ:%3d PHCO:%3d E:%7.3f %7.3f ",
               hc_track_row, hc_row, hc_clu->ecorr(), hc_track->energy);
    }
  }
  f77_print_track_hcal_(itk);
}


void alpha::tests::print_charged_track_relations( bool print, int32_t first, int32_t last )  {
  if( print )  {
    std::set<int32_t> vertices;
    
    for(int32_t itk=first; itk <= last; ++itk )  {
      const auto* track  = params.qvec_table->at(itk);
      int32_t ivx_origin = track->koriv();
      int32_t ivx_end    = track->kendv();
      print_charged_track_mothers (itk);
      print_charged_track_daughters (itk);
      print_charged_track_opts1 (itk);
      print_charged_track_opts2 (itk);
      print_charged_track_trackfit (itk);
      print_charged_track_ecal (itk);
      print_charged_track_hcal (itk);
      if( ivx_origin )  {
        vertices.insert(ivx_origin);
      }
      if( ivx_end )  {
        vertices.insert(ivx_end);
      }
    }
    for( auto ivx : vertices )   {
      print_vertex( ivx );
    }
    if( !vertices.empty() )  {
      //::printf(".\n");
    }
  }
}

#include <alpha/qvrt.h>
#include <alpha/fver.h>
#include <alpha/yv0v.h>

void alpha::tests::print_vertex( int32_t ivx )  {
  const auto* vertex = params.qvrt_table->at(ivx);
  f77_print_vertex_1_(ivx);
  if( vertex )  {
    auto* yv0v = vertex->yv0v();
    auto* fver = vertex->fver();
    int k1 = yv0v ? yv0v->k1() : -1;
    int k2 = yv0v ? yv0v->k2() : -1;
    ::printf("C++ Vertex[%2d] %2d typ:%1d x:%7.2f y:%7.2f z:%7.2f yv0v:%16p fver:%16p in:%3d%3d%3d #out:%2d"
             "%8.2f%8.2f%8.2f%8.2f%8.2f%8.2f%8.2f%8.2f%8.2f",
             ivx, vertex->kvn(), vertex->kvtype(),
             vertex->qvx(), vertex->qvy(), vertex->qvz(), yv0v, fver,
             vertex->kvincp(), k1, k2, vertex->kvndau(),
             vertex->qvem(0,0), vertex->qvem(0,1), vertex->qvem(0,2), 
             vertex->qvem(1,0), vertex->qvem(1,1), vertex->qvem(1,2), 
             vertex->qvem(2,0), vertex->qvem(2,1), vertex->qvem(2,2)
             );
    ::printf("\n");
    f77_print_vertex_2_(ivx);
  }
}

void alpha::tests::print_mc_tracks(bool print)  {
  if( print )  {
    for( int32_t itk=qcde.KFMCT; itk<= qcde.KLMCT; ++itk )  {
      print_mc_track(itk);
    }
  }
}

void alpha::tests::print_mc_track(int32_t itk)  {
  const auto* track  = params.qvec_table->at(itk);
  ::printf("C++ MCT[%2d] cl:%1d %6d %-12s %6.1f %6.1f %6.1f koriv:%3d kendv:%3d ",
           itk, track->kclass(), track->ktpcod(), track->cqtpn().c_str(),
           track->qx(), track->qy(), track->qz(),
           track->koriv(), track->kendv()
           );
  if( track->koriv() )  {
    const auto* vx = track->origin_vtx();
    const auto* fver = vx->fver();
    ::printf("Koriv:%3d kvn:%3d in:%3d out:%3d %7.2f %7.2f %7.2f fver: %16p",
             track->koriv(), vx->kvn(), vx->kvincp(), vx->kvndau(), vx->x, vx->y, vx->z, fver);
    if( fver )  {
      std::string volNam(fver->volNam(), fver->volNam()+4);
      std::string mech(fver->vertexMechanism(), fver->vertexMechanism()+4);      
      ::printf(" TOf: %6.2f%s%s", fver->tof(),
               volNam[0]==' ' ? "" : (" vol:"+volNam).c_str(),
               mech[0] == ' ' ? "" : (" mech:"+mech).c_str());
    }
  }
  ::printf("\n");
}


void alpha::tests::print_vertex_relations( bool print, int32_t first, int32_t last )  {
  if( print )  {

    for(int32_t ivx=first; ivx <= last; ++ivx )  {
      print_vertex (ivx);
    }
  }
}

#include <alpha/efol.h>

void alpha::tests::process_event()  {
  ::printf("+++++++  Calling %s!\n", __FUNCTION__);
  tests::check_qcde_alignment(1);
  tests::print_mc_tracks(1);
  tests::print_bank_lists(0);
  tests::access_bank_lists(0);
  tests::print_charged_tracks(0, qcde.KFCHT, qcde.KLCHT);
  tests::print_peco(0);
  tests::print_phco(0);

  tests::print_charged_track_relations(1, qcde.KFCHT, qcde.KLCHT);
  //tests::print_vertex_relations(1, qcde.KFCHT, qcde.KLCHT);
  for(int itk=qcde.KFGAT; itk<=qcde.KLGAT; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
    const auto* pgac  = track->xpgac() ? track->pgac() : nullptr;
    const auto* pdlt  = track->xpdlt() ? track->pdlt() : nullptr;
    const auto* pmlt  = track->xpmlt() ? track->pmlt() : nullptr;
    // Only valid in GAT section: KFGAT, KLGAT, KNGAT
    print_track(itk, "C++  Photon");
    ::printf("PGAC:%c/%s ", true_false(track->xpgac()), hex_addr(pgac).c_str());
    if( pgac ) :: printf("p:%2d ec:%3d ", pgac->parentNumber(), pgac->peCO());
    ::printf("PDLT:%c/%s ", true_false(track->xpdlt()), hex_addr(pdlt).c_str());
    if( pdlt ) :: printf("jet:%3d tr:%3d ", pdlt->jeTnumber(), pdlt->frFT());
    ::printf("PMLT:%c/%s ", true_false(track->xpmlt()), hex_addr(pmlt).c_str());
    if( pmlt ) :: printf("LEP:%3d tr:%3d ", pmlt->flagLEpton(), pmlt->flagMEss());
    ::printf("\n");
  }
  for(int itk=qcde.KFNET; itk<=qcde.KLNET; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
    const auto* pcqa  = track->xpcqa() ? track->pcqa() : nullptr;
    print_track(itk, "C++ Neutral");
    ::printf("PCQA:%c/%s ", true_false(track->xpcqa()), hex_addr(pcqa).c_str());
    if( pcqa ) :: printf("n:%3X ", pcqa->natu());
    ::printf("\n");
  }

  /// Energy flow objects
  for(int itk=qcde.KFEFT; itk<=qcde.KLEFT; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
    const auto* efol  = track->xefol() ? track->efol() : nullptr;
    print_track(itk, "C++ EneFlow");
    ::printf("EFOL:%c/%s ", true_false(track->xefol()), hex_addr(efol).c_str());
    if( efol ) :: printf("ty:%1d e:%7.3f %3d %3d %3d %3d %3d",
                         efol->type(), efol->energyW(),
                         efol->linkEcal(), efol->linkTrak(), efol->linkHcal(),
                         efol->linkCalo(), efol->linkJet());
    ::printf("\n");
  }
  /// Energy flow jets
  for(int itk=qcde.KFJET; itk<=qcde.KLJET; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
     print_track(itk, "C++ JET    ");
    ::printf("\n");
  }
  /// Calorimeter objects
  for(int itk=qcde.KFIST; itk<=qcde.KLAST; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
    print_track(itk, "C++ CaloObj");
    ::printf("KNEC:%2d ", track->knecal());
    for(uint32_t iec=0; iec < track->knecal(); ++iec )  {
      const auto* ec_clu = track->peco(iec);
      auto        ec_row = track->peco_rownum(iec);
      ::printf("%d/%.2f ", ec_row, ec_clu->ecorr());
    }
    ::printf("KNHC:%2d ", track->knhcal());
    for(uint32_t ihc=0; ihc < track->knhcal(); ++ihc )  {
      const auto* hc_clu = track->phco(ihc);
      auto        hc_row = track->phco_rownum(ihc);
      ::printf("%d/%.2f ", hc_row, hc_clu->ecorr());
    }
    ::printf("\n");
  }
  /// Tracks from standard V0 Vertices
  for(int itk=qcde.KFDCT; itk<=qcde.KLDCT; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
    print_track(itk, "C++ V0Track");
    ::printf("\n");
  }
#if 0
  /// Tracks from standard V0 Vertices
  for(int itk=qcde.KFV0T; itk<=qcde.KLV0T; ++itk)  {
    const auto* track  = params.qvec_table->at(itk);
    print_track(itk, "C++ V0Track");
    ::printf("\n");
  }
#endif  
}
