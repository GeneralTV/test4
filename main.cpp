#include <windows.h>
#include <wininet.h>
#include <string>
#include <filesystem>

#pragma comment(lib, "wininet.lib")

namespace fs = std::filesystem;

// Конфигурация (замените на свои ссылки)
const std::string GITHUB_INDEX = "https://raw.githubusercontent.com/yourname/repo/main/index.html";
const std::string GITHUB_JS = "https://raw.githubusercontent.com/yourname/repo/main/custom.js";

const std::string LOCAL_INDEX = ".\\uiresources\\index.html";
const std::string LOCAL_JS = ".\\uiresources\\assets\\custom.js";

// Функция для скачивания файлов
bool DownloadFile(const std::string& url, const std::string& savePath) {
    fs::create_directories(fs::path(savePath).parent_path());
    
    HINTERNET hInternet = InternetOpenA("RadmirLoader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, 
                                    INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return false;
    }

    FILE* fp = fopen(savePath.c_str(), "wb");
    if (!fp) {
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    char buffer[1024];
    DWORD bytesRead;
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fwrite(buffer, 1, bytesRead, fp);
    }

    fclose(fp);
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return true;
}

// Функция для удаления файлов
void CleanFiles() {
    DeleteFileA(LOCAL_INDEX.c_str());
    DeleteFileA(LOCAL_JS.c_str());
}

/************************************************
  ОСНОВНЫЕ ФУНКЦИИ ASI-МОДА
************************************************/

extern "C" __declspec(dllexport) void ScriptMain() {
    // Главная функция мода (аналог DllMain для ASI)
    DownloadFile(GITHUB_INDEX, LOCAL_INDEX);
    DownloadFile(GITHUB_JS, LOCAL_JS);
}

extern "C" __declspec(dllexport) void InitializeASI() {
    // Функция инициализации (вызывается загрузчиком)
    ScriptMain();
}

extern "C" __declspec(dllexport) void OnProcessDetach() {
    // Функция очистки при выгрузке
    CleanFiles();
}

// Точка входа (оставлена для совместимости)
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        ScriptMain();
    }
    else if (reason == DLL_PROCESS_DETACH) {
        OnProcessDetach();
    }
    return TRUE;
}
