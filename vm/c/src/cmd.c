#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#include <math.h>
#include <stdio.h>
#define EXPR(x, y) x = (y((x)))

inline double
_ctan(double x) __attribute__((always_inline));

inline double
_actan(double x) __attribute__((always_inline));

inline double
_log(double y, double x) __attribute__((always_inline));

inline void
fsub(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[cond->fsp + 1] - stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] - stack[cond->fsp + 1];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fsum(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[cond->fsp + 1] + stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] + stack[cond->fsp + 1];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fmul(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[cond->fsp + 1] * stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] * stack[cond->fsp + 1];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fdiv(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[cond->fsp + 1] / stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] / stack[cond->fsp];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fpow(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = pow(stack[cond->fsp + 1], stack[cond->fsp]);
    } else {
        stack[cond->fsp] = pow(stack[cond->fsp], stack[cond->fsp + 1]);
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
flog(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = _log(stack[cond->fsp + 1], stack[cond->fsp]);
    } else {
        stack[cond->fsp] = _log(stack[cond->fsp], stack[cond->fsp]);
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fsin(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], sin);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fcos(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], cos);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
ftan(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], tan);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fctan(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], _ctan);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fasin(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], asin);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
facos(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], acos);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fatan(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], atan);
    pc->pcmd += sizeof (trig_cmd_t);
}


inline void
fact(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[cond->fsp - ((trig_cmd_t *) pc->pcmd)->r], _actan);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fld_reg(double *stack, cond_t *cond, pc_t *pc)
{
    stack[cond->fsp + 1] = stack[cond->fsp - ((ld_cmd_reg_t *) pc->pcmd)->reg];
    ++(cond->fsp);
    pc->pcmd += sizeof (ld_cmd_reg_t);
}

inline void
push_reg(double *stack, cond_t *cond, sp_t *sp, pc_t *pc)
{
    *(double *)sp->top = stack[cond->fsp - ((ld_cmd_reg_t *) pc->pcmd)->reg];
    sp->top += sizeof (double);
    pc->pcmd += sizeof (ld_cmd_reg_t);
}

inline void
pop(sp_t *sp, pc_t *pc)
{
    sp->top -= sizeof (double);
    pc->pcmd += sizeof (pop_t);
}

inline void
call(sp_t *sp, pc_t *pc, tmp_sp_t *tsp)
{
    *(uint8_t **)sp->top = pc->pcmd;
    sp->top += sizeof (uint8_t *);
    *(uint8_t **)sp->top = tsp->prev_stack_top;
    tsp->prev_stack_top = sp->top;
    sp->top += sizeof (uint8_t *);
    pc->pcmd += *(uint16_t *) ((call_cmd_t *) pc->pcmd)->offset;
}

inline void
ret(sp_t *sp, pc_t *pc, tmp_sp_t *tsp)
{
    sp->top = tsp->prev_stack_top;
    tsp->prev_stack_top = *(uint8_t **) sp->top;
    sp->top -= sizeof (uint8_t *);
    pc->pcmd = *(uint8_t **) sp->top;
}

inline void
fld_mem(double *stack, cond_t *cond, pc_t *pc, uint8_t *mem)
{
    ++(cond->fsp);
    stack[cond->fsp] = mem[*(uint16_t *) ((ld_cmd_mem_t *) pc->pcmd)->offset];
    pc->pcmd += sizeof (ld_cmd_mem_t) + sizeof (uint16_t);
}

inline void
fld_real(double *stack, cond_t *cond, pc_t *pc)
{
    ++(cond->fsp);
    stack[cond->fsp] = *(double *) ((ld_cmd_real_t *) pc->pcmd)->real;
    pc->pcmd += sizeof (ld_cmd_real_t) + sizeof (double);
}

inline void
push_mem(sp_t *sp, pc_t *pc, uint8_t *mem)
{
    *(double *) sp->top = mem[*(uint16_t *) ((ld_cmd_mem_t *) pc->pcmd)->offset];
    sp->top += sizeof (double);
    pc->pcmd += sizeof (ld_cmd_mem_t) + sizeof (uint16_t);
}

inline void
push_real(sp_t *sp, pc_t *pc)
{
    *(double *) sp->top = *(double *) ((ld_cmd_real_t *) pc->pcmd)->real;
    sp->top += sizeof (double);
    pc->pcmd += sizeof (ld_cmd_real_t) + sizeof (double);
}

inline void
end(pc_t *pc, cond_t *cond, int *running)
{
    cond->stat = ((end_cmd_t *) pc->pcmd)->exit_code;
    *running = 0;
}

inline double
_actan(double x)
{
    return M_PI_2 - atan(x);
}

inline double
_ctan(double x)
{
    return 1 / tan(x);
}

inline double
_log(double y, double x)
{
    return log2(x) / log2(y);
}
