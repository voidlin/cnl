
//          Copyright John McFarlane 2017.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests of features from library proposal, P0827

#include <cnl/bits/config.h>

#if __cpp_return_type_deduction>=201304 && __cpp_static_assert>=201411 && __cpp_template_auto>=201606 \
    && defined(CNL_EXCEPTIONS_ENABLED)

#include <cstdint>
#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////
// test helper

template<typename Expected, typename Actual>
constexpr auto identical(Expected const& expected, Actual const& actual) {
    static_assert(std::is_same<Expected, Actual>::value);
    return expected == actual;
}

////////////////////////////////////////////////////////////////////////////////
// constant definition

template<auto Value>
struct constant {
    using value_type = decltype(Value);
    static constexpr value_type value = Value;

    constexpr operator value_type() const {
        return value;
    }
};

////////////////////////////////////////////////////////////////////////////////
// operator overloads (incomplete set)

// unary
template<auto Value1>
constexpr auto operator!(constant<Value1>) noexcept {
    return constant<! Value1>{};
}

template<auto Value1>
constexpr auto operator+(constant<Value1>) noexcept {
    return constant<+ Value1>{};
}

template<auto Value1>
constexpr auto operator-(constant<Value1>) noexcept {
    return constant<- Value1>{};
}

// binary
template<auto Value1, auto Value2>
constexpr auto operator+(constant<Value1>, constant<Value2>) noexcept {
    return constant<Value1 + Value2>{};
}

template<auto Value1, auto Value2>
constexpr auto operator-(constant<Value1>, constant<Value2>) noexcept {
    return constant<Value1 - Value2>{};
}

template<auto Value1, auto Value2>
constexpr auto operator*(constant<Value1>, constant<Value2>) noexcept {
    return constant<Value1 * Value2>{};
}

template<auto Value1, auto Value2>
constexpr auto operator/(constant<Value1>, constant<Value2>) noexcept {
    return constant<Value1 / Value2>{};
}

template<auto Value1, auto Value2>
constexpr auto operator==(constant<Value1>, constant<Value2>) noexcept {
    return constant<Value1 == Value2>{};
}

template<auto Value1, auto Value2>
constexpr auto operator!=(constant<Value1>, constant<Value2>) noexcept {
    return constant<Value1 != Value2>{};
}

static_assert(identical(constant<3>{}, constant<1>{} + constant<2>{}));

////////////////////////////////////////////////////////////////////////////////
// user-defined literal

namespace udl_impl {

    ////////////////////////////////////////////////////////////////////////////////
    // digit parsers

    constexpr int parse_bin_char(char c) {
        return (c == '0') ? 0 : (c == '1') ? 1 : throw std::invalid_argument("invalid hexadecimal digits");
    }

    constexpr int parse_dec_char(char c) {
        return (c >= '0' && c <= '9') ? c - '0' : throw std::invalid_argument("invalid decimal digits");
    }

    constexpr int parse_oct_char(char c) {
        return (c >= '0' && c <= '7') ? c - '0' : throw std::invalid_argument("invalid octal digits");
    }

    constexpr int parse_hex_char(char c) {
        return (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'z') ? c + 10 - 'a' : (c >= 'A' && c <= 'Z') ? c
                + 10 - 'A' : throw std::invalid_argument("invalid hexadecimal digits");
    }

    ////////////////////////////////////////////////////////////////////////////////
    // known-base integer literal parser

    template<typename ParseDigit>
    constexpr std::intmax_t parse(char const* s, int base, ParseDigit parse_digit, std::intmax_t value = 0) {
        auto c = *s;
        if (!c) {
            return value;
        }

        return parse(s + 1, base, parse_digit, parse_digit(c) + value * base);
    }

    static_assert(identical(std::intmax_t{0}, parse("0", 10, parse_dec_char)));
    static_assert(identical(std::intmax_t{1}, parse("1", 10, parse_dec_char)));
    static_assert(identical(std::intmax_t{9}, parse("9", 10, parse_dec_char)));
    static_assert(identical(std::intmax_t{10}, parse("10", 10, parse_dec_char)));
    static_assert(identical(std::intmax_t{123}, parse("123", 10, parse_dec_char)));

    ////////////////////////////////////////////////////////////////////////////////
    // general purpose integer literal parser

    template<int NumChars>
    constexpr std::intmax_t parse(const char (& s)[NumChars]) {
        if (s[0] == '0') {
            if (s[1] == 'b' || s[1] == 'B') {
                return parse(s + 2, 2, parse_bin_char);
            }
            else if (s[1] == 'x' || s[1] == 'X') {
                return parse(s + 2, 16, parse_hex_char);
            }
            else {
                return parse(s + 1, 8, parse_oct_char);
            }
        }
        else {
            return parse(s, 10, parse_dec_char);
        }
    }

    static_assert(identical(parse("123"), std::intmax_t{123}));
    static_assert(identical(parse("0b10"), std::intmax_t{2}));
    static_assert(identical(parse("07"), std::intmax_t{7}));
    static_assert(identical(parse("0x2A"), std::intmax_t{42}));
    static_assert(identical(parse("3"), std::intmax_t{3}));
}

template<char ... Chars>
constexpr auto operator "" _static() noexcept {
    return constant<udl_impl::parse<sizeof...(Chars) + 1>({Chars..., '\0'})>{};
}

static_assert(identical(1_static, constant<std::intmax_t{1}>{}));
static_assert(identical(3_static, 1_static + 2_static));

////////////////////////////////////////////////////////////////////////////////
// demonstration

constexpr auto z = 0b10_static * 07_static + 0x2A_static / -3_static;

// value of z is result of 2*7 + 42/-3
static_assert(z == 0);

// type of z is an instance of constant<>
static_assert(std::is_same<const constant<intmax_t{0}>, decltype(z)>::value);

#endif
