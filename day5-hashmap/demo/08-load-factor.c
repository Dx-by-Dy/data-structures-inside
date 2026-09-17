/* Демо 8. Сколько стоит поиск при разной заполненности таблицы.
 *
 *   gcc -std=c11 -Wall -Wextra 08-load-factor.c -o 08-load-factor
 *   ./08-load-factor
 *
 * Таблица из 65536 слотов, открытая адресация, линейное пробирование.
 * Она заполняется до заданной доли, после чего измеряется, сколько слотов
 * просматривает поиск — существующего ключа и отсутствующего.
 *
 * До запуска: во сколько раз подорожает поиск, когда занято 99% слотов
 * вместо 50%?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CAP 65536
#define KEY_LEN 9

static char table[CAP][KEY_LEN];
static char used[CAP];

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

static uint64_t rng_state;

static uint32_t next_random(void)
{
    rng_state = rng_state * 6364136223846793005u + 1442695040888963407u;
    return (uint32_t)(rng_state >> 33);
}

static void random_key(char *out, char first)
{
    out[0] = first;
    for (int i = 1; i < KEY_LEN - 1; i++)
        out[i] = (char)('a' + next_random() % 26);
    out[KEY_LEN - 1] = '\0';
}

static void insert(const char *key)
{
    size_t i = fnv1a(key) % CAP;
    while (used[i])
        i = (i + 1) % CAP;
    used[i] = 1;
    memcpy(table[i], key, KEY_LEN);
}

/* Сколько слотов просмотрел поиск. Останавливается на пустом слоте. */
static size_t probes_for(const char *key)
{
    size_t i = fnv1a(key) % CAP;
    size_t probes = 1;
    while (used[i]) {
        if (strcmp(table[i], key) == 0)
            return probes;
        i = (i + 1) % CAP;
        probes++;
    }
    return probes;
}

static void run(double load)
{
    memset(used, 0, sizeof used);
    rng_state = 12345;

    size_t count = (size_t)(CAP * load);
    char key[KEY_LEN];
    for (size_t k = 0; k < count; k++) {
        /* Ключи начинаются с 'a': отсутствующие будут начинаться с 'z'. */
        random_key(key, 'a');
        insert(key);
    }

    size_t hit = 0;
    for (size_t i = 0; i < CAP; i++)
        if (used[i])
            hit += probes_for(table[i]);

    size_t miss = 0;
    const size_t misses = 10000;
    for (size_t k = 0; k < misses; k++) {
        random_key(key, 'z');
        miss += probes_for(key);
    }

    printf("  %8.2f   %8zu   %17.2f   %19.2f\n",
           load, count, (double)hit / (double)count,
           (double)miss / (double)misses);
}

int main(void)
{
    printf("слотов %d\n\n", CAP);
    printf("  загрузка     ключей     слотов на поиск      слотов на промах\n");
    const double loads[] = {0.10, 0.25, 0.50, 0.75, 0.90, 0.95, 0.99};
    for (size_t i = 0; i < sizeof loads / sizeof loads[0]; i++)
        run(loads[i]);
    return 0;
}
