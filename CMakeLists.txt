cmake_minimum_required(VERSION 3.20)
project(gen-os)

# Language support
enable_language(CXX ASM_NASM)

# Source file
set(SOURCES
    src/boot/boot.asm
    src/kernel/kernel.cpp
)

# Kernel binary
add_executable(kernel.elf ${SOURCES})

# C++ flags — freestanding
target_compile_options(kernel.elf PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:
        -ffreestanding
        -fno-exceptions
        -fno-rtti
        -nostdlib
        -fno-stack-protector
        -mno-red-zone
    >
    $<$<COMPILE_LANGUAGE:ASM_NASM>:
        -f elf64
    >
)

# Linker script
set_target_properties(kernel.elf PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib -ffreestanding -no-pie"
)

# Create iso folder and copy kernel files
add_custom_command(
    TARGET kernel.elf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/iso/boot/grub
    COMMAND ${CMAKE_COMMAND} -E copy kernel.elf ${CMAKE_SOURCE_DIR}/iso/boot/kernel.elf
)

# Create GRUB config
file(WRITE ${CMAKE_SOURCE_DIR}/iso/boot/grub/grub.cfg
"set timeout=0
set default=0

menuentry \"gen-os\" {
    multiboot2 /boot/kernel.elf
    boot
}
"
)

# Create ISO
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/gen-os.iso
    COMMAND grub-mkrescue -o ${CMAKE_SOURCE_DIR}/gen-os.iso ${CMAKE_SOURCE_DIR}/iso
    DEPENDS kernel.elf
)

add_custom_target(iso DEPENDS ${CMAKE_SOURCE_DIR}/gen-os.iso)
add_custom_target(run
    COMMAND qemu-system-x86_64 -cdrom ${CMAKE_SOURCE_DIR}/gen-os.iso
    DEPENDS ${CMAKE_SOURCE_DIR}/gen-os.iso
)