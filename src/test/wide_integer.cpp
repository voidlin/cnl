
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <v3/int.h>
#include <cnl/fixed_point.h>

#include <gtest/gtest.h>

template<int Digits, typename Signedness, int Exponent>
using wide_fixed_point = cnl::fixed_point<std::wide_integer<(Digits+CHAR_BIT-1)&(~(CHAR_BIT-1)), Signedness>, Exponent>;

//TEST(wide_integer, bitwise_shift) {
//    std::wide_integer<256, signed> x{1};
//    auto y = x << 128;
//    EXPECT_EQ(1 << 30, y >> (128-30));
//}
//
//TEST(wide_integer, cnl__impl_default_scale) {
//    std::wide_integer<256, signed> x{1};
//    auto y = cnl::_impl::default_scale<128, 2, decltype(x)>()(x);
//    EXPECT_EQ(1 << 30, y >> (128-30));
//}

//TEST(wide_integer, cnl_shift) {
//    std::wide_integer<256, signed> x{1};
//    auto y = cnl::shift<128, 2, decltype(x)>(x);
//    EXPECT_EQ(1 << 30, y >> (128-30));
//}

TEST(wide_fixed_point, bitwise_shift) {
    using fp_127_128 = wide_fixed_point<255, signed, -128>;
    auto a = fp_127_128{1};
//    auto b = a << 126;
//    auto b = a / 3;
//    EXPECT_NEAR(b, .333333333333333333333, .00000000001);
}
