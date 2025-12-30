/*
 * ██████╗  █████╗ ██╗  ██╗███████╗
 * ██╔══██╗██╔══██╗╚██╗██╔╝██╔════╝
 * ██║  ██║███████║ ╚███╔╝ █████╗  
 * ██║  ██║██╔══██║ ██╔██╗ ██╔══╝  
 * ██████╔╝██║  ██║██╔╝ ██╗███████╗
 * ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
 *
 * D.A's Axe - Cut through C++ verbosity
 * Created by Dhimas Ardinata (D.A)
 *
 * PHILOSOPHY:
 * 1. Safe by Default     - Options, bounds checks
 * 2. Zero Abstraction    - No overhead wrappers
 * 3. Explicit > Implicit - Clear naming, no magic
 * 4. General by Default  - Smart type detection
 * 5. Composable          - Small, chainable utilities
 * 6. Semantic Naming     - No cryptic abbreviations
 *
 * COMPATIBILITY: C++11 through C++26
 *
 * USAGE:
 *   #include <daxe.h>
 *
 * NAMESPACE:
 *   All types in `dax::` namespace
 *   Auto-imported to global (disable with DAXE_NO_GLOBAL)
 */

#ifndef DAXE_H
#define DAXE_H

// Configuration
#include "daxe/config.h"

// Core types
#include "daxe/base.h"
#include "daxe/english.h"
#include "daxe/pairs.h"
#include "daxe/vectors.h"
#include "daxe/containers.h"

// Macros and utilities
#include "daxe/macros.h"

// Safety (needed by functions.h)
#include "daxe/safe.h"

// I/O and functions
#include "daxe/io.h"
#include "daxe/math.h"
#include "daxe/functions.h"
#include "daxe/random.h"
#include "daxe/time.h"
#include "daxe/convert.h"

// Pythonic features
#include "daxe/pythonic.h"
#include "daxe/range.h"
#include "daxe/grid.h"
#include "daxe/graph.h"

// Debug utilities
#include "daxe/debug.h"

// Auto-import namespace to global scope (unless disabled)
// This aligns with "Cut through C++ verbosity" philosophy
#if !defined(DAXE_NO_GLOBAL) && !defined(DAXE_NO_NAMESPACE)
    using namespace dax;
#endif

#endif // DAXE_H
