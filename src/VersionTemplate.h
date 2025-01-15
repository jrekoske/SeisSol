// SPDX-FileCopyrightText: 2019-2024 SeisSol Group
//
// SPDX-License-Identifier: BSD-3-Clause
// SPDX-LicenseComments: Full text under /LICENSE and /LICENSES/

#ifndef SEISSOL_SRC_VERSIONTEMPLATE_H_
#define SEISSOL_SRC_VERSIONTEMPLATE_H_

/*
* This file is automatically generated by the build process
* DO NOT EDIT!
*/

#ifndef VERSION_STRING
#define VERSION_STRING "@PACKAGE_GIT_VERSION@"
#endif // VERSION_STRING
#ifndef COMMIT_HASH
#define COMMIT_HASH "@PACKAGE_GIT_HASH@"
#endif // COMMIT_HASH
#ifndef COMMIT_YEAR
#define COMMIT_YEAR @PACKAGE_GIT_YEAR@
#endif // COMMIT_YEAR
#ifndef COMMIT_TIMESTAMP
#define COMMIT_TIMESTAMP "@PACKAGE_GIT_TIMESTAMP@"
#endif // COMMIT_TIMESTAMP
#ifndef SEISSOL_HOST_ARCH
#define SEISSOL_HOST_ARCH "@HOST_ARCH@"
#endif // SEISSOL_HOST_ARCH
#ifndef SEISSOL_DEVICE_ARCH
#define SEISSOL_DEVICE_ARCH "@DEVICE_ARCH@"
#endif // SEISSOL_DEVICE_ARCH
#ifndef SEISSOL_DEVICE_BACKEND
#define SEISSOL_DEVICE_BACKEND "@DEVICE_BACKEND@"
#endif // SEISSOL_DEVICE_BACKEND

#endif // SEISSOL_SRC_VERSIONTEMPLATE_H_
