#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#define DEFAULT_MEM_SIZE (1 << 16)
#define FPU_STACK_SIZE 8
#include <stdio.h>
#include <stdlib.h>

double
run(uint8_t *prog)
{
    uint8_t *mem = calloc(DEFAULT_MEM_SIZE, sizeof(uint8_t));
    double *stack = (double *) mem;
    mem += sizeof(double) << 3;
    cond_t cond = {0};
    sp_t sp = {.top = mem};
    pc_t pc = {.pcmd = prog};
    tmp_sp_t tsp = {0};

    int running = 1;
    while (running)
    {
        cmd_t *cmd = (cmd_t *) pc.pcmd;
        switch (cmd->id)
        {
            case END:
                end(&pc, &cond, &running);
                break;
            case FSUB:
                fsub(stack, &cond, &pc);
                break;
            case FSUM:
                fsum(stack, &cond, &pc);
                break;
            case FMUL:
                fmul(stack, &cond, &pc);
                break;
            case FDIV:
                fdiv(stack, &cond, &pc);
                break;
            case FPOW:
                fpow(stack, &cond, &pc);
                break;
            case FLOG:
                flog(stack, &cond, &pc);
                break;
            case FSIN:
                fsin(stack, &cond, &pc);
                break;
            case FCOS:
                fcos(stack, &cond, &pc);
                break;
            case FTAN:
                ftan(stack, &cond, &pc);
                break;
            case FCTAN:
                fctan(stack, &cond, &pc);
                break;
            case FASIN:
                fasin(stack, &cond, &pc);
                break;
            case FACOS:
                facos(stack, &cond, &pc);
                break;
            case FATAN:
                fatan(stack, &cond, &pc);
                break;
            case FACT:
                fact(stack, &cond, &pc);
                break;
            case FLD_REG:
                fld_reg(stack, &cond, &pc);
                break;
            case RET:
                ret(&sp, &pc, &tsp);
                break;
            case PUSH_REG:
                push_reg(stack, &cond, &sp, &pc);
                break;
            case POP:
                pop(&sp, &pc);
                break;
            case CALL:
                call(&sp, &pc, &tsp);
                break;
            case FLD_MEM:
                fld_mem(stack, &cond, &pc, mem);
                break;
            case PUSH_MEM:
                push_mem(&sp, &pc, mem);
                break;
            case FLD_REAL:
                fld_real(stack, &cond, &pc);
                break;
            case PUSH_REAL:
                push_real(&sp, &pc);
                break;
            default:
                printf("error, unknown opcode");
                running = !running;
                break;
        }
    }
    return 0;
}

#include "cmd.c"
