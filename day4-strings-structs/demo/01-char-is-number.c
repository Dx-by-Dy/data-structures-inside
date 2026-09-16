/* Демо 1. Символ — это число.
 *
 *   gcc -std=c11 -Wall -Wextra 01-char-is-number.c -o 01-char-is-number
 *   ./01-char-is-number
 *
 * До запуска: чему равно sizeof('A')? И что напечатает 'a' - 'A'?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <limits.h>

int main(void)
{
    char c = 'A';

    printf("тип char:\n");
    printf("  sizeof(char) = %zu, CHAR_BIT = %d\n", sizeof(char), CHAR_BIT);
    printf("  диапазон     = %d .. %d\n\n", CHAR_MIN, CHAR_MAX);

    printf("одно и то же значение двумя способами:\n");
    printf("  как символ: %c\n", c);
    printf("  как число:  %d\n\n", c);

    /* Литерал 'A' и переменная типа char — не одно и то же. */
    printf("sizeof('A') = %zu, sizeof(c) = %zu\n\n", sizeof('A'), sizeof(c));

    printf("арифметика:\n");
    printf("  'a' - 'A' = %d\n", 'a' - 'A');
    printf("  'A' + 1   = %d, как символ: %c\n", 'A' + 1, 'A' + 1);
    printf("  '7' - '0' = %d\n\n", '7' - '0');

    printf("подряд идущие коды:\n  ");
    for (char ch = 'a'; ch <= 'e'; ch++)
        printf("%c=%d ", ch, ch);
    printf("\n\n");

    /* Символы, у которых нет изображения, записываются escape-последовательностью. */
    printf("служебные символы:\n");
    printf("  '\\n' = %d, '\\t' = %d, '\\0' = %d\n", '\n', '\t', '\0');

    return 0;
}
