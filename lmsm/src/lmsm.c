#include "lmsm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//======================================================
//  Utilities
//======================================================

void lmsm_cap_value(int *val) {
    if (*val > 999) {
        *val = 999;
    } else if (*val < -999) {
        *val = -999;
    }
}

int lmsm_has_two_values_on_stack(lmsm *our_little_machine) {
    // Check if there are at least two values on the stack
    return (our_little_machine->stack_pointer < TOP_OF_MEMORY - 1) ? 0 : 1;
}

//======================================================
//  Instruction Implementation
//======================================================

void lmsm_i_jal(lmsm *our_little_machine, int location) {
    // Ensure there's space on the stack before pushing
    if (our_little_machine->stack_pointer > 0) {
        // Push the current program counter (return address) onto the stack
        our_little_machine->stack[--our_little_machine->stack_pointer] = our_little_machine->program_counter;
        
        // Jump to the specified location
        our_little_machine->program_counter = location;
    } else {
        our_little_machine->error_code = ERROR_STACK_OVERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_ret(lmsm *our_little_machine) {
    // Ensure there's something to pop from the stack
    if (our_little_machine->stack_pointer < TOP_OF_MEMORY) {
        // Pop the return address from the stack
        our_little_machine->program_counter = our_little_machine->stack[our_little_machine->stack_pointer++];
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_push(lmsm *our_little_machine) {
    // Ensure there's space on the stack before pushing
    if (our_little_machine->stack_pointer > 0) {
        our_little_machine->stack[--our_little_machine->stack_pointer] = our_little_machine->accumulator;
    } else {
        our_little_machine->error_code = ERROR_STACK_OVERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_pop(lmsm *our_little_machine) {
    // Ensure there's something to pop from the stack
    if (our_little_machine->stack_pointer < TOP_OF_MEMORY) {
        our_little_machine->accumulator = our_little_machine->stack[our_little_machine->stack_pointer++];
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_dup(lmsm *our_little_machine) {
    if (our_little_machine->stack_pointer < TOP_OF_MEMORY) {
        int value = our_little_machine->stack[our_little_machine->stack_pointer];
        our_little_machine->stack[--our_little_machine->stack_pointer] = value;
    } else {
        our_little_machine->error_code = ERROR_STACK_OVERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_drop(lmsm *our_little_machine) {
    if (our_little_machine->stack_pointer < TOP_OF_MEMORY) {
        our_little_machine->stack_pointer++;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_swap(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int temp = our_little_machine->stack[our_little_machine->stack_pointer];
        our_little_machine->stack[our_little_machine->stack_pointer] = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        our_little_machine->stack[our_little_machine->stack_pointer + 1] = temp;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_sadd(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int a = our_little_machine->stack[our_little_machine->stack_pointer];
        int b = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        our_little_machine->stack[our_little_machine->stack_pointer + 1] = a + b;
        lmsm_cap_value(&our_little_machine->stack[our_little_machine->stack_pointer + 1]);
        our_little_machine->stack_pointer++;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_ssub(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int a = our_little_machine->stack[our_little_machine->stack_pointer];
        int b = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        our_little_machine->stack[our_little_machine->stack_pointer + 1] = b - a;
        lmsm_cap_value(&our_little_machine->stack[our_little_machine->stack_pointer + 1]);
        our_little_machine->stack_pointer++;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_smax(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int a = our_little_machine->stack[our_little_machine->stack_pointer];
        int b = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        our_little_machine->stack[our_little_machine->stack_pointer + 1] = (a > b) ? a : b;
        lmsm_cap_value(&our_little_machine->stack[our_little_machine->stack_pointer + 1]);
        our_little_machine->stack_pointer++;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_smin(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int a = our_little_machine->stack[our_little_machine->stack_pointer];
        int b = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        our_little_machine->stack[our_little_machine->stack_pointer + 1] = (a < b) ? a : b;
        lmsm_cap_value(&our_little_machine->stack[our_little_machine->stack_pointer + 1]);
        our_little_machine->stack_pointer++;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_smul(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int a = our_little_machine->stack[our_little_machine->stack_pointer];
        int b = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        our_little_machine->stack[our_little_machine->stack_pointer + 1] = a * b;
        lmsm_cap_value(&our_little_machine->stack[our_little_machine->stack_pointer + 1]);
        our_little_machine->stack_pointer++;
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_sdiv(lmsm *our_little_machine) {
    if (lmsm_has_two_values_on_stack(our_little_machine)) {
        int a = our_little_machine->stack[our_little_machine->stack_pointer];
        int b = our_little_machine->stack[our_little_machine->stack_pointer + 1];
        if (a != 0) {
            our_little_machine->stack[our_little_machine->stack_pointer + 1] = b / a;
            lmsm_cap_value(&our_little_machine->stack[our_little_machine->stack_pointer + 1]);
            our_little_machine->stack_pointer++;
        } else {
            our_little_machine->error_code = ERROR_DIVIDE_BY_ZERO;
            our_little_machine->status = STATUS_HALTED;
        }
    } else {
        our_little_machine->error_code = ERROR_STACK_UNDERFLOW;
        our_little_machine->status = STATUS_HALTED;
    }
}

void lmsm_i_out(lmsm *our_little_machine) {
    // Append the current accumulator to the output_buffer
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "%d ", our_little_machine->accumulator);
    strncat(our_little_machine->output_buffer, buffer, sizeof(our_little_machine->output_buffer) - strlen(our_little_machine->output_buffer) - 1);
}

void lmsm_i_inp(lmsm *our_little_machine) {
    // Read a value from the command line and store it as an int in the accumulator
    int value;
    printf("Enter an integer: ");
    scanf("%d", &value);
    our_little_machine->accumulator = value;
    lmsm_cap_value(&our_little_machine->accumulator);
}

void lmsm_i_load(lmsm *our_little_machine, int location) {
    our_little_machine->accumulator = our_little_machine->memory[location];
    lmsm_cap_value(&our_little_machine->accumulator);
}

void lmsm_i_add(lmsm *our_little_machine, int location) {
    our_little_machine->accumulator += our_little_machine->memory[location];
    lmsm_cap_value(&our_little_machine->accumulator);
}

void lmsm_i_sub(lmsm *our_little_machine, int location) {
    our_little_machine->accumulator -= our_little_machine->memory[location];
    lmsm_cap_value(&our_little_machine->accumulator);
}

void lmsm_i_load_immediate(lmsm *our_little_machine, int value) {
    our_little_machine->accumulator = value;
    lmsm_cap_value(&our_little_machine->accumulator);
}

void lmsm_i_store(lmsm *our_little_machine, int location) {
    our_little_machine->memory[location] = our_little_machine->accumulator;
}

void lmsm_i_halt(lmsm *our_little_machine) {
    our_little_machine->status = STATUS_HALTED;
}

void lmsm_i_branch_unconditional(lmsm *our_little_machine, int location) {
    our_little_machine->program_counter = location;
}

void lmsm_i_branch_if_zero(lmsm *our_little_machine, int location) {
    if (our_little_machine->accumulator == 0) {
        our_little_machine->program_counter = location;
    }
}

void lmsm_i_branch_if_positive(lmsm *our_little_machine, int location) {
    if (our_little_machine->accumulator > 0) {
        our_little_machine->program_counter = location;
    }
}

void lmsm_step(lmsm *our_little_machine) {
    // If the machine is not halted, read the instruction in memory
    if (our_little_machine->status != STATUS_HALTED) {
        int next_instruction = our_little_machine->memory[our_little_machine->program_counter];
        our_little_machine->program_counter++;
        our_little_machine->current_instruction = next_instruction;
        int instruction = our_little_machine->current_instruction;
        lmsm_exec_instruction(our_little_machine, instruction);
    }
}

//======================================================
//  LMSM Implementation
//======================================================

void lmsm_exec_instruction(lmsm *our_little_machine, int instruction) {
    // Dispatch the instruction set and implement the instructions
    if (instruction == 0) {
        lmsm_i_halt(our_little_machine);
    } else if (100 <= instruction && instruction <= 199) {
        lmsm_i_add(our_little_machine, instruction - 100);
    } else if (200 <= instruction && instruction <= 299) {
        lmsm_i_sub(our_little_machine, instruction - 200);
    } else if (600 <= instruction && instruction <= 699) {
        lmsm_i_branch_unconditional(our_little_machine, instruction - 600);
    } else if (instruction == 920) {
        lmsm_i_push(our_little_machine);
    } else if (instruction == 930) {
        lmsm_i_sadd(our_little_machine);
    } else {
        our_little_machine->error_code = ERROR_UNKNOWN_INSTRUCTION;
        our_little_machine->status = STATUS_HALTED;
    }
    lmsm_cap_value(&our_little_machine->accumulator);
}

void lmsm_load(lmsm *our_little_machine, int *program, int length) {
    for (int i = 0; i < length; ++i) {
        our_little_machine->memory[i] = program[i];
    }
}

void lmsm_init(lmsm *the_machine) {
    the_machine->accumulator = 0;
    the_machine->status = STATUS_READY;
    the_machine->error_code = ERROR_NONE;
    the_machine->program_counter = 0;
    the_machine->current_instruction = 0;
    the_machine->stack_pointer = TOP_OF_MEMORY + 1;
    the_machine->return_address_pointer = TOP_OF_MEMORY - 100;
    memset(the_machine->output_buffer, 0, sizeof(char) * 1000);
    memset(the_machine->memory, 0, sizeof(int) * (TOP_OF_MEMORY + 1));
}

void lmsm_reset(lmsm *our_little_machine) {
    lmsm_init(our_little_machine);
}

void lmsm_run(lmsm *our_little_machine) {
    our_little_machine->status = STATUS_RUNNING;
    while (our_little_machine->status != STATUS_HALTED) {
        lmsm_step(our_little_machine);
    }
}

lmsm *lmsm_create() {
    lmsm *the_machine = malloc(sizeof(lmsm));
    lmsm_init(the_machine);
    return the_machine;
}

void lmsm_delete(lmsm *the_machine) {
    free(the_machine);
}
