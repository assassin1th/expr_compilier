#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <stdint.h>

typedef struct {
    uint16_t fsp:3;
    uint16_t stat:3;
} cond_t;

typedef struct {
    uint8_t *pcmd;
} pc_t;

typedef struct {
    uint8_t *top;
} sp_t;

typedef struct
{
    uint8_t *prev_stack_top;
} tmp_sp_t;

#endif // REGISTERS_H_
