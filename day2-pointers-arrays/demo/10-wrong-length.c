/* Демо 10. Три вызова с разной длиной.
 *
 *   gcc -std=c11 -Wall -Wextra 10-wrong-length.c -o 10-wrong-length
 *   ./10-wrong-length
 *
 * Массив из пяти элементов, а функции сообщают длину 3, потом 5, потом 50.
 * Про каждый вызов скажите заранее три вещи: верный ли будет ответ, скажет ли
 * что-нибудь компилятор, скажет ли что-нибудь санитайзер.
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>

static long sum(const int *a, size_t n)
{
    long s = 0;
    for (size_t i = 0; i < n; i++)
        s += a[i];
    return s;
}

int main(void)
{
    int arr[5] = { 10, 20, 30, 40, 50 };

    printf("массив: 10 20 30 40 50, всего пять элементов\n\n");
    printf("sum(arr, 3)  = %ld\n", sum(arr, 3));
    printf("sum(arr, 5)  = %ld\n", sum(arr, 5));
    fflush(stdout);
    printf("sum(arr, 50) = %ld\n", sum(arr, 50));

    return 0;
}
