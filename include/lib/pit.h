#pragma once

#ifndef PIT_H
#define PIT_H

#include "interrupts.h"

void init_timer(uint32_t frequency);

static void timer_func(isr_frame_t* frame);

void pit_disable();

void pit_deadline(uint64_t delay);

#endif