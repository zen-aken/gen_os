#include <stdint.h>
#include <limine.h>
#include <mm/PhysicalMM.h>

__attribute__((used, section(".limine_requests"))) static volatile limine_framebuffer_request fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0};

__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine_start[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine_end[] = LIMINE_REQUESTS_END_MARKER;

__attribute__((used, section(".limine_requests"))) static volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0};

extern "C" void kernel_main(void)
{
    PhysicalMM::init(memmap_request.response);
    while (1)
    {
        asm volatile("hlt");
    }
}