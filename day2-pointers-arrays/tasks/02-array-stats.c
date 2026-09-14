/* Задача 2. Массив и его длина.  Условие — 02-array-stats.md
 *
 *   make
 *   printf '3 -1 4 -1 5 -1\n' | ./02-array-stats
 *   ./02-array-stats --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все четыре
 * функции пока возвращают заведомо неверное. Ваша работа — их тела; main и
 * печать менять не нужно.
 */
#include <stdio.h>
#include <string.h>

#define CAP 100

/* Сумма n элементов. Тип результата шире int намеренно. */
static long long sum(const int *a, size_t n)
{
    /* TODO */
    (void)a; (void)n;
    return 0;
}

/* Наименьший элемент. Вызывается только при n > 0 — проверку делает main. */
static int minimum(const int *a, size_t n)
{
    /* TODO */
    (void)a; (void)n;
    return 0;
}

/* Наибольший элемент. Вызывается только при n > 0. */
static int maximum(const int *a, size_t n)
{
    /* TODO */
    (void)a; (void)n;
    return 0;
}

/* Сколько раз value встречается среди n элементов. */
static size_t count_equal(const int *a, size_t n, int value)
{
    /* TODO */
    (void)a; (void)n; (void)value;
    return 0;
}

static void report(const int *a, size_t n)
{
    printf("прочитано чисел: %zu\n", n);
    if (n == 0) {
        printf("массив пуст, считать нечего\n");
        return;
    }

    long long s = sum(a, n);
    int lo = minimum(a, n);
    int hi = maximum(a, n);

    printf("сумма    = %lld\n", s);
    printf("среднее  = %.3f\n", (double)s / (double)n);
    printf("минимум  = %d (совпадений: %zu)\n", lo, count_equal(a, n, lo));
    printf("максимум = %d (совпадений: %zu)\n", hi, count_equal(a, n, hi));
}

/* ── самопроверка ────────────────────────────────────────────────────────
   expect_* сравнивают то, что вернули ваши функции, с тем, что должно быть.
   Ожидаемые значения здесь — часть условия. */
static int checks_run = 0, checks_failed = 0;

static void expect_ll(const char *what, long long got, long long want)
{
    checks_run++;
    if (got == want) {
        printf("  ok      %s = %lld\n", what, got);
        return;
    }
    checks_failed++;
    printf("  ОШИБКА  %s = %lld, ожидалось %lld\n", what, got, want);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

/* Краевые случаи. minimum и maximum на пустом массиве не вызываются: условие
   обещает, что проверку делает вызывающий. */
static int self_check(void)
{
    static const int one[]   = { 42 };
    static const int same[]  = { -7, -7, -7, -7 };
    static const int big[]   = { 2000000000, 2000000000, 2000000000 };
    static const int edges[] = { 2147483647, -2147483648, 0 };
    static const int mixed[] = { 3, -1, 4, -1, 5, -1 };

    expect_ll("sum пустого", sum(NULL, 0), 0);

    expect_ll("sum{42}", sum(one, 1), 42);
    expect_ll("minimum{42}", minimum(one, 1), 42);
    expect_ll("maximum{42}", maximum(one, 1), 42);
    expect_ll("count_equal{42}, 42", (long long)count_equal(one, 1, 42), 1);
    expect_ll("count_equal{42}, 0", (long long)count_equal(one, 1, 0), 0);

    expect_ll("sum четырёх -7", sum(same, 4), -28);
    expect_ll("minimum четырёх -7", minimum(same, 4), -7);
    expect_ll("maximum четырёх -7", maximum(same, 4), -7);
    expect_ll("count_equal четырёх -7", (long long)count_equal(same, 4, -7), 4);

    /* Ради этой строки sum возвращает long long: в int такая сумма не влезает. */
    expect_ll("sum трёх по 2000000000", sum(big, 3), 6000000000LL);

    expect_ll("sum{INT_MAX, INT_MIN, 0}", sum(edges, 3), -1);
    expect_ll("minimum{INT_MAX, INT_MIN, 0}", minimum(edges, 3), -2147483648LL);
    expect_ll("maximum{INT_MAX, INT_MIN, 0}", maximum(edges, 3), 2147483647LL);

    expect_ll("sum примера", sum(mixed, 6), 9);
    expect_ll("minimum примера", minimum(mixed, 6), -1);
    expect_ll("maximum примера", maximum(mixed, 6), 5);
    expect_ll("count_equal примера, -1", (long long)count_equal(mixed, 6, -1), 3);
    expect_ll("count_equal примера, 5", (long long)count_equal(mixed, 6, 5), 1);
    expect_ll("count_equal примера, 7", (long long)count_equal(mixed, 6, 7), 0);
    expect_ll("count_equal пустого", (long long)count_equal(NULL, 0, 0), 0);

    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    int a[CAP];
    size_t n = 0;
    int v;
    while (n < CAP && scanf("%d", &v) == 1)
        a[n++] = v;

    report(a, n);
    return 0;
}
