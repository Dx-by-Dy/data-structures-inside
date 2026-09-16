/* Демо 7. Длина строки нигде не хранится.
 *
 *   gcc -std=c11 -Wall -Wextra 07-strlen-cost.c -o 07-strlen-cost
 *   ./07-strlen-cost
 *
 * Два цикла считают одно и то же по одной и той же строке. Отличаются они
 * только тем, где стоит вызов strlen.
 *
 * До запуска: во сколько раз они будут различаться по времени?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 200000

int main(void)
{
    char *s = malloc(N + 1);
    if (s == NULL)
        return 1;
    for (size_t i = 0; i < N; i++)
        s[i] = 'a' + (char)(i % 26);
    s[N] = '\0';

    printf("длина строки: %zu\n\n", strlen(s));

    /* Вариант 1: strlen стоит в условии цикла. */
    clock_t t0 = clock();
    size_t count1 = 0;
    for (size_t i = 0; i < strlen(s); i++)
        if (s[i] == 'q')
            count1++;
    clock_t t1 = clock();

    /* Вариант 2: длина снята один раз до цикла. */
    size_t len = strlen(s);
    size_t count2 = 0;
    for (size_t i = 0; i < len; i++)
        if (s[i] == 'q')
            count2++;
    clock_t t2 = clock();

    double ms1 = 1000.0 * (double)(t1 - t0) / CLOCKS_PER_SEC;
    double ms2 = 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;

    printf("strlen в условии цикла: %zu букв 'q' за %.3f мс\n", count1, ms1);
    printf("длина снята заранее:    %zu букв 'q' за %.3f мс\n", count2, ms2);
    printf("\nотношение времён: %.0f\n", ms2 > 0 ? ms1 / ms2 : 0);

    free(s);
    return 0;
}
