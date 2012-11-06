//============================================================================//
// Copyright (c) <2012> <Guillaume Blanc>                                     //
//                                                                            //
// This software is provided 'as-is', without any express or implied          //
// warranty. In no event will the authors be held liable for any damages      //
// arising from the use of this software.                                     //
//                                                                            //
// Permission is granted to anyone to use this software for any purpose,      //
// including commercial applications, and to alter it and redistribute it     //
// freely, subject to the following restrictions:                             //
//                                                                            //
// 1. The origin of this software must not be misrepresented; you must not    //
// claim that you wrote the original software. If you use this software       //
// in a product, an acknowledgment in the product documentation would be      //
// appreciated but is not required.                                           //
//                                                                            //
// 2. Altered source versions must be plainly marked as such, and must not be //
// misrepresented as being the original software.                             //
//                                                                            //
// 3. This notice may not be removed or altered from any source               //
// distribution.                                                              //
//============================================================================//

#include "ozz/base/maths/simd_math.h"

#include "gtest/gtest.h"

#include "ozz/base/gtest_helper.h"
#include "ozz/base/maths/math_constant.h"
#include "ozz/base/maths/math_ex.h"
#include "ozz/base/maths/gtest_math_helper.h"

using ozz::math::SimdFloat4;
using ozz::math::SimdInt4;

OZZ_STATIC_ASSERT(sizeof(SimdFloat4) == 4 * sizeof(float));

TEST(LoadFloat, ozz_simd_math) {
  const SimdFloat4 fX = ozz::math::simd_float4::LoadX(15.f);
  EXPECT_SIMDFLOAT_EQ(fX, 15.f, 0.f, 0.f, 0.f);

  const SimdFloat4 f1 = ozz::math::simd_float4::Load1(15.f);
  EXPECT_SIMDFLOAT_EQ(f1, 15.f, 15.f, 15.f, 15.f);

  const SimdFloat4 f4 = ozz::math::simd_float4::Load(1.f, -1.f, 2.f, -3.f);
  EXPECT_SIMDFLOAT_EQ(f4, 1.f, -1.f, 2.f, -3.f);
}

TEST(LoadFloatPtr, ozz_simd_math) {
  union Data {
    float f[4 + 4];  // The 2nd float isn't aligned to a SimdFloat4.
    SimdFloat4 f4[2];  // Forces alignment.
    char c[(4 + 4) * sizeof(float)];  // The 2nd char isn't aligned to a float.
  };

  const Data d_in = {{-1.f, 1.f, 2.f, 3.f, 4.f}};

  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::LoadPtrU(d_in.f + 1), 1.f, 2.f, 3.f, 4.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::LoadPtrU(reinterpret_cast<const float*>(d_in.c + 1)), "alignment");
  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::LoadPtr(d_in.f), -1.f, 1.f, 2.f, 3.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::LoadPtr(d_in.f + 1), "alignment");

  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::LoadXPtrU(d_in.f + 1), 1.f, 0.f, 0.f, 0.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::LoadXPtrU(reinterpret_cast<const float*>(d_in.c + 1)), "alignment");
  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::Load1PtrU(d_in.f + 1), 1.f, 1.f, 1.f, 1.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::Load1PtrU(reinterpret_cast<const float*>(d_in.c + 1)), "alignment");

  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::LoadXPtr(d_in.f), -1.f, 0.f, 0.f, 0.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::LoadXPtr(d_in.f + 1), "alignment");
  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::Load1Ptr(d_in.f), -1.f, -1.f, -1.f, -1.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::Load1Ptr(d_in.f + 1), "alignment");

  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::Load2Ptr(d_in.f), -1.f, 1.f, 0.f, 0.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::Load2Ptr(d_in.f + 1), "alignment");
  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::Load2PtrU(d_in.f + 1), 1.f, 2.f, 0.f, 0.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::Load2PtrU(reinterpret_cast<const float*>(d_in.c + 1)), "alignment");

  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::Load3Ptr(d_in.f), -1.f, 1.f, 2.f, 0.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::Load3Ptr(d_in.f + 1), "alignment");
  EXPECT_SIMDFLOAT_EQ(ozz::math::simd_float4::Load3PtrU(d_in.f + 1), 1.f, 2.f, 3.f, 0.f);
  EXPECT_ASSERTION(ozz::math::simd_float4::Load3PtrU(reinterpret_cast<const float*>(d_in.c + 1)), "alignment");
}

TEST(GetFloat, ozz_simd_math) {
  const SimdFloat4 f = ozz::math::simd_float4::Load(1.f, -1.f, 2.f, -3.f);

  EXPECT_FLOAT_EQ(ozz::math::GetX(f), 1.f);
  EXPECT_FLOAT_EQ(ozz::math::GetY(f), -1.f);
  EXPECT_FLOAT_EQ(ozz::math::GetZ(f), 2.f);
  EXPECT_FLOAT_EQ(ozz::math::GetW(f), -3.f);
}

TEST(SetFloat, ozz_simd_math) {
  const SimdFloat4 i = ozz::math::simd_float4::Load(1.f, -1.f, 2.f, -3.f);

  const SimdFloat4 j = ozz::math::SetX(i, 11.f);
  EXPECT_SIMDFLOAT_EQ(j, 11.f, -1.f, 2.f, -3.f);

  const SimdFloat4 k = ozz::math::SetY(i, -11.f);
  EXPECT_SIMDFLOAT_EQ(k, 1.f, -11.f, 2.f, -3.f);

  const SimdFloat4 l = ozz::math::SetZ(i, 21.f);
  EXPECT_SIMDFLOAT_EQ(l, 1.f, -1.f, 21.f, -3.f);

  const SimdFloat4 m = ozz::math::SetW(i, -31.f);
  EXPECT_SIMDFLOAT_EQ(m, 1.f, -1.f, 2.f, -31.f);
}

TEST(StoreFloatPtr, ozz_simd_math) {
  const SimdFloat4 f4 = ozz::math::simd_float4::Load(-1.f, 1.f, 2.f, 3.f);

  union Data {
    float f[4 + 4];  // The 2nd float isn't aligned to a SimdFloat4.
    SimdFloat4 f4[2];  // Forces alignment.
    char c[(4 + 4) * sizeof(float)];  // The 2nd char isn't aligned to a float.
  };

  {
    Data d_out = {};
    ozz::math::StorePtrU(f4, d_out.f + 1);
    EXPECT_FLOAT_EQ(d_out.f[0], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[1], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 1.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 2.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 3.f);
    EXPECT_ASSERTION(ozz::math::StorePtrU(f4, reinterpret_cast<float*>(d_out.c + 1)), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::Store1PtrU(f4, d_out.f + 1);
    EXPECT_FLOAT_EQ(d_out.f[0], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[1], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::Store1PtrU(f4, reinterpret_cast<float*>(d_out.c + 1)), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::Store2PtrU(f4, d_out.f + 1);
    EXPECT_FLOAT_EQ(d_out.f[0], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[1], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 1.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::Store2Ptr(f4, reinterpret_cast<float*>(d_out.c + 1)), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::Store3PtrU(f4, d_out.f + 1);
    EXPECT_FLOAT_EQ(d_out.f[0], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[1], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 1.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 2.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::Store3Ptr(f4, reinterpret_cast<float*>(d_out.c + 1)), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::StorePtr(f4, d_out.f);
    EXPECT_FLOAT_EQ(d_out.f[0], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[1], 1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 2.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 3.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::StorePtr(f4, d_out.f + 1), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::Store1Ptr(f4, d_out.f);
    EXPECT_FLOAT_EQ(d_out.f[0], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[1], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::Store1Ptr(f4, d_out.f + 1), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::Store2Ptr(f4, d_out.f);
    EXPECT_FLOAT_EQ(d_out.f[0], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[1], 1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::Store2Ptr(f4, d_out.f + 1), "alignment");
  }
  {
    Data d_out = {};
    ozz::math::Store3Ptr(f4, d_out.f);
    EXPECT_FLOAT_EQ(d_out.f[0], -1.f);
    EXPECT_FLOAT_EQ(d_out.f[1], 1.f);
    EXPECT_FLOAT_EQ(d_out.f[2], 2.f);
    EXPECT_FLOAT_EQ(d_out.f[3], 0.f);
    EXPECT_FLOAT_EQ(d_out.f[4], 0.f);
    EXPECT_ASSERTION(ozz::math::Store3Ptr(f4, d_out.f + 1), "alignment");
  }
}

TEST(ConstantFloat, ozz_simd_math) {
  const SimdFloat4 zero = ozz::math::simd_float4::zero();
  EXPECT_SIMDFLOAT_EQ(zero, 0.f, 0.f, 0.f, 0.f);

  const SimdFloat4 one = ozz::math::simd_float4::one();
  EXPECT_SIMDFLOAT_EQ(one, 1.f, 1.f, 1.f, 1.f);

  const SimdFloat4 x_axis = ozz::math::simd_float4::x_axis();
  EXPECT_SIMDFLOAT_EQ(x_axis, 1.f, 0.f, 0.f, 0.f);

  const SimdFloat4 y_axis = ozz::math::simd_float4::y_axis();
  EXPECT_SIMDFLOAT_EQ(y_axis, 0.f, 1.f, 0.f, 0.f);

  const SimdFloat4 z_axis = ozz::math::simd_float4::z_axis();
  EXPECT_SIMDFLOAT_EQ(z_axis, 0.f, 0.f, 1.f, 0.f);

  const SimdFloat4 w_axis = ozz::math::simd_float4::w_axis();
  EXPECT_SIMDFLOAT_EQ(w_axis, 0.f, 0.f, 0.f, 1.f);
}

TEST(SplatFloat, ozz_simd_math) {
  const SimdFloat4 f = ozz::math::simd_float4::Load(1.f, -1.f, 2.f, -3.f);

  const SimdFloat4 x = ozz::math::SplatX(f);
  EXPECT_SIMDFLOAT_EQ(x, 1.f, 1.f, 1.f, 1.f);

  const SimdFloat4 y = ozz::math::SplatY(f);
  EXPECT_SIMDFLOAT_EQ(y, -1.f, -1.f, -1.f, -1.f);

  const SimdFloat4 z = ozz::math::SplatZ(f);
  EXPECT_SIMDFLOAT_EQ(z, 2.f, 2.f, 2.f, 2.f);

  const SimdFloat4 w = ozz::math::SplatW(f);
  EXPECT_SIMDFLOAT_EQ(w, -3.f, -3.f, -3.f, -3.f);
}

TEST(ArithmeticFloat, ozz_simd_math) {
  const ozz::math::SimdFloat4 a = ozz::math::simd_float4::Load(0.5f, 1.f, 2.f, 3.f);
  const ozz::math::SimdFloat4 b = ozz::math::simd_float4::Load(4.f, 5.f, -6.f, 7.f);
  const ozz::math::SimdFloat4 c = ozz::math::simd_float4::Load(-8.f, 9.f, 10.f, 11.f);

  const ozz::math::SimdFloat4 add = a + b;
  EXPECT_SIMDFLOAT_EQ(add, 4.5f, 6.f, -4.f, 10.f);

  const ozz::math::SimdFloat4 sub = a - b;
  EXPECT_SIMDFLOAT_EQ(sub, -3.5f, -4.f, 8.f, -4.f);

  const ozz::math::SimdFloat4 neg = -b;
  EXPECT_SIMDFLOAT_EQ(neg, -4.f, -5.f, 6.f, -7.f);

  const ozz::math::SimdFloat4 mul = a * b;
  EXPECT_SIMDFLOAT_EQ(mul, 2.f, 5.f, -12.f, 21.f);

  const ozz::math::SimdFloat4 div = a / b;
  EXPECT_SIMDFLOAT_EQ(div, 0.5f/4.f, 1.f/5.f, -2.f/6.f, 3.f/7.f);

  const ozz::math::SimdFloat4 madd = ozz::math::MAdd(a, b, c);
  EXPECT_SIMDFLOAT_EQ(madd, -6.f, 14.f, -2.f, 32.f);

  const ozz::math::SimdFloat4 divx = ozz::math::DivX(a, b);
  EXPECT_SIMDFLOAT_EQ(divx, 0.5f/4.f, 1.f, 2.f, 3.f);
  
  const ozz::math::SimdFloat4 hadd2 = ozz::math::HAdd2(a);
  EXPECT_SIMDFLOAT_EQ(hadd2, 1.5f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 hadd3 = ozz::math::HAdd3(a);
  EXPECT_SIMDFLOAT_EQ(hadd3, 3.5f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 hadd4 = ozz::math::HAdd4(a);
  EXPECT_SIMDFLOAT_EQ(hadd4, 6.5f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 dot2 = ozz::math::Dot2(a, b);
  EXPECT_SIMDFLOAT_EQ(dot2, 7.f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 dot3 = ozz::math::Dot3(a, b);
  EXPECT_SIMDFLOAT_EQ(dot3, -5.f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 dot4 = ozz::math::Dot4(a, b);
  EXPECT_SIMDFLOAT_EQ(dot4, 16.f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 cross = ozz::math::Cross3(
    ozz::math::simd_float4::Load(1.f, -2.f, 3.f, 46.f),
    ozz::math::simd_float4::Load(4.f, 5.f, 6.f, 27.f));
  EXPECT_SIMDFLOAT_EQ(cross, -27.f, 6.f, 13.f, 0.f);
  
  const ozz::math::SimdFloat4 rcp = ozz::math::RcpEst(b);
  EXPECT_SIMDFLOAT_EQ_EST(rcp, 1.f/4.f, 1.f/5.f, -1.f/6.f, 1.f/7.f);

  const ozz::math::SimdFloat4 rcpx = ozz::math::RcpEstX(b);
  EXPECT_SIMDFLOAT_EQ_EST(rcpx, 1.f/4.f, 5.f, -6.f, 7.f);
  
  const ozz::math::SimdFloat4 sqrt = ozz::math::Sqrt(a);
  EXPECT_SIMDFLOAT_EQ(sqrt, .7071068f, 1.f, 1.4142135f, 1.7320508f);

  const ozz::math::SimdFloat4 sqrtx = ozz::math::SqrtX(a);
  EXPECT_SIMDFLOAT_EQ(sqrtx, .7071068f, 1.f, 2.f, 3.f);
  
  const ozz::math::SimdFloat4 rsqrt = ozz::math::RSqrtEst(a);
  EXPECT_SIMDFLOAT_EQ_EST(rsqrt, 1.f/.7071068f, 1.f, 1.f/1.4142135f, 1.f/1.7320508f);

  const ozz::math::SimdFloat4 rsqrtx = ozz::math::RSqrtEstX(a);
  EXPECT_SIMDFLOAT_EQ_EST(rsqrtx, 1.f/.7071068f, 1.f, 2.f, 3.f);

  const ozz::math::SimdFloat4 abs = ozz::math::Abs(b);
  EXPECT_SIMDFLOAT_EQ(abs, 4.f, 5.f, 6.f, 7.f);
}

TEST(LengthFloat, ozz_simd_math) {
  const SimdFloat4 f = ozz::math::simd_float4::Load(1.f, 2.f, 4.f, 8.f);

  const SimdFloat4 len2 = ozz::math::Length2(f);
  EXPECT_SIMDFLOAT_EQ(len2, 2.236068f, 2.f, 4.f, 8.f);

  const SimdFloat4 len3 = ozz::math::Length3(f);
  EXPECT_SIMDFLOAT_EQ(len3, 4.5825758f, 2.f, 4.f, 8.f);

  const SimdFloat4 len4 = ozz::math::Length4(f);
  EXPECT_SIMDFLOAT_EQ(len4, 9.2195444f, 2.f, 4.f, 8.f);
}

TEST(NormalizeFloat, Float4x4) {
  const SimdFloat4 f = ozz::math::simd_float4::Load(1.f, 2.f, 4.f, 8.f);
  const SimdFloat4 unit = ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f);
  const SimdFloat4 zero = ozz::math::simd_float4::Load1(0.f);

  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized2(f), 0, 0, 0, 0);
  const SimdFloat4 norm2 = ozz::math::Normalize2(f);
  EXPECT_SIMDFLOAT_EQ(norm2, .44721359f, .89442718f, 4.f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized2(norm2), 0xffffffff, 0, 0, 0);

  const SimdFloat4 norm_est2 = ozz::math::NormalizeEst2(f);
  EXPECT_SIMDFLOAT_EQ_EST(norm_est2, .44721359f, .89442718f, 4.f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalizedEst2(norm_est2), 0xffffffff, 0, 0, 0);

  EXPECT_ASSERTION(ozz::math::Normalize2(zero), "_v is not normalizable");
  EXPECT_ASSERTION(ozz::math::NormalizeEst2(zero), "_v is not normalizable");

  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized3(f), 0, 0, 0, 0);
  const SimdFloat4 norm3 = ozz::math::Normalize3(f);
  EXPECT_SIMDFLOAT_EQ(norm3, .21821788f, .43643576f, .87287152f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized3(norm3), 0xffffffff, 0, 0, 0);

  const SimdFloat4 norm_est3 = ozz::math::NormalizeEst3(f);
  EXPECT_SIMDFLOAT_EQ_EST(norm_est3, .21821788f, .43643576f, .87287152f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalizedEst3(norm_est3), 0xffffffff, 0, 0, 0);

  EXPECT_ASSERTION(ozz::math::Normalize3(zero), "_v is not normalizable");
  EXPECT_ASSERTION(ozz::math::NormalizeEst3(zero), "_v is not normalizable");

  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized4(f), 0, 0, 0, 0);
  const SimdFloat4 norm4 = ozz::math::Normalize4(f);
  EXPECT_SIMDFLOAT_EQ(norm4, .1084652f, .2169304f, .4338609f, .86772186f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized4(norm4), 0xffffffff, 0, 0, 0);

  const SimdFloat4 norm_est4 = ozz::math::NormalizeEst4(f);
  EXPECT_SIMDFLOAT_EQ_EST(norm_est4, .1084652f, .2169304f, .4338609f, .86772186f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalizedEst4(norm_est4), 0xffffffff, 0, 0, 0);

  EXPECT_ASSERTION(ozz::math::Normalize4(zero), "_v is not normalizable");
  EXPECT_ASSERTION(ozz::math::NormalizeEst4(zero), "_v is not normalizable");

  const SimdFloat4 safe2 = ozz::math::NormalizeSafe2(f, unit);
  EXPECT_SIMDFLOAT_EQ(safe2, .4472136f, .8944272f, 4.f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized2(safe2), 0xffffffff, 0, 0, 0);
  const SimdFloat4 safe_est2 = ozz::math::NormalizeSafeEst2(f, unit);
  EXPECT_SIMDFLOAT_EQ_EST(safe_est2, .4472136f, .8944272f, 4.f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalizedEst2(safe_est2), 0xffffffff, 0, 0, 0);

  const SimdFloat4 safe3 = ozz::math::NormalizeSafe3(f, unit);
  EXPECT_SIMDFLOAT_EQ(safe3, .21821788f, .43643576f, .87287152f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized3(safe3), 0xffffffff, 0, 0, 0);
  const SimdFloat4 safe_est3 = ozz::math::NormalizeSafeEst3(f, unit);
  EXPECT_SIMDFLOAT_EQ_EST(safe_est3, .21821788f, .43643576f, .87287152f, 8.f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalizedEst3(safe_est3), 0xffffffff, 0, 0, 0);

  const SimdFloat4 safe4 = ozz::math::NormalizeSafe4(f, unit);
  EXPECT_SIMDFLOAT_EQ(safe4, .1084652f, .2169305f, .433861f, .8677219f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalized4(safe4), 0xffffffff, 0, 0, 0);
  const SimdFloat4 safe_est4 = ozz::math::NormalizeSafeEst4(f, unit);
  EXPECT_SIMDFLOAT_EQ_EST(safe_est4, .1084652f, .2169305f, .433861f, .8677219f);
  EXPECT_SIMDINT_EQ(ozz::math::IsNormalizedEst4(safe_est4), 0xffffffff, 0, 0, 0);

  const SimdFloat4 safer2 = ozz::math::NormalizeSafe2(zero, unit);
  EXPECT_SIMDFLOAT_EQ(safer2, 1.f, 0.f, 0.f, 0.f);
  const SimdFloat4 safer_est2 = ozz::math::NormalizeSafeEst2(zero, unit);
  EXPECT_SIMDFLOAT_EQ_EST(safer_est2, 1.f, 0.f, 0.f, 0.f);

  const SimdFloat4 safer3 = ozz::math::NormalizeSafe3(zero, unit);
  EXPECT_SIMDFLOAT_EQ(safer3, 1.f, 0.f, 0.f, 0.f);
  const SimdFloat4 safer_est3 = ozz::math::NormalizeSafeEst3(zero, unit);
  EXPECT_SIMDFLOAT_EQ_EST(safer_est3, 1.f, 0.f, 0.f, 0.f);

  const SimdFloat4 safer4 = ozz::math::NormalizeSafe4(zero, unit);
  EXPECT_SIMDFLOAT_EQ(safer4, 1.f, 0.f, 0.f, 0.f);
  const SimdFloat4 safer_est4 = ozz::math::NormalizeSafeEst4(zero, unit);
  EXPECT_SIMDFLOAT_EQ_EST(safer_est4, 1.f, 0.f, 0.f, 0.f);
}

TEST(CompareFloat, ozz_simd_math) {
  const SimdFloat4 a = ozz::math::simd_float4::Load(0.5f, 1.f, 2.f, 3.f);
  const SimdFloat4 b = ozz::math::simd_float4::Load(4.f, 1.f, -6.f, 7.f);
  const SimdFloat4 c = ozz::math::simd_float4::Load(4.f, 5.f, 6.f, 7.f);

  const SimdFloat4 min = ozz::math::Min(a, b);
  EXPECT_SIMDFLOAT_EQ(min, 0.5f, 1.f, -6.f, 3.f);

  const SimdFloat4 max = ozz::math::Max(a, b);
  EXPECT_SIMDFLOAT_EQ(max, 4.f, 1.f, 2.f, 7.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::Clamp(a, ozz::math::simd_float4::Load(-12.f, 2.f, 9.f, 3.f), c), .5f, 2.f, 6.f, 3.f);

  const SimdInt4 eq1 = ozz::math::CmpEq(a, b);
  EXPECT_SIMDINT_EQ(eq1, 0, 0xffffffff, 0, 0);

  const SimdInt4 eq2 = ozz::math::CmpEq(a, a);
  EXPECT_SIMDINT_EQ(eq2, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);

  const SimdInt4 neq1 = ozz::math::CmpNe(a, b);
  EXPECT_SIMDINT_EQ(neq1, 0xffffffff, 0, 0xffffffff, 0xffffffff);

  const SimdInt4 neq2 = ozz::math::CmpNe(a, a);
  EXPECT_SIMDINT_EQ(neq2, 0, 0, 0, 0);

  const SimdInt4 lt = ozz::math::CmpLt(a, b);
  EXPECT_SIMDINT_EQ(lt, 0xffffffff, 0, 0, 0xffffffff);

  const SimdInt4 le = ozz::math::CmpLe(a, b);
  EXPECT_SIMDINT_EQ(le, 0xffffffff, 0xffffffff, 0u, 0xffffffff);

  const SimdInt4 gt = ozz::math::CmpGt(a, b);
  EXPECT_SIMDINT_EQ(gt, 0u, 0u, 0xffffffff, 0u);

  const SimdInt4 ge = ozz::math::CmpGe(a, b);
  EXPECT_SIMDINT_EQ(ge, 0u, 0xffffffff, 0xffffffff, 0u);
}

TEST(LerpFloat, ozz_simd_math) {
  const SimdFloat4 a = ozz::math::simd_float4::Load(0.f, 1.f, 2.f, 4.f);
  const SimdFloat4 b = ozz::math::simd_float4::Load(0.f, -1.f, -2.f, -4.f);
  const SimdFloat4 zero = ozz::math::simd_float4::Load1(0.f);
  const SimdFloat4 one = ozz::math::simd_float4::Load1(1.f);

  const SimdFloat4 lerp0 = ozz::math::Lerp(a, b, zero);
  EXPECT_SIMDFLOAT_EQ(lerp0, 0.f, 1.f, 2.f, 4.f);

  const SimdFloat4 lerp1 = ozz::math::Lerp(a, b, one);
  EXPECT_SIMDFLOAT_EQ(lerp1, 0.f, -1.f, -2.f, -4.f);

  const SimdFloat4 lhalf = ozz::math::Lerp(a, b, ozz::math::simd_float4::Load1(0.5f));
  EXPECT_SIMDFLOAT_EQ(lhalf, 0.f, 0.f, 0.f, 0.f);

  const SimdFloat4 lmixed = ozz::math::Lerp(a, b, ozz::math::simd_float4::Load(0.f, -1.f, 0.5f, 2.f));
  EXPECT_SIMDFLOAT_EQ(lmixed, 0.f, 3.f, 0.f, -12.f);
}

TEST(TrigonometryFloat, ozz_simd_math) {
  using ozz::math::k2Pi;
  using ozz::math::kPi;
  using ozz::math::kPi_2;

  const SimdFloat4 angle = ozz::math::simd_float4::Load(kPi, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  const SimdFloat4 cos = ozz::math::simd_float4::Load(-1.f, .86602539f, 0.f, 0.f);
  const SimdFloat4 sin = ozz::math::simd_float4::Load(0.f, .5f, -1.f, 1.f);

  const SimdFloat4 angle_tan = ozz::math::simd_float4::Load(0.f, kPi / 6.f, -kPi / 3.f, 9.f * kPi / 4.f);
  const SimdFloat4 tan = ozz::math::simd_float4::Load(0.f, .57735f, -1.73205f, 1.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::Cos(angle), -1.f, .86602539f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::Cos(angle + ozz::math::simd_float4::Load1(k2Pi)), -1.f, .86602539f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::Cos(angle + ozz::math::simd_float4::Load1(k2Pi * 12.f)), -1.f, .86602539f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::Cos(angle - ozz::math::simd_float4::Load1(k2Pi * 24.f)), -1.f, .86602539f, 0.f, 0.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::CosX(angle), -1.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::CosX(angle + ozz::math::simd_float4::LoadX(k2Pi)), -1.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::CosX(angle + ozz::math::simd_float4::LoadX(k2Pi * 12.f)), -1.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::CosX(angle - ozz::math::simd_float4::LoadX(k2Pi * 24.f)), -1.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);

  EXPECT_SIMDFLOAT_EQ(ozz::math::Sin(angle), 0.f, .5f, -1.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::Sin(angle + ozz::math::simd_float4::Load1(k2Pi)), 0.f, 0.5f, -1.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::Sin(angle + ozz::math::simd_float4::Load1(k2Pi * 12.f)), 0.f, .5f, -1.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::Sin(angle - ozz::math::simd_float4::Load1(k2Pi * 24.f)), 0.f, .5f, -1.f, 1.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::SinX(angle), 0.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::SinX(angle + ozz::math::simd_float4::LoadX(k2Pi)), 0.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::SinX(angle + ozz::math::simd_float4::LoadX(k2Pi * 12.f)), 0.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::SinX(angle - ozz::math::simd_float4::LoadX(k2Pi * 24.f)), 0.f, kPi / 6.f, -kPi_2, 5.f * kPi_2);

  EXPECT_SIMDFLOAT_EQ(ozz::math::ACos(cos), kPi, kPi / 6.f, kPi_2, kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::ACosX(cos), kPi, .86602539f, 0.f, 0.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::ASin(sin), 0.f, kPi / 6.f, -kPi_2, kPi_2);
  EXPECT_SIMDFLOAT_EQ(ozz::math::ASinX(sin), 0.f, .5f, -1.f, 1.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::Tan(angle_tan), 0.f, 0.57735f, -1.73205f, 1.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::TanX(angle_tan), 0.f, kPi / 6.f, -kPi / 3.f, 9.f * kPi / 4.f);

  EXPECT_SIMDFLOAT_EQ(ozz::math::ATan(tan), 0.f, kPi / 6.f, -kPi / 3.f, kPi / 4.f);
  EXPECT_SIMDFLOAT_EQ(ozz::math::ATanX(tan), 0.f, .57735f, -1.73205f, 1.f);
}

TEST(LogicalFloat, ozz_simd_math) {
  const SimdFloat4 a = ozz::math::simd_float4::Load(0.f, 1.f, 2.f, 3.f);
  const SimdFloat4 b = ozz::math::simd_float4::Load(1.f, -1.f, -3.f, -4.f);
  const SimdInt4 m = ozz::math::simd_int4::Load(0xffffffff, 0, 0x80000000, 0x7fffffff);

  const SimdFloat4 select = ozz::math::Select(m, a , b);
  EXPECT_SIMDFLOAT_EQ(select, 0.f, -1.f, 3.f, -3.f);

  const SimdFloat4 andm = ozz::math::And(b, m);
  EXPECT_SIMDFLOAT_EQ(andm, 1.f, 0.f, 0.f, 4.f);

  const SimdFloat4 orm = ozz::math::Or(a, m);
  union {float f; unsigned int i;} orx = {ozz::math::GetX(orm)};
  EXPECT_EQ(orx.i, 0xffffffff);
  EXPECT_FLOAT_EQ(ozz::math::GetY(orm), 1.f);
  EXPECT_FLOAT_EQ(ozz::math::GetZ(orm), -2.f);
  union {float f; int i;} orw = {ozz::math::GetW(orm)};
  EXPECT_TRUE(orw.i == 0x7fffffff);

  const SimdFloat4 xorm = ozz::math::Xor(a, m);
  union {float f; unsigned int i;} xorx = {ozz::math::GetX(xorm)};
  EXPECT_TRUE(xorx.i == 0xffffffff);
  EXPECT_FLOAT_EQ(ozz::math::GetY(xorm), 1.f);
  EXPECT_FLOAT_EQ(ozz::math::GetZ(xorm), -2.f);
  union {float f; unsigned int i;} xorw = {ozz::math::GetW(xorm)};
  EXPECT_TRUE(xorw.i == 0x3fbfffff);
}
