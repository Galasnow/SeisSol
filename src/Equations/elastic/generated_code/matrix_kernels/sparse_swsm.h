// Copyright (c) 2015, Intel Corporation
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright notice,
//       this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holder nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// @file
// This file is part of SeisSol.
// 
// @author Alexander Breuer (breuer AT mytum.de, http://www5.in.tum.de/wiki/index.php/Dipl.-Math._Alexander_Breuer)
// @author Alexander Heinecke (alexander.heinecke AT mytum.de, http://www5.in.tum.de/wiki/index.php/Alexander_Heinecke,_M.Sc.,_M.Sc._with_honors)
// 
// @date 2015-08-14 10:02:00.754041
// 
// @section LICENSE
// Copyright (c) 2012-2015, SeisSol Group
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// 
// @section DESCRIPTION
// Remark: This file was generated.
#ifndef SPARSESWSMH
#define SPARSESWSMH

#if defined( __SSE3__) || defined(__MIC__)
#include <immintrin.h>
#endif

#include <cstddef>
#ifndef NDEBUG
extern long long libxsmm_num_total_flops;
#endif

void ssparse_kXiDivMT_m1_n9_k4_ldAna2_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kEtaDivMT_m1_n9_k4_ldAna2_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kZetaDivMT_m1_n9_k4_ldAna2_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna2_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna3_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna3_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m10_n9_k20_ldAna4_ldB20_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m10_n9_k9_ldA12_ldBna4_ldC12_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m4_n9_k10_ldAna4_ldB12_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna4_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m1_n9_k4_ldAna4_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna4_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m20_n9_k35_ldAna5_ldB36_ldC20_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m20_n9_k9_ldA20_ldBna5_ldC20_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m10_n9_k20_ldAna5_ldB20_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m10_n9_k9_ldA12_ldBna5_ldC12_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m4_n9_k10_ldAna5_ldB12_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna5_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m1_n9_k4_ldAna5_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna5_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m35_n9_k56_ldAna6_ldB56_ldC36_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m35_n9_k9_ldA36_ldBna6_ldC36_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m20_n9_k35_ldAna6_ldB36_ldC20_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m20_n9_k9_ldA20_ldBna6_ldC20_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m10_n9_k20_ldAna6_ldB20_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m10_n9_k9_ldA12_ldBna6_ldC12_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m4_n9_k10_ldAna6_ldB12_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna6_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m1_n9_k4_ldAna6_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna6_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m56_n9_k84_ldAna7_ldB84_ldC56_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m56_n9_k9_ldA56_ldBna7_ldC56_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m35_n9_k56_ldAna7_ldB56_ldC36_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m35_n9_k9_ldA36_ldBna7_ldC36_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m20_n9_k35_ldAna7_ldB36_ldC20_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m20_n9_k9_ldA20_ldBna7_ldC20_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m10_n9_k20_ldAna7_ldB20_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m10_n9_k9_ldA12_ldBna7_ldC12_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m4_n9_k10_ldAna7_ldB12_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna7_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivMT_m1_n9_k4_ldAna7_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna7_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m84_n9_k9_ldA84_ldBna8_ldC84_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m56_n9_k9_ldA56_ldBna8_ldC56_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m35_n9_k9_ldA36_ldBna8_ldC36_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m20_n9_k9_ldA20_ldBna8_ldC20_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m10_n9_k9_ldA12_ldBna8_ldC12_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna8_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m1_n9_k9_ldA4_ldBna8_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_kXiDivM_m4_n9_k4_ldAna2_ldB4_ldC4_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m4_n9_k9_ldA4_ldBna2_ldC4_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m10_n9_k9_ldA12_ldBna3_ldC12_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fM1DivM_m10_n9_k10_ldAna3_ldB12_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP113DivM_m10_n9_k10_ldAna3_ldB12_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP111DivM_m10_n9_k10_ldAna3_ldB12_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP112DivM_m10_n9_k10_ldAna3_ldB12_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP222DivM_m10_n9_k10_ldAna3_ldB12_ldC12_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m20_n9_k9_ldA20_ldBna4_ldC20_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fM1DivM_m20_n9_k20_ldAna4_ldB20_ldC20_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m35_n9_k9_ldA36_ldBna5_ldC36_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fM1DivM_m35_n9_k35_ldAna5_ldB36_ldC36_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP113DivM_m35_n9_k35_ldAna5_ldB36_ldC36_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP111DivM_m35_n9_k35_ldAna5_ldB36_ldC36_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m56_n9_k9_ldA56_ldBna6_ldC56_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fM1DivM_m56_n9_k56_ldAna6_ldB56_ldC56_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP113DivM_m56_n9_k56_ldAna6_ldB56_ldC56_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP111DivM_m56_n9_k56_ldAna6_ldB56_ldC56_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP112DivM_m56_n9_k56_ldAna6_ldB56_ldC56_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP222DivM_m56_n9_k56_ldAna6_ldB56_ldC56_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m84_n9_k9_ldA84_ldBna7_ldC84_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fM1DivM_m84_n9_k84_ldAna7_ldB84_ldC84_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_fP113DivM_m84_n9_k84_ldAna7_ldB84_ldC84_beta0_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
void ssparse_starMatrix_m120_n9_k9_ldA120_ldBna8_ldC120_beta1_pfsigonly(const real *i_A, const real *i_B, real *io_C,const real *i_APrefetch, const real *i_BPrefetch, const real *i_CPrefetch );
#endif
