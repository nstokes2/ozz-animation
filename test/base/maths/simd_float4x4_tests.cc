//============================================================================//
//                                                                            //
// ozz-animation, 3d skeletal animation libraries and tools.                  //
// https://code.google.com/p/ozz-animation/                                   //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
// Copyright (c) 2012-2015 Guillaume Blanc                                    //
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
//                                                                            //
//============================================================================//

#include "ozz/base/maths/simd_math.h"

#include "gtest/gtest.h"

#include "ozz/base/gtest_helper.h"
#include "ozz/base/maths/math_constant.h"
#include "ozz/base/maths/math_ex.h"
#include "ozz/base/maths/gtest_math_helper.h"

using ozz::math::SimdFloat4;
using ozz::math::Float4x4;

TEST(Constant, Float4x4) {
  const Float4x4 identity = Float4x4::identity();
  EXPECT_FLOAT4x4_EQ(identity, 1.f, 0.f, 0.f, 0.f,
                               0.f, 1.f, 0.f, 0.f,
                               0.f, 0.f, 1.f, 0.f,
                               0.f, 0.f, 0.f, 1.f);
}

TEST(Arithmetic, Float4x4) {
  const Float4x4 m0 = {{ozz::math::simd_float4::Load(0.f, 1.f, 2.f, 3.f),
                        ozz::math::simd_float4::Load(4.f, 5.f, 6.f, 7.f),
                        ozz::math::simd_float4::Load(8.f, 9.f, 10.f, 11.f),
                        ozz::math::simd_float4::Load(12.f, 13.f, 14.f, 15.f)}};
  const Float4x4 m1 = {{ozz::math::simd_float4::Load(-0.f, -1.f, -2.f, -3.f),
                        ozz::math::simd_float4::Load(-4.f, -5.f, -6.f, -7.f),
                        ozz::math::simd_float4::Load(-8.f, -9.f, -10.f, -11.f),
                        ozz::math::simd_float4::Load(-12.f, -13.f, -14.f, -15.f)}};
  const Float4x4 m2 = {{ozz::math::simd_float4::Load(0.f, -1.f, 2.f, 3.f),
                        ozz::math::simd_float4::Load(-4.f, 5.f, 6.f, -7.f),
                        ozz::math::simd_float4::Load(8.f, -9.f, -10.f, 11.f),
                        ozz::math::simd_float4::Load(-12.f, 13.f, -14.f, 15.f)}};
  const SimdFloat4 v = ozz::math::simd_float4::Load(-0.f, -1.f, -2.f, -3.f);

  const SimdFloat4 mul_vector = m0 * v;
  EXPECT_SIMDFLOAT_EQ(mul_vector, -56.f, -62.f, -68.f, -74.f);

  const SimdFloat4 transform_point = TransformPoint(m0, v);
  EXPECT_SIMDFLOAT_EQ(transform_point, -8.f, -10.f, -12.f, -14.f);

  const SimdFloat4 transform_vector = TransformVector(m0, v);
  EXPECT_SIMDFLOAT_EQ(transform_vector, -20.f, -23.f, -26.f, -29.f);

  const Float4x4 mul_mat = m0 * m1;
  EXPECT_FLOAT4x4_EQ(mul_mat, -56.f, -62.f, -68.f, -74.f,
                              -152.f, -174.f, -196.f, -218.f,
                              -248.f, -286.f, -324.f, -362.f,
                              -344.f, -398.f, -452.f, -506.f);

  const Float4x4 add_mat = m0 + m1;
  EXPECT_FLOAT4x4_EQ(add_mat, 0.f, 0.f, 0.f, 0.f,
                              0.f, 0.f, 0.f, 0.f,
                              0.f, 0.f, 0.f, 0.f,
                              0.f, 0.f, 0.f, 0.f);

  const Float4x4 sub_mat = m0 - m1;
  EXPECT_FLOAT4x4_EQ(sub_mat, 0.f, 2.f, 4.f, 6.f,
                              8.f, 10.f, 12.f, 14.f,
                              16.f, 18.f, 20.f, 22.f,
                              24.f, 26.f, 28.f, 30.f);

  const Float4x4 transpose = Transpose(m0);
  EXPECT_FLOAT4x4_EQ(transpose, 0.f, 4.f, 8.f, 12.f,
                                1.f, 5.f, 9.f, 13.f,
                                2.f, 6.f, 10.f, 14.f,
                                3.f, 7.f, 11.f, 15.f);

  const Float4x4 invert_ident = Invert(Float4x4::identity());
  EXPECT_FLOAT4x4_EQ(invert_ident, 1.f, 0.f, 0.f, 0.f,
                                   0.f, 1.f, 0.f, 0.f,
                                   0.f, 0.f, 1.f, 0.f,
                                   0.f, 0.f, 0.f, 1.f);

  //EXPECT_ASSERTION(Invert(m0), "Matrix is not invertible");
  const Float4x4 invert = Invert(m2);
  EXPECT_FLOAT4x4_EQ(invert, .216667f, 2.75f, 1.6f, .066666f,
                             .2f, 2.5f, 1.4f, .1f,
                             .25f, .5f, .25f, 0.f,
                             .233333f, .5f, .3f, .03333f);

  const Float4x4 invert_mul = m2 * invert;
  EXPECT_FLOAT4x4_EQ(invert_mul, 1.f, 0.f, 0.f, 0.f,
                                 0.f, 1.f, 0.f, 0.f,
                                 0.f, 0.f, 1.f, 0.f,
                                 0.f, 0.f, 0.f, 1.f);
}


TEST(Normal, Float4x4) {
  const Float4x4 not_orthogonal = {{ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f)}};
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(not_orthogonal)));
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, -1.f, 1.f, 0.f)))));
  EXPECT_FALSE(ozz::math::AreAllTrue3(IsNormalized(
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, 46.f, 1.f, 0.f)))));
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(Float4x4::identity())));
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(
    Float4x4::FromAxisAngle(ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 1.24f)))));
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(
    Float4x4::Translation(ozz::math::simd_float4::Load(46.f, 0.f, 0.f, 1.f)))));
}

TEST(Orthogonal, Float4x4) {
  const Float4x4 zero = {{ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 0.f),
                          ozz::math::simd_float4::Load(0.f, 1.f, 0.f, 0.f),
                          ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f),
                          ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f)}};
  const Float4x4 not_orthogonal = {{ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f)}};

  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(not_orthogonal)));
  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(zero)));

  const Float4x4 reflexion1x =
    Float4x4::Scaling(ozz::math::simd_float4::Load(-1.f, 1.f, 1.f, 0.f));
  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion1x)));
  const Float4x4 reflexion1y =
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, -1.f, 1.f, 0.f));
  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion1y)));
  const Float4x4 reflexion1z =
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, 1.f, -1.f, 0.f));
  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion1z)));
  const Float4x4 reflexion2x =
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, -1.f, -1.f, 0.f));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion2x)));
  const Float4x4 reflexion2y =
    Float4x4::Scaling(ozz::math::simd_float4::Load(-1.f, 1.f, -1.f, 0.f));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion2y)));
  const Float4x4 reflexion2z =
    Float4x4::Scaling(ozz::math::simd_float4::Load(-1.f, -1.f, 1.f, 0.f));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion2z)));
  const Float4x4 reflexion3 =
    Float4x4::Scaling(ozz::math::simd_float4::Load(-1.f, -1.f, -1.f, 0.f));
  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(reflexion3)));

  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(Float4x4::identity())));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(
    Float4x4::Translation(ozz::math::simd_float4::Load(46.f, 0.f, 0.f, 1.f)))));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(
    Float4x4::FromAxisAngle(ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 1.24f)))));
}

TEST(Translate, Float4x4) {
  const SimdFloat4 v = ozz::math::simd_float4::Load(-1.f, 1.f, 2.f, 3.f);
  const Float4x4 m0 = {{ozz::math::simd_float4::Load(0.f, 1.f, 2.f, 3.f),
                        ozz::math::simd_float4::Load(4.f, 5.f, 6.f, 7.f),
                        ozz::math::simd_float4::Load(8.f, 9.f, 10.f, 11.f),
                        ozz::math::simd_float4::Load(12.f, 13.f, 14.f, 15.f)}};

  const Float4x4 translation = Float4x4::Translation(v);
  EXPECT_FLOAT4x4_EQ(translation, 1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  -1.f, 1.f, 2.f, 1.f);

  const Float4x4 translate_mul = m0 * translation;
  EXPECT_FLOAT4x4_EQ(translate_mul, 0.f, 1.f, 2.f, 3.f,
                                    4.f, 5.f, 6.f, 7.f,
                                    8.f, 9.f, 10.f, 11.f,
                                    32.f, 35.f, 38.f, 41.f);

  const Float4x4 translate = Translate(m0, v);
  EXPECT_FLOAT4x4_EQ(translate, 0.f, 1.f, 2.f, 3.f,
                                4.f, 5.f, 6.f, 7.f,
                                8.f, 9.f, 10.f, 11.f,
                                32.f, 35.f, 38.f, 41.f);
}

TEST(Scale, Float4x4) {
  const SimdFloat4 v = ozz::math::simd_float4::Load(-1.f, 1.f, 2.f, 3.f);
  const Float4x4 m0 = {{ozz::math::simd_float4::Load(0.f, 1.f, 2.f, 3.f),
                        ozz::math::simd_float4::Load(4.f, 5.f, 6.f, 7.f),
                        ozz::math::simd_float4::Load(8.f, 9.f, 10.f, 11.f),
                        ozz::math::simd_float4::Load(12.f, 13.f, 14.f, 15.f)}};

  const Float4x4 scaling = Float4x4::Scaling(v);
  EXPECT_FLOAT4x4_EQ(scaling, -1.f, 0.f, 0.f, 0.f,
                              0.f, 1.f, 0.f, 0.f,
                              0.f, 0.f, 2.f, 0.f,
                              0.f, 0.f, 0.f, 1.f);

  const Float4x4 scale_mul = m0 * scaling;
  EXPECT_FLOAT4x4_EQ(scale_mul, 0.f, -1.f, -2.f, -3.f,
                                4.f, 5.f, 6.f, 7.f,
                                16.f, 18.f, 20.f, 22.f,
                                12.f, 13.f, 14.f, 15.f);

  const Float4x4 scale = Scale(m0, v);
  EXPECT_FLOAT4x4_EQ(scale, 0.f, -1.f, -2.f, -3.f,
                            4.f, 5.f, 6.f, 7.f,
                            16.f, 18.f, 20.f, 22.f,
                            12.f, 13.f, 14.f, 15.f);
}

TEST(ColumnMultiply, Float4x4) {
  const SimdFloat4 v = ozz::math::simd_float4::Load(-1.f, -2.f, -3.f, -4.f);
  const Float4x4 m0 = {{ozz::math::simd_float4::Load(0.f, 1.f, 2.f, 3.f),
                        ozz::math::simd_float4::Load(4.f, 5.f, 6.f, 7.f),
                        ozz::math::simd_float4::Load(8.f, 9.f, 10.f, 11.f),
                        ozz::math::simd_float4::Load(12.f, 13.f, 14.f, 15.f)}};

  const Float4x4 column_multiply = ozz::math::ColumnMultiply(m0, v);
  EXPECT_FLOAT4x4_EQ(column_multiply, 0.f, -2.f, -6.f, -12.f,
                                      -4.f, -10.f, -18.f, -28.f,
                                      -8.f, -18.f, -30.f, -44.f,
                                      -12.f, -26.f, -42.f, -60.f);
}

TEST(Rotate, Float4x4) {
  const Float4x4 euler_identity = Float4x4::FromEuler(ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 0.f));
  EXPECT_FLOAT4x4_EQ(euler_identity, 1.f, 0.f, 0.f, 0.f,
                                     0.f, 1.f, 0.f, 0.f,
                                     0.f, 0.f, 1.f, 0.f,
                                     0.f, 0.f, 0.f, 1.f);

  const Float4x4 euler = Float4x4::FromEuler(ozz::math::simd_float4::Load(ozz::math::kPi_2, 0.f, 0.f, 0.f));
  EXPECT_FLOAT4x4_EQ(euler, 0.f, 0.f, 1.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            -1.f, 0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f, 1.f);
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(euler)));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(euler)));

  EXPECT_ASSERTION(Float4x4::FromQuaternion(ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 1.f)), "IsNormalized");
  const Float4x4 quaternion_identity = Float4x4::FromQuaternion(ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f));
  EXPECT_FLOAT4x4_EQ(quaternion_identity, 1.f, 0.f, 0.f, 0.f,
                                          0.f, 1.f, 0.f, 0.f,
                                          0.f, 0.f, 1.f, 0.f,
                                          0.f, 0.f, 0.f, 1.f);
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(quaternion_identity)));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(quaternion_identity)));

  const Float4x4 quaternion = Float4x4::FromQuaternion(ozz::math::simd_float4::Load(0.f, .70710677f, 0.f, .70710677f));
  EXPECT_FLOAT4x4_EQ(quaternion, 0.f, 0.f, -1.f, 0.f,
                                 0.f, 1.f, 0.f, 0.f,
                                 1.f, 0.f, 0.f, 0.f,
                                 0.f, 0.f, 0.f, 1.f);

  EXPECT_ASSERTION(Float4x4::FromAxisAngle(ozz::math::simd_float4::Load(1.f, 1.f, 0.f, 0.f)), "IsNormalized");
  const Float4x4 axis_angle_identity = Float4x4::FromAxisAngle(ozz::math::simd_float4::Load(0.f, 1.f, 0.f, 0.f));
  EXPECT_FLOAT4x4_EQ(axis_angle_identity, 1.f, 0.f, 0.f, 0.f,
                                          0.f, 1.f, 0.f, 0.f,
                                          0.f, 0.f, 1.f, 0.f,
                                          0.f, 0.f, 0.f, 1.f);

  const Float4x4 axis_angle = Float4x4::FromAxisAngle(ozz::math::simd_float4::Load(0.f, 1.f, 0.f, ozz::math::kPi_2));
  EXPECT_FLOAT4x4_EQ(axis_angle, 0.f, 0.f, -1.f, 0.f,
                                 0.f, 1.f, 0.f, 0.f,
                                 1.f, 0.f, 0.f, 0.f,
                                 0.f, 0.f, 0.f, 1.f);
  EXPECT_TRUE(ozz::math::AreAllTrue3(IsNormalized(axis_angle)));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(axis_angle)));
}

TEST(Affine, Float4x4) {
  EXPECT_ASSERTION(Float4x4::FromAffine(ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 0.f),
                                        ozz::math::simd_float4::Load(0.f, 1.f, 0.f, 1.f),
                                        ozz::math::simd_float4::Load(1.f, 1.f, 1.f, 1.f)), "IsNormalized");

  const Float4x4 identity = Float4x4::FromAffine(ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 0.f),
                                                 ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f),
                                                 ozz::math::simd_float4::Load(1.f, 1.f, 1.f, 1.f));
  EXPECT_FLOAT4x4_EQ(identity, 1.f, 0.f, 0.f, 0.f,
                               0.f, 1.f, 0.f, 0.f,
                               0.f, 0.f, 1.f, 0.f,
                               0.f, 0.f, 0.f, 1.f);

  const Float4x4 affine = Float4x4::FromAffine(ozz::math::simd_float4::Load(-12.f, 46.f, 12.f, 9.f),
                                               ozz::math::simd_float4::Load(0.f, .70710677f, 0.f, .70710677f),
                                               ozz::math::simd_float4::Load(2.f, 46.f, 3.f, 1.f));
  EXPECT_FLOAT4x4_EQ(affine, 0.f, 0.f, -2.f, 0.f,
                             0.f, 46.f, 0.f, 0.f,
                             3.f, 0.f, 0.f, 0.f,
                             -12.f, 46.f, 12.f, 1.f);
  EXPECT_FALSE(ozz::math::AreAllTrue3(IsNormalized(affine)));
  EXPECT_TRUE(ozz::math::AreAllTrue1(IsOrthogonal(affine)));

  const Float4x4 affine_reflexion = Float4x4::FromAffine(ozz::math::simd_float4::Load(-12.f, 46.f, 12.f, 9.f),
                                               ozz::math::simd_float4::Load(0.f, .70710677f, 0.f, .70710677f),
                                               ozz::math::simd_float4::Load(2.f, -1.f, 3.f, 1.f));
  EXPECT_FLOAT4x4_EQ(affine_reflexion, 0.f, 0.f, -2.f, 0.f,
                             0.f, -1.f, 0.f, 0.f,
                             3.f, 0.f, 0.f, 0.f,
                             -12.f, 46.f, 12.f, 1.f);
  EXPECT_FALSE(ozz::math::AreAllTrue3(IsNormalized(affine_reflexion)));
  EXPECT_FALSE(ozz::math::AreAllTrue1(IsOrthogonal(affine_reflexion)));
}

TEST(ToQuaternion, Float4x4) {
#ifndef NDEBUG
  const Float4x4 not_normalized = {{ozz::math::simd_float4::Load(1.1f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 1.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f)}};
  const Float4x4 not_orthogonal = {{ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f),
                                    ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f)}};
#endif  // NDEBUG

  EXPECT_ASSERTION(ToQuaternion(not_normalized), "IsNormalized");
  EXPECT_ASSERTION(ToQuaternion(not_orthogonal), "IsOrthogonal");
  EXPECT_SIMDFLOAT_EQ(ToQuaternion(Float4x4::identity()), 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f))),
    0.f, 0.f, 1.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(0.f, 1.f, 0.f, 0.f))),
    0.f, 1.f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f))),
    1.f, 0.f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(.70710677f, 0.f, 0.f, .70710677f))),
    .70710677f, 0.f, 0.f, .70710677f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(.4365425f, .017589169f, -.30435428f, .84645736f))),
    .4365425f, .017589169f, -.30435428f, .84645736f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(.56098551f, -.092295974f, 0.43045932f, .70105737f))),
    .56098551f, -.092295974f, 0.43045932f, .70105737f);
  EXPECT_SIMDFLOAT_EQ(
    ToQuaternion(Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(-.6172133f, -.1543033f, 0.f, .7715167f))),
    -.6172133f, -.1543033f, 0.f, .7715167f);
}

TEST(ToAffine, Float4x4) {
  SimdFloat4 translate = ozz::math::simd_float4::zero();
  SimdFloat4 rotate = ozz::math::simd_float4::zero();
  SimdFloat4 scale = ozz::math::simd_float4::zero();

  EXPECT_FALSE(ToAffine(
    Float4x4::Scaling(ozz::math::simd_float4::Load(0.f, 0.f, 1.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_FALSE(ToAffine(
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, 0.f, 0.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_FALSE(ToAffine(
    Float4x4::Scaling(ozz::math::simd_float4::Load(0.f, 1.f, 0.f, 0.f)),
    &translate, &rotate, &scale));

  EXPECT_TRUE(ToAffine(Float4x4::identity(), &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(scale, 1.f, 1.f, 1.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Scaling(ozz::math::simd_float4::Load(0.f, 1.f, 1.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(scale, 0.f, 1.f, 1.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, 0.f, 1.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(scale, 1.f, 0.f, 1.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Scaling(ozz::math::simd_float4::Load(1.f, 1.f, 0.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(scale, 1.f, 1.f, 0.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Translation(ozz::math::simd_float4::Load(46.f, 69.f, 58.f, 1.f)) *
    Float4x4::Scaling(ozz::math::simd_float4::Load(2.f, 3.f, 4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 46.f, 69.f, 58.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, 3.f, 4.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Translation(ozz::math::simd_float4::Load(46.f, -69.f, -58.f, 1.f)) *
    Float4x4::Scaling(ozz::math::simd_float4::Load(-2.f, 3.f, 4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 46.f, -69.f, -58.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 1.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, -3.f, 4.f, 1.f);

  EXPECT_TRUE(ToAffine(Float4x4::Scaling(ozz::math::simd_float4::Load(2.f, -3.f, 4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, -3.f, 4.f, 1.f);

  EXPECT_TRUE(ToAffine(Float4x4::Scaling(ozz::math::simd_float4::Load(2.f, 3.f, -4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 1.f, 0.f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, -3.f, 4.f, 1.f);

   // This one is not a reflexion.
  EXPECT_TRUE(ToAffine(Float4x4::Scaling(ozz::math::simd_float4::Load(-2.f, -3.f, 4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 0.f, 0.f, 1.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, 3.f, 4.f, 1.f);

   // This one is not a reflexion.
  EXPECT_TRUE(ToAffine(Float4x4::Scaling(ozz::math::simd_float4::Load(2.f, -3.f, -4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, 1.f, 0.f, 0.f, 0.f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, 3.f, 4.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Translation(ozz::math::simd_float4::Load(46.f, -69.f, -58.f, 1.f)) *
    Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(-.6172133f, -.1543033f, 0.f, .7715167f)) *
    Float4x4::Scaling(ozz::math::simd_float4::Load(2.f, 3.f, 4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 46.f, -69.f, -58.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, -.6172133f, -.1543033f, 0.f, .7715167f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, 3.f, 4.f, 1.f);

  EXPECT_TRUE(ToAffine(
    Float4x4::Translation(ozz::math::simd_float4::Load(46.f, -69.f, -58.f, 1.f)) *
    Float4x4::FromQuaternion(
      ozz::math::simd_float4::Load(.70710677f, 0.f, 0.f, .70710677f)) *
    Float4x4::Scaling(ozz::math::simd_float4::Load(2.f, -3.f, 4.f, 0.f)),
    &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 46.f, -69.f, -58.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, .70710677f, 0.f, 0.f, .70710677f);
  EXPECT_SIMDFLOAT_EQ(scale, 2.f, -3.f, 4.f, 1.f);

  Float4x4 trace = {{
    ozz::math::simd_float4::Load(-.916972f, 0.f, -.398952f, 0.f),
    ozz::math::simd_float4::Load(0.f, -1, 0.f, 0.f),
    ozz::math::simd_float4::Load(-.398952f, 0, .916972f, 0.f),
    ozz::math::simd_float4::Load(0.f, 0.f, 0.f, 1.f)}};
  EXPECT_TRUE(ToAffine(trace, &translate, &rotate, &scale));
  EXPECT_SIMDFLOAT_EQ(translate, 0.f, 0.f, 0.f, 1.f);
  EXPECT_SIMDFLOAT_EQ(rotate, -.20375007f, 0.f, .97902298f, 0.f);
  EXPECT_SIMDFLOAT_EQ(scale, 1.f, 1.f, 1.f, 1.f);
}
