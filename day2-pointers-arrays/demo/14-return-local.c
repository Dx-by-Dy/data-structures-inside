/* Демо 14. Функция возвращает адрес своей локальной переменной.
 *
 *   gcc -std=c11 -Wall -Wextra 14-return-local.c -o 14-return-local
 *   ./14-return-local
 *
 * Сначала читайте, что сказал компилятор, и только потом запускайте.
 *
 * До запуска: какое значение напечатает строка «return_local() вернул»?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

static int *return_local(void)
{
    int local = 42;
    printf("return_local: local = %d  @ %p\n", local, (void *)&local);
    return &local;
}

int main(void)
{
    int *p = return_local();
    printf("return_local() вернул %p\n", (void *)p);
    fflush(stdout);   /* иначе строки выше потеряются, если программа упадёт */
    printf("читаем по нему: %d\n", *p);

    return 0;
}
