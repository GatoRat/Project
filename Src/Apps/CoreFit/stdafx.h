#pragma warning(push,3)	  // drop compiler to level 3 and save current level
// QT headers only
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include <QtWidgets>
#include <QTranslator>
//#include <QNetworkAccessManager>
//#include <QUrl>
//#include <QNetworkRequest>
//#include <QNetworkReply>
//#include <QSslConfiguration>
#pragma warning(pop)      // restore compiler warning level

// All other headers
#include <limits.h>
#include "Util/Detect/DetectCompiler.h"
#include "Util/Log/Log.h"
#include "CoreLib/ErrCode.h"
#include "CoreLib/QGetPath.h"
#include "CoreLib/Preferences.h"

