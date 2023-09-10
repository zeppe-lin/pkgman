//! \file   pathnames.h
//! \brief  Absolute filenames that pkgman wants for various defaults.
//!         See COPYING and COPYRIGHT files for corresponding information.

#pragma once

//!< Default location for pkgman configuration.
#define PATH_CONFIG         "/etc/pkgman.conf"

//!< Default location for the package database.
#define PATH_PKG_DB         "/var/lib/pkg/db"

//!< Default location for the locked packages.
#define PATH_LOCK_DB        "/var/lib/pkg/locked"

//!< Default location for man(1) executable.
#define PATH_MAN_BIN        "/usr/bin/man"

//!< Default location for pkgmk(8) executable.
#define PATH_PKGMK_BIN      "/usr/sbin/pkgmk"

//!< Default location for pkgmk(8) configuration.
#define PATH_PKGMK_CONFIG   "/etc/pkgmk.conf"

//!< Default location for pkgadd(8) executable.
#define PATH_PKGADD_BIN     "/usr/sbin/pkgadd"

//!< Default location for pkgadd(8) configuration.
#define PATH_PKGADD_CONFIG  "/etc/pkgadd.conf"

//!< Default location for pkgrm(8) executable.
#define PATH_PKGRM_BIN      "/usr/sbin/pkgrm"

// vim: sw=2 ts=2 sts=2 et cc=72 tw=70
// End of file.
