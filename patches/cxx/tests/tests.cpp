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

#include <alpha/alpha.h>

#include <alpha/qcde.h>
#include <alpha/eidt.h>
#include <alpha/hmad.h>
#include <alpha/mcad.h>
#include <alpha/muid.h>
#include <alpha/peco.h>
#include <alpha/phco.h>
#include <alpha/pgac.h>
#include <alpha/pcqa.h>
#include <alpha/qdet.h>
#include <alpha/frft.h>
#include <alpha/frtl.h>
#include <alpha/frid.h>
#include <alpha/texs.h>
#include <alpha/qvrt.h>
#include <alpha/fver.h>
#include <alpha/yv0v.h>

/// C/C++ include files
#include <iostream>
#include <cstring>
#include <cstdio>
#include <set>

using bos77::bcs;
using alpha::hex_addr;

namespace {

  struct text_io_t  {
    int32_t len;
    char    text[1024];
    std::string str()  {
      if( this->len > 0 )  {
        this->text[this->len] = ' ';
        this->text[this->len+1] = 0;
        return this->text;
      }
      return { };
    }
    std::string print()  {
      if( this->len > 0 )  {
        this->text[this->len] = 0;
        ::printf("%s\n", this->text);
        return this->text;
      }
      return { };
    }
  };
  bool print_f77 = false;
  
  void print_track(int which, const char* prefix = nullptr)  {
    using namespace alpha;
    const auto* track  = params.qvec_table->row(which);
    const auto* pgac  = track->xpgac() ? track->pgac() : nullptr;
    const auto* pdlt  = track->xpdlt() ? track->pdlt() : nullptr;
    const auto* pmlt  = track->xpmlt() ? track->pmlt() : nullptr;
    if( prefix ) ::printf("%s", prefix);
    ::printf("[%3d] %12s px:%7.2f py:%7.2f pz:%7.2f m:%7.1f e:%7.2f #m:%2d #d:%2d ",
             which, track->cqtpn().c_str(), track->px, track->py, track->pz,
             track->mass*1000e0, track->energy,
             track->knmoth(), track->kndau() );
    ::printf("PGAC:%c/%s ", true_false(track->xpgac()), hex_addr(pgac).c_str());
    if( pgac ) :: printf("p:%2d ec:%3d ", pgac->parentNumber(), pgac->peCO());
    ::printf("PDLT:%c/%s ", true_false(track->xpdlt()), hex_addr(pdlt).c_str());
    if( pdlt ) :: printf("jet:%3d tr:%3d ", pdlt->jeTnumber(), pdlt->frFT());
    ::printf("PMLT:%c/%s ", true_false(track->xpmlt()), hex_addr(pmlt).c_str());
    if( pmlt ) :: printf("LEP:%3d tr:%3d ", pmlt->flagLEpton(), pmlt->flagMEss());
  }

  void check_result(std::string& f77_result, char* cxx_result, bool pf77, bool pcxx )  {
    if( f77_result.rfind(' ') == f77_result.length()-1 )
      f77_result[f77_result.length()-1] = 0;
    const char* f77_ptr = f77_result.c_str();
    const char* check_status = "[Checks OK]";
    std::size_t len = strlen(cxx_result);
    if( len > 0 && cxx_result[len-1] == ' ' ) cxx_result[len-1] = 0;
    
    if( std::string(f77_ptr) != cxx_result )  {
      ::printf(">>>>>>>>> !!!!! PROBLEM !!!!!  different results:\n");
      check_status = "[Checks FAILED]";
      pf77 = pcxx = true;
    }
    if( pf77 && f77_result.length() > 1 )  {
      ::printf("F77 %s %s\n", f77_ptr, check_status);
    }
    if( pcxx && len > 1 )  {
      ::printf("C++ %s %s\n", cxx_result, check_status);
    }
  }
 
}

extern "C" text_io_t textio_;

extern "C" int64_t mem_addr_(int32_t& value)  {
  int64_t val = (int64_t)&value;
  return val;
}

namespace alpha  {
  /// Access PECO objects as calo_object entities
  using calo_peco = object_table<peco>;
  /// Access PHCO objects as calo_object entities
  using calo_phco = object_table<phco>;
  /// Access PHCO objects as calo_object entities
  using calo_hcal = object_table<phco>;
}

namespace f77 {

  
  std::string print_loop_vars(const char* pref, const char* tag, int32_t count, int32_t& first, int32_t& last)  {
    f77_print_loop_vars_(pref, tag, count, first, last);
    return textio_.print();
  }
  std::string get_peco(int itk)  {
    f77_print_peco_(itk);
    return textio_.str();
  }
  std::string get_phco(int itk)  {
    f77_print_phco_(itk);
    return textio_.str();
  }
  std::string print_track(int i)  {
    f77_print_track_(i);
    return textio_.print();
  }
  std::string get_mc_track(int i)  {
    f77_print_mc_track_(i);
    return textio_.str();
  }
  std::string get_track_mothers_daughters(int itk, const char* pref)  {
    f77_print_track_mothers_daughters_(itk, pref, strlen(pref));
    return textio_.str();
  }
  std::string get_track_opts1(int itk)  {
    f77_print_track_opts1_(itk);
    return textio_.str();
  }
  std::string get_track_calo(int itk)  {
    f77_print_track_calo_(itk);
    return textio_.str();
  }
  std::string print_track_trackfit(int itk)  {
    if( print_f77 )  {
      f77_print_track_trackfit_(itk);
      return textio_.print();
    }
    return { };
  }
  std::string print_vertex_1(int ivx)  {
    f77_print_vertex_1_(ivx);
    return textio_.print();
  }
  std::string print_vertex_2(int ivx)  {
    f77_print_vertex_2_(ivx);
    return textio_.print();
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
    // f77_print_qcde_();
    f77::print_loop_vars("C++", "CHT", qcde.KNCHT, qcde.KFCHT, qcde.KLCHT);
    f77::print_loop_vars("C++", "JET", qcde.KNJET, qcde.KFJET, qcde.KLJET);
    f77::print_loop_vars("C++", "FRT",         -1, qcde.KFFRT, qcde.KLFRT);
    f77::print_loop_vars("C++", "EHT", qcde.KNEHT, qcde.KFEHT, qcde.KLEHT);
    f77::print_loop_vars("C++", "IST", qcde.KNIST, qcde.KFIST, qcde.KLIST);
    f77::print_loop_vars("C++", "AST", qcde.KNAST, qcde.KFAST, qcde.KLAST);
    f77::print_loop_vars("C++", "V0T", qcde.KNV0T, qcde.KFV0T, qcde.KLV0T);
    f77::print_loop_vars("C++", "NET", qcde.KNNET, qcde.KFNET, qcde.KLNET);
    f77::print_loop_vars("C++", "OVT", qcde.KNOVT, qcde.KFOVT, qcde.KLOVT);
    f77::print_loop_vars("C++", "REV", qcde.KNREV, qcde.KFREV, qcde.KLREV);
    f77::print_loop_vars("C++", "MCT", qcde.KNMCT, qcde.KFMCT, qcde.KLMCT);
    f77::print_loop_vars("C++", "MCV", qcde.KNMCV, qcde.KFMCV, qcde.KLMCV);
    f77::print_loop_vars("C++", "GAT", qcde.KNGAT, qcde.KFGAT, qcde.KLGAT);
    f77::print_loop_vars("C++", "EFT", qcde.KNEFT, qcde.KFEFT, qcde.KLEFT);
  }
}

void alpha::tests::print_bank_lists(bool print)  {
  if( print )  {
    bos77::print_bank_lists(bos77::bos_bank_lists);
    ::printf("\n");
  }
}

std::size_t alpha::tests::access_bank_lists(bool print)  {
  std::size_t total_mem = 0;
  if( print )  {
    for( int h=0; h<5; ++h )  {
      char bank_list = bos77::bos_bank_lists[h];
      std::string names;
      alpha::aublis(bank_list);
      names.reserve(255);
      ::printf("+++ %s: BANKLIST: %c\n", __FUNCTION__, bank_list);
      for( int i = 1; ; ++i )  {
        std::string bnam = bos77::nlistb(i, bank_list);
        if( bnam.empty() )  {
          break;
        }
        names += bnam;
        names += " ";
        if( print ) total_mem += bos77::print_banks_of_type(bnam);
      }
      ::printf(" +++ %s: BANKLIST: %c  %ld words\n\n", __FUNCTION__, bank_list, total_mem);
    }
  }
  return total_mem;
}

void alpha::tests::print_charged_tracks( bool print, int32_t first, int32_t last )  {
  if( print )  {
    auto*   native = params.qvec_table;
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
      auto  p  = native->row(i);
      float qx = bcs.rw[id + qvec::JQVEQX];
      float qy = bcs.rw[id + qvec::JQVEQY];
      float qz = bcs.rw[id + qvec::JQVEQZ];
      ::fprintf(stdout,
                "C++ Track[%3d] %9.3f %9.3f %9.3f || %9.3f  %9.3f  %9.3f  ||    ",
                i, qx, qy, qz, p->px, p->py, p->pz );
      ::fflush(stdout);
      auto output = f77::print_track(i);
      ::printf("%s\n", output.c_str());
    }
  }
}

/// Loop and print PECO objects
void alpha::tests::print_peco(bool print)  {
  char text[2024];
  auto& peco = alpha::calo_peco::get("PECO");
  if( print ) ::printf("+++ Checking bank PECO:\n");
  for(uint32_t i=0; i < peco.size(); ++i )   {
    //auto hit = peco.at(i);
    const auto* hit = params.peco_table->at(i);
    std::string fret = f77::get_peco(i);
    ::snprintf(text, sizeof(text),
               "PECO %3d %8lX ENERGY:%7.3f CORR:%7.3f THETA: %4.2f PHI: %4.2f"
               " REGION:%2d CC:%2d REL:%2d ID:%5d",
               i, uint64_t(hit), hit->eraw(), hit->ecorr(), hit->theta(), hit->phi(),
               hit->kdrg(), hit->ccode(), hit->rbits(), hit->pcOB());
    check_result(fret, text, print_f77, print);
  }
}

/// Loop and print PHCO objects
void alpha::tests::print_phco(bool print)  {
  char text[2024];
  auto& phco = alpha::calo_phco::get("PHCO");
  if( print ) ::printf("+++ Checking bank PHCO:\n");
  for(uint32_t i=0; i < phco.size(); ++i )   {
    auto hit = phco.at(i);
    std::string fret = f77::get_phco(i);
    ::snprintf(text, sizeof(text),
               "PHCO %3d %8lX ENERGY:%7.3f CORR:%7.3f THETA: %4.2f PHI: %4.2f"
               " REGION:%1d CC:%1d RB:%2d NOISE:%2d ID:%3d",
               i, uint64_t(hit), hit->eraw(), hit->ecorr(), hit->theta(), hit->phi(),
               hit->kdrg(), hit->ccode(), hit->rbits(), hit->noiseFlag(), hit->pcOB());
    check_result(fret, text, print_f77, print);
  }
}

/// Verify track-mother relationships
void alpha::tests::print_track_mothers_daughters(bool print, int32_t itk, const char* prefix)  {
  char text[2024] = " ";
  std::size_t len = sizeof(text);
  const auto* track = params.qvec_table->row(itk);
  auto fret = f77::get_track_mothers_daughters(itk, prefix);
  
  if( (track->kndau()+track->knmoth()) > 0 )  {
    len = snprintf(text, sizeof(text), "%s[%3d] ", prefix, itk);
    if( track->knmoth() > 0 )  {
      /// Mothers ?? Only for MC tracks
      // KNMOTH(KI)    = IW(KOQVEC+KI*KCQVEC+JQVENO);  --> qvec::number_mothers
      //                 IW(KOQVEC+KI*KCQVEC+JQVEOL);  --> qvec::offset first mother
      // KMOTH(KI,KI1) = IW(KOQLIN+KI1 + IW(KOQVEC+KI*KCQVEC+JQVEOL));
      len += snprintf(text+len, sizeof(text)-len, "#MOT:%2d ", track->knmoth());
      for(uint32_t im=0; im < track->knmoth(); ++im )  {
        auto* m = track->mother(im);
        len += snprintf(text+len, sizeof(text)-len, "M:%3d px:%7.3f ", track->kmoth(im), m->px);
      }
    }
    if( track->kndau() > 0 )  {
      len += snprintf(text+len, sizeof(text)-len, "#DAU:%2d ", track->kndau());
      for(uint32_t idau=0; idau < track->kndau(); ++idau )  {
        auto* d = track->daughter(idau);
        len += snprintf(text+len, sizeof(text)-len, "D:%3d px:%7.3f ", track->kdau(idau), d->px);
      }
    }
  }
  check_result(fret, text, print_f77, print);
}

/// Check links from QVEC to other banks (1)
void alpha::tests::print_charged_track_opts1(bool print, int32_t itk)  {
  auto fret = f77::get_track_opts1(itk);
  char text[1024] = " ";
  std::size_t len;
  
  const auto* track = params.qvec_table->row(itk);
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

  len = snprintf(text, sizeof(text),
                 "Track[%3d] d0:%6.1f z0:%6.1f "
                 "FRFT:%2d d0:%6.1f z0:%6.1f ",
                 itk, track->d0, track->z0,
                 track->ktn(), pfrft->d0(), pfrft->z0());
  if( pfrtl )  {
    len += snprintf(text+len, sizeof(text)-len, "FRTL:%8lX %1d %1d %2d ",
                    uint64_t(pfrtl), pfrtl->narcV(), pfrtl->narcI(), pfrtl->narcT());
  }
  if( pfrid )  {
    len += snprintf(text+len, sizeof(text)-len, "FRID:%8lX e-:%4.3f pi:%4.3f ",
                    uint64_t(pfrid), pfrid->probElec(), pfrid->probpIon());
  }
#if 0
  if( track->koriv() )  {
    const auto* vx   = track->origin_vtx();
    const auto* yv0v = vx->yv0v();
    len += ::snprintf(text+len, sizeof(text)-len,
                      "Org:kvn:%3d in:%3d out:%3d %7.2f %7.2f %7.2f yv0v:%8lX ",
                      vx->kvn(), vx->kvincp(), vx->kvndau(),
                      vx->x, vx->y, vx->z, uint64_t(yv0v));
  }
  if( track->kendv() )  {
    const auto* vx   = track->end_vtx();
    const auto* yv0v = vx->yv0v();
    len += ::snprintf(text+len, sizeof(text)-len,
                      "End:kvn:%3d in:%3d out:%3d %7.2f %7.2f %7.2f yv0v:%8lX ",
                      vx->kvn(), vx->kvincp(), vx->kvndau(),
                      vx->x, vx->y, vx->z, uint64_t(yv0v));
  }
#endif  
  /// Electron identification: Bank EIDT
  /// XEID(KI)=IW(KJQDET(KI)+JQDEAE).NE.KQZER
  /// QEIDEC(KI)=RW(IW(KJQDET(KI)+JQDEAE)+JEIDEC)
  const auto* eidt = track->eidt();
  len += ::snprintf(text+len, sizeof(text)-len, "EIDT:%c ", track->xeidt() ? 'T' : 'F');
  if( track->xeidt() )  {
    len += ::snprintf(text+len, sizeof(text)-len, "%8lX T:%2d EC:%2d ",
                      uint64_t(eidt), eidt->frFT(), eidt->peCO());
  }
  /// Muon − HCAL association: Bank HMAD
  /// XHMA(KI)=IW(KJQDET(KI)+JQDEAH).NE.KQZER
  const auto* hmad = track->hmad();
  len += ::snprintf(text+len, sizeof(text)-len, "HMAD:%c ", track->xhmad() ? 'T' : 'F');
  if( track->xhmad() )  {
    len += ::snprintf(text+len, sizeof(text)-len, "%8lX T:%2d PL:%2d ",
                      uint64_t(hmad), hmad->trackNo(), hmad->nplaFired());
  }
  /// Muon chamber data: Bank MCAD
  /// XMCA(KI)=IW(KJQDET(KI)+JQDEAM).NE.KQZER
  const auto* mcad = track->mcad();
  len += ::snprintf(text+len, sizeof(text)-len, "MCAD:%c ", track->xmcad() ? 'T' : 'F');
  if( track->xmcad() )  {
    len += ::snprintf(text+len, sizeof(text)-len, "%8lX T:%2d NH:%2d ",
                      uint64_t(mcad), mcad->trackNo(), mcad->nassHit()[0]);
  }
  /// QMUIDO Muon Identification: Bank MUID
  /// XMUI(KI)=IW(KJQDET(KI)+JQDEMU).NE.KQZER
  const auto* muid = track->muid();
  len += ::snprintf(text+len, sizeof(text)-len, "MUID:%c ", track->xmuid() ? 'T' : 'F');
  if( track->xmuid() )  {
    len += ::snprintf(text+len, sizeof(text)-len, "%8lX T:%2d ID:%2d ",
                      uint64_t(muid), muid->trackNumber(), muid->idFlag());
  }
  len += ::snprintf(text+len, sizeof(text)-len, "TEXS:%c %2d ",
                    true_false(pqdet->xtexs()), pqdet->kntexs() );
  //
  for(uint32_t i=0; i<pqdet->kntexs(); ++i)  {
    const class texs* ptexs = pqdet->texs(i);
    len += ::snprintf(text+len, sizeof(text)-len, "%d: SEG:%2d #S:%2d ",
                      i, ptexs->segmentId(), ptexs->numberSamples());
  }

  // Consistency checks:
  if( eidt && eidt->frFT() != track->ktn() )  {
    len += ::snprintf(text+len, sizeof(text)-len,
                      " !!!!! EIDT: ERROR %d != %d !!!!!", eidt->frFT(), track->ktn());
  }
  if( hmad && hmad->trackNo() != track->ktn() )  {
    len += ::snprintf(text+len, sizeof(text)-len,
                      " !!!!! HMAD: ERROR %d != %d!!!!!", hmad->trackNo(), track->ktn());
  }
  if( mcad && mcad->trackNo() != track->ktn() )  {
    len += ::snprintf(text+len, sizeof(text)-len,
                      " !!!!! MCAD: ERROR %d != %d !!!!!", mcad->trackNo(), track->ktn());
  }
  if( muid && muid->trackNumber() != track->ktn() )  {
    len += ::snprintf(text+len, sizeof(text)-len,
                      " !!!!! MUID: ERROR %d != %d!!!!!", muid->trackNumber(), track->ktn());
  }
  check_result(fret, text, print_f77, print);
}

/// Check links from QVEC track fit parameters
void alpha::tests::print_charged_track_trackfit(int32_t itk)  {
  const auto* track = params.qvec_table->row(itk);
  f77::print_track_trackfit(itk);
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
void alpha::tests::print_charged_track_calo( bool print, int32_t itk)  {
  //
  // KTN(KI)=IW(KOQVEC+KI*KCQVEC+JQVETN)
  //
  // KJQDET(KI)   = IW(KOQVEC+KI*KCQVEC+JQVEQD)          --> qvec::offset_qdet
  // KNECAL(KI)   = IW(KJQDET(KI)+JQDENE)                --> qdet::num_associated_ecal
  // KECAL(KI,KI1)= IW(KOQLIN+KI1+IW(KJQDET(KI)+JQDEEL)) --> 
  //                                            = 20
  const auto* track = params.qvec_table->row(itk);
  const auto* qdet  = track->qdet();
  auto fret = f77::get_track_calo(itk);
  char text[1024] = " ";
  std::size_t len = 0;

  if( qdet->knecal()+qdet->knhcal() > 0 )  {
    len += ::snprintf(text, sizeof(text), "Track[%3d] CALO info ", itk);
    if( qdet->knecal() > 0 )  {
     len += ::snprintf(text+len, sizeof(text)-len, "ECAL #EC:%1d ", track->knecal());
      for(uint32_t iec=0; iec < track->knecal(); ++iec )  {
        const class peco* ec_clu = track->peco(iec);
        auto        ec_row       = qdet->peco_rownum(iec);
        const auto* ec_track     = track->ecal_track(iec);
        auto        ec_track_row = qdet->ecal_track_rownum(iec);
        len += ::snprintf(text+len, sizeof(text)-len, "OBJ:%3d PECO:%3d E:%7.3f TE:%7.3f ",
                          ec_track_row, ec_row, ec_clu->ecorr(), ec_track->energy);
      }
    }
    if( qdet->knhcal() > 0 )  {
      len += ::snprintf(text+len, sizeof(text)-len, "HCAL #HC:%1d ", track->knhcal());
      for(uint32_t ihc=0; ihc < track->knhcal(); ++ihc )  {
        const auto* hc_clu       = track->phco(ihc);
        auto        hc_row       = qdet->phco_rownum(ihc);
        const auto* hc_track     = track->hcal_track(ihc);
        auto        hc_track_row = qdet->hcal_track_rownum(ihc);
        len += ::snprintf(text+len, sizeof(text)-len, "OBJ:%3d PHCO:%3d E:%7.3f TE:%7.3f ",
                          hc_track_row, hc_row, hc_clu->ecorr(), hc_track->energy);
      }
    }
  }
  check_result(fret, text, print_f77, print);
}

void alpha::tests::print_charged_track_relations( bool print, int32_t first, int32_t last )  {
  if( print )  {
    std::set<int32_t> vertices;

    for(int32_t itk=first; itk <= last; ++itk )  {
      const auto* track  = params.qvec_table->row(itk);
      ::printf("%s\n", track->to_string().c_str());
    }
    if( print ) ::printf("+++ Checking Charged Track Mother-Daughter relaionships:\n");
    for(int32_t itk=first; itk <= last; ++itk )
      print_track_mothers_daughters (print, itk,"Charged");

    for(int32_t itk=first; itk <= last; ++itk )
      print_charged_track_calo (print, itk);

    for(int32_t itk=first; itk <= last; ++itk )
      print_charged_track_opts1 (print, itk);

    for(int32_t itk=first; itk <= last; ++itk )
      print_charged_track_calo (print, itk);
    
    for(int32_t itk=first; itk <= last; ++itk )  {
      const auto* track  = params.qvec_table->row(itk);
      int32_t ivx_origin = track->koriv();
      int32_t ivx_end    = track->kendv();
      print_charged_track_trackfit (itk);
      if( ivx_origin )  {
        vertices.insert(ivx_origin);
      }
      if( ivx_end )  {
        vertices.insert(ivx_end);
      }
    }
    for(int32_t itk=first; itk <= last; ++itk )  {
      const auto* track  = params.qvec_table->row(itk);
      int32_t ivx_origin = track->koriv();
      int32_t ivx_end    = track->kendv();
      print_charged_track_trackfit (itk);
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

void alpha::tests::print_vertex( int32_t ivx )  {
  const auto* vertex = params.qvrt_table->row(ivx);
  f77::print_vertex_1(ivx);
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
    f77::print_vertex_2(ivx);
  }
}

void alpha::tests::print_mc_track(bool print, int32_t itk)  {
  char text[1024];
  const auto* track  = params.qvec_table->row(itk);
  std::string fret = f77::get_mc_track(itk);
  std::size_t len =
    ::snprintf(text, sizeof(text),
               "MCT[%2d] cl:%2d %6d %-12s %6.1f %6.1f %6.1f koriv:%3d kendv:%3d ",
               itk, track->kclass(), track->ktpcod(), track->cqtpn().c_str(),
               track->qx(), track->qy(), track->qz(),
               track->koriv(), track->kendv()
               );
  if( track->koriv() )  {
    const auto* vx   = track->origin_vtx();
    const auto* fver = vx->fver();
    len += ::snprintf(text+len, sizeof(text)-len,
                      "Org:kvn:%3d in:%3d out:%3d %7.2f %7.2f %7.2f fver:%8lX ",
                      vx->kvn(), vx->kvincp(), vx->kvndau(),
                      vx->x, vx->y, vx->z, uint64_t(fver));
    if( fver )  {
      std::string vol(fver->volNam(), fver->volNam()+4);
      std::string mec(fver->vertexMechanism(), fver->vertexMechanism()+4);
      if( vol[0] != ' ' ) len += ::snprintf(text+len,sizeof(text)-len,"vol:%s ",_trim(vol).c_str());
      if( mec[0] != ' ' ) len += ::snprintf(text+len,sizeof(text)-len,"mech:%s ",_trim(mec).c_str());
    }
  }
  if( track->kendv() )  {
    const auto* vx   = track->end_vtx();
    const auto* fver = vx->fver();
    len += ::snprintf(text+len, sizeof(text)-len,
                      "End:kvn:%3d in:%3d out:%3d %7.2f %7.2f %7.2f fver:%8lX ",
                      vx->kvn(), vx->kvincp(), vx->kvndau(),
                      vx->x, vx->y, vx->z, uint64_t(fver));
    if( fver )  {
      std::string vol(fver->volNam(), fver->volNam()+4);
      std::string mec(fver->vertexMechanism(), fver->vertexMechanism()+4);      
      if( vol[0] != ' ' ) len += ::snprintf(text+len,sizeof(text)-len,"vol:%s ",_trim(vol).c_str());
      if( mec[0] != ' ' ) len += ::snprintf(text+len,sizeof(text)-len,"mech:%s ",_trim(mec).c_str());
    }
  }
  check_result(fret, text, print_f77, print);
}

void alpha::tests::print_mc_tracks(bool print)  {
  if( print )  {
    for( int32_t itk=qcde.KFMCT; itk<= qcde.KLMCT; ++itk )
      print_mc_track(print, itk);
    if( print ) ::printf("+++ Checking Monte-Carlo Track Mother-Daughter relaionships:\n");
    for( int32_t itk=qcde.KFMCT; itk<= qcde.KLMCT; ++itk )
      print_track_mothers_daughters(print, itk, "MCT");
  }
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
  static bool first = false;
  ::printf("+++++++  Calling %s!\n", __FUNCTION__);
  if( first )  {
    first = false;
    tests::print_bank_lists(1);
    tests::access_bank_lists(1);
    tests::check_qcde_alignment(1);
    bos77::print_banks_of_type("VDXY");
    bos77::print_banks_of_type("VDZT");
    bos77::print_banks_of_type("VDCO");
    bos77::print_banks_of_type("VDHT");

    bos77::print_banks_of_type("VDFK");
    bos77::print_banks_of_type("VUFK");
    bos77::print_banks_of_type("VCPL");
  }
  bool prt = true;
  bool nprt = false;
  tests::print_peco(prt);
  tests::print_phco(prt);

  tests::print_mc_tracks(prt);
  //tests::print_charged_tracks(0, qcde.KFCHT, qcde.KLCHT);

  tests::print_charged_track_relations(prt, qcde.KFCHT, qcde.KLCHT);
  //tests::print_vertex_relations(1, qcde.KFCHT, qcde.KLCHT);
  for(int itk=qcde.KFGAT; itk<=qcde.KLGAT; ++itk)  {
    print_track(itk, "C++ Photon");
    ::printf("\n");
  }
  for(int itk=qcde.KFNET; itk<=qcde.KLNET; ++itk)  {
    const auto* track  = params.qvec_table->row(itk);
    const auto* pcqa  = track->xpcqa() ? track->pcqa() : nullptr;
    print_track(itk, "C++ Neutral");
    ::printf("PCQA:%c/%s ", true_false(track->xpcqa()), hex_addr(pcqa).c_str());
    if( pcqa ) :: printf("n:%3X ", pcqa->natu());
    ::printf("\n");
  }

  /// Energy flow objects
  for(int itk=qcde.KFEFT; itk<=qcde.KLEFT; ++itk)  {
    const auto* track  = params.qvec_table->row(itk);
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
    //const auto* track  = params.qvec_table->row(itk);
     print_track(itk, "C++ JET    ");
    ::printf("\n");
  }
  /// Calorimeter objects
  for(int itk=qcde.KFIST; itk<=qcde.KLIST; ++itk)  {
    const auto* track  = params.qvec_table->row(itk);
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
    //const auto* track  = params.qvec_table->row(itk);
    print_track(itk, "C++ V0Track");
    ::printf("\n");
  }
#if 0
  /// Tracks from standard V0 Vertices
  for(int itk=qcde.KFV0T; itk<=qcde.KLV0T; ++itk)  {
    const auto* track  = params.qvec_table->row(itk);
    print_track(itk, "C++ V0Track");
    ::printf("\n");
  }
#endif  
}

#include <alpha/processor.h>
namespace alpha  {
  class test_processor;
}

/// Framework event callback
template <> void alpha::processor<alpha::test_processor>::handle_event(alpha::constants_t& /* par */)  {
  if( debug ) ::printf("test_processor:  +++++++  Calling %s   KNEVT:%d\n", __FUNCTION__, qcde.KNEVT);
  tests::process_event();
}

/// Framework termination callback
template <> void alpha::processor<alpha::test_processor>::terminate()  {
  tests::finalize();
}

/// Framework termination callback
template <> void alpha::processor<alpha::test_processor>::initialize()  {
  tests::initialize();
}
