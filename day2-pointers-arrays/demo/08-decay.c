/* Демо 8. sizeof внутри функции.
 *
 *   gcc -std=c11 -Wall -Wextra 08-decay.c -o 08-decay && ./08-decay
 *
 * Функция объявлена как sum(int a[5], size_t n). До запуска ответьте: что
 * напечатает sizeof a ВНУТРИ этой функции и что скажет компилятор.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

static long sum(int a[5], size_t n)
{
    printf("  внутри функции: sizeof a = %zu\n", sizeof a);
    long s = 0;
    for (size_t i = 0; i < n; i++)
        s += a[i];
    return s;
}

int main(void)
{
    int arr[5] = { 10, 20, 30, 40, 50 };

    printf("в main:\n");
    printf("  sizeof arr    = %zu\n", sizeof arr);
    printf("  sizeof arr[0] = %zu\n", sizeof arr[0]);
    printf("  sizeof arr / sizeof arr[0] = %zu\n\n",
           sizeof arr / sizeof arr[0]);

    printf("вызываем sum(arr, 5):\n");
    printf("  результат = %ld\n", sum(arr, 5));

    return 0;
}
