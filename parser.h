#ifndef PARSER_H
#define PARSER_H

#include "treap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TREAP_INVALID = -1,
    TREAP_INSERT = 0,
    TREAP_SIZE,
    TREAP_FIND,
    TREAP_REMOVE,
    TREAP_PRINT,
} treap_action_t;

#define MODE_VERBOSE 1
#define MODE_MASK_PRINT 2

int treap_run_from_file (FILE *in, FILE *out, int mode);

#endif