/// @file config_int64_t.h
/// @author Nikolaos D. Kallimanis (nkallima@gmail.com)
/// @brief This file provides important parameters and constants for the benchmarks and the library.
#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef VOID_PTR_OBJECT
#include "config_void_ptr.h"
#elif defined(INT64_T_OBJECT)
#include "config_int64_t.h"
#else
#error "No object type defined"
#endif

#endif
