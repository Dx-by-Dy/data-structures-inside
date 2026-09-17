/* Демо 7. Удаление из таблицы с открытой адресацией.
 *
 *   gcc -std=c11 -Wall -Wextra 07-tombstone.c -o 07-tombstone
 *   ./07-tombstone
 *
 * Три ключа, которым досталась одна корзина, кладутся подряд. Потом средний
 * из них удаляется — двумя способами, — и все три ключа ищутся снова.
 *
 * До запуска: поиск идёт от корзины ключа вперёд и останавливается на первом
 * свободном слоте. Что случится с третьим ключом, если освободить слот
 * второго?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CAP 8

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

/* Слот пуст, если в нём NULL; надгробие — особое значение, которое не равно
   ни одному ключу и не равно NULL. */
static const char TOMBSTONE[] = "";
static const char *slots[CAP];

static void reset(void)
{
    for (size_t i = 0; i < CAP; i++)
        slots[i] = NULL;
}

static void insert(const char *key)
{
    size_t i = fnv1a(key) % CAP;
    while (slots[i] != NULL && slots[i] != TOMBSTONE)
        i = (i + 1) % CAP;
    slots[i] = key;
}

static int find(const char *key)
{
    size_t i = fnv1a(key) % CAP;
    for (size_t step = 0; step < CAP; step++) {
        if (slots[i] == NULL)
            return -1;
        if (slots[i] != TOMBSTONE && strcmp(slots[i], key) == 0)
            return (int)i;
        i = (i + 1) % CAP;
    }
    return -1;
}

static void erase_clear(const char *key)
{
    int at = find(key);
    if (at >= 0)
        slots[at] = NULL;
}

static void erase_tombstone(const char *key)
{
    int at = find(key);
    if (at >= 0)
        slots[at] = TOMBSTONE;
}

static void print_map(void)
{
    printf("     ");
    for (size_t i = 0; i < CAP; i++) {
        if (slots[i] == NULL)
            printf("%-8s", ".");
        else if (slots[i] == TOMBSTONE)
            printf("%-8s", "X");
        else
            printf("%-8s", slots[i]);
    }
    putchar('\n');
}

static void find_all(const char *a, const char *b, const char *c)
{
    printf("     поиск: %s -> %d, %s -> %d, %s -> %d\n",
           a, find(a), b, find(b), c, find(c));
}

int main(void)
{
    /* Три ключа, у которых одна и та же корзина: найдены перебором ключей
       вида wordNNN. */
    char a[16], b[16], c[16], d[16];
    char *found[4] = {a, b, c, d};
    size_t count = 0;
    for (int i = 0; count < 4 && i < 100000; i++) {
        char key[16];
        snprintf(key, sizeof key, "word%d", i);
        if (fnv1a(key) % CAP == 3)
            snprintf(found[count++], 16, "%s", key);
    }

    printf("ключи %s, %s, %s и %s — все в корзине 3\n\n", a, b, c, d);

    printf("положили три ключа подряд:\n");
    reset();
    insert(a); insert(b); insert(c);
    print_map();
    find_all(a, b, c);

    printf("\nудалили %s, освободив слот:\n", b);
    erase_clear(b);
    print_map();
    find_all(a, b, c);

    printf("\nто же самое, но в слоте оставлено надгробие (X):\n");
    reset();
    insert(a); insert(b); insert(c);
    erase_tombstone(b);
    print_map();
    find_all(a, b, c);

    printf("\nновый ключ той же корзины кладётся в надгробие:\n");
    insert(d);
    print_map();
    find_all(a, d, c);

    return 0;
}
