#include "grbl/grbl.h"

static plan_block_t block_buffer[BLOCK_BUFFER_SIZE];  // A ring buffer for motion instructions
plan_block_t *get_block_buffer() { return block_buffer; }

static plan_block_t *block_buffer_head;       // Index of the next block to be pushed
plan_block_t *get_block_buffer_head() { return block_buffer_head; }

static plan_block_t *block_buffer_tail;       // Index of the next block to be pushed
plan_block_t *get_block_buffer_tail() { return block_buffer_tail; }
