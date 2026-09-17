/* Демо 10. Таблица, в которой всё в одной корзине.
 *
 *   gcc -std=c11 -Wall -Wextra 10-adversary.c -o 10-adversary
 *   ./10-adversary
 *
 * Пять тысяч ключей — это перестановки одних и тех же десяти букв. Они
 * кладутся в одну и ту же таблицу с цепочками дважды: сначала с хешем
 * «сумма байтов», потом с FNV-1a.
 *
 * До запуска: что делает сумма байтов с перестановками одной строки?
 * Демонстрация 2 это уже показывала.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEYS    5000
#define BUCKETS 4096
#define KEY_LEN 10

struct Node {
    char         key[KEY_LEN + 1];
    struct Node *next;
};

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

static size_t comparisons;

/* Кладёт ключ, если его ещё нет. Как и в любой таблице, вставка начинается с
   поиска: одинаковых ключей в словаре не бывает. */
static void put(struct Node **buckets, const char *key, uint32_t (*hash)(const char *))
{
    size_t b = hash(key) % BUCKETS;
    for (struct Node *n = buckets[b]; n != NULL; n = n->next) {
        comparisons++;
        if (strcmp(n->key, key) == 0)
            return;
    }
    struct Node *n = malloc(sizeof *n);
    if (n == NULL)
        exit(1);
    memcpy(n->key, key, KEY_LEN + 1);
    n->next = buckets[b];
    buckets[b] = n;
}

/* Следующая перестановка букв в лексикографическом порядке. */
static int next_permutation(char *s, size_t len)
{
    size_t i = len - 1;
    while (i > 0 && s[i - 1] >= s[i])
        i--;
    if (i == 0)
        return 0;

    size_t j = len - 1;
    while (s[j] <= s[i - 1])
        j--;
    char t = s[i - 1]; s[i - 1] = s[j]; s[j] = t;

    for (size_t a = i, b = len - 1; a < b; a++, b--) {
        t = s[a]; s[a] = s[b]; s[b] = t;
    }
    return 1;
}

static void run(const char *title, uint32_t (*hash)(const char *))
{
    struct Node **buckets = calloc(BUCKETS, sizeof *buckets);
    if (buckets == NULL)
        exit(1);

    char key[KEY_LEN + 1] = "abcdefghij";
    comparisons = 0;
    clock_t t0 = clock();
    for (size_t k = 0; k < KEYS; k++) {
        put(buckets, key, hash);
        if (!next_permutation(key, KEY_LEN))
            break;
    }
    clock_t t1 = clock();

    size_t busy = 0, longest = 0;
    for (size_t b = 0; b < BUCKETS; b++) {
        size_t len = 0;
        for (struct Node *n = buckets[b]; n != NULL; n = n->next)
            len++;
        if (len > 0)
            busy++;
        if (len > longest)
            longest = len;
    }

    printf("  %-16s %10zu %12zu %10zu %12.1f\n",
           title, busy, longest, comparisons,
           1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC);

    for (size_t b = 0; b < BUCKETS; b++) {
        struct Node *n = buckets[b];
        while (n != NULL) {
            struct Node *next = n->next;
            free(n);
            n = next;
        }
    }
    free(buckets);
}

int main(void)
{
    printf("ключей %d, корзин %d\n", KEYS, BUCKETS);
    printf("первые три ключа: abcdefghij, abcdefghji, abcdefgihj\n\n");
    printf("  %-16s %10s %12s %10s %12s\n",
           "hash", "busy", "longest", "strcmp", "ms");
    run("sum", sum_hash);
    run("fnv1a", fnv1a);
    return 0;
}
