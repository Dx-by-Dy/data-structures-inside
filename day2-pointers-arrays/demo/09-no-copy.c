/* Демо 9. Функция и массив вызывающего.
 *
 *   gcc -std=c11 -Wall -Wextra 09-no-copy.c -o 09-no-copy && ./09-no-copy
 *
 * До запуска: fill_with_squares получает arr и что-то с ним делает. Изменится
 * ли массив в main — и что именно функция получила?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

static void fill_with_squares(int *a, size_t n)
{
    printf("  внутри функции a = %p\n", (void *)a);
    for (size_t i = 0; i < n; i++)
        a[i] = (int)(i * i);
}

int main(void)
{
    int arr[5] = { 10, 20, 30, 40, 50 };

    printf("до вызова:  ");
    for (size_t i = 0; i < 5; i++)
        printf("%d ", arr[i]);
    printf("\n  в main arr = %p\n", (void *)arr);

    fill_with_squares(arr, 5);

    printf("после вызова: ");
    for (size_t i = 0; i < 5; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}
