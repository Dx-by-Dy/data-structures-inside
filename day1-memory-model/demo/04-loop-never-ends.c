/* Демо 4. Один исходник, две сборки.
 *
 *   gcc -std=c11 -Wall -Wextra -O0 04-loop-never-ends.c -o loop0 && ./loop0
 *   gcc -std=c11 -Wall -Wextra -O2 04-loop-never-ends.c -o loop2 && ./loop2
 *
 * Если какая-то из двух версий не заканчивается, прерывайте её через Ctrl+C.
 *
 * До запуска: при каком значении i условие i > 0 обязано стать ложным — и что
 * происходит в программе на шаг раньше?
 *
 * Отдельно посмотрите, что печатает сам компилятор при сборке с -O2.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <limits.h>

int main(void)
{
    printf("INT_MAX = %d\n", INT_MAX);
    printf("считаем от 1, пока i > 0\n\n");

    long long steps = 0;
    for (int i = 1; i > 0; i++) {
        steps++;
        if (steps % 500000000LL == 0)
            printf("  ... %lld шагов, i = %d\n", steps, i);
    }

    printf("\nцикл завершился на %lld шагах\n", steps);
    return 0;
}
