/* Демо 5. Цепочки: что происходит, когда ключей больше, чем корзин.
 *
 *   gcc -std=c11 -Wall -Wextra 05-chaining.c -o 05-chaining
 *   ./05-chaining
 *
 * Тысяча ключей раскладывается в таблицу с цепочками, и это повторяется для
 * разного числа корзин. Каждый ключ потом ищется, и считается, сколько раз
 * при этом пришлось сравнить строки.
 *
 * До запуска: во сколько раз изменится число сравнений, если корзин станет
 * вдвое меньше?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KEYS 1000

struct Node {
    char        *key;
    struct Node *next;
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

static size_t comparisons;   /* сколько раз вызвали strcmp */

static struct Node *lookup(struct Node **buckets, size_t cap, const char *key)
{
    for (struct Node *n = buckets[fnv1a(key) % cap]; n != NULL; n = n->next) {
        comparisons++;
        if (strcmp(n->key, key) == 0)
            return n;
    }
    return NULL;
}

static void run(size_t cap)
{
    struct Node **buckets = calloc(cap, sizeof *buckets);
    if (buckets == NULL)
        exit(1);

    char key[32];
    for (int i = 0; i < KEYS; i++) {
        snprintf(key, sizeof key, "key%d", i);
        struct Node *n = malloc(sizeof *n);
        if (n == NULL)
            exit(1);
        n->key = malloc(strlen(key) + 1);
        if (n->key == NULL)
            exit(1);
        memcpy(n->key, key, strlen(key) + 1);
        size_t b = fnv1a(key) % cap;
        n->next = buckets[b];
        buckets[b] = n;
    }

    comparisons = 0;
    for (int i = 0; i < KEYS; i++) {
        snprintf(key, sizeof key, "key%d", i);
        if (lookup(buckets, cap, key) == NULL)
            printf("  потерялся ключ %s\n", key);
    }

    size_t longest = 0, empty = 0;
    for (size_t b = 0; b < cap; b++) {
        size_t len = 0;
        for (struct Node *n = buckets[b]; n != NULL; n = n->next)
            len++;
        if (len > longest)
            longest = len;
        if (len == 0)
            empty++;
    }

    printf("  %6zu   %8.2f   %14.2f   %13zu   %8zu\n",
           cap, (double)KEYS / (double)cap,
           (double)comparisons / KEYS, longest, empty);

    for (size_t b = 0; b < cap; b++) {
        struct Node *n = buckets[b];
        while (n != NULL) {
            struct Node *next = n->next;
            free(n->key);
            free(n);
            n = next;
        }
    }
    free(buckets);
}

int main(void)
{
    printf("%d ключей в таблице с цепочками\n\n", KEYS);
    printf("  корзин   загрузка   сравнений/поиск   самая длинная   пустых\n");
    for (size_t cap = 4096; cap >= 8; cap /= 2)
        run(cap);
    return 0;
}
