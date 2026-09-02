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


/// Create MC particle
void alpha::edm4hep_output::event_t::process_mc_particles()  {
  this->fkin2edm4hep_particles.clear();
  this->alpha2edm4hep_particles.clear();
  if( this->data.debug_mc_particles )  {
    std::cout << "+++ MC Tracks: KFMCT=" << qcde.KFMCT << " KLMCT:" << qcde.KLMCT << std::endl;
  }
  for( int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )  {
    std::size_t  key    = this->particles_mc.size();
    auto         mcp    = this->particles_mc.create();
    auto*        track  = this->data.qvec.row<class qvec>(itk);
    const auto*  vstart = track->origin_vtx();
    const auto*  vend   = track->end_vtx();
    int32_t fkin_track  = track->ktn();
    double       ptime  = 0e0;
    int32_t sim_status  = 0;
    if( this->data.debug_mc_particles )  {
      std::cout << track->to_string() << std::endl;
    }
    this->alpha2edm4hep_particles[itk]       = key;
    this->fkin2edm4hep_particles[fkin_track] = key;

    mcp.setMass(_ENE(track->qm()));
    mcp.setTime(_TIM(ptime));
    mcp.setCharge(track->qch());
    mcp.setPDG(track->ktpcod());
    mcp.setSimulatorStatus(sim_status);

    mcp.setMomentum( {           _ENE(track->qx()), _ENE(track->qy()), _ENE(track->qz()) } );
    mcp.setMomentumAtEndpoint( { _ENE(track->qx()), _ENE(track->qy()), _ENE(track->qz()) } );

    mcp.setVertex(   {
        vstart ? _LEN(vstart->x) : 0e0,
        vstart ? _LEN(vstart->y) : 0e0,
        vstart ? _LEN(vstart->z) : 0e0 } );
    mcp.setEndpoint( {
        vend ? _LEN(vend->x) : 0e0,
        vend ? _LEN(vend->y) : 0e0,
        vend ? _LEN(vend->z) : 0e0 } );

    // Set generator status
    mcp.setGeneratorStatus(track->klunds());
    // Set stability code
    mcp.setSimulatorStatus(track->kstabc());

#if EDM4HEP_MCPARTICLE_HAS_HELICITY
    //mcp.setHelicity(p->spin[2]);
#else
    //mcp.setSpin(p->spin);
#endif
  }

  /// Now that all MC particles are registered,
  /// we establish the mother-daughter relationships
  for(int itk = qcde.KFMCT; itk <= qcde.KLMCT; ++itk )  {
    auto* table = this->data.qvec.table<class qvec>();
    auto* track = table->row(itk);
    auto  part  = this->particle_mc_alpha(itk);
    /// Assign the mothers
    for( uint32_t im=0; im < track->knmoth(); ++im )  {
      const auto& mot = this->particle_mc_alpha(track->kmoth(im));
      part.addToParents(mot);
    }
    /// Assign the daughters
    for( uint32_t id=0; id < track->kndau(); ++id )  {
      const auto& dau = this->particle_mc_alpha(track->kdau(id));
      part.addToDaughters(dau);
    }
  }
}

