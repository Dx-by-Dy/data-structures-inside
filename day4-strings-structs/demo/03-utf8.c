/* Демо 3. Буква и байт — разные вещи.
 *
 *   gcc -std=c11 -Wall -Wextra 03-utf8.c -o 03-utf8 && ./03-utf8
 *
 * До запуска: откройте рядом питон и посчитайте len("привет"). Что напечатает
 * strlen для той же строки?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <string.h>

static void dump(const char *s)
{
    printf("  \"%s\": strlen = %zu, байты:", s, strlen(s));
    for (size_t i = 0; i < strlen(s); i++)
        printf(" %02X", (unsigned char)s[i]);
    printf("\n");
}

int main(void)
{
    dump("hello");
    dump("привет");
    dump("ёж");
    printf("\n");

    const char *word = "привет";

    /* Обращение по индексу выдаёт байт, а не букву. */
    printf("word[0] = %d, word[1] = %d\n", (unsigned char)word[0],
           (unsigned char)word[1]);
    printf("первый байт как символ: [%c]\n\n", word[0]);

    /* Печать первых трёх байт и первых двух. */
    printf("первые три байта: [%.3s]\n", word);
    printf("первые два байта: [%.2s]\n", word);

    return 0;
}
