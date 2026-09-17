/* Демо 6. Открытая адресация: ключ живёт в самой таблице.
 *
 *   gcc -std=c11 -Wall -Wextra 06-probing.c -o 06-probing
 *   ./06-probing
 *
 * Таблица из шестнадцати слотов заполняется по одному ключу. Если слот занят,
 * берётся следующий по кругу, и так до первого свободного. После каждой
 * вставки печатается карта таблицы и число просмотренных слотов.
 *
 * До запуска: сколько слотов придётся просмотреть, чтобы положить ключ,
 * если таблица пуста?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CAP  16
#define KEYS 12

static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

static const char *slots[CAP];

static void print_map(void)
{
    printf("      ");
    for (size_t i = 0; i < CAP; i++)
        printf("%-6s", slots[i] == NULL ? "." : slots[i]);
    putchar('\n');
}

static size_t insert(const char *key)
{
    size_t home = fnv1a(key) % CAP;
    size_t probes = 1;
    size_t i = home;

    while (slots[i] != NULL) {
        if (strcmp(slots[i], key) == 0)
            return probes;
        i = (i + 1) % CAP;
        probes++;
    }
    slots[i] = key;

    printf("  %-6s корзина %2zu, лёг в слот %2zu, просмотрено слотов %zu\n",
           key, home, i, probes);
    return probes;
}

int main(void)
{
    const char *keys[KEYS] = {
        "red", "green", "blue", "cyan", "lime", "gold",
        "black", "white", "gray", "pink", "brown", "olive",
    };

    printf("слоты:");
    for (size_t i = 0; i < CAP; i++)
        printf("%-6zu", i);
    printf("\n\n");

    size_t total = 0;
    for (size_t k = 0; k < KEYS; k++) {
        total += insert(keys[k]);
        print_map();
    }

    printf("\nключей %d, слотов %d, загрузка %.2f\n",
           KEYS, CAP, (double)KEYS / CAP);
    printf("всего просмотрено слотов при вставке: %zu, в среднем %.2f на ключ\n",
           total, (double)total / KEYS);

    return 0;
}
