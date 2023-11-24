// uint32_t*	=	4 bytes
// uintptr_t*	=	8 bytes
// float*		=	float

// Localplayer = ([[[R6GPM + 0x88]] + 0x30]) -0x6F
// rewrite all pointers to one line
// find the pointer for current weapon held, so it's possible to check if a weapon is held, and only THEN execute recoil / ammo 


#define WIN_32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <thread>
#include "readwrite.h"

namespace offset
{
	constexpr std::ptrdiff_t dwMagazineAmmo = 0x0;
	constexpr std::ptrdiff_t dwFireRate = 0x0;
}

// hack fn
void injected_thread(HMODULE instance) noexcept
{
    AllocConsole();
    FILE* f = new FILE;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "I am the alpha and the omega \n";

    uintptr_t* pBaseAddress = (uintptr_t*)GetModuleHandleA("RainbowSix.exe");
    int count = 1;

    while (!GetAsyncKeyState(VK_INSERT))
    {
        Sleep(10);

        __try 
        {
            uintptr_t* R6GamerProfileManager = reinterpret_cast<uintptr_t*>((uintptr_t)pBaseAddress + 0x5E32C50);
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

            uintptr_t* gun_recoil = reinterpret_cast<uintptr_t*>(*intermediate7 + 0x140);
            uint32_t* gun_firerate = reinterpret_cast<uint32_t*>(*intermediate9 + 0x3C);
            uint32_t* gun_ammo = reinterpret_cast<uint32_t*>(*intermediate5 + 0x124);
            uint32_t* gun_damage = reinterpret_cast<uint32_t*>(*intermediate11 + 0x6C);


            *gun_recoil = 0;
            *gun_firerate = 9999;
            *gun_ammo = 1000;
            *gun_damage = 9999;
            std::cout << "dereference succesful     " << count << "\n";
            count++;
        }

        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            std::cout << "dereference failed        " << count << "\n";
            count++;
        }
    }
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

		//create hack thread
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
