/* Демо 10. Четыре стратегии роста.
 *
 *   gcc -std=c11 -Wall -Wextra -O2 10-growth.c -o 10-growth
 *   ./10-growth            # 20000 элементов
 *   ./10-growth 100000
 *
 * -O2 здесь не для красоты: одна из четырёх стратегий делает работу,
 * пропорциональную квадрату длины.
 *
 * Каждая стратегия добавляет n элементов по одному в массив на куче. Когда
 * место кончается, выделяется новый блок, старые элементы переносятся, старый
 * блок освобождается. Копирование написано руками, а не отдано realloc:
 * считать надо именно перенесённые элементы.
 *
 * До запуска предскажите два числа: во сколько раз «по одному» перенесёт
 * больше элементов, чем «вдвое», и во сколько раз дольше будет работать.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Как выбирается новая вместимость, когда в блок на cap элементов нужно
   положить len + 1. Четыре ответа на один вопрос. */
static size_t grow_by_one(size_t cap)   { return cap + 1; }
static size_t grow_by_eight(size_t cap) { return cap + 8; }
static size_t grow_double(size_t cap)   { return cap < 4 ? 4 : cap * 2; }

/* Формула CPython из listobject10-growth.c. newsize — сколько элементов должно
   поместиться; результат округляется вниз до кратного четырём. */
static size_t grow_cpython(size_t cap)
{
    size_t newsize = cap + 1;
    return (newsize + (newsize >> 3) + 6) & ~(size_t)3;
}

struct result {
    size_t reallocations;   /* сколько раз выделялся новый блок */
    size_t copied;          /* сколько элементов при этом перенесено */
    double seconds;
};

static struct result build(const char *name, size_t n, size_t (*next_cap)(size_t))
{
    struct result r = {0, 0, 0.0};
    clock_t start = clock();

    int *a = NULL;
    size_t len = 0, cap = 0;

    for (size_t value = 0; value < n; value++) {
        if (len == cap) {
            size_t new_cap = next_cap(cap);
            int *bigger = malloc(new_cap * sizeof *bigger);
            if (bigger == NULL) {
                fprintf(stderr, "%s: память кончилась на длине %zu\n", name, len);
                free(a);
                exit(1);
            }
            for (size_t i = 0; i < len; i++)
                bigger[i] = a[i];
            free(a);
            a = bigger;
            cap = new_cap;
            r.reallocations++;
            r.copied += len;
        }
        a[len++] = (int)value;
    }

    r.seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("%-10s блоков: %7zu   перенесено: %12zu   %8.1f на элемент   %7.3f c\n",
           name, r.reallocations, r.copied, (double)r.copied / (double)n,
           r.seconds);
    free(a);
    return r;
}

int main(int argc, char **argv)
{
    size_t n = 20000;
    if (argc > 1) {
        long got = strtol(argv[1], NULL, 10);
        if (got <= 0) {
            fprintf(stderr, "нужно положительное число элементов\n");
            return 1;
        }
        n = (size_t)got;
    }

    printf("добавляем %zu элементов по одному\n\n", n);

    struct result one    = build("cap + 1", n, grow_by_one);
    struct result eight  = build("cap + 8", n, grow_by_eight);
    struct result twice  = build("cap * 2", n, grow_double);
    struct result python = build("CPython", n, grow_cpython);

    double base = (double)(twice.copied ? twice.copied : 1);
    printf("\nво сколько раз больше переносится, чем при росте вдвое:\n");
    printf("  cap + 1   %.0f\n", (double)one.copied / base);
    printf("  cap + 8   %.0f\n", (double)eight.copied / base);
    printf("  CPython   %.2f\n", (double)python.copied / base);

    return 0;
}
