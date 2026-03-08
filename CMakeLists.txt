cmake_minimum_required(VERSION 3.20)
project(gen-os)

enable_language(CXX ASM_NASM)

set(SOURCES
    src/boot/boot.asm
    src/mm/PhysicalMM.cpp
    src/drivers/framebuffer/framebuffer.cpp
    src/utils/string.cpp
    src/utils.cpp
    src/kernel.cpp
)

add_executable(kernel.elf ${SOURCES})

target_compile_options(kernel.elf PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wall
        -Wextra
        -ffreestanding
        -fno-exceptions
        -fno-rtti
        -fno-stack-protector
        -fno-stack-check
        -fno-lto
        -fno-PIC
        -ffunction-sections
        -fdata-sections
        -nostdlib
        -m64
        -march=x86-64
        -mno-mmx
        -mno-sse
        -mno-sse2
        -mno-red-zone
        -mcmodel=kernel
    >
    $<$<COMPILE_LANGUAGE:ASM_NASM>:
        -f elf64
    >
)

target_include_directories(kernel.elf PRIVATE
    include
    limine-protocol/include
)

set_target_properties(kernel.elf PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib -ffreestanding -no-pie -Wl,-z,max-page-size=0x1000 -Wl,--gc-sections"
)

add_custom_command(
    TARGET kernel.elf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/iso/boot/limine
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/iso/EFI/BOOT
    COMMAND ${CMAKE_COMMAND} -E copy kernel.elf ${CMAKE_SOURCE_DIR}/iso/boot/kernel.elf
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/limine/limine-bios.sys ${CMAKE_SOURCE_DIR}/iso/boot/limine/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/limine/limine-bios-cd.bin ${CMAKE_SOURCE_DIR}/iso/boot/limine/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/limine/limine-uefi-cd.bin ${CMAKE_SOURCE_DIR}/iso/boot/limine/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/limine/BOOTX64.EFI ${CMAKE_SOURCE_DIR}/iso/EFI/BOOT/
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/limine/BOOTIA32.EFI ${CMAKE_SOURCE_DIR}/iso/EFI/BOOT/
)

file(WRITE ${CMAKE_SOURCE_DIR}/iso/boot/limine/limine.conf
"timeout=0

/Gen-OS
    protocol: limine
    path: boot():/boot/kernel.elf
    resolution: 1280x720x32
")

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/gen-os.iso
    COMMAND xorriso -as mkisofs -R -r -J
        -b boot/limine/limine-bios-cd.bin
        -no-emul-boot -boot-load-size 4 -boot-info-table
        -hfsplus -apm-block-size 2048
        --efi-boot boot/limine/limine-uefi-cd.bin
        -efi-boot-part --efi-boot-image --protective-msdos-label
        ${CMAKE_SOURCE_DIR}/iso -o ${CMAKE_SOURCE_DIR}/gen-os.iso
    COMMAND ${CMAKE_SOURCE_DIR}/limine/limine bios-install ${CMAKE_SOURCE_DIR}/gen-os.iso
    DEPENDS kernel.elf
)

add_custom_target(iso DEPENDS ${CMAKE_SOURCE_DIR}/gen-os.iso)
add_custom_target(run
    COMMAND qemu-system-x86_64 -M q35 -cdrom ${CMAKE_SOURCE_DIR}/gen-os.iso -boot d -m 2G
    DEPENDS ${CMAKE_SOURCE_DIR}/gen-os.iso
)