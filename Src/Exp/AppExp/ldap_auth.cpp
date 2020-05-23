#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <Winldap.h>
#include <plog/Log.h>
#include <string>

#ifdef UNICODE
typedef std::wstring        string_type;
#define CONSOLE_OUT         std::wcout
#define CONSOLE_IN          std::wcin
#define CONSOLE_ERR         std::wcerr
#else
typedef std::string         string_type;
#define CONSOLE_OUT         std::cout
#define CONSOLE_IN          std::cin
#define CONSOLE_ERR         std::cerr
#endif

int main()
{
    SEC_WINNT_AUTH_IDENTITY sec;
    string_type hostName = TEXT("192.168.1.49");
    string_type domain = TEXT("WIN-49MT1TDDGOC.darksorrow.com");
    string_type userName = TEXT("ds");
    string_type password = TEXT("1234567890");
    PLDAP ld = nullptr;
    int ldapReturnCode;
    CONSOLE_OUT << TEXT("Hello World!\n");
    plog::init(plog::error, "error.dat", 1048576, 3); //1 M.B.
    ld = ldap_init(const_cast<PWSTR>(hostName.c_str()), LDAP_PORT);

    if (ld == nullptr)
    {
        LOG_ERROR << ldap_err2string(LdapGetLastError());
        CONSOLE_OUT << ldap_err2string(LdapGetLastError());
    }
    const int version = LDAP_VERSION3;
    ldapReturnCode = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, std::addressof(version));
    if (ldapReturnCode not_eq LDAP_SUCCESS)
    {
        LOG_ERROR << ldap_err2string(ldapReturnCode);
        CONSOLE_OUT << ldap_err2string(ldapReturnCode);
    }
    ldapReturnCode = ldap_connect(ld, NULL);
    if (ldapReturnCode not_eq LDAP_SUCCESS)
    {
        LOG_ERROR << ldap_err2string(ldapReturnCode);
        CONSOLE_OUT << ldap_err2string(ldapReturnCode);
    }
    PWSTR   dn = nullptr;

    sec.Domain          = (unsigned short *) domain.data();
    sec.DomainLength    = domain.length();
    sec.User            = (unsigned short *) userName.data();
    sec.UserLength      = userName.length();
    sec.Password        = (unsigned short *) password.data();
    sec.PasswordLength  = password.length();
    sec.Flags           = SEC_WINNT_AUTH_IDENTITY_UNICODE;

    CONSOLE_OUT << domain << std::endl;
    CONSOLE_OUT << userName << std::endl;
    CONSOLE_OUT << password << std::endl;

    ldapReturnCode = ldap_bind_s(ld, dn, (PWCHAR)std::addressof(sec), LDAP_AUTH_NEGOTIATE);
    if (ldapReturnCode not_eq LDAP_SUCCESS)
    {
        LOG_ERROR << ldap_err2string(ldapReturnCode);
        CONSOLE_OUT << ldap_err2string(ldapReturnCode);
    }
    else
        CONSOLE_OUT << "Authentication Success\n";
    ldap_unbind(ld);
    return EXIT_SUCCESS;
}