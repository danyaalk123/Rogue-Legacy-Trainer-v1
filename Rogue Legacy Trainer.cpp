// Rogue Legacy Trainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "proc.h"


int main()
{
    //Get ProcId of the target process
    DWORD procId = GetProcId(L"RogueLegacy.exe");
    std::cout << "Proc Id = " <<  std::dec << procId << std::endl;
    //Getmodulebaseaddress
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"RogueLegacy.exe");

    std::cout << "Base Adress = " << "0x" << std::hex << moduleBase << std::endl;


    //Get Handle to Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    //Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0X9FB24;

    std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << dynamicPtrBaseAddr << std::endl;

    //Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0xA4, 0x5C, 0x18, 0x350, 0x1AC, 0x0, 0x28 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

    std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;

    //Read Ammo value
    int ammoValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Curent ammo = " << std::dec << ammoValue << std::endl;

    //Write to it
    int newAmmo = 1337;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

    //Read out again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

    std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

    getchar();

    return 0;
}

