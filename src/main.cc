#include <napi.h>

#include <Windows.h>
#include <iostream>
#include <vector>

struct EnumDisplayStruct {
    std::vector<RECT> displayRects;
};

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
                              HDC      hdcMonitor,
                              LPRECT   lprcMonitor,
                              LPARAM   dwData)
{
    EnumDisplayStruct* sc = (EnumDisplayStruct*)dwData;
    MONITORINFO info;
    info.cbSize = sizeof(info);
    if (GetMonitorInfo(hMonitor, &info))
    {
        // std::cout << "Monitor x: "<< info.rcMonitor.left
        //           <<" y: "        << info.rcMonitor.top
        //           <<" w: "        << info.rcMonitor.right - info.rcMonitor.left
        //           <<" h: "        << info.rcMonitor.bottom - info.rcMonitor.top
        //           << std::endl;
        
        sc->displayRects.push_back(info.rcMonitor);
    }
    return TRUE; 
}


Napi::Value setAsDesktopFrame(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    HWND browserWindow = (HWND)(std::uint32_t)(info[0].ToNumber());
    auto displayIndex = 0;
    if(info.Length() > 1) 
    {
        displayIndex = info[1].ToNumber();
    }

    auto setted = false;
    auto width = GetSystemMetrics(SM_CXSCREEN);
    auto height = GetSystemMetrics(SM_CYSCREEN);
    HWND desktop = GetDesktopWindow();
    HWND hWorkerW = NULL;
    HWND windowPM = FindWindowExW(nullptr, nullptr, L"Progman", L"Program Manager");
    DWORD result = 0;
    BOOL r = SendMessageTimeoutW(windowPM,
                                 0x052C,
                                 0,
                                 0,
                                 SMTO_NORMAL,
                                 1000,
                                 &result);
    if (r)
    {
        HWND hShellViewWin = NULL;
        OSVERSIONINFOEX os;
        os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        os.dwMajorVersion = 0;
        os.dwMinorVersion = 0;
        BOOL bIsWin7OrBelow = FALSE;

        if (GetVersionEx((OSVERSIONINFO *)&os))
        {
            if (os.dwMajorVersion < 6 || (os.dwMajorVersion == 6 && os.dwMinorVersion <= 1))
            {
                bIsWin7OrBelow = TRUE;
            }
        }

        if (bIsWin7OrBelow)
        {
            do
            {
                hWorkerW = FindWindowEx(desktop, hWorkerW, L"WorkerW", NULL);
                hShellViewWin = FindWindowEx(hWorkerW, 0, L"SHELLDLL_DefView", 0);
            } while (hShellViewWin == NULL && hWorkerW != NULL);

            hWorkerW = FindWindowExW(nullptr, hWorkerW, L"WorkerW", NULL);
            if (hWorkerW)
                ::ShowWindow(hWorkerW, SW_HIDE);
            hWorkerW = windowPM;
        }
        else
        {
            do
            {
                hWorkerW = FindWindowEx(desktop, hWorkerW, L"WorkerW", NULL);
                hShellViewWin = FindWindowEx(hWorkerW, 0, L"SHELLDLL_DefView", 0);
            } while (hShellViewWin == NULL && hWorkerW != NULL);

            if (hWorkerW)
            {
                // std::cout << "First hWorkerW: " << (std::uint32_t)hWorkerW << std::endl;

                hWorkerW = FindWindowExW(nullptr, hWorkerW, L"WorkerW", NULL);
                if (hWorkerW)
                {
                    // std::cout << "Find hWorkerW: " << (std::uint32_t)hWorkerW << std::endl;
                    ::ShowWindow(hWorkerW, SW_SHOW);
                }
            }
        }

        if (hWorkerW)
        {
            auto x = 0;
            auto y = 0;
            EnumDisplayStruct sc;
            EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&sc);

            if(displayIndex != 0)
            {
                if(displayIndex < sc.displayRects.size()) 
                {
                    x = sc.displayRects[displayIndex].left;
                    y = sc.displayRects[displayIndex].top;
                    width = sc.displayRects[displayIndex].right - sc.displayRects[displayIndex].left;
                    height = sc.displayRects[displayIndex].bottom - sc.displayRects[displayIndex].top;
                }
            }

            SetParent(browserWindow, hWorkerW);
            SetWindowPos(browserWindow, NULL, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
            setted = true;
        }
    }

    return Napi::Value::From(env, setted);
}

inline Napi::Value clearDesktopFrame(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    if (info.Length() != 0)
    {
        HWND browserWindow = (HWND)(std::uint32_t)(info[0].ToNumber());
        SetParent(browserWindow, NULL);
    }

    WCHAR pszFile[MAX_PATH] = {0};
    if (SystemParametersInfoW(SPI_GETDESKWALLPAPER, sizeof(pszFile) / sizeof(WCHAR), (PVOID)(pszFile), NULL))
    {
        SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)(pszFile), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
    }

    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "setAsDesktopFrame"), Napi::Function::New(env, setAsDesktopFrame));
    exports.Set(Napi::String::New(env, "clearDesktopFrame"), Napi::Function::New(env, clearDesktopFrame));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)