#define WIN_32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <thread>

namespace offset
{
	constexpr std::ptrdiff_t dwMagazineAmmo = 0x0;
	constexpr std::ptrdiff_t dwFireRate = 0x0;
}



// hack fn
void injected_thread(HMODULE instance) noexcept
{

	uintptr_t* pBaseAddress = (uintptr_t*)GetModuleHandleA("RainbowSix.exe");

	while (!GetAsyncKeyState(VK_INSERT))
	{	
		uintptr_t* R6GamerProfileManager = reinterpret_cast<uintptr_t*>((uintptr_t)pBaseAddress + 0x5E32C50);
		uintptr_t* intermediate1 = reinterpret_cast<uintptr_t*>(*R6GamerProfileManager + 0x88);
		uintptr_t* intermediate2 = reinterpret_cast<uintptr_t*>(*intermediate1 + 0x0);
		uintptr_t* intermediate3 = reinterpret_cast<uintptr_t*>(*intermediate2 + 0x30);
		uintptr_t* intermediate4 = reinterpret_cast<uintptr_t*>(*intermediate3 + 0x21);
		uintptr_t* intermediate5 = reinterpret_cast<uintptr_t*>(*intermediate4 + 0x70);
		uintptr_t* ammo_magazine = reinterpret_cast<uintptr_t*>(*intermediate5 + 0x124);

		if (!R6GamerProfileManager && !intermediate1 && !intermediate2 && !intermediate3 && !intermediate4 && !intermediate5 && !ammo_magazine)
			continue;
		*ammo_magazine = 999;

		Sleep(1);
	}

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