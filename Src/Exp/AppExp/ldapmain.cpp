//  Add Wldap32.lib to your project.
//  Add Wldap32.dll to your project.

//  SSLSESSION: Run the following code example by passing the server name as a
//  command line parameter. If no server name is passed, a serverless bind attempt is performed.
//  Example:
//    sslsession.exe [myserver.fabrikam.com]

#include "stdafx.h"
#include "windows.h"
#include "ntldap.h"
#include "winldap.h"
#include "schnlsp.h"
#include "stdio.h"
#include "tchar.h"

#pragma comment(lib, "Wldap32.lib")

const size_t newsize = 100;

//  Entry point for your application
int ldapmain()
{
    LDAP* pLdapConnection = NULL;
    INT returnCode = 0;
    INT connectSuccess = 0;
    PWCHAR hostName = NULL;
    ULONG version = LDAP_VERSION3;
    SecPkgContext_ConnectionInfo sslInfo;
    LONG lv = 0;

    ////  Verify that the user passed a hostname.
    //if (argc > 1)
    //{
    //    
    //    // Convert argv[] to a wchar_t*
    //    size_t origsize = strlen(argv[1]) + 1;
    //    size_t convertedChars = 0;
    //    wchar_t wcstring[newsize];
    //    mbstowcs_s(&amp; convertedChars, wcstring, origsize, argv[1], _TRUNCATE);
    //    wcscat_s(wcstring, L" (wchar_t *)");
    //    hostName = wcstring;


    //    printf("\nConnecting to host \"%s\" ...\n", hostName);
    //}
    ////  If not, perform a serverless bind.
    //else
    //{
    //    hostName = NULL;
    //    printf("\nConnecting to DEFAULT LDAP host ...\n");
    //}

    //  Initialize an LDAP session using SSL.
    pLdapConnection = ldap_sslinit(L"10.20.5.50"/*L"dc01.gamingarts.com"*//*hostName*/, LDAP_SSL_PORT, 1);
    if (pLdapConnection == NULL)
    {
        HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
        printf("ldap_sslinit failed with 0x%x.\n", hr);
        return -1;
    }

    //  Specify version 3; the default is version 2.
    printf("Setting Protocol version to 3.\n");
    returnCode = ldap_set_option(pLdapConnection,
        LDAP_OPT_PROTOCOL_VERSION,
        (void*)&version);
    if (returnCode != LDAP_SUCCESS)
        goto FatalExit;

    //  Verify that SSL is enabled on the connection.
    //  (returns LDAP_OPT_ON/_OFF).
    printf("Checking if SSL is enabled\n");
    returnCode = ldap_get_option(pLdapConnection, LDAP_OPT_SSL, (void*)&lv);
    if (returnCode != LDAP_SUCCESS)
        goto FatalExit;

    //  If SSL is not enabled, enable it.
    if ((void*)lv == LDAP_OPT_ON)
        printf("SSL is enabled\n");
    else
    {
        printf("SSL not enabled.\n SSL being enabled...\n");
        returnCode = ldap_set_option(pLdapConnection, LDAP_OPT_SSL, LDAP_OPT_ON);
        if (returnCode != LDAP_SUCCESS)
            goto FatalExit;
    }

    //  Connect to the server.
    connectSuccess = ldap_connect(pLdapConnection, NULL);

    if (connectSuccess == LDAP_SUCCESS)
        printf("ldap_connect succeeded \n");
    else
    {
        printf("ldap_connect failed with 0x%x.\n", connectSuccess);
        goto FatalExit;
    }

    //  Bind with current credentials. 
    //printf("Binding ...\n");
    //returnCode = ldap_bind_s(pLdapConnection, NULL, NULL, LDAP_AUTH_NEGOTIATE);
    //if (returnCode != LDAP_SUCCESS)
    //    goto FatalExit;

    PWCHAR user_name = L"jwoodbury@gamingarts";
    PWCHAR password = L"Red4was2`";

    auto lRtn = ldap_simple_bind_s(pLdapConnection, user_name, password);
    if (lRtn == LDAP_SUCCESS)
    {
        printf("ldap_simple_bind_s succeeded \n");
    }
    else
    {
        printf("ldap_simple_bind_s failed with 0x%lx.\n", lRtn);
        goto FatalExit;
    }


    //  Retrieve the SSL cipher strength.
    printf("Getting SSL info\n");
    returnCode = ldap_get_option(pLdapConnection, LDAP_OPT_SSL_INFO, &sslInfo);
    if (returnCode != LDAP_SUCCESS)
        goto FatalExit;

    printf("SSL cipher strength = %d bits\n", sslInfo.dwCipherStrength);

    goto NormalExit;

    //  Perform cleanup.
NormalExit:
    if (pLdapConnection != NULL)
        ldap_unbind_s(pLdapConnection);
    return 0;

    //  Perform cleanup after an error.
FatalExit:
    if (pLdapConnection != NULL)
        ldap_unbind_s(pLdapConnection);
    printf("\n\nERROR: 0x%x\n", returnCode);
    return returnCode;
}