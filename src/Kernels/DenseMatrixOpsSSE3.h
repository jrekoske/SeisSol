// SPDX-FileCopyrightText: 2015-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * This file is part of SeisSol.
 *
 * @author Carsten Uphoff (c.uphoff AT tum.de, http://www5.in.tum.de/wiki/index.php/Carsten_Uphoff,_M.Sc.)
 *
 */

#ifndef SEISSOL_SRC_KERNELS_DENSEMATRIXOPSSSE3_H_
#define SEISSOL_SRC_KERNELS_DENSEMATRIXOPSSSE3_H_

#if defined(DOUBLE_PRECISION)

#define DMO_INCREMENT 2
#define DMO_BROADCAST(IN, OUT) __m128d OUT = _mm_loaddup_pd(IN);
#define DMO_STREAM(IN, OUT) _mm_stream_pd(OUT, _mm_load_pd(IN));
#define DMO_SXT(S, X, Y) __m128d x = _mm_load_pd(X); _mm_store_pd(Y, _mm_mul_pd(S, x));
#define DMO_SXTYP(S, X, Y) __m128d x = _mm_load_pd(X); _mm_store_pd(Y, _mm_add_pd(_mm_mul_pd(S, x), _mm_load_pd(Y)));
#define DMO_XYMST(S, X, Y, Z) __m128d x = _mm_load_pd(X); __m128d y = _mm_load_pd(Y); _mm_store_pd(Z, _mm_mul_pd(S, _mm_sub_pd(x, y)));
#define DMO_XYMSTZP(S, X, Y, Z) __m128d x = _mm_load_pd(X); __m128d y = _mm_load_pd(Y); _mm_store_pd(Z, _mm_add_pd(_mm_mul_pd(S, _mm_sub_pd(x, y)), _mm_load_pd(Z)));

#elif defined(SINGLE_PRECISION)

#define DMO_INCREMENT 4
#define DMO_BROADCAST(IN, OUT) __m128 OUT = _mm_load_ss(IN); OUT = _mm_shuffle_ps(OUT, OUT, 0x00);
#define DMO_STREAM(IN, OUT) _mm_stream_ps(OUT, _mm_load_ps(IN));
#define DMO_SXT(S, X, Y) __m128 x = _mm_load_ps(X); _mm_store_ps(Y, _mm_mul_ps(S, x));
#define DMO_SXTYP(S, X, Y) __m128 x = _mm_load_ps(X); _mm_store_ps(Y, _mm_add_ps(_mm_mul_ps(S, x), _mm_load_ps(Y)));
#define DMO_XYMST(S, X, Y, Z) __m128 x = _mm_load_ps(X); __m128 y = _mm_load_ps(Y); _mm_store_ps(Z, _mm_mul_ps(S, _mm_sub_ps(x, y)));
#define DMO_XYMSTZP(S, X, Y, Z) __m128 x = _mm_load_ps(X); __m128 y = _mm_load_ps(Y); _mm_store_ps(Z, _mm_add_ps(_mm_mul_ps(S, _mm_sub_ps(x, y)), _mm_load_ps(Z)));

#else
#error no precision was defined
#endif

#endif // SEISSOL_SRC_KERNELS_DENSEMATRIXOPSSSE3_H_

