#include <windows.h>
#include <wininet.h>
#include <string>
#include <filesystem>
#include <shlobj.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shell32.lib")

namespace fs = std::filesystem;

// Конфигурация (ЗАМЕНИТЕ НА СВОИ ССЫЛКИ)
const std::string GITHUB_INDEX = "https://raw.githubusercontent.com/YOUR_USER/YOUR_REPO/main/index.html";
const std::string GITHUB_JS = "https://raw.githubusercontent.com/YOUR_USER/YOUR_REPO/main/custom.js";

// Пути к файлам в Radmir
std::string GetGamePath() {
    char path[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_PROGRAM_FILES, NULL, 0, path);
    return std::string(path) + "\\Radmir CRMP\\";
}

std::string LOCAL_INDEX = GetGamePath() + "uiresources\\index.html";
std::string LOCAL_JS = GetGamePath() + "uiresources\\assets\\custom.js";

// Скачивание файла с проверкой
bool DownloadFile(const std::string& url, const std::string& savePath) {
    HINTERNET hInternet = InternetOpenA("RadmirLoader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return false;

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, 
                                    INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return false;
    }

    // Создаем директории
    fs::create_directories(fs::path(savePath).parent_path();

    HANDLE hFile = CreateFileA(savePath.c_str(), GENERIC_WRITE, 0, NULL, 
                             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        InternetCloseHandle(hUrl);
        InternetCloseHandle(hInternet);
        return false;
    }

    DWORD bytesRead;
    char buffer[8192];
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        DWORD bytesWritten;
        WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
    }

    CloseHandle(hFile);
    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return true;
}

// Удаление файлов
void CleanFiles() {
    DeleteFileA(LOCAL_INDEX.c_str());
    DeleteFileA(LOCAL_JS.c_str());
}

// Точка входа DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            DownloadFile(GITHUB_INDEX, LOCAL_INDEX);
            DownloadFile(GITHUB_JS, LOCAL_JS);
            break;
            
        case DLL_PROCESS_DETACH:
            CleanFiles();
            break;
    }
    return TRUE;
}

// Экспортируемые функции для ручного управления
extern "C" __declspec(dllexport) void UpdateFiles() {
    DownloadFile(GITHUB_INDEX, LOCAL_INDEX);
    DownloadFile(GITHUB_JS, LOCAL_JS);
}

extern "C" __declspec(dllexport) void RemoveFiles() {
    CleanFiles();
}
