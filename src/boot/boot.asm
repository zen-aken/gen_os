section .multiboot
align 8
multiboot_header_start:
    dd 0xE85250D6
    dd 0
    dd multiboot_header_end - multiboot_header_start
    dd -(0xE85250D6 + 0 + (multiboot_header_end - multiboot_header_start))

    align 8
    dw 4
    dw 0
    dd 8

    align 8
    dw 5
    dw 0
    dd 20
    dd 1280
    dd 720
    dd 32

    align 8 
    dw 6
    dw 0
    dd 8

    align 8
    dw 0
    dw 0
    dd 8
multiboot_header_end:

section .bss
align 16
stack_bottom:
    resb 32768
stack_top:

align 4096
pml4:
    resb 4096
pdpt:
    resb 4096
pd:
    resb 4096
pt:
    resb 4096

multiboot_magic: resd 1
multiboot_info:  resd 1

section .rodata
gdt64:
    dq 0
.code:
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
bits 32
global start_32
start_32:
    mov [multiboot_magic], eax
    mov [multiboot_info], ebx

    mov esp, stack_top

    mov eax, cr0
    and eax, ~(1 << 31)
    mov cr0, eax

    mov eax, pdpt
    or eax, 0x3
    mov [pml4], eax
    mov dword [pml4 + 4], 0

    mov eax, pd
    or eax, 0x3
    mov [pdpt], eax
    mov dword [pdpt + 4], 0

    mov eax, pt
    or eax, 0x3
    mov [pd], eax
    mov dword [pd + 4], 0

    mov ecx, 0
.pt_loop:
    mov eax, 0x1000
    mul ecx
    or eax, 0x3
    mov [pt + ecx * 8], eax
    mov dword [pt + ecx * 8 + 4], 0
    inc ecx
    cmp ecx, 512
    jne .pt_loop

    mov eax, cr4
    or eax, (1 << 5)
    mov cr4, eax

    mov eax, pml4
    mov cr3, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, (1 << 8)
    wrmsr

    mov eax, cr0
    or eax, (1 << 31) | (1 << 0)
    mov cr0, eax

    lgdt [gdt64.pointer]
    jmp 0x08:start_64

bits 64
start_64:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov edi, [multiboot_magic]
    mov esi, [multiboot_info]

    extern kernel_main
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang