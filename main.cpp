#include <windows.h>
#include <wininet.h>
#include <filesystem>

#pragma comment(lib, "wininet.lib")

namespace fs = std::filesystem;

const char* GITHUB_INDEX = "https://raw.githubusercontent.com/GeneralTV/Teest/main/index.html";
const char* GITHUB_JS = "https://raw.githubusercontent.com/GeneralTV/Teest/main/custom.js";
const char* LOCAL_INDEX = "uiresources\\index.html";
const char* LOCAL_JS = "uiresources\\assets\\custom.js";

void DownloadFile(const char* url, const char* path) {
    fs::create_directories(fs::path(path).parent_path());
    URLDownloadToFileA(NULL, url, path, 0, NULL);
}

extern "C" __declspec(dllexport) void ScriptMain() {
    DownloadFile(GITHUB_INDEX, LOCAL_INDEX);
    DownloadFile(GITHUB_JS, LOCAL_JS);
}

extern "C" __declspec(dllexport) void InitializeASI() { ScriptMain(); }
extern "C" __declspec(dllexport) void OnProcessDetach() {}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        ScriptMain();
    }
    return TRUE;
}
