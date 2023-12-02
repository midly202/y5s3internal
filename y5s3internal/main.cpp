// uint32_t*    =	4 bytes
// uintptr_t*   =	8 bytes
// float*       =   float

// Localplayer = ([[[R6GPM + 0x88]] + 0x30]) -0x6F
// get better system for pointers, this is absolutely retarded
// figure out classes are used for? idk read on UC or something

// to the person reading this on github, no I don't know what I'm doing, this is my first attempt at it, yes it's probably horrible.
// If you wanna tell me what I'm doing wrong, add me on discord: midly.



#define WIN_32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <thread>


// not used atm
namespace offset
{
	constexpr std::ptrdiff_t dwMagazineAmmo = 0x0;
	constexpr std::ptrdiff_t dwFireRate = 0x0;
}

// cheat fn
void injected_thread(HMODULE instance) noexcept
{
    // allocates console and output stream
    AllocConsole();
    FILE* f = new FILE;
    freopen_s(&f, "CONOUT$", "w", stdout);

    // wow look at me using fancy words
    std::cout << "I am the alpha and the omega \n";

    // get base address
    uintptr_t* pBaseAddress = (uintptr_t*)GetModuleHandleA("RainbowSix.exe");
    int count = 1;

    // as long as INS isn't pressed, execute loop
    while (!GetAsyncKeyState(VK_INSERT))
    {
        Sleep(10);

        // everything below if fucking retarded, but I don't know how to make it not retarded.
        __try 
        {
            uintptr_t* R6GamerProfileManager = reinterpret_cast<uintptr_t*>((uintptr_t)pBaseAddress + 0x5E32C50);
            uintptr_t* R6base2 = reinterpret_cast<uintptr_t*>((uintptr_t)pBaseAddress + 0x05E07850);
            uintptr_t* R6base3 = reinterpret_cast<uintptr_t*>((uintptr_t)pBaseAddress + 0x07EF0EB8);
            uintptr_t* intermediate1 = reinterpret_cast<uintptr_t*>(*R6GamerProfileManager + 0x88);
            uintptr_t* intermediate2 = reinterpret_cast<uintptr_t*>(*intermediate1 + 0x0);
            uintptr_t* intermediate3 = reinterpret_cast<uintptr_t*>(*intermediate2 + 0x30);
            uintptr_t* intermediate4 = reinterpret_cast<uintptr_t*>(*intermediate3 + 0x21);
            uintptr_t* intermediate5 = reinterpret_cast<uintptr_t*>(*intermediate4 + 0x70);
            uintptr_t* intermediate6 = reinterpret_cast<uintptr_t*>(*intermediate5 + 0x108);
            uintptr_t* intermediate7 = reinterpret_cast<uintptr_t*>(*intermediate6 + 0x0);
            uintptr_t* intermediate8 = reinterpret_cast<uintptr_t*>(*intermediate7 + 0x150);
            uintptr_t* intermediate9 = reinterpret_cast<uintptr_t*>(*intermediate8 + 0x0);
            uintptr_t* intermediate10 = reinterpret_cast<uintptr_t*>(*intermediate7 + 0x148);
            uintptr_t* intermediate11 = reinterpret_cast<uintptr_t*>(*intermediate10 + 0x0);
            uintptr_t* intermediate12 = reinterpret_cast<uintptr_t*>(*R6base2 + 0x0);
            uintptr_t* intermediate13 = reinterpret_cast<uintptr_t*>(*intermediate12 + 0x28);
            uintptr_t* intermediate14 = reinterpret_cast<uintptr_t*>(*intermediate13 + 0x0);
            uintptr_t* intermediate15 = reinterpret_cast<uintptr_t*>(*intermediate14 + 0xE0);
            uintptr_t* intermediate16 = reinterpret_cast<uintptr_t*>(*intermediate15 + 0xA8);
            uintptr_t* intermediate17 = reinterpret_cast<uintptr_t*>(*intermediate16 + 0x110);
            uintptr_t* intermediate18 = reinterpret_cast<uintptr_t*>(*R6base3 + 0x10);

            float* player_fov = reinterpret_cast<float*>(*intermediate18 + 0xBB8);
            float* gun_fov = reinterpret_cast<float*>(*intermediate18 + 0xBBC);
            byte* gun_firemode = reinterpret_cast<byte*>(*intermediate5 + 0x120);
            uintptr_t* gun_recoil = reinterpret_cast<uintptr_t*>(*intermediate7 + 0x140);
            uint32_t* gun_firerate = reinterpret_cast<uint32_t*>(*intermediate9 + 0x3C);
            uint32_t* gun_ammo = reinterpret_cast<uint32_t*>(*intermediate5 + 0x124);
            uint32_t* gun_damage = reinterpret_cast<uint32_t*>(*intermediate11 + 0x6C);
            uint32_t* player_speed = reinterpret_cast<uint32_t*>(*intermediate17 + 0x58);


            *gun_recoil = 0;
            *gun_firerate = 9999;
            *gun_ammo = 1000;
            *gun_damage = 9999;
            *player_speed = 200;
            *player_fov = 2.0f;
            *gun_fov = 2.0f;
            *gun_firemode = 0;
            std::cout << "dereference succesful     " << count << "\n";
            count++;
        }

        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            std::cout << "dereference failed        " << count << "\n";
            count++;
        }
    }
    
    // frees the console and output stream
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(instance, 0);
}



// entry point
int __stdcall DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	// DLL PROCESS ATTACH
	if (reason == 1)
	{
		DisableThreadLibraryCalls(instance);

		// create cheat thread
		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(injected_thread),
			instance,
			0,
			nullptr
		);

		if (thread)
		{
			CloseHandle(thread);
		}
	}
	return TRUE;
}