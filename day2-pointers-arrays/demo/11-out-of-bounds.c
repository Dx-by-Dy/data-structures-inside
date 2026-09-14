/* Демо 11. Индекс приходит со входа.
 *
 *   gcc -std=c11 -Wall -Wextra 11-out-of-bounds.c -o oob
 *   echo 4 | ./oob
 *
 * Массив из четырёх элементов, вокруг него две обычные переменные. До запуска:
 * что напечатает программа при индексе 4 и завершится ли она вообще?
 *
 * Остальные заходы и вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    int before = 111;
    int a[4]   = { 10, 20, 30, 40 };
    int after  = 999;

    int idx;
    if (scanf("%d", &idx) != 1) {
        fprintf(stderr, "на входе должен быть индекс\n");
        return 1;
    }

    printf("массив занимает %zu байт по адресу %p\n", sizeof a, (void *)a);
    printf("соседи: before = %d по адресу %p\n", before, (void *)&before);
    printf("        after  = %d по адресу %p\n\n", after, (void *)&after);

    printf("читаем a[%d] = %d\n", idx, a[idx]);

    printf("пишем a[%d] = -1\n", idx);
    a[idx] = -1;

    printf("\nчто стало с соседями:\n");
    printf("  before = %d\n", before);
    printf("  after  = %d\n", after);
    printf("  массив: ");
    for (int i = 0; i < 4; i++)
        printf("%d ", a[i]);
    printf("\n");

    return 0;
}
