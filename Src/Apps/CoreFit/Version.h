#pragma once

#define VER_MAJOR                   0
#define VER_MINOR                   9
#define VER_BUILD                   0
#define VER_REVISION                0

#define STR_HELPER(x)               #x
#define STR(x)                      STR_HELPER(x)

#define VER_VERSION                 VER_MAJOR,VER_MINOR,VER_BUILD,VER_REVISION
#define VER_VERSION_STR	            STR(VER_MAJOR) "." STR(VER_MINOR) "." STR(VER_BUILD) "." STR(VER_REVISION)
#define VER_VERSION_STR_RC          VER_VERSION_STR "\0"

#define VER_FILEVERSION             VER_VERSION
#define VER_FILEVERSION_STR         VER_VERSION_STR_RC

#define VER_PRODUCTVERSION          VER_VERSION
#define VER_PRODUCTVERSION_STR      VER_VERSION_STR_RC

#define VER_COMPANYNAME_STR         "CoreFit LLC"
#define VER_FILEDESCRIPTION_STR     "CoreFit"
#define VER_LEGALCOPYRIGHT_STR      "© 2018 Joseph M Woodbury, All Rights Reserved"
#define VER_ORIGINALFILENAME_STR    "CoreFit.exe"
#define VER_PRODUCTNAME_STR         "CoreFit"

#define VER_COMPANYDOMAIN_STR       "corefit.com"

#define SAVED_STATE_VERSION         0
