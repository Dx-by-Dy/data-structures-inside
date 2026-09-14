/* Задача 3. Деление как в Python.  Условие — 03-pydiv.md
 *
 *   make
 *   printf '7 2\n-7 2\n7 -2\n-7 -2\n' | ./03-pydiv
 *   ./03-pydiv --check      встроенные краевые случаи
 *   ./03-pydiv --pairs      режим для сверки с питоном, см. условие
 *
 * Это скелет: он собирается без предупреждений и запускается, но обе функции
 * пока возвращают заведомо неверное. Ваша работа — их тела; main и печать
 * менять не нужно.
 */
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

/* То же, что a // b в Python: округление вниз. */
static int floor_div(int a, int b)
{
    /* TODO
     * Сишное a / b уже посчитано за вас компилятором. Осталось решить, когда
     * его результат надо поправить на единицу и в какую сторону.
     */
    (void)a; (void)b;
    return 0;
}

/* То же, что a % b в Python: знак совпадает со знаком b. */
static int floor_mod(int a, int b)
{
    /* TODO
     * Проще выразить через floor_div, чем считать заново.
     */
    (void)a; (void)b;
    return 0;
}

/* Единственное место, где решается, можно ли вообще делить. */
static bool divisible(int a, int b)
{
    return b != 0 && !(a == INT_MIN && b == -1);
}

static void print_row(int a, int b)
{
    if (!divisible(a, b)) {
        printf("%d / %d:  делить нельзя\n", a, b);
        return;
    }
    printf("%d / %d:  C -> %d %d   Python -> %d %d\n",
           a, b, a / b, a % b, floor_div(a, b), floor_mod(a, b));
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

/* Краевые случаи. Два последних числа в строке — ответы питона, то есть то,
   что должны вернуть floor_div и floor_mod. Пары, для которых делить нельзя,
   вынесены отдельно: там проверяется, что программа их не считает. */
static int self_check(void)
{
    static const struct { int a, b, q, r; } cases[] = {
        {  7,  2,  3,  1 },
        { -7,  2, -4,  1 },
        {  7, -2, -4, -1 },
        { -7, -2,  3, -1 },
        {  0,  3,  0,  0 },
        {  0, -3,  0,  0 },
        { -1,  2, -1,  1 },
        {  1, -2, -1, -1 },
        { -8,  2, -4,  0 },
        { INT_MIN,  1, INT_MIN, 0 },
        { INT_MAX, -1, -INT_MAX, 0 },
        { INT_MIN,  2, INT_MIN / 2, 0 },
    };
    char what[64];

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        int a = cases[i].a, b = cases[i].b;
        snprintf(what, sizeof what, "floor_div(%d, %d)", a, b);
        expect_ll(what, floor_div(a, b), cases[i].q);
        snprintf(what, sizeof what, "floor_mod(%d, %d)", a, b);
        expect_ll(what, floor_mod(a, b), cases[i].r);
    }

    /* Пары, на которых делить нельзя вовсе. Считать их нельзя ни C, ни вам:
       floor_div здесь не вызывается, проверяется только сам отказ. */
    static const struct { int a, b; } forbidden[] = {
        { 5, 0 }, { -5, 0 }, { 0, 0 }, { INT_MIN, -1 },
    };
    for (size_t i = 0; i < sizeof forbidden / sizeof forbidden[0]; i++) {
        snprintf(what, sizeof what, "divisible(%d, %d)",
                 forbidden[i].a, forbidden[i].b);
        expect_ll(what, divisible(forbidden[i].a, forbidden[i].b), 0);
    }

    /* Главное обещание задачи: при b > 0 остаток неотрицателен. */
    int bad_sign = 0;
    for (int a = -50; a <= 50; a++)
        for (int b = 1; b <= 7; b++)
            if (floor_mod(a, b) < 0 || floor_mod(a, b) >= b)
                bad_sign++;
    expect_ll("floor_mod при b > 0 вне диапазона [0, b) — случаев", bad_sign, 0);

    /* И тождество, которое обязано выполняться на всех парах. */
    int bad_id = 0;
    for (int a = -50; a <= 50; a++)
        for (int b = -7; b <= 7; b++)
            if (b != 0 && floor_div(a, b) * b + floor_mod(a, b) != a)
                bad_id++;
    expect_ll("нарушений (a//b)*b + a%b == a", bad_id, 0);

    return checks_total();
}

int main(int argc, char **argv)
{
    bool pairs_mode = (argc > 1 && strcmp(argv[1], "--pairs") == 0);

    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    int a, b;
    while (scanf("%d %d", &a, &b) == 2) {
        if (pairs_mode) {
            /* Формат для сверки с питоном: a b a//b a%b, по строке на пару. */
            if (divisible(a, b))
                printf("%d %d %d %d\n", a, b, floor_div(a, b), floor_mod(a, b));
        } else {
            print_row(a, b);
        }
    }

    return 0;
}
