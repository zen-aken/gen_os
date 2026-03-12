#pragma once
#include <stdint.h>

class VirtualMM
{
private:
    static uint64_t offset;
    static uint64_t *PML4;

    static uintptr_t PhystoVirt(uintptr_t addr);
    static uintptr_t VirttoPhys(uintptr_t addr);
    static void mapSystem();

public:
    static void init(limine_hhdm_response *_HHDM);
    static void initPML4();
    static void mapPage(uint64_t virt, uint64_t phsy, uint64_t flags);
    static uint64_t getOffset();
};
