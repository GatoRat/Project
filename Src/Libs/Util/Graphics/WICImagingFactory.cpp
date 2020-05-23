// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "WICImagingFactory.h"
#include <memory>
#include "../Error/XceptionT.h"

using namespace std;

namespace WICImaging
{
    IWICImagingFactory* GetFactory()
    {
        struct Factory
        {
            Factory()
            {
                CoInitialize(nullptr);
                CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<LPVOID*>(&wicImagingFactory));
            }

            ~Factory()
            {
                wicImagingFactory.Release();
                CoUninitialize();
            }

            CComPtr<IWICImagingFactory> wicImagingFactory;
        };

        static unique_ptr<Factory> factory = make_unique<Factory>();
        return factory->wicImagingFactory;
    }
} // namespace WICImaging
