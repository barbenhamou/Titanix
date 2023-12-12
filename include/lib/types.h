#pragma once

#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long qw_t;
typedef unsigned long long uint64_t;
typedef unsigned int dw_t;
typedef unsigned int uint32_t;
typedef unsigned short word_t;
typedef unsigned short uint16_t;
typedef unsigned char byte_t;
typedef unsigned char char_t;
typedef unsigned char uint8_t;
typedef byte_t bool_t;

#define TRUE 1
#define FALSE 0

#define va_list __builtin_va_list
#define va_start(p1, p2) __builtin_va_start(p1, p2)
#define va_end(p) __builtin_va_end(p)
#define va_arg(p1, p2) __builtin_va_arg(p1, p2)

#endif