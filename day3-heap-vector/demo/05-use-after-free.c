/* Демо 5. Чтение после free.
 *
 *   gcc -std=c11 -Wall -Wextra 05-use-after-free.c -o 05-use-after-free
 *   ./05-use-after-free
 *   echo $?
 *
 * Сначала прочитайте предупреждение компилятора, потом запускайте. До
 * запуска: что напечатает последняя строка и с каким кодом завершится
 * программа?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *a = malloc(4 * sizeof *a);
    if (a == NULL)
        return 1;
    for (int i = 0; i < 4; i++)
        a[i] = 100 + i;
    printf("блок %p, содержимое: %d %d %d %d\n",
           (void *)a, a[0], a[1], a[2], a[3]);

    free(a);

    printf("после free читаем a[0]: ");
    fflush(stdout);
    printf("%d\n", a[0]);

    return 0;
}
