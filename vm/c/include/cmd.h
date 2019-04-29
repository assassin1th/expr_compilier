#ifndef CMD_H_
#define CMD_H_

#include <stdint.h>

enum cmd
{
    EXIT = 0x0, FSUB, FSUM, FMUL, FDIV, FPOW, FLOG, FSIN,
    FCOS, FTAN, FCTAN, FASIN, FACOS, FATAN, FACT, FLD_REG,
    RET, PUSH_REG, POP, CALL, FLD_MEM_OR_REAL, PUSH_MEM_OR_REAL
};

typedef struct
{
    uint8_t id:5;
} cmd_t;

typedef struct
{
    uint8_t id:5;
    uint8_t mode_flag:1;
    uint8_t unused_bits:2;
} arith_cmd_t;

typedef struct
{
    uint8_t id:5;
} ret_t;

typedef struct
{
    uint8_t id:5;
} pop_t;

typedef struct
{
    uint8_t id:5;

} ld_cmd_reg_t;

typedef struct
{
    uint8_t id:5;
    uint8_t src_mode:1;
    uint8_t data_type:2;
    uint8_t offset[0];
} ld_cmd_mem_t;

typedef struct
{
    uint8_t id:5;
    uint8_t src_mode:1;
    uint8_t data_type:2;
    uint8_t real[0];
} ld_cmd_real_t;

typedef struct
{
    uint8_t id:5;
    uint8_t unused_bits:3;
    uint8_t offset[0];
} call_cmd_t;

typedef struct
{
    uint8_t id:5;
    uint8_t exit_code:3;
} exit_cmd_t;

#endif // CMD_H_
