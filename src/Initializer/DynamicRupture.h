/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 * @section LICENSE
 * Copyright (c) 2016, SeisSol Group
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @section DESCRIPTION
 **/
 
#ifndef INITIALIZER_DR_H_
#define INITIALIZER_DR_H_

#include <Initializer/typedefs.hpp>
#include <Initializer/tree/LTSTree.hpp>
#include <generated_code/tensor.h>

namespace seissol {
  namespace initializers {
    struct DynamicRupture;
    struct DR_FL_2;
    struct DR_FL_16;
    struct DR_FL_33;
  }
}

struct seissol::initializers::DynamicRupture {
  virtual ~DynamicRupture() {}
  Variable<real*>                                                   timeDerivativePlus;
  Variable<real*>                                                   timeDerivativeMinus;
  Variable<real[tensor::QInterpolated::size()]>                     imposedStatePlus;
  Variable<real[tensor::QInterpolated::size()]>                     imposedStateMinus;
  Variable<DRGodunovData>                                           godunovData;
  Variable<real[tensor::fluxSolver::size()]>                        fluxSolverPlus;
  Variable<real[tensor::fluxSolver::size()]>                        fluxSolverMinus;
  Variable<DRFaceInformation>                                       faceInformation;
  Variable<model::IsotropicWaveSpeeds>                              waveSpeedsPlus;
  Variable<model::IsotropicWaveSpeeds>                              waveSpeedsMinus;

  //friction Data
  Variable<FrictionData>                                            frictionData;
  //size padded for vectorization
  Variable<real[ init::QInterpolated::Stop[0] ]>                    rupture_time;
  Variable<bool[ init::QInterpolated::Stop[0] ]>                    RF;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    mu;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    slip;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    slip1;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    slip2;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    slipRate1;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    slipRate2;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    tracXY;
  Variable<real[ init::QInterpolated::Stop[0] ]>                    tracXZ;

  virtual void addTo(LTSTree& tree) {
    LayerMask mask = LayerMask(Ghost);
    tree.addVar(      timeDerivativePlus,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(     timeDerivativeMinus,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(        imposedStatePlus,             mask,     PAGESIZE_HEAP,      seissol::memory::Standard );
    tree.addVar(       imposedStateMinus,             mask,     PAGESIZE_HEAP,      seissol::memory::Standard );
    tree.addVar(             godunovData,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(          fluxSolverPlus,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(         fluxSolverMinus,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(         faceInformation,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(          waveSpeedsPlus,             mask,                 1,      seissol::memory::Standard );
    tree.addVar(         waveSpeedsMinus,             mask,                 1,      seissol::memory::Standard );

    tree.addVar(      frictionData,                   mask,                 1,      seissol::memory::Standard );
    tree.addVar(      rupture_time,                   mask,                 1,      seissol::memory::Standard );
    tree.addVar(      RF,                             mask,                 1,      seissol::memory::Standard );
    tree.addVar(      mu,                             mask,                 1,      seissol::memory::Standard );
    tree.addVar(      slip,                           mask,                 1,      seissol::memory::Standard );
    tree.addVar(      slip1,                          mask,                 1,      seissol::memory::Standard );
    tree.addVar(      slip2,                          mask,                 1,      seissol::memory::Standard );
    tree.addVar(      slipRate1,                      mask,                 1,      seissol::memory::Standard );
    tree.addVar(      slipRate2,                      mask,                 1,      seissol::memory::Standard );
    tree.addVar(      tracXY,                         mask,                 1,      seissol::memory::Standard );
    tree.addVar(      tracXZ,                         mask,                 1,      seissol::memory::Standard );
  }
};

struct seissol::initializers::DR_FL_2 : public seissol::initializers::DynamicRupture {
    Variable<real[init::QInterpolated::Stop[0]][6]>                 initialStressInFaultCS;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  cohesion;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  d_c;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  mu_S;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  mu_D;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  forced_rupture_time;
    Variable<real>                                                  t_0;                    //face independent
    Variable<bool>                                                  magnitude_out;
    Variable<bool[ init::QInterpolated::Stop[0] ]>                  DS;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  peakSR;
    Variable<real>                                                  averaged_Slip;
    Variable<real[ init::QInterpolated::Stop[0] ]>                  dynStress_time;


    virtual void addTo(initializers::LTSTree& tree) {
        seissol::initializers::DynamicRupture::addTo(tree);
        LayerMask mask = LayerMask(Ghost);
        tree.addVar(      initialStressInFaultCS,           mask,                 1,      seissol::memory::Standard );
        tree.addVar(      cohesion,                         mask,                 1,      seissol::memory::Standard );
        tree.addVar(      d_c,                              mask,                 1,      seissol::memory::Standard );
        tree.addVar(      mu_S,                             mask,                 1,      seissol::memory::Standard );
        tree.addVar(      mu_D,                             mask,                 1,      seissol::memory::Standard );
        tree.addVar(      forced_rupture_time,              mask,                 1,      seissol::memory::Standard );
        tree.addVar(      t_0,                              mask,                 1,      seissol::memory::Standard );
        tree.addVar(      magnitude_out,                    mask,                 1,      seissol::memory::Standard );
        tree.addVar(      DS,                               mask,                 1,      seissol::memory::Standard );
        tree.addVar(      peakSR,                           mask,                 1,      seissol::memory::Standard );
        tree.addVar(      averaged_Slip,                    mask,                 1,      seissol::memory::Standard );
        tree.addVar(      dynStress_time,                   mask,                 1,      seissol::memory::Standard );


        std::cout << "add additional variables for FL_2\n";
    }
};

struct seissol::initializers::DR_FL_16 : public seissol::initializers::DynamicRupture {
    int InitStress{};

    virtual void addTo(initializers::LTSTree& tree) {
        seissol::initializers::DynamicRupture::addTo(tree);
        std::cout << "add additional variables for FL_16\n";
    }
};

struct seissol::initializers::DR_FL_33 : public seissol::initializers::DynamicRupture {
    int InitPressure{};

    virtual void addTo(initializers::LTSTree& tree) {
        seissol::initializers::DynamicRupture::addTo(tree);
        std::cout << "add additional variables for FL_33\n";
    }
};

#endif
