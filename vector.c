#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
vector *vector_alloc(vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
                     vector_elem_free elem_free_func)
{
    if (elem_copy_func == NULL || elem_cmp_func == NULL || elem_free_func == NULL)
    {
        return NULL;
    }
    vector* vec = (vector*) malloc(sizeof(vector));

    if (vec == NULL)
    {
        printf("Error: allocation failed");
        return NULL;
    }

    vec->size = 0;
    vec->capacity = VECTOR_INITIAL_CAP;
    vec->elem_cmp_func = elem_cmp_func;
    vec->elem_copy_func = elem_copy_func;
    vec->elem_free_func = elem_free_func;

    void** vec_array = malloc(sizeof(void*) * vec->capacity);
    if (vec_array == NULL)
    {
        printf("Error: allocation failed");
        return NULL;
    }

    vec->data = vec_array;

    return vec;
}

void vector_free(vector **p_vector)
{
    for(size_t i = 0; i < (*p_vector)->size; i++)
    {
        (*p_vector)->elem_free_func(&((*p_vector)->data)[i]);
    }

    free((*p_vector)->data);
    free(*p_vector);
    *p_vector = NULL;

}

void *vector_at(const vector *vector, size_t ind)
{
    if (vector == NULL || ind < 0 || ind >= vector->size)
    {
        return NULL;
    }

    return (vector->data)[ind];
}

int vector_find(const vector *vector, const void *value)
{
    if (vector == NULL || value == NULL)
    {
        return -1;
    }

    for(size_t i = 0; i < vector->size; i++)
    {
        if (vector->elem_cmp_func((vector->data)[i], value) == 0)
        {
            return (int)i;
        }
    }
    return -1;
}

double vector_get_load_factor(const vector *vector)
{
    if (vector == NULL)
    {
        return -1;
    }
    return (double)vector->size / vector->capacity;
}

int realloc_vector(vector *vector)
{

    void **new_data = malloc(sizeof(void*) * vector->capacity);
    if (new_data == NULL)
    {
        return 0;
    }
    for (size_t i = 0; i < vector->size; i++)
    {
        new_data[i] = vector->elem_copy_func((vector->data)[i]);
    }
    void **old_data = vector->data;
    free(old_data);
    vector->data = new_data;
    return 1;


}

int vector_push_back(vector *vector, const void *value)
{
    if (vector == NULL || value == NULL)
    {
        return 0;
    }

    (vector->size)++;
    if (vector_get_load_factor(vector) > VECTOR_MAX_LOAD_FACTOR)
    {
        (vector->size)--;
        vector->capacity *= VECTOR_GROWTH_FACTOR;
        int res = realloc_vector(vector);
        if (res == 0)
        {
            vector->capacity /= VECTOR_GROWTH_FACTOR;
            return 0;
        }
        (vector->size)++;
    }

    (vector->data)[vector->size - 1] = vector->elem_copy_func(value);
    return 1;
}
void rearrange_vector(vector* vector, int ind)
{
    for (size_t i = ind + 1; i <= vector->size; i++)
    {
        (vector->data)[i - 1] = (vector->data)[i];
    }
    (vector->data)[vector->size] = NULL;

}
int vector_erase(vector *vector, size_t ind)
{
    if (vector == NULL || ind < 0 || ind >= vector->capacity)
    {
        return 0;
    }

    (vector->size)--;

    if (vector_get_load_factor(vector) < VECTOR_MIN_LOAD_FACTOR)
    {
        vector->capacity /= VECTOR_GROWTH_FACTOR;
        (vector->size)++;
        int res = realloc_vector(vector);
        (vector->size)--;
        if (res == 0)
        {
            vector->size++;
            vector->capacity *= VECTOR_GROWTH_FACTOR;
            return 0;
        }
    }

    vector->elem_free_func(&(vector->data)[ind]);
    (vector->data)[ind] = NULL;
    rearrange_vector(vector, ind);

    return 1;
}

void vector_clear(vector *vector)
{
    if (vector == NULL)
    {
        return;
    }

    while (vector->size > 0)
    {
        vector_erase(vector, 0);
    }
}

