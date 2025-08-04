#include <Windows.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <plugin.h>

namespace fs = std::filesystem;

class ResourceLoader {
public:
    ResourceLoader() {
        // Пути к файлам внутри папки с плагином
        fs::path pluginDir = GetPluginDir();
        fs::path indexSrc = pluginDir / "uiresources" / "index.html";
        fs::path customJsSrc = pluginDir / "uiresources" / "assets" / "custom.js";

        // Пути назначения в папке игры
        fs::path gameDir = GetGameDir();
        fs::path indexDst = gameDir / "uiresources" / "index.html";
        fs::path customJsDst = gameDir / "uiresources" / "assets" / "custom.js";

        // Создаем папки, если их нет
        fs::create_directories(indexDst.parent_path());
        fs::create_directories(customJsDst.parent_path());

        // Копируем файлы
        bool success = true;
        success &= CopyFile(indexSrc, indexDst);
        success &= CopyFile(customJsSrc, customJsDst);

        if (success) {
            Debug::Log("[ResourceLoader] Файлы успешно скопированы!");
        } else {
            Debug::Log("[ResourceLoader] Ошибка копирования файлов!");
        }
    }

private:
    fs::path GetPluginDir() {
        char path[MAX_PATH];
        GetModuleFileNameA(GetModuleHandle(NULL), path, MAX_PATH);
        return fs::path(path).parent_path();
    }

    fs::path GetGameDir() {
        char path[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, path);
        return fs::path(path);
    }

    bool CopyFile(const fs::path& src, const fs::path& dst) {
        try {
            fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
            return true;
        } catch (...) {
            return false;
        }
    }
} loader;
