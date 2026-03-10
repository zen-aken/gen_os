#include <stdint.h>
#include <limine.h>
#include <x86_64/interrupts/idt.h>
#include <mm/PhysicalMM.h>
#include <mm/VirtualMM.h>
#include <drivers/framebuffer.h>
#include <utils/io.h>

__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine_start[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine_end[] = LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests"))) static volatile limine_framebuffer_request fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = nullptr};

__attribute__((used, section(".limine_requests"))) static volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0,
    .response = nullptr};

__attribute__((used, section(".limine_requests"))) static volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0,
    .response = nullptr};

extern "C" void kernel_main(void)
{
    Framebuffer::init(fb_request.response);
    PhysicalMM::init(memmap_request.response);
    VirtualMM::init(hhdm_request.response);
    idt_init();
    log(LogType::INFO, false, "Kernel", "initialization succesfull\n");
    while (1)
    {
        asm volatile("hlt");
    }
}