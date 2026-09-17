/* Демо 2. Сумма байтов в роли хеша.
 *
 *   gcc -std=c11 -Wall -Wextra 02-hash-sum.c -o 02-hash-sum
 *   ./02-hash-sum
 *
 * До запуска: посмотрите на пары слов в выводе и скажите, какие из них
 * получат одинаковое число.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Хеш «сумма байтов»: короткий, понятный и законный — равные строки дают
   равные числа. */
static uint32_t sum_hash(const char *s)
{
    uint32_t h = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++)
        h += *p;
    return h;
}

int main(void)
{
    const char *pairs[][2] = {
        {"abc",    "bca"},
        {"listen", "silent"},
        {"hello",  "olleh"},
        {"ab",     "ba"},
        {"cat",    "act"},
        {"hello",  "world"},
    };
    const size_t count = sizeof pairs / sizeof pairs[0];

    printf("  key         hash   key         hash   equal\n");
    for (size_t i = 0; i < count; i++) {
        uint32_t a = sum_hash(pairs[i][0]);
        uint32_t b = sum_hash(pairs[i][1]);
        printf("  %-8s %6u   %-8s %6u   %s\n",
               pairs[i][0], a, pairs[i][1], b, a == b ? "да" : "нет");
    }

    printf("\nсколько разных значений вообще возможно:\n");
    for (size_t len = 1; len <= 16; len *= 2)
        printf("  строка из %2zu байт: от 0 до %zu\n", len, len * 255);

    printf("\nключи key000 … key199: ");
    uint32_t seen[8192] = {0};
    size_t distinct = 0;
    char key[16];
    for (int i = 0; i < 200; i++) {
        snprintf(key, sizeof key, "key%03d", i);
        uint32_t h = sum_hash(key);
        if (seen[h] == 0)
            distinct++;
        seen[h]++;
    }
    printf("разных значений хеша %zu на 200 ключей\n", distinct);

    return 0;
}
