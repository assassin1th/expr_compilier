#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#define DEFAULT_MEM_SIZE (1 << 16)
#define FPU_STACK_SIZE 8
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    uint8_t *mem;
    double *stack;
    sp_t sp;
    pc_t pc;
    tmp_sp_t tsp;
    cond_t cond;
} vm_t;

static void
exec(vm_t *vm)
{
    int running = 1;
    while (running)
    {
        cmd_t *cmd = (cmd_t *) vm->pc.pcmd;
        switch (cmd->id)
        {
            case END:
                end(&vm->pc, &vm->cond, &running);
                break;
            case FSUB:
                fsub(vm->stack, &vm->cond, &vm->pc);
                break;
            case FSUM:
                fsum(vm->stack, &vm->cond, &vm->pc);
                break;
            case FMUL:
                fmul(vm->stack, &vm->cond, &vm->pc);
                break;
            case FDIV:
                fdiv(vm->stack, &vm->cond, &vm->pc);
                break;
            case FPOW:
                fpow(vm->stack, &vm->cond, &vm->pc);
                break;
            case FLOG:
                flog(vm->stack, &vm->cond, &vm->pc);
                break;
            case FSIN:
                fsin(vm->stack, &vm->cond, &vm->pc);
                break;
            case FCOS:
                fcos(vm->stack, &vm->cond, &vm->pc);
                break;
            case FTAN:
                ftan(vm->stack, &vm->cond, &vm->pc);
                break;
            case FCTAN:
                fctan(vm->stack, &vm->cond, &vm->pc);
                break;
            case FASIN:
                fasin(vm->stack, &vm->cond, &vm->pc);
                break;
            case FACOS:
                facos(vm->stack, &vm->cond, &vm->pc);
                break;
            case FATAN:
                fatan(vm->stack, &vm->cond, &vm->pc);
                break;
            case FACT:
                fact(vm->stack, &vm->cond, &vm->pc);
                break;
            case FLD_REG:
                fld_reg(vm->stack, &vm->cond, &vm->pc);
                break;
            case RET:
                ret(&vm->sp, &vm->pc, &vm->tsp);
                break;
            case PUSH_REG:
                push_reg(vm->stack, &vm->cond, &vm->sp, &vm->pc);
                break;
            case POP:
                pop(&vm->sp, &vm->pc);
                break;
            case CALL:
                call(&vm->sp, &vm->pc, &vm->tsp);
                break;
            case FLD_MEM:
                fld_mem(vm->stack, &vm->cond, &vm->pc, vm->mem);
                break;
            case PUSH_MEM:
                push_mem(&vm->sp, &vm->pc, vm->mem);
                break;
            case FLD_REAL:
                fld_real(vm->stack, &vm->cond, &vm->pc);
                break;
            case PUSH_REAL:
                push_real(&vm->sp, &vm->pc);
                break;
            default:
                printf("error, unknown opcode: %x", *(uint8_t *) cmd);
                running = !running;
                break;
        }
    }
}


double
run(const char *prog, size_t n_arg, double *args)
{
    vm_t vm = {
        .stack = calloc(DEFAULT_MEM_SIZE, sizeof(uint8_t)),
        .pc = {.pcmd = prog},
        .sp = {0},
        .tsp = {0},
        .cond = {0}
    };
    vm.mem = (uint8_t *) (vm.stack + FPU_STACK_SIZE);

    for (int i = 0; i < n_arg; ++i)
    {
        ((double *)vm.mem)[i] = args[n_arg - 1 - i];
    }
    vm.mem += sizeof (double) * n_arg;

    exec(&vm);

    double result = vm.stack[vm.cond.fsp];
    free(vm.stack);

    return result;
}

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
        stack[cond->fsp] = stack[(cond->fsp + 1) & 7] - stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] - stack[(cond->fsp + 1) & 7];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fsum(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[(cond->fsp + 1) & 7] + stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] + stack[(cond->fsp + 1) & 7];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fmul(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[(cond->fsp + 1) & 7] * stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] * stack[(cond->fsp + 1) & 7];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fdiv(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = stack[(cond->fsp + 1) & 7] / stack[cond->fsp];
    } else {
        stack[cond->fsp] = stack[cond->fsp] / stack[(cond->fsp + 1) & 7];
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fpow(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = pow(stack[(cond->fsp + 1) & 7], stack[cond->fsp]);
    } else {
        stack[cond->fsp] = pow(stack[cond->fsp], stack[(cond->fsp + 1) & 7]);
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
flog(double *stack, cond_t *cond, pc_t *pc)
{
    --(cond->fsp);
    if (((arith_cmd_t *) pc->pcmd)->mode_flag)
    {
        stack[cond->fsp] = _log(stack[(cond->fsp + 1) & 7], stack[cond->fsp]);
    } else {
        stack[cond->fsp] = _log(stack[cond->fsp], stack[(cond->fsp + 1) & 7]);
    }
    pc->pcmd += sizeof (arith_cmd_t);
}

inline void
fsin(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], sin);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fcos(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], cos);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
ftan(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], tan);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fctan(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], _ctan);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fasin(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], asin);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
facos(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], acos);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fatan(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], atan);
    pc->pcmd += sizeof (trig_cmd_t);
}


inline void
fact(double *stack, cond_t *cond, pc_t *pc)
{
    EXPR(stack[(cond->fsp - ((trig_cmd_t *) pc->pcmd)->r) & 7], _actan);
    pc->pcmd += sizeof (trig_cmd_t);
}

inline void
fld_reg(double *stack, cond_t *cond, pc_t *pc)
{
    stack[(cond->fsp + 1) & 7] = stack[(cond->fsp - ((ld_cmd_reg_t *) pc->pcmd)->reg) & 7];
    ++(cond->fsp);
    pc->pcmd += sizeof (ld_cmd_reg_t);
}

inline void
push_reg(double *stack, cond_t *cond, sp_t *sp, pc_t *pc)
{
    *(double *)sp->top = stack[(cond->fsp - ((ld_cmd_reg_t *) pc->pcmd)->reg) & 7];
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
    stack[cond->fsp] = ((double *) mem)[*(int16_t *) ((ld_cmd_mem_t *) pc->pcmd)->offset / sizeof (double) - 1];
    pc->pcmd += sizeof (ld_cmd_mem_t);
}

inline void
fld_real(double *stack, cond_t *cond, pc_t *pc)
{
    ++(cond->fsp);
    stack[cond->fsp] = *(double *) ((ld_cmd_real_t *) pc->pcmd)->real;
    pc->pcmd += sizeof (ld_cmd_real_t);
}

inline void
push_mem(sp_t *sp, pc_t *pc, uint8_t *mem)
{
    *(double *) sp->top = mem[*(uint16_t *) ((ld_cmd_mem_t *) pc->pcmd)->offset];
    sp->top += sizeof (double);
    pc->pcmd += sizeof (ld_cmd_mem_t);
}

inline void
push_real(sp_t *sp, pc_t *pc)
{
    *(double *) sp->top = *(double *) ((ld_cmd_real_t *) pc->pcmd)->real;
    sp->top += sizeof (double);
    pc->pcmd += sizeof (ld_cmd_real_t);
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

