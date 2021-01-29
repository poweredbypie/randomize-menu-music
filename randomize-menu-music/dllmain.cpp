// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "classes/hk.h"

namespace globals {
    void(__stdcall* fadeInMusicGate)(const char*) = nullptr;

    std::vector<std::string> validLoops;
    std::default_random_engine generator;
    std::uniform_int_distribution distribution;
}

void __declspec(dllexport) __stdcall fadeInMusicHook(const char* filename) {
    using namespace globals;
    return fadeInMusicGate(validLoops[distribution(generator)].c_str());
}

DWORD WINAPI attach(void* hModule) {
    using namespace std::filesystem;
    constexpr std::string_view filter = " !#$%&'()+,-.0123456789;=@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmnopqrstuvwxyz{}~";

    path menuLoops = current_path() / "menuLoops";

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
                        globals::validLoops.push_back("menuLoops/" + loop.path().filename().u8string());
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

    if (globals::validLoops.empty()) {
        MessageBox(0, L"ERROR: menuLoops is empty!\n"
            L"I can't give you a random menu loop if you don't have any loops for me to choose from!",
            L"menuLoop randomizer", MB_ICONERROR | MB_OK);
    }
    else {
        char* gdBase = reinterpret_cast<char*>(GetModuleHandle(0));
        globals::distribution = std::uniform_int_distribution(0,
            static_cast<int>(
                globals::validLoops.size() - 1));

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
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

