#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#include <math.h>
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
}
inline void
push_reg(uint8_t *mem, sp_t *sp, pc_t *pc)
{

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























































