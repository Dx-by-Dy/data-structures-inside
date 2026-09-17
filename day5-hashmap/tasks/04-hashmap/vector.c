/* Вектор записей словаря. Файл выдан готовым — менять его не нужно.
 *
 * Здесь нет ничего, чего не было во вчерашней задаче 4: блок в куче, длина,
 * вместимость и удвоение при нехватке места.
 */
#include <stdlib.h>

#include "vector.h"

size_t vec_stat_growths = 0;
size_t vec_stat_copied  = 0;

void vec_init(struct Vec *v)
{
    v->data = NULL;
    v->len = 0;
    v->cap = 0;
}

void vec_free(struct Vec *v)
{
    free(v->data);
    vec_init(v);
}

bool vec_reserve(struct Vec *v, size_t need)
{
    if (need <= v->cap)
        return true;

    size_t new_cap = v->cap == 0 ? 4 : v->cap;
    while (new_cap < need)
        new_cap *= 2;

    struct Entry *moved = realloc(v->data, new_cap * sizeof *moved);
    if (moved == NULL)
        return false;

    v->data = moved;
    v->cap = new_cap;
    vec_stat_growths++;
    vec_stat_copied += v->len;
    return true;
}

bool vec_push(struct Vec *v, struct Entry entry)
{
    if (!vec_reserve(v, v->len + 1))
        return false;
    v->data[v->len++] = entry;
    return true;
}
