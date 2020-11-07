//
// config.hpp
// ~~~~~~~~~~
//
// Copyright 2020 Lince
//

#ifndef ASYNCNET_DETAIL_CONFIG_HPP
#define ASYNCNET_DETAIL_CONFIG_HPP

#if !defined(ASYNCNET_HEADER_ONLY) & !defined(ASYNCNET_STANDARDESE)
#  if !defined(ASYNCNET_SEPARATE_COMPILATION)
#    if !defined(ASYNCNET_DYN_LINK)
#      define ASYNCNET_HEADER_ONLY 1
#    endif
#  endif
#endif

#if defined(ASYNCNET_HEADER_ONLY)
#  define ASYNCNET_DECL inline
#elif defined(ASYNCNET_DYN_LINK)
#  if defined(_MSC_VER)
#    if defined(ASYNCNET_SOURCE)
#      define ASYNCNET_DECL __declspec(dllexport)
#    else
#      define ASYNCNET_DECL __declspec(dllimport)
#    endif
#  elif defined(__GNUC__)
#    define ASYNCNET_DECL __attribute__((visibility("default")))
#  else
#    define ASYNCNET_DECL
#  endif
#else
#  define ASYNCNET_DECL
#endif

#ifndef ASYNCNET_DECL_INLINE
#  if defined(__GNUC__) && !defined(_WIN32)
#    define ASYNCNET_DECL_INLINE __always_inline
#  elif defined(_MSC_VER) || defined(_WIN32)
#    define ASYNCNET_DECL_INLINE __forceinline
#  else
#    define ASYNCNET_DECL_INLINE inline
#  endif
#endif

#ifdef _WIN32
#  define ASYNCNET_WINDOWS
#endif

#ifdef __COBALT__
#  define ASYNCNET_XENOMAI
#endif

#if !defined(_WIN32) || defined(__COBALT__)
#  define ASYNCNET_POSIX
#endif

#ifdef ASYNCNET_WINDOWS
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#endif

#endif
