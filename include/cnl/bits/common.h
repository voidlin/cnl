
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(CNL_COMMON_H)
#define CNL_COMMON_H 1

#include <utility>

#define CNL_STR_HELPER(x) #x
#define CNL_STR(x) CNL_STR_HELPER(x)

////////////////////////////////////////////////////////////////////////////////
// CNL_ASSERT - with passing resemblance to code from github.com/Microsoft/GSL

#if defined(CNL_LIKELY)
#error CNL_LIKELY already defined.
#elif defined(CNL_ASSUME)
#error CNL_ASSUME already defined.
#elif defined(CNL_ASSERT)
#error CNL_ASSERT already defined.
#endif

// Customization point: define either CNL_DEBUG or CNL_RELEASE
// to affect how contract violations are handled
// in builds that don't support contract [[attributes]].
#if defined(CNL_DEBUG) && defined(CNL_RELEASE)
#error Mutually exclusive macros, CNL_DEBUG are both defined CNL_RELEASE.
#elif !defined(CNL_DEBUG) && !defined(CNL_RELEASE)
#if defined(NDEBUG)
#define CNL_RELEASE
#else
#define CNL_DEBUG
#endif
#endif

// CNL_LIKELY - hints that a condition is likely to be true
#if defined(__clang__) || defined(__GNUC__)
#define CNL_LIKELY(CONDITION) __builtin_expect(!!(CONDITION), 1)
#define CNL_UNLIKELY(CONDITION) __builtin_expect(!!(CONDITION), 0)
#else
#define CNL_LIKELY(CONDITION) (!!(CONDITION))
#define CNL_UNLIKELY(CONDITION) (!!(CONDITION))
#endif

// CNL_ASSUME - hints that a condition *must* be true
#ifdef _MSC_VER
#define CNL_ASSUME(cond) __assume(cond)
#elif defined(__GNUC__)
#define CNL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define CNL_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif

// CNL_ASSERT - enforces that a condition *must* be true
#if defined(CNL_RELEASE)
// in release builds,
#define CNL_ASSERT(CONDITION) CNL_ASSUME(CONDITION)
#elif defined(CNL_DEBUG)
#include <exception>
#define CNL_ASSERT(CONDITION) (CNL_LIKELY(CONDITION) ? static_cast<void>(0) : std::terminate())
#else
#error internal library error
#endif

namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::min

        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::deleted_fn

        // a great function to call if you need a compiler error
        template<typename T>
        constexpr T deleted_fn() = delete;
    }
}

#endif  // CNL_COMMON_H
