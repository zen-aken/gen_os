#pragma once
#include <stdint.h>

struct InterruptFrame
{
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

void isr0(InterruptFrame *frame, uint64_t error_code);
void isr1(InterruptFrame *frame, uint64_t error_code);
void isr2(InterruptFrame *frame, uint64_t error_code);
void isr3(InterruptFrame *frame, uint64_t error_code);
void isr4(InterruptFrame *frame, uint64_t error_code);
void isr5(InterruptFrame *frame, uint64_t error_code);
void isr6(InterruptFrame *frame, uint64_t error_code);
void isr7(InterruptFrame *frame, uint64_t error_code);
void isr8(InterruptFrame *frame, uint64_t error_code);
void isr9(InterruptFrame *frame, uint64_t error_code);
void isr10(InterruptFrame *frame, uint64_t error_code);
void isr11(InterruptFrame *frame, uint64_t error_code);
void isr12(InterruptFrame *frame, uint64_t error_code);
void isr13(InterruptFrame *frame, uint64_t error_code);
void isr14(InterruptFrame *frame, uint64_t error_code);