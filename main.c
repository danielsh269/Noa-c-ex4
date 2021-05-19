#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "pair.h"
#include "hashmap.h"
#include "test_suite.h"

void* int_copy(const void* x)
{
    int* res = (int*)malloc(sizeof(int));
    *res = *((int*)x);
    return res;
}

void int_free(void** x)
{
    free(*x);
}

int cmp(const void* x, const void* y)
{
    int* x2 = (int*)x;
    int* y2 = (int*)y;

    if (*x2 > *y2)
    {
        return 1;
    }
    else if (*x2 < *y2)
    {
        return -1;
    }
    return 0;
}
int main() {

    test_hash_map_insert();
    return 0;

}