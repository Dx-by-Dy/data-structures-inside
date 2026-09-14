/* Демо 13. Адрес, отданный через выходной параметр.
 *
 *   gcc -std=c11 -Wall -Wextra 13-dangling-save.c -o 13-dangling-save
 *   ./13-dangling-save
 *
 * Сначала прочитайте предупреждение компилятора, потом запускайте.
 *
 * До запуска: *q печатается дважды, и между печатями q никто не трогает.
 * Совпадут ли два напечатанных значения?
 *
 * Вторая сборка и остальные вопросы — в README.md.
 */
#include <stdio.h>

static void save_local(int **out)
{
    int local = 111;
    printf("save_local:     local = %d  @ %p\n", local, (void *)&local);
    *out = &local;
}

/* Ничего не знает ни про save_local, ни про q: просто следующий вызов
   из main. */
static void unrelated_call(void)
{
    int other = 777;
    printf("unrelated_call: other = %d  @ %p\n", other, (void *)&other);
}

int main(void)
{
    int *q;
    save_local(&q);
    printf("main:           *q    = %d  @ %p\n", *q, (void *)q);

    unrelated_call();
    printf("main:           *q    = %d  (q не менялся)\n", *q);

    return 0;
}
