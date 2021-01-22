// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "classes/hk.h"

namespace globals {
    void(__stdcall* fadeInMusicGate)(const char*) = nullptr;

    std::string randomLoop = "";
}

void __declspec(dllexport) __stdcall fadeInMusicHook(const char* filename) {
    std::string path = "menuLoops\\" + globals::randomLoop;
    return globals::fadeInMusicGate(path.c_str());
}

DWORD WINAPI attach(void* hModule) {
    using namespace std::filesystem;
    constexpr std::string_view filter = " !#$%&'()+,-.0123456789;=@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{}~";

    path menuLoops = current_path() / "menuLoops";
    std::vector<std::string> validLoops;

    if (exists(menuLoops)) {
        if (is_directory(menuLoops)) {
            for (directory_entry loop : directory_iterator{ menuLoops }) {
                if (loop.path().extension().string() == ".mp3") {
                    bool valid = true;
                    for (char c : loop.path().filename().u8string()) {
                        if (filter.find(c) == filter.npos) {
                            valid = false;
                            break;
                        }
                    }

                    if (valid) {
                        validLoops.push_back(loop.path().filename().u8string());
                    }
                }
            }
        }
        else {
            MessageBox(0, L"ERROR: menuLoops already exists as a file.\n"
                L"Please delete it to use this mod.",
                L"menuLoop randomizer", MB_ICONERROR | MB_OK);
        }
    }
    else
        create_directories(menuLoops);

    if (validLoops.empty()) {
        MessageBox(0, L"ERROR: menuLoops is empty!\n"
            "I can't give you a random menu loop if you don't have any loops for me to choose from!",
            L"menuLoop randomizer", MB_ICONERROR | MB_OK);
    }
    else {
        srand(time(0));
        globals::randomLoop = validLoops[rand() % (validLoops.size() - 1)];

        char* gdBase = reinterpret_cast<char*>(GetModuleHandle(0));

        hk fadeInMusic = {
            gdBase + 0xC4BD0,
            fadeInMusicHook,
            &globals::fadeInMusicGate,
            5
        };
        fadeInMusic.hook();
    }

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        HANDLE _;
    case DLL_PROCESS_ATTACH:
        _ = CreateThread(0, 0, attach, hModule, 0, 0);
        if (_) CloseHandle(_);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

