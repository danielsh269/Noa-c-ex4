#include "vector.h"
#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include "pair.h"

hashmap *hashmap_alloc (hash_func func)
{
    hashmap* hm = (hashmap*) malloc(sizeof(hashmap));
    if (hm == NULL)
    {
        printf("Error: allocation failed");
        return NULL;
    }

    hm->size = 0;
    hm->capacity = HASH_MAP_INITIAL_CAP;
    hm->hash_func = func;
    hm->buckets = (vector **) malloc(sizeof(vector *) * hm->capacity);
    if (hm->buckets == NULL)
    {
        printf("Error: allocation failed");
        return NULL;
    }

    for (size_t i = 0; i < hm->capacity; i++)
    {
        hm->buckets[i] = vector_alloc(pair_copy, pair_cmp, pair_free);
    }

    return hm;
}

void hashmap_free (hashmap **p_hash_map)
{
    for (size_t i = 0; i < (*p_hash_map)->capacity; i++)
    {
        vector_free(&(*p_hash_map)->buckets[i]);
    }
    free((*p_hash_map)->buckets);
    free(*p_hash_map);
    *p_hash_map = NULL;
}

double hashmap_get_load_factor (const hashmap *hash_map)
{
    if (hash_map == NULL)
    {
        return -1;
    }
    return (double)hash_map->size / hash_map->capacity;
}

valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
    if (hash_map == NULL || key == NULL)
    {
        return NULL;
    }
    pair *p;
    unsigned long k = (hash_map->hash_func(key)) & (hash_map->capacity - 1);
    for (size_t i = 0; i < hash_map->buckets[k]->size; i++)
    {
        p = vector_at((hash_map->buckets)[k], i);
        if (p != NULL && p->key_cmp(p->key, key) == 1)
        {
            return p->value;
        }
    }
    return NULL;
}

int rehash(hashmap *hm, unsigned long old_capacity)
{
    vector** new_buckets = (vector**)malloc(sizeof(vector*) * hm->capacity);
    if (new_buckets == NULL)
    {
        return 0;
    }
    for (size_t i = 0; i < hm->capacity; i++)
    {
        new_buckets[i] = vector_alloc(pair_copy, pair_cmp, pair_free);
    }

    for (size_t i = 0; i < old_capacity; i++)
    {
        for (size_t j = 0; j < hm->buckets[i]->size; j++)
        {
            unsigned long k = (hm->hash_func(((pair*)vector_at(hm->buckets[i],j))->key)) & (hm->capacity - 1);
            vector_push_back(new_buckets[k], vector_at(hm->buckets[i],j));
        }
    }

    vector** old_buckets = hm->buckets;
    for (size_t i = 0; i < old_capacity; i++)
    {
        vector_free(&old_buckets[i]);
    }
    free(old_buckets);
    hm->buckets = new_buckets;
    return 1;
}
int hashmap_insert(hashmap *hash_map, const pair *in_pair)
{
    if (hash_map == NULL || in_pair == NULL)
    {
        return 0;
    }

    unsigned long k = (hash_map->hash_func(in_pair->key)) & (hash_map->capacity - 1);

    for(size_t i = 0; i < hash_map->buckets[k]->size; i++)
    {
        pair *p = vector_at(hash_map->buckets[k], i);
        if (p->key_cmp(p->key,in_pair->key) == 1)
        {
            return 0;
        }
    }

    hash_map->size++;
    if (hashmap_get_load_factor(hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
    {
        (hash_map->size)--;
        hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
        int res = rehash(hash_map, hash_map->capacity / HASH_MAP_GROWTH_FACTOR);
        if (res == 0)
        {
            hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
            return 0;
        }
        (hash_map->size)++;
    }

    int res = vector_push_back(hash_map->buckets[k], in_pair);
    return res;
}

int hashmap_erase (hashmap *hash_map, const_keyT key)
{
    if (hash_map == NULL || key == NULL)
    {
        return 0;
    }

    unsigned long k = (hash_map->hash_func(key)) & (hash_map->capacity - 1);
    pair* p;
    int flag = -1;
    for (size_t i = 0; i < hash_map->buckets[k]->size; i++)
    {
        p = vector_at(hash_map->buckets[k], i);
        if (p->key_cmp(p->key, key) == 1)
        {
            flag = i;
            break;
        }
    }

    if (flag == -1)
    {
        return 0;
    }

    hash_map->size--;
    if (hashmap_get_load_factor(hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
    {
        hash_map->capacity /= HASH_MAP_GROWTH_FACTOR;
        int res = rehash(hash_map, hash_map->capacity * HASH_MAP_GROWTH_FACTOR);
        if (res == 0)
        {
            hash_map->capacity *= HASH_MAP_GROWTH_FACTOR;
            hash_map->size++;
            return 0;
        }
    }
    vector_erase(hash_map->buckets[k], flag);
    return 1;
}

int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func, valueT_func valT_func)
{
    if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
        return -1;
    }
    int count = 0;
    for (size_t i = 0; i < hash_map->capacity; i++)
    {
        for (size_t j = 0; j < hash_map->buckets[i]->size; j++)
        {
            if(keyT_func(((pair*)vector_at(hash_map->buckets[i],j))->key) == 1)
            {
                valT_func(((pair*)vector_at(hash_map->buckets[i],j))->value);
                count++;

            }
        }
    }
    return count;
}