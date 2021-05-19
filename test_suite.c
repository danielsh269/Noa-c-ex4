
#include "hashmap.h"
#include "hash_funcs.h"
#include "pair.h"
#include "test_pairs.h"
#include <stdlib.h>
#include <stdio.h>
#define NDEBUG
#include <assert.h>
char *ch1, *ch2, *ch3, *ch4;
int *num1, *num2, *num3, *num4;
pair *p1, *p2, *p3, *p4;
hashmap* hm;
void setup()
{
    ch1 = (char*)malloc(sizeof(char));
    num1 = (int*)malloc(sizeof(int));
    *ch1 = 'a';
    *num1 = 97;
    p1 = pair_alloc(ch1, num1, char_key_cpy,int_value_cpy,char_key_cmp,int_value_cmp,char_key_free,
                          int_value_free);

    ch2 = (char*)malloc(sizeof(char));
    num2 = (int*)malloc(sizeof(int));
    *ch2 = 'b';
    *num2 = 98;
    p2 = pair_alloc(ch2, num2, char_key_cpy,int_value_cpy,char_key_cmp,int_value_cmp,char_key_free,
                          int_value_free);

    ch3 = (char*)malloc(sizeof(char));
    num3 = (int*)malloc(sizeof(int));
    *ch3 = 'c';
    *num3 = 99;
    p3 = pair_alloc(ch3, num3, char_key_cpy,int_value_cpy,char_key_cmp,int_value_cmp,char_key_free,
                          int_value_free);

    ch4 = (char*)malloc(sizeof(char));
    num4 = (int*)malloc(sizeof(int));
    *ch4 = 'd';
    *num4 = 100;
    p4 = pair_alloc(ch4, num4, char_key_cpy,int_value_cpy,char_key_cmp,int_value_cmp,char_key_free,
                          int_value_free);

    hm = hashmap_alloc(hash_char);

}

void free_all_alocations()
{
    free(ch1);
    free(num1);
    pair_free((void**)&p1);

    free(ch2);
    free(num2);
    pair_free((void**)&p2);

    free(ch3);
    free(num3);
    pair_free((void**)&p3);

    free(ch4);
    free(num4);
    pair_free((void**)&p4);

    hashmap_free(&hm);
}
void test_hash_map_insert()
{
    setup();
    int res1 = hashmap_insert(hm, p1);
    assert(res1 == 1);
    assert(hm->capacity == HASH_MAP_INITIAL_CAP);
    assert(hm->size == 1);
    unsigned long k = hm->hash_func(ch1) & (hm->capacity - 1);
    assert(hm->buckets[k]->capacity == VECTOR_INITIAL_CAP);
    assert(hm->buckets[k]->size == 1);
    pair* p = vector_at(hm->buckets[k], 0);
    assert(char_key_cmp(p->key, p1->key) == 1);
    assert(int_value_cmp(p->value, p1->value == 1));

    free_all_alocations();
    printf("test_hash_map_insert passed");

}