/* Демо 5. Вопрос дня: 2 в степени 1000.
 *
 *   gcc -std=c11 -Wall -Wextra 05-pow2.c -o 05-pow2 && ./05-pow2
 *
 * Откройте рядом питон и посчитайте 2 ** 1000. Программа делает то же самое:
 * удваивает единицу тысячу раз в самом широком целом типе, какой есть в C.
 *
 * До запуска: на каком шаге результат перестанет совпадать с питоновским и
 * что окажется в переменной в конце?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("LLONG_MAX = %lld\n", LLONG_MAX);
    printf("бит в long long: %zu\n\n", sizeof(long long) * CHAR_BIT);

    unsigned long long p = 1;
    for (int i = 1; i <= 1000; i++) {
        p = p * 2;
        /* Каждый десятый шаг, а с 62-го по 66-й — каждый. */
        if ((i % 10 == 0 && i <= 60) || (i >= 62 && i <= 66))
            printf("2^%-4d = %llu\n", i, p);
    }
    printf("...\n");
    printf("2^1000 = %llu\n", p);

    return 0;
}
