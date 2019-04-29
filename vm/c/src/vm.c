#include "vm/c/include/cmd.h"
#include "vm/c/include/registers.h"
#define DEFAULT_MEM_SIZE (1 << 16)
#define FPU_STACK_SIZE 8
#include <stdlib.h>

double
run(uint8_t *prog)
{
    uint8_t *mem = calloc(DEFAULT_MEM_SIZE, sizeof(uint8_t));
    cond_t *cond = (cond_t *) mem;
    pc_t *pc = (pc_t *) (cond + 1);
    pc->pcmd = prog;
    sp_t *sp = (sp_t *) (pc + 1);
    double *fpu_stack = (double *) (sp + 1);
    mem = (uint8_t *) (fpu_stack + FPU_STACK_SIZE);

    int running = 1;
    while (running)
    {
        cmd_t *cmd = (cmd_t *) pc->pcmd;
        switch (cmd->id)
        {

        }
    }
    return 0;
}
