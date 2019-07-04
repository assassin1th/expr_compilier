#ifndef VM_H_
#define VM_H_

#include <stddef.h>

extern "C" double
run(const char *prog, size_t n_arg, double *args);

#endif // VM_H_
