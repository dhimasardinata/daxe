/*
 * DAXE CORE - Minimal Include
 * D.A's Axe - Cut through C++ verbosity
 * 
 * Use this for embedded/lightweight scenarios.
 * Includes: Types, Macros, Safety, Math (no I/O, no Random, no Debug)
 */

#ifndef DAXE_CORE_H
#define DAXE_CORE_H

// Configuration
#include "daxe/config.h"

// Core types
#include "daxe/base.h"
#include "daxe/english.h"
#include "daxe/pairs.h"
#include "daxe/vectors.h"
#include "daxe/containers.h"

// Macros
#include "daxe/macros.h"

// Safety
#include "daxe/safe.h"

// Math (no RNG dependency)
#include "daxe/math.h"

#endif // DAXE_CORE_H
