#include <stdlib.h>
#include <inttypes.h>

#define N_HISTORY 256
#define M_BUFFER 10000
#define MAX_CMD 10000

typedef struct{
    uint64_t hash;
    uint64_t pID;
    char cmd[MAX_CMD];
    int status;
} execution_watch;