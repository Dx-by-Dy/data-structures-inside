/* Демо 7. Указатель на середину массива.
 *
 *   gcc -std=c11 -Wall -Wextra 07-slice.c -o 07-slice && ./07-slice
 *
 * До запуска: mid показывает на третий элемент. Что напечатает mid[-1] и
 * законна ли эта запись?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

static void print_all(const char *label, const int *a, size_t n)
{
    printf("%s", label);
    for (size_t i = 0; i < n; i++)
        printf(" %d", a[i]);
    printf("\n");
}

int main(void)
{
    int a[5] = { 10, 20, 30, 40, 50 };

    print_all("весь массив: ", a, 5);

    int *mid = a + 2;
    printf("\nmid = a + 2 = %p\n", (void *)mid);
    printf("  mid[0]  = %d\n", mid[0]);
    printf("  mid[1]  = %d\n", mid[1]);
    printf("  mid[-1] = %d\n", mid[-1]);

    /* Для принимающей стороны это самостоятельный массив из трёх элементов. */
    print_all("\nmid как массив длины 3:", mid, 3);
    print_all("хвост из двух элементов:", a + 3, 2);

    return 0;
}
