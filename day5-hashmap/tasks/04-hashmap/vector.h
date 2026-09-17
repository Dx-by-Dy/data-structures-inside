/* Вектор записей словаря. Задача 4, файл выдан готовым — менять его не нужно.
 *
 * Это вчерашний динамический массив, у которого элемент стал структурой.
 * Заголовок содержит только объявления: типы и обещания про функции.
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Одна запись словаря. Ключ — копия в куче, принадлежащая записи; NULL в
   поле key означает, что запись удалена, а место под неё осталось. */
struct Entry {
    char    *key;
    uint32_t hash;
    long     value;
};

struct Vec {
    struct Entry *data;   /* блок в куче или NULL */
    size_t        len;    /* сколько записей занято */
    size_t        cap;    /* сколько помещается без переезда */
};

void vec_init(struct Vec *v);

/* Освобождает только блок записей. Ключи внутри записей — не его дело:
   их освобождает тот, кто их выделял. */
void vec_free(struct Vec *v);

/* Обеспечивает вместимость не меньше need. Единственное место, где
   вызывается realloc. */
bool vec_reserve(struct Vec *v, size_t need);

/* Дописывает запись в конец. */
bool vec_push(struct Vec *v, struct Entry entry);

/* Сколько раз вектор переезжал и сколько записей при этом перенесено.
   Определены в vector.c, объявлены здесь. */
extern size_t vec_stat_growths;
extern size_t vec_stat_copied;

#endif
