This is currently just a copy of an early version of the platform specific stuff.
Since then each platform has been made it's own file for simplicity.

#include "pch.h"
#include "SysInfo.h"
#include "crc32c.h"

#ifdef WIN32
    #include "_lean_windows_.h"
    #pragma warning(default:4996)
#elif defined(__linux__)
    #include <sys/sysctl.h>
#elif defined(__APPLE__)
    #include <sys/sysctl.h>
#endif

using namespace std;

string GetProcessorString()
{
	static char processorStr[128] = { 0 };
	if (!processorStr[0])
	{
#ifdef WIN32
		int cpuInfo[4] = { -1 };
		__cpuid(cpuInfo, 0x80000000);
		unsigned int numExIds = cpuInfo[0];

		char cpuBrandString[0x40] = { 0 };
		for (unsigned int i=0x80000000; i <= numExIds; ++i)
		{
			__cpuid(cpuInfo, i);

			// Interpret CPU brand string and cache information.
			if (i == 0x80000002)
			{
				memcpy(cpuBrandString, cpuInfo, sizeof(cpuInfo));
			}
			else if (i == 0x80000003)
			{
				memcpy(cpuBrandString + 16, cpuInfo, sizeof(cpuInfo));
			}
			else if (i == 0x80000004)
			{
				memcpy(cpuBrandString + 32, cpuInfo, sizeof(cpuInfo));
			}
		}

		const char* pSrc = cpuBrandString;
		while (*pSrc > 0 && *pSrc <= 32)
			pSrc++;

		int processorStrLen = 0;
		while (processorStrLen < sizeof(processorStr) - 1 && *pSrc)
		{
			if (pSrc[0] == '(' && pSrc[1] == 'R' && pSrc[2] == ')')
			{
				//processorStr[dst++] = (char)(uchar)174;
				pSrc += 3;
			}
			else if (pSrc[0] == '(' && pSrc[1] == 'C' && pSrc[2] == ')')
			{
				//processorStr[dst++] = (char)(uchar)169;
				pSrc += 3;
			}
			else if  (pSrc[0] == '(' && pSrc[1] == 'T' && pSrc[2] == 'M' && pSrc[3] == ')')
			{
				//processorStr[dst++] = (char)(uchar)153;
				pSrc += 4;
			}
			else if  (pSrc[0] == 'C' && pSrc[1] == 'P' && pSrc[2] == 'U' && pSrc[3] == ' ')
			{
				pSrc += 4;
			}
			else if (*pSrc > 0 && *pSrc <= 32)
			{
				while (*pSrc > 0 && *pSrc <= 32)
					pSrc++;

				processorStr[processorStrLen++] = ' ';
			}
			else 
			{
				processorStr[processorStrLen++] = *pSrc++;
			}
		}

#elif defined(__linux__)

#elif defined(__APPLE__)

#endif

		processorStr[processorStrLen] = 0;
	}

	return processorStr;
}

uint64_t GetSystemMemory(uint64_t* pAvailableMemory)
{
#ifdef WIN32

    MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(memInfo);
	GlobalMemoryStatusEx(&memInfo);

	if (pAvailableMemory)
		*pAvailableMemory = memInfo.ullAvailPhys;

	return memInfo.ullTotalPhys;

#elif defined(__linux__)

    sysinfo info;
    if (sysinfo(&info))
    {
        if (pAvailableMemory)
            *pAvailableMemory = info.freeram;

        return info.totalram;
    }

    if (pAvailableMemory)
        *pAvailableMemory = 0;

    return 0;

#elif defined(__APPLE__)

    if (pAvailableMemory)
        *pAvailableMemory = 0;

    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;            /* OSX. --------------------- */

    int64_t size = 0;               /* 64-bit */
    size_t len = sizeof(size);
    return (sysctl(mib, 2, &size, &len, nullptr, 0) == 0) ? (size_t)size : 0;

#endif
}

string GetOsString(bool /*useShort*/, bool /*includeLocaleData*/)
{
    string osString;

#ifdef WIN32

    static QString[2] = { QStringLiteral("32"), QStringLiteral("64") };

	#ifdef Q_OS_WIN64
		int osSize = 64;
	#else
		int osSize = 32;
		BOOL is64 = FALSE;
		if (IsWow64Process(GetCurrentProcess(), &is64) && f64)
			osSize = 64;
	#endif

    QString osStr;

    OSVERSIONINFOEX verInfo;
    verInfo.dwOSVersionInfoSize = sizeof(verInfo);
    if (GetVersionEx((OSVERSIONINFO*)&verInfo))
    {
        char* pOS = nullptr;

        if (verInfo.wProductType == VER_NT_WORKSTATION)
        {
            switch (verInfo.dwMajorVersion)
            {
                case 6:
                    switch (verInfo.dwMinorVersion)
                    {
                        case 0:
                            if (IsWindowsVistaSP2OrGreater())
                            {
                                pOS = useShort ? "Vista SP2" : "Windows Vista SP2";
                            }
                            else if (IsWindowsVistaSP1OrGreater())
                            {
                                pOS = useShort ? "Vista SP1" : "Windows Vista SP1";
                            }
                            else
                            {
                                pOS = useShort ? "Vista" : "Windows Vista";
                            }
                            break;

                        case 1:
                            if (IsWindows7SP1OrGreater())
                            {
                                pOS = useShort ? "Win 7 SP1" : "Windows 7 SP1";
                            }
                            else
                            {
                                pOS = useShort ? "Win 7" : "Windows 7";
                            }
                            break;

                        case 2:
                            if (IsWindows8Point1OrGreater())
                            {
                                pOS = useShort ? "Win 8.1" : "Windows 8.1";
                            }
                            else
                            {
                                pOS = useShort ? "Win 8" : "Windows 8";
                            }
                            break;
                    }
                    break;

                case 10:
                    switch (verInfo.dwMinorVersion)
                    {
                        case 0:
                            pOS = useShort ? "Win 10" : "Windows 10";
                            break;
                    }
                    break;
            }

            if (!pOS)
            {
                pOS = useShort ? "Win 10+" : "Windows 10+";
            }
        }
        else
        {
            switch (verInfo.dwMajorVersion)
            {
                case 6:
                    switch (verInfo.dwMinorVersion)
                    {
                        case 0:
                            pOS = useShort ? "Win 2008" : "Windows Server 2008";
                            break;

                        case 1:
                            pOS = useShort ? "Win 2008 R2" :  "Windows Server 2008 R2";
                            break;

                        case 2:
                            pOS = useShort ? "Win 2012" : "Windows Server 2012";
                            break;

                        case 3:
                            pOS = useShort ? "Win 2012 R2" : "Windows Server 2012 R2";
                            break;
                    }
                    break;

                case 10:
                    switch (verInfo.dwMinorVersion)
                    {
                        case 0:
                            pOS = useShort ? "Win 2016" : "Windows Server 2016";
                            break;
                    }
                    break;
            }

            if (!pOS)
            {
                pOS = useShort ? "Win 2016+" : "Windows Server 2016+";
            }
        }

        osStr = pOS;
        osStr += (verInfo.wSuiteMask & VER_SUITE_PERSONAL) == VER_SUITE_PERSONAL ? " Home" : (useShort ? " Pro" : " Professional");
        osStr = pOS;

        osStr += QString(QStringLiteral(" SP %1").arg(verInfo.wServicePackMajor);

        if (verInfo.wServicePackMajor || verInfo.wServicePackMinor)
        {
            osStr += QString(QStringLiteral(" SP %1").arg(verInfo.wServicePackMajor);
        }

        QString(QStringLiteral("%1 %2 %3%4").arg(pOS).arb
                    ));


            SafeFormat(pBuffer, bufferLen, "%s %s %d%s (%d.%d.%d.%d)%s",
                pOS,
                (verInfo.wSuiteMask & VER_SUITE_PERSONAL) == VER_SUITE_PERSONAL ? "Home" : (useShort ? "Pro" : "Professional"),
                osSize,
                servicePackStr,
                verInfo.dwMajorVersion, verInfo.dwMinorVersion,
                verInfo.wServicePackMajor, verInfo.wServicePackMinor,
                localeData);
    }
    else if ()
    {
        osStr += useShort ? "Win %d" : "Windows %d%s"


            SafeFormat(pBuffer, bufferLen, (), osSize, localeData);
    }

    if (includeLocaleData)
    {
        wchar_t systemLocale[LOCALE_NAME_MAX_LENGTH];
        GetSystemDefaultLocaleName(systemLocale, _countof(systemLocale));

        osStr += QChar('{');
        osStr += systemLocale;

        wchar_t userLocale[LOCALE_NAME_MAX_LENGTH];
        GetUserDefaultLocaleName(userLocale, _countof(userLocale));

        osStr += QChar(',');
        osStr += userLocale;

        osStr += QLocale().bcp47Name();

        osStr += QChar('}');
    }

#elif defined(__ANDRIOD__)

    osString = "Android";

#elif defined(__linux__)

    osString = "Linux";

#elif defined(__IOS__)

    osString = "iOS";

#elif defined(__APPLE__)

		long versionMajor;
		long versionMinor;
		long versionBugFix;
		
		Gestalt(gestaltSystemVersionMajor, &versionMajor);
		Gestalt(gestaltSystemVersionMinor, &versionMinor);
		Gestalt(gestaltSystemVersionBugFix, &versionBugFix);

		const char* pOS = "OS X";
		const char* pName = "";

		switch (versionMajor)
		{
			case 10:
				switch (versionMinor)
				{
					case 6:
						pOS = "Mac OS X";
						pName = " Snow Leopard";
						break;

					case 7:
						pOS = "Mac OS X";
						pName = " Lion";
						break;

					case 8:
						pName = " Mountain Lion";
						break;

					case 9:
						pName = " Mavericks";
						break;
				}
				break;
		}
//		SafeFormat(pBuffer, bufferLen, "%s v%lld.%lld.%lld%s",
//			pOS, versionMajor, versionMinor, versionBugFix, pName);

#else
	#error Unknown Operating System
#endif

	return osString;
}

bool IsSSE42Supported()
{
	static bool supportsSSE42 = false;
	static bool checkedSSE42 = false;
	if (!checkedSSE42)
	{
		checkedSSE42 = true;
#ifdef WIN32
        int CPUInfo[4] = { -1 };
        __cpuid(CPUInfo, 1);
        supportsSSE42 = (CPUInfo[2] & 0x100000) != 0;
#else
    #include <allegro.h>
        cpu_capabilities

        unsigned int __get_cpuid_max (unsigned int __ext, unsigned int *__sig)
            int __get_cpuid (unsigned int __level,
                unsigned int *__eax, unsigned int *__ebx,
                unsigned int *__ecx, unsigned int *__edx)
#endif

        //supportsSSE42 = qCpuHasFeature(SSE4_2);
        //supportsSSE42 = (qCpuFeatures() & SSE4_2) != 0;
	}
	return supportsSSE42;
}

SysInfo::SysInfo(bool /*useShortOsStr*/)
{
	memset(this, 0, sizeof(*this));

#ifdef WIN32
	#ifdef WIN64
		is64bitOS = true;
	#else
		BOOL is64 = FALSE;
		is64bitOS = IsWow64Process(GetCurrentProcess(), &is64) != FALSE;
	#endif

	wchar_t _systemLocale[LOCALE_NAME_MAX_LENGTH];
	GetSystemDefaultLocaleName(_systemLocale, _countof(systemLocale));
	WideCharToMultiByte(CP_UTF8, 0, _systemLocale, -1, systemLocale, sizeof(systemLocale), nullptr, FALSE);

	wchar_t _userLocale[LOCALE_NAME_MAX_LENGTH];
	GetUserDefaultLocaleName(_userLocale, _countof(userLocale));
    WideCharToMultiByte(CP_UTF8, 0, _userLocale, -1, userLocale, sizeof(userLocale), nullptr, FALSE);

#elif defined(__ANDRIOD__)

#elif defined(__linux__)

#elif defined(__IOS__)

#elif defined(__APPLE__)

#else
    #error Unknown Operating System
#endif

//	is64bitApp = _COMPILED_SIZE == 64;

    //GetOsString(os, sizeof(os), useShortOsStr, false);
//	SafeStrCopy(appLocale, QLocale().bcp47Name().toUtf8().constData(), sizeof(appLocale));
//	SafeStrCopy(cpu, GetProcessorString(), sizeof(cpu));
//	physicalMemory = GetSystemMemory(&availableMemory);
//	isSSE42Supported = IsSSE42Supported();
//	isCrc32cUsingSSE42 = Crc32cUsingSSE42();
}

AppInfo::AppInfo()
{
	memset(this, 0, sizeof(*this));

//	SafeStrCopy(name, QCoreApplication::applicationName().toUtf8().constData(), sizeof(name));
//	SafeStrCopy(version, QCoreApplication::applicationVersion().toLatin1().constData(), sizeof(version));
//	is64bit = _COMPILED_SIZE == 64;
}

string AppInfo::ToString()
{
	char buffer[512];
	return QString::fromLatin1(ToString(buffer, sizeof(buffer)));
}

const char* AppInfo::ToString(char* pBuffer, int /*bufferLen*/)
{
#ifdef WIN32
//	SafeFormat(pBuffer, bufferLen, "%s %s (%d-bit)", name, version, _COMPILED_SIZE);
#else
//	SafeFormat(pBuffer, bufferLen, includeVersion ? "%s %s", pEdition, version);
#endif
	return pBuffer;
}

AppVersion appVersion;
