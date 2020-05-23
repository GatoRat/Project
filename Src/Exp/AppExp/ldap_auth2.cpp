#include "stdafx.h"
#include <Windows.h>
#include <Winldap.h>

int Attempt()
{
    PWSTR host_name = L"ad.server.ip.address";

    LDAP* pLdapConnection = NULL;
    pLdapConnection = ldap_init(host_name, LDAP_PORT);
    if (pLdapConnection == NULL)
    {
        printf("ldap_init failed with 0x%x.\n", LdapGetLastError());
        ldap_unbind(pLdapConnection);
        return -1;
    }
    else
        printf("ldap_init succeeded \n");

    ULONG version = LDAP_VERSION3;
    ULONG lRtn = 0;

    lRtn = ldap_set_option(
        pLdapConnection,           // Session handle
        LDAP_OPT_PROTOCOL_VERSION, // Option
            (void*)&version);         // Option value

    if (lRtn == LDAP_SUCCESS)
        printf("ldap version set to 3.0 \n");
    else
    {
        printf("SetOption Error:%0lX\n", lRtn);
        ldap_unbind(pLdapConnection);
        return 0;
    }


    lRtn = ldap_connect(pLdapConnection, NULL);

    if (lRtn == LDAP_SUCCESS)
        printf("ldap_connect succeeded \n");
    else
    {
        printf("ldap_connect failed with 0x%lx.\n", lRtn);
        ldap_unbind(pLdapConnection);
        return -1;
    }


    PWCHAR user_name = L"adm@ad.domain.name";
    PWCHAR password = L"mypassword";

    lRtn = ldap_simple_bind_s(pLdapConnection, user_name, password);
    if (lRtn == LDAP_SUCCESS)
    {
        printf("ldap_simple_bind_s succeeded \n");
    }
    else
    {
        printf("ldap_simple_bind_s failed with 0x%lx.\n", lRtn);
        ldap_unbind(pLdapConnection);
        return -1;
    }
}