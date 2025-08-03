#include <windows.h>
#include <wininet.h>
#include <shlwapi.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "urlmon.lib")  // Добавлено для URLDownloadToFileA

const char* GITHUB_INDEX = "https://raw.githubusercontent.com/GeneralTV/Teest/main/index.html";
const char* GITHUB_JS = "https://raw.githubusercontent.com/GeneralTV/Teest/main/custom.js";
const char* LOCAL_INDEX = "uiresources\\index.html";
const char* LOCAL_JS = "uiresources\\assets\\custom.js";

void CreateDirectoryRecursive(const char* path) {
    char dir[MAX_PATH];
    strcpy_s(dir, MAX_PATH, path);
    for (char* p = strchr(dir + 1, '\\'); p; p = strchr(p + 1, '\\')) {
        *p = '\0';
        CreateDirectoryA(dir, NULL);
        *p = '\\';
    }
}

void DownloadFile(const char* url, const char* path) {
    CreateDirectoryRecursive(path);
    URLDownloadToFileA(NULL, url, path, 0, NULL);
}

extern "C" __declspec(dllexport) void ScriptMain() {
    DownloadFile(GITHUB_INDEX, LOCAL_INDEX);
    DownloadFile(GITHUB_JS, LOCAL_JS);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        ScriptMain();
    }
    return TRUE;
}
