/* Демо 15. Из чего сложен питоновский dict.
 *
 *   gcc -std=c11 -Wall -Wextra 15-compact-dict.c -o 15-compact-dict
 *   ./15-compact-dict
 *
 * Восемь ключей кладутся в таблицу на шестнадцать корзин двумя способами:
 * записи прямо в корзинах и записи отдельным массивом, а в корзинах — их
 * номера. Ключи, хеши и корзины в обоих случаях одни и те же.
 *
 * До запуска: откройте питон и посмотрите, в каком порядке печатается
 * словарь из этих же восьми ключей.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CAP  16
#define KEYS 8

struct Entry {
    uint32_t    hash;
    const char *key;
    int         value;
};

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

/* ── способ 1: запись лежит в своей корзине ─────────────────────────────── */

static struct Entry slots[CAP];

static void slots_put(const char *key, int value)
{
    uint32_t h = fnv1a(key);
    size_t i = h % CAP;
    while (slots[i].key != NULL)
        i = (i + 1) % CAP;
    slots[i].hash = h;
    slots[i].key = key;
    slots[i].value = value;
}

/* ── способ 2: корзины хранят номер записи ──────────────────────────────── */

#define EMPTY (-1)
#define DEAD  (-2)

static int32_t      index_table[CAP];
static struct Entry entries[KEYS];
static size_t       entry_count;

static void compact_put(const char *key, int value)
{
    uint32_t h = fnv1a(key);
    size_t i = h % CAP;
    while (index_table[i] != EMPTY && index_table[i] != DEAD)
        i = (i + 1) % CAP;
    index_table[i] = (int32_t)entry_count;
    entries[entry_count].hash = h;
    entries[entry_count].key = key;
    entries[entry_count].value = value;
    entry_count++;
}

static void compact_erase(const char *key)
{
    uint32_t h = fnv1a(key);
    size_t i = h % CAP;
    while (index_table[i] != EMPTY) {
        int32_t at = index_table[i];
        if (at >= 0 && entries[at].key != NULL && strcmp(entries[at].key, key) == 0) {
            entries[at].key = NULL;
            index_table[i] = DEAD;
            return;
        }
        i = (i + 1) % CAP;
    }
}

/* ── печать ─────────────────────────────────────────────────────────────── */

static void print_slots(void)
{
    printf("  слот   ключ        хеш %% %d\n", CAP);
    for (size_t i = 0; i < CAP; i++) {
        if (slots[i].key == NULL)
            printf("  %4zu   %-10s\n", i, ".");
        else
            printf("  %4zu   %-10s %zu\n", i, slots[i].key, (size_t)(slots[i].hash % CAP));
    }
    printf("  обход по слотам: ");
    for (size_t i = 0; i < CAP; i++)
        if (slots[i].key != NULL)
            printf("%s ", slots[i].key);
    printf("\n\n");
}

static void print_compact(void)
{
    printf("  корзины: ");
    for (size_t i = 0; i < CAP; i++)
        printf("%3d", index_table[i]);
    printf("\n");
    printf("  записи:\n");
    for (size_t k = 0; k < entry_count; k++) {
        if (entries[k].key == NULL)
            printf("  %4zu   удалён\n", k);
        else
            printf("  %4zu   %-10s %d\n", k, entries[k].key, entries[k].value);
    }
    printf("  обход по записям: ");
    for (size_t k = 0; k < entry_count; k++)
        if (entries[k].key != NULL)
            printf("%s ", entries[k].key);
    printf("\n\n");
}

static size_t cap_for(size_t n)
{
    size_t cap = 8;
    while (n * 3 > cap * 2)
        cap *= 2;
    return cap;
}

int main(void)
{
    const char *keys[KEYS] = {"apple", "banana", "cherry", "date",
                              "elder", "fig", "grape", "kiwi"};

    for (size_t i = 0; i < CAP; i++)
        index_table[i] = EMPTY;
    for (size_t k = 0; k < KEYS; k++) {
        slots_put(keys[k], (int)k + 1);
        compact_put(keys[k], (int)k + 1);
    }

    printf("размер одной записи в байтах: %zu (хеш %zu, ключ %zu, значение %zu)\n\n",
           sizeof(struct Entry), sizeof(uint32_t), sizeof(char *), sizeof(int));

    printf("запись лежит в своей корзине:\n");
    print_slots();

    printf("в корзине лежит номер записи (-1 пусто, -2 надгробие):\n");
    print_compact();

    printf("удалили cherry:\n");
    compact_erase("cherry");
    print_compact();

    printf("сколько байт занимают корзины и записи вместе:\n");
    printf("   ключей      корзин   запись в корзине     номер в корзине\n");
    size_t sizes[] = {8, 100, 1000, 1000000};
    for (size_t i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
        size_t n = sizes[i], cap = cap_for(n);
        size_t plain   = cap * sizeof(struct Entry);
        size_t compact = cap * sizeof(int32_t) + n * sizeof(struct Entry);
        printf("  %7zu   %9zu   %15zu   %17zu\n", n, cap, plain, compact);
    }

    return 0;
}
