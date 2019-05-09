#ifndef CMD_H_
#define CMD_H_

#include <stdint.h>
#include "registers.h"
enum cmd
{
    END = 0x0, FSUB, FSUM, FMUL, FDIV, FPOW, FLOG, FSIN,
    FCOS, FTAN, FCTAN, FASIN, FACOS, FATAN, FACT, FLD_REG,
    RET, PUSH_REG, POP, CALL, FLD_MEM, PUSH_MEM, FLD_REAL, PUSH_REAL
};

typedef struct
{
    uint8_t unused_bits:3;
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
    uint8_t r:3;
} trig_cmd_t;

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
    uint8_t reg:3;
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
} end_cmd_t;

inline void
fsub(double *stack, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fsum(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fmul(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fdiv(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fpow(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
flog(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fsin(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fcos(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
ftan(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fctan(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fasin(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
facos(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fatan(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fact(double *stackk, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
fld_reg(double *stack, cond_t *cond, pc_t *pc) __attribute((always_inline));

inline void
push_reg(double *stack, cond_t *cond, sp_t *sp, pc_t *pc) __attribute__((always_inline));

inline void
pop(sp_t *sp, pc_t *pc) __attribute__((always_inline));

inline void
call(sp_t *sp, pc_t *pc, tmp_sp_t *tsp) __attribute__((always_inline));

inline void
ret(sp_t *sp, pc_t *pc, tmp_sp_t *tsp) __attribute__((always_inline));

inline void
fld_mem(double *stack, cond_t *cond, pc_t *pc, uint8_t *mem) __attribute__((always_inline));

inline void
fld_real(double *stack, cond_t *cond, pc_t *pc) __attribute__((always_inline));

inline void
push_mem(sp_t *sp, pc_t *pc, uint8_t *mem) __attribute__((always_inline));

inline void
push_real(sp_t *sp, pc_t *pc) __attribute__((always_inline));

inline void
end(pc_t *pc, cond_t *cond, int *running) __attribute__((always_inline));

#endif // CMD_H_
