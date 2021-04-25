#include <windows.h>

#include <fstream>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

#define GETSTRING_JSON(name) \
    json d##name##_json = data[#name]; \
    if (d##name##_json.type() != json::value_t::string) { \
        std::cout << "Invalid type for object member '" #name "'" << std::endl; \
        return -3; \
    } \
    std::string name = d##name##_json.get<std::string>(); \

bool injectDLL(int pid, const char* dll_path) {
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (process == NULL) return false;

    int length = strlen(dll_path);

    LPVOID path_loc = VirtualAllocEx(
        process,
        NULL,
        length + 1,
        MEM_COMMIT |
        MEM_RESERVE,
        PAGE_READWRITE
    );
    if (path_loc == NULL) return false;

    bool success = WriteProcessMemory(
        process,
        path_loc,
        dll_path,
        length + 1,
        NULL
    );
    if (!success) return false;

    HMODULE kernel = LoadLibraryA("kernel32");

    LPTHREAD_START_ROUTINE routine = (LPTHREAD_START_ROUTINE)GetProcAddress(
        kernel,
        "LoadLibraryA"
    );

    HANDLE thread = CreateRemoteThread(
        process,
        NULL, 0,
        routine,
        path_loc,
        0, NULL
    );

    if (thread != NULL) return true;
    else return false;
}

int pollGD(std::string title) {
    HWND window;
    int  timing = 0;

    while (true) {
        window = FindWindowA(NULL, title.c_str());
        if (window != NULL) break;

        Sleep(50);
        timing += 50;

        if (timing >= 60 * 1000) return 0;
    }

    DWORD process;
    GetWindowThreadProcessId(window, &process);

    return process;
}

int main() {
    std::ifstream f(getenv("APPDATA") + std::string("\\gdext\\injector.json"));
    if (!f.is_open()) {
        std::cout << "Could not open injector.json" << std::endl;
        return -1;
    }

    std::string str((std::istreambuf_iterator<char>(f)),
                     std::istreambuf_iterator<char>());

    json data = json::parse(str, nullptr, false);

    if (data.type() != json::value_t::object) {
        std::cout << "Invalid type of json, object expected" << std::endl;
        return -2;
    }

    GETSTRING_JSON(gd_title)
    GETSTRING_JSON(dll_path)

    int process = pollGD(gd_title);
    std::cout << "Geometry Dash found at PID: " << process << std::endl;

    if ( injectDLL(process, dll_path.c_str()) ) {
        std::cout << "The DLL has probably been injected" << std::endl;
        return 0;
    } else {
        std::cout << "The DLL could not be injected!" << std::endl;
        return -1;
    }
}