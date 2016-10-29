#include <stdlib.h>
#include <stdio.h>
#include "runt.h"

static runt_int simple_plugin(runt_vm *vm, runt_ptr p)
{
    printf("this is a plugin!\n");
}

void runt_plugin_init(runt_vm *vm)
{
    runt_word_define(vm, "test", 4, simple_plugin);
}
