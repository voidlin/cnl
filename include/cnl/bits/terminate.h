
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_TERMINATE_H
#define CNL_TERMINATE_H

#include "config.h"

#include <exception>
#include <cstdio>

namespace cnl {
    namespace _impl {
#if defined(CNL_DEBUG)
        template<class Result>
        [[noreturn]] CNL_RELAXED_CONSTEXPR Result terminate(char const* message) noexcept
        {
            std::fputs(message, stderr);
            std::terminate();
        }
#endif

#if defined(CNL_RELEASE)
        template<class Result>
        [[noreturn]] CNL_RELAXED_CONSTEXPR Result terminate(char const*) noexcept
        {
            std::terminate();
        }
#endif
    }
}

#endif //CNL_TERMINATE_H
