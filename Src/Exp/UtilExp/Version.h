// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#define VER_MAJOR                   1
#define VER_MINOR                   2
#define VER_BUILD                   3
#define VER_REVISION                4

#define STR_HELPER(x)               #x
#define STR(x)                      STR_HELPER(x)

#define VER_VERSION                 VER_MAJOR,VER_MINOR,VER_BUILD,VER_REVISION
#define VER_VERSION_STR             STR(VER_MAJOR) "." STR(VER_MINOR) "." STR(VER_BUILD) "." STR(VER_REVISION)
#define VER_VERSION_STR_RC          VER_VERSION_STR "\0"

#define VER_FILEVERSION             VER_VERSION
#define VER_FILEVERSION_STR         VER_VERSION_STR_RC

#define VER_PRODUCTVERSION          VER_VERSION
#define VER_PRODUCTVERSION_STR      VER_VERSION_STR_RC

#define VER_COMPANYNAME_STR         "Joe Woodbury"
#define VER_FILEDESCRIPTION_STR     "Util Experiment"
#define VER_LEGALCOPYRIGHT_STR      "© 2019 Joseph M Woodbury"
#define VER_ASCIICOPYRIGHT_STR      "Copyright 2019 Joseph M Woodbury"
#define VER_ORIGINALFILENAME_STR    "UtilExp.exe"
#define VER_PRODUCTNAME_STR         "Util"

#define VER_COMPANYDOMAIN_STR       "http://www..com/"
