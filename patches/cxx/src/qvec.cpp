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
#include <alpha/qvec.h>

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

/// Direct access to the charged daughter track
const class alpha::qvec* alpha::qvec::daughter(uint32_t i)  const {
  const auto* qlin = params.qlin_table->at(this->offset_first_daughter + i + 1);
  return qlin ? params.qvec_table->at(qlin->link) : nullptr;
}

/// Direct access to the charged mother track
const class alpha::qvec* alpha::qvec::mother(uint32_t i)  const {
  const auto* qlin = params.qlin_table->at(this->offset_first_mother + i + 1);
  return qlin ? params.qvec_table->at(qlin->link) : nullptr;
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
