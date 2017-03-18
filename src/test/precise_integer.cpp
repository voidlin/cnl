
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <sg14/auxiliary/precise_integer.h>
#include <sg14/bits/common.h>

#include <gtest/gtest.h>

namespace {
    using sg14::precise_integer;
    using std::is_same;
    using sg14::_impl::identical;

    namespace number_base_traits {
        using sg14::_impl::number_base;
        using sg14::_impl::is_number_base;
        using sg14::_impl::is_number;

        static_assert(!is_number_base<int>::value, "");
        static_assert(is_number_base<number_base<precise_integer<>, int>>::value, "");
        static_assert(!is_number_base<precise_integer<>>::value, "");

        static_assert(!is_number<int>::value, "");
        static_assert(!is_number<number_base<precise_integer<>, int>>::value, "");
        static_assert(is_number<precise_integer<>>::value, "");
    }

    namespace default_parameters {
        using sg14::closest_rounding_policy;

        using default_rep = int;

        template<typename T>
        using default_policy = sg14::closest_rounding_policy<T>;

        static_assert(is_same<precise_integer<>, precise_integer<default_rep, default_policy<default_rep>>>::value, "sg14::precise_integer parameter default test failed");

        static_assert(is_same<precise_integer<>::rep, default_rep>::value, "sg14::precise_integer parameter default test failed");
        static_assert(is_same<precise_integer<>::rounding, default_policy<default_rep>>::value, "sg14::precise_integer parameter default test failed");
    }

    namespace to_rep {
        using sg14::to_rep;

        static_assert(identical(123, to_rep(123)), "sg14::to_rep test failed");
        static_assert(identical(321, to_rep(precise_integer<>{321})), "sg14::to_rep test failed");
    }

    namespace test_closest_rounding_policy {
        using sg14::closest_rounding_policy;
        static_assert(closest_rounding_policy<int>::convert(0.) == 0, "sg14::closest_rounding_policy test failed");
        static_assert(closest_rounding_policy<int>::convert(-1.) == -1, "sg14::closest_rounding_policy test failed");
    }

    namespace closest {
        using precise_integer = sg14::precise_integer<>;

        static_assert(precise_integer{0.} == 0, "sg14::precise_integer test failed");
        static_assert(precise_integer{1.} == 1, "sg14::precise_integer test failed");
        static_assert(precise_integer{-1.} == -1, "sg14::precise_integer test failed");
        static_assert(precise_integer{.5} == 1, "sg14::precise_integer test failed");
        static_assert(precise_integer{-.5} == -1, "sg14::precise_integer test failed");
        static_assert(precise_integer{0.499} == 0, "sg14::precise_integer test failed");
        static_assert(precise_integer{-0.501} == -1, "sg14::precise_integer test failed");
    }
}
