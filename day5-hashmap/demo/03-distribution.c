/* Демо 3. Как два хеша раскладывают одни и те же ключи по корзинам.
 *
 *   gcc -std=c11 -Wall -Wextra 03-distribution.c -o 03-distribution
 *   ./03-distribution
 *
 * Двести ключей key000 … key199 раскладываются по шестидесяти четырём
 * корзинам двумя способами.
 *
 * До запуска: сколько ключей попало бы в корзину, если разложить поровну?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>

#define KEYS    200
#define BUCKETS 64

static uint32_t sum_hash(const char *s)
{
    uint32_t h = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++)
        h += *p;
    return h;
}

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

static void histogram(const char *title, uint32_t (*hash)(const char *))
{
    size_t counts[BUCKETS] = {0};
    char key[16];

    for (int i = 0; i < KEYS; i++) {
        snprintf(key, sizeof key, "key%03d", i);
        counts[hash(key) % BUCKETS]++;
    }

    printf("%s\n", title);
    for (size_t b = 0; b < BUCKETS; b += 16) {
        printf("  %2zu:", b);
        for (size_t k = b; k < b + 16; k++)
            printf(" %2zu", counts[k]);
        putchar('\n');
    }

    size_t empty = 0, max = 0;
    for (size_t b = 0; b < BUCKETS; b++) {
        if (counts[b] == 0)
            empty++;
        if (counts[b] > max)
            max = counts[b];
    }
    printf("  пустых корзин %zu из %d, в самой длинной %zu ключей\n\n",
           empty, BUCKETS, max);
}

int main(void)
{
    printf("ключей %d, корзин %d, поровну было бы по %d\n\n",
           KEYS, BUCKETS, KEYS / BUCKETS);
    histogram("сумма байтов:", sum_hash);
    histogram("FNV-1a:", fnv1a);
    return 0;
}
