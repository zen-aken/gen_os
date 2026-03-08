#define PML4_INDEX(addr) ((addr >> 39) & 0x1FF)
#define PDPT_INDEX(addr) ((addr >> 30) & 0x1FF)
#define PD_INDEX(addr) ((addr >> 21) & 0x1FF)
#define PT_INDEX(addr) ((addr >> 12) & 0x1FF)

#include <limine.h>
#include <utils/io.h>
#include <mm/PhysicalMM.h>
#include <mm/VirtualMM.h>
#include <drivers/framebuffer.h>

uint64_t VirtualMM::offset;
uint64_t *VirtualMM::PML4;
uint64_t *VirtualMM::PDPT;
uint64_t *VirtualMM::PD;
uint64_t *VirtualMM::PT;

void VirtualMM::init(limine_hhdm_response *HHDM)
{
    offset = HHDM->offset;
    initPML4();
    mapSystem();
    asm volatile("mov %0, %%cr3" ::"r"((uintptr_t)PML4 - offset));
}

void VirtualMM::initPML4()
{
    uint64_t phys_addr = PhysicalMM::allocFreeMem();
    PML4 = (uint64_t *)PhystoVirt(phys_addr);
    for (int i = 0; i < 512; i++)
    {
        PML4[i] = 0;
    }
}

uintptr_t VirtualMM::PhystoVirt(uintptr_t addr)
{
    return addr + offset;
}
uintptr_t VirtualMM::VirttoPhys(uintptr_t virt)
{
    uint64_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));

    uint64_t *pml4 = (uint64_t *)PhystoVirt(cr3 & ~0xFFF);
    uint64_t *pdpt = (uint64_t *)PhystoVirt(pml4[PML4_INDEX(virt)] & ~0xFFF);
    uint64_t *pd = (uint64_t *)PhystoVirt(pdpt[PDPT_INDEX(virt)] & ~0xFFF);
    uint64_t *pt = (uint64_t *)PhystoVirt(pd[PD_INDEX(virt)] & ~0xFFF);

    return (pt[PT_INDEX(virt)] & ~0xFFF) + (virt & 0xFFF);
}

void resetTable(uint64_t *addr)
{
    for (size_t i = 0; i < 512; i++)
    {
        addr[i] = 0;
    }
}

void VirtualMM::mapPage(uint64_t virt, uint64_t phys, uint64_t flags)
{
    uint8_t standard_flags = 0x3;

    if (!(PML4[PML4_INDEX(virt)] & 1))
    {
        uint64_t pdpt_phys = PhysicalMM::allocFreeMem();
        uint64_t *pdpt = (uint64_t *)PhystoVirt(pdpt_phys);
        resetTable(pdpt);
        PML4[PML4_INDEX(virt)] = pdpt_phys | standard_flags;
    }
    PDPT = (uint64_t *)PhystoVirt(PML4[PML4_INDEX(virt)] & ~0xFFF);

    if (!(PDPT[PDPT_INDEX(virt)] & 1))
    {
        uint64_t pd_phys = PhysicalMM::allocFreeMem();
        uint64_t *pd = (uint64_t *)PhystoVirt(pd_phys);
        resetTable(pd);
        PDPT[PDPT_INDEX(virt)] = pd_phys | standard_flags;
    }
    PD = (uint64_t *)PhystoVirt(PDPT[PDPT_INDEX(virt)] & ~0xFFF);

    if (!(PD[PD_INDEX(virt)] & 1))
    {
        uint64_t pt_phys = PhysicalMM::allocFreeMem();
        uint64_t *pt = (uint64_t *)PhystoVirt(pt_phys);
        resetTable(pt);
        PD[PD_INDEX(virt)] = pt_phys | standard_flags;
    }
    PT = (uint64_t *)PhystoVirt(PD[PD_INDEX(virt)] & ~0xFFF);

    PT[PT_INDEX(virt)] = phys | flags;
}

void VirtualMM::mapSystem()
{
    limine_memmap_response *memory = PhysicalMM::getMemmap();
    for (size_t i = 0; i < memory->entry_count; i++)
    {
        uint64_t base = memory->entries[i]->base;
        uint64_t len = memory->entries[i]->length;
        for (size_t addr = base; addr < base + len; addr += 4096)
        {
            mapPage(PhystoVirt(addr), addr, 0x3);
        }
    }

    extern char _kernel_start[];
    extern char _kernel_end[];
    uint64_t kstart = (uint64_t)_kernel_start;
    uint64_t kend = (uint64_t)_kernel_end;
    for (uint64_t addr = kstart; addr < kend; addr += 4096)
    {
        mapPage(addr, VirttoPhys(addr), 0x3);
    }
}