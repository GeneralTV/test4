#include <windows.h>
#include <wininet.h>
#include <string>
#include <filesystem>

#pragma comment(lib, "wininet.lib")

namespace fs = std::filesystem;

// Конфигурация (замените на свои ссылки)
const std::string GITHUB_INDEX_URL = "https://raw.githubusercontent.com/yourusername/yourrepo/main/index.html";
const std::string GITHUB_JS_URL = "https://raw.githubusercontent.com/yourusername/yourrepo/main/custom.js";

const std::string LOCAL_INDEX_PATH = "uiresources\\index.html";
const std::string LOCAL_JS_PATH = "uiresources\\assets\\custom.js";

// Функция для скачивания файлов
bool DownloadFile(const std::string& url, const std::string& savePath) {
    // Создаем все необходимые директории
    fs::create_directories(fs::path(savePath).parent_path();

    HINTERNET hInternet = InternetOpenA("RadmirLoader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, 
                                    INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, 0);
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
    DWORD bytesRead = 0;
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        fwrite(buffer, 1, bytesRead, fp);
    }

    fclose(fp);
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return true;
}

// Функция для удаления файлов
void CleanupFiles() {
    try {
        if (fs::exists(LOCAL_INDEX_PATH)) fs::remove(LOCAL_INDEX_PATH);
        if (fs::exists(LOCAL_JS_PATH)) fs::remove(LOCAL_JS_PATH);
    } catch (...) {
        // Игнорируем ошибки удаления
    }
}

// Точка входа DLL
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            // Скачиваем файлы при загрузке
            DownloadFile(GITHUB_INDEX_URL, LOCAL_INDEX_PATH);
            DownloadFile(GITHUB_JS_URL, LOCAL_JS_PATH);
            break;
        }
        case DLL_PROCESS_DETACH: {
            // Удаляем файлы при выгрузке
            CleanupFiles();
            break;
        }
    }
    return TRUE;
}

// Экспортируемая функция для ручной загрузки
extern "C" __declspec(dllexport) void LoadResources() {
    DownloadFile(GITHUB_INDEX_URL, LOCAL_INDEX_PATH);
    DownloadFile(GITHUB_JS_URL, LOCAL_JS_PATH);
}

// Экспортируемая функция для очистки
extern "C" __declspec(dllexport) void UnloadResources() {
    CleanupFiles();
}
