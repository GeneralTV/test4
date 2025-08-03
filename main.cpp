#include <windows.h>
#include <wininet.h>
#include <filesystem>

#pragma comment(lib, "wininet.lib")

const char* GITHUB_INDEX = "https://raw.githubusercontent.com/GeneralTV/Teest/main/index.html";
const char* GITHUB_JS = "https://raw.githubusercontent.com/GeneralTV/Teest/main/custom.js";
const char* LOCAL_INDEX = "uiresources\\index.html";
const char* LOCAL_JS = "uiresources\\assets\\custom.js";

void DownloadFile(const char* url, const char* path) {
    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
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
