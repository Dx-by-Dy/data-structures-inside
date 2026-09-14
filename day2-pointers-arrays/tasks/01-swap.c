/* Задача 1. Функция, которая меняет ваши переменные.  Условие — 01-swap.md
 *
 *   make
 *   printf '7 2\n-9 4\n5 0\n' | ./01-swap
 *   ./01-swap --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все три
 * функции пока ничего не делают. Ваша работа — их тела; main трогать не нужно.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

/* Меняет местами значения, лежащие по адресам a и b. */
static void swap(int *a, int *b)
{
    /* TODO
     * Работать надо со значениями по адресам: *a и *b.
     * Временная переменная — обычный int, а не указатель.
     */
    (void)a; (void)b;  /* уберите эту строку, когда напишете тело: без неё
                          -Wextra справедливо ругается на неиспользуемые
                          параметры */
}

/* Частное и остаток по правилам C. Возвращает false, если результат не
   определён; в этом случае *q и *r не трогает. */
static bool divmod(int a, int b, int *q, int *r)
{
    /* TODO
     * Случаев, когда считать нельзя, ровно два. Оба вы разбирали вчера:
     * деление на ноль и INT_MIN / -1. Проверка — ДО деления.
     */
    (void)a; (void)b; (void)q; (void)r;
    return false;
}

/* Меньшее из двух чисел кладёт по адресу min, большее — по адресу max. */
static void minmax(int a, int b, int *min, int *max)
{
    /* TODO */
    (void)a; (void)b; (void)min; (void)max;
}

/* Печатает четыре строки для одной пары. swap меняет a и b, поэтому всё, что
   печатается ниже него, относится уже к новым значениям. */
static void report(int a, int b)
{
    printf("a = %d, b = %d\n", a, b);

    int q, r;
    if (divmod(a, b, &q, &r))
        printf("  divmod: %d / %d = %d, остаток %d\n", a, b, q, r);
    else
        printf("  divmod: не определено\n");

    /* Начальные значения нужны, чтобы программа печатала осмысленные числа,
       пока minmax ещё пустая. В готовом решении их перезапишет сама функция. */
    int lo = 0, hi = 0;
    minmax(a, b, &lo, &hi);
    printf("  minmax: min = %d, max = %d\n", lo, hi);

    swap(&a, &b);
    printf("  swap:   a = %d, b = %d\n", a, b);
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

/* Краевые случаи. q и r — ожидаемые частное и остаток по правилам C;
   ok показывает, должен ли divmod вообще посчитать эту пару. */
static int self_check(void)
{
    static const struct { int a, b; bool ok; int q, r; } cases[] = {
        {  7,  2, true,   3,  1 },
        { -9,  4, true,  -2, -1 },
        {  9, -4, true,  -2,  1 },
        { -9, -4, true,   2, -1 },
        {  0,  3, true,   0,  0 },
        {  3,  3, true,   1,  0 },
        { -3, -3, true,   1,  0 },
        {  5,  0, false,  0,  0 },
        {  0,  0, false,  0,  0 },
        { INT_MIN, -1, false, 0, 0 },
        { INT_MIN,  1, true, INT_MIN, 0 },
        { INT_MAX, -1, true, -INT_MAX, 0 },
    };
    char what[64];

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        int a = cases[i].a, b = cases[i].b;
        int q = -12345, r = -12345;
        bool ok = divmod(a, b, &q, &r);

        snprintf(what, sizeof what, "divmod(%d, %d) посчитал", a, b);
        expect_ll(what, ok, cases[i].ok);
        if (cases[i].ok && ok) {
            snprintf(what, sizeof what, "divmod(%d, %d) частное", a, b);
            expect_ll(what, q, cases[i].q);
            snprintf(what, sizeof what, "divmod(%d, %d) остаток", a, b);
            expect_ll(what, r, cases[i].r);
        }
        if (!cases[i].ok) {
            /* Условие обещает: при отказе *q и *r не трогаются. */
            snprintf(what, sizeof what, "divmod(%d, %d) не тронул *q", a, b);
            expect_ll(what, q, -12345);
        }

        int lo = 0, hi = 0;
        minmax(a, b, &lo, &hi);
        snprintf(what, sizeof what, "minmax(%d, %d) min", a, b);
        expect_ll(what, lo, a < b ? a : b);
        snprintf(what, sizeof what, "minmax(%d, %d) max", a, b);
        expect_ll(what, hi, a > b ? a : b);

        int x = a, y = b;
        swap(&x, &y);
        snprintf(what, sizeof what, "swap(%d, %d) первое", a, b);
        expect_ll(what, x, b);
        snprintf(what, sizeof what, "swap(%d, %d) второе", a, b);
        expect_ll(what, y, a);
    }

    /* Оба адреса одинаковые: divmod(7, 2, &q, &q). Что должно получиться —
       разберитесь по своему коду, здесь проверяется только то, что программа
       это переживает. */
    int both = 0;
    (void)divmod(7, 2, &both, &both);
    printf("  (divmod(7, 2, &q, &q) оставил в q %d)\n", both);

    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    int a, b;

    /* scanf возвращает количество прочитанных значений. Пара прочитана
       полностью, только если он вернул 2. */
    while (scanf("%d %d", &a, &b) == 2)
        report(a, b);

    return 0;
}
