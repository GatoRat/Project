#pragma warning(push,3)     // drop compiler to level 3 and save current level
// QT headers only
#include <QtCore>
#include <QtNetwork>
#pragma warning(pop)        // restore compiler warning level

// All other headers
#include <climits>
#include <string>
#include "Util/Detect/DetectCompiler.h"
#include "Util/Detect/DetectOperatingSystem.h"
#include "Util/Misc/Defines.Misc.h"
#include "CoreLib/QGetPath.h"
