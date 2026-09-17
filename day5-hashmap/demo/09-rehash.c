/* Демо 9. Таблица, которая растёт.
 *
 *   gcc -std=c11 -Wall -Wextra 09-rehash.c -o 09-rehash
 *   ./09-rehash
 *
 * Сто тысяч ключей кладутся в таблицу, которая начинает с восьми слотов и
 * удваивается, как только занято больше трёх четвертей. При каждом
 * удвоении все ключи раскладываются заново.
 *
 * До запуска: сколько раз таблица переедет и сколько всего ключей придётся
 * при этом переложить?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KEYS 100000

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

struct Table {
    char  **slots;   /* NULL в пустом слоте */
    size_t  cap;
    size_t  len;
};

static size_t stat_rehashes = 0;   /* сколько раз таблица переезжала */
static size_t stat_moved    = 0;   /* сколько ключей при этом переложено */
static size_t stat_probes   = 0;   /* сколько слотов просмотрено при вставке */

static void place(struct Table *t, char *key)
{
    size_t i = fnv1a(key) % t->cap;
    while (t->slots[i] != NULL) {
        i = (i + 1) % t->cap;
        stat_probes++;
    }
    stat_probes++;
    t->slots[i] = key;
}

static void grow(struct Table *t)
{
    size_t new_cap = t->cap == 0 ? 8 : t->cap * 2;
    char **old = t->slots;
    size_t old_cap = t->cap;

    t->slots = calloc(new_cap, sizeof *t->slots);
    if (t->slots == NULL)
        exit(1);
    t->cap = new_cap;

    for (size_t i = 0; i < old_cap; i++)
        if (old[i] != NULL) {
            place(t, old[i]);
            stat_moved++;
        }

    free(old);
    stat_rehashes++;
    printf("  %6zu ключей — таблица %7zu -> %7zu, переложено %6zu\n",
           t->len, old_cap, new_cap, t->len);
}

static void put(struct Table *t, char *key)
{
    if (t->cap == 0 || (t->len + 1) * 4 > t->cap * 3)
        grow(t);
    place(t, key);
    t->len++;
}

int main(void)
{
    struct Table t = {NULL, 0, 0};
    char **keys = malloc(KEYS * sizeof *keys);
    if (keys == NULL)
        return 1;

    for (int i = 0; i < KEYS; i++) {
        char buf[32];
        snprintf(buf, sizeof buf, "key%d", i);
        keys[i] = malloc(strlen(buf) + 1);
        if (keys[i] == NULL)
            return 1;
        memcpy(keys[i], buf, strlen(buf) + 1);
        put(&t, keys[i]);
    }

    printf("\n  вставлено ключей:         %d\n", KEYS);
    printf("  переездов:                %zu\n", stat_rehashes);
    printf("  переложено при переездах: %zu\n", stat_moved);
    printf("  итоговый размер таблицы:  %zu, занято %.2f\n",
           t.cap, (double)t.len / (double)t.cap);
    printf("  раскладок всего:          %zu (вставки и переезды)\n",
           (size_t)KEYS + stat_moved);
    printf("  просмотрено слотов:       %zu, на одну раскладку %.2f\n",
           stat_probes, (double)stat_probes / (double)(KEYS + stat_moved));

    for (int i = 0; i < KEYS; i++)
        free(keys[i]);
    free(keys);
    free(t.slots);
    return 0;
}
