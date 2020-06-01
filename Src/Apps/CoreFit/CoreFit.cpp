// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "Util/Misc/AppInfo.h"
#include "Util/System/SingleInstance.h"
#include "CoreLib/Database/Database.h"
#include "MainWindow.h"
#include "Version.h"

using namespace std;

constexpr char* pBaseWindowName = "CoreFit";

enum ReturnCode
{
    Success,
    NotSingle,
    LibError,
};

static ReturnCode DbOpen(const QStringList& /*args*/)
{
    ReturnCode result = Success;
    
    QString errStr;
    ErrCode errCode = DbOpen();
    switch (errCode)
    {
        case ErrCode::NoError:
            break;

        case ErrCode::DbCreated:
            if (!DbCreate())
            {
                errStr = QCoreApplication::translate("main: OpenDb", "The database could not be created.");
            }
            break;

        case ErrCode::DbOpenFailed:
            errStr = QCoreApplication::translate("main: OpenDb", "The database could not be opened.");
            break;

        case ErrCode::DbAlreadyOpen:
            errStr = QCoreApplication::translate("main: OpenDb", "The database is already open by another program.");
            break;

        //case ErrCode::LibCorrupt:
        //case ErrCode::LibInvalidSchema:
        //case ErrCode::LibBadVersion:
        default:
            errStr = QCoreApplication::translate("main: OpenDb", "The database is invalid or has been damaged.");
            break;
    }

    if (!errStr.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText(QCoreApplication::translate("main", "A Fatal Error Has Been Encountered!"));
        msgBox.setInformativeText(errStr % QChar(' ') % QCoreApplication::translate("main", "The application will close."));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        result = LibError;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    AppInfo::SetVersion(VER_MAJOR, VER_MINOR, VER_BUILD, VER_REVISION);
    AppInfo::SetProduct(VER_COMPANYNAME_STR, VER_PRODUCTNAME_STR);

    QApplication::setApplicationVersion(VER_VERSION_STR);
    QApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    QApplication::setOrganizationName(VER_COMPANYNAME_STR);

    QStringList arguments = QCoreApplication::arguments();
    arguments.removeFirst();

    int result = Success;

    for (const QString& arg : arguments)
    {
        if (arg[0] == QChar('-'))
        {

        }
        else
        {
            //pathnames += arg;
        }
    }

    InitDirectories(arguments, VER_PRODUCTNAME_STR);

    QString pathname;

    // This is here so we know which library is being used and in case preferences override our choices
    SingleInstance singleInstance;
    if (!singleInstance.lock(pBaseWindowName))
    {
        result = NotSingle;
    }
    else
    {
        PreferencesInit preferencesInit;
        Log::Init logInit;

        result = DbOpen(arguments);
        if (result == Success)
        {
            MainWindow mainWindow(pathname);
            mainWindow.show();
            result = QApplication::exec();
            DbClose();
        }
    }
    return result;
}
