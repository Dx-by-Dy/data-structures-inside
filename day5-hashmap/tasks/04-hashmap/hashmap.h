/* Словарь: хеш-таблица поверх вектора записей. Задача 4.
 *
 * Заголовок выдан готовым: он описывает, что обязана уметь ваша hashmap.c.
 * Менять его не нужно — main.c рассчитывает ровно на эти объявления.
 */
#ifndef HASHMAP_H
#define HASHMAP_H

#include "vector.h"

/* Что лежит в корзине: номер записи в векторе либо одно из двух особых
   значений. */
#define MAP_EMPTY (-1)   /* корзина никогда не была занята */
#define MAP_DEAD  (-2)   /* здесь был ключ, его удалили */

struct Map {
    struct Vec entries;     /* записи в порядке вставки */
    int32_t   *index;       /* корзины: номера записей, MAP_EMPTY, MAP_DEAD */
    size_t     index_cap;   /* сколько корзин; степень двойки */
    size_t     live;        /* сколько записей не удалено */
};

/* Хеш ключа. */
uint32_t map_hash(const char *key);

void map_init(struct Map *m);
void map_free(struct Map *m);

/* d[key] = value. Ключ копируется в кучу. Возвращает false, только если не
   хватило памяти. */
bool map_put(struct Map *m, const char *key, long value);

/* d[key] в *out. false, если ключа нет. */
bool map_get(const struct Map *m, const char *key, long *out);

/* del d[key]. false, если ключа нет. */
bool map_del(struct Map *m, const char *key);

/* len(d) */
size_t map_len(const struct Map *m);

/* Счётчики. Их увеличивает hashmap.c, печатает main.c. */
extern size_t map_stat_probes;     /* просмотрено корзин */
extern size_t map_stat_compares;   /* сравнений ключей строка-в-строку */
extern size_t map_stat_rehashes;   /* сколько раз таблица перестраивалась */

#endif
