/* Задача 4. Арифметика без UB.  Условие — 04-safe-add.md
 *
 *   make
 *   printf 'add 2 3\nadd 2147483647 1\n' | ./04-safe-add
 *   ./04-safe-add --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все три
 * функции пока говорят «не помещается». Ваша работа — их тела; main и печать
 * менять не нужно.
 *
 * Главное требование условия: проверка стоит ДО операции. Если внутри функции
 * выполняется a + b, a - b или a * b раньше, чем проверка их разрешила, —
 * решение неверно, даже если оно печатает правильные числа на -O0.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

/* Кладут результат в *out и возвращают true, если он поместился в int.
   Если не поместился — возвращают false, а *out не трогают. */

static bool safe_add(int a, int b, int *out)
{
    /* TODO
     * Начните с вопроса: если b зафиксировано и положительно, при каком
     * наибольшем a сумма ещё помещается в int? Потом разберите b < 0.
     */
    (void)a; (void)b; (void)out;
    return false;
}

static bool safe_sub(int a, int b, int *out)
{
    /* TODO
     * Через safe_add выразить не выйдет: -INT_MIN в int не помещается.
     */
    (void)a; (void)b; (void)out;
    return false;
}

static bool safe_mul(int a, int b, int *out)
{
    /* TODO
     * Разберите случаи по знакам. Отдельно a == 0 и b == 0, отдельно -1.
     */
    (void)a; (void)b; (void)out;
    return false;
}

/* Печатает одну строку отчёта. op — "add", "sub" или "mul". */
static void run(const char *op, int a, int b)
{
    int r = 0;
    bool ok;

    if (strcmp(op, "add") == 0)      ok = safe_add(a, b, &r);
    else if (strcmp(op, "sub") == 0) ok = safe_sub(a, b, &r);
    else if (strcmp(op, "mul") == 0) ok = safe_mul(a, b, &r);
    else {
        printf("неизвестная операция: %s\n", op);
        return;
    }

    if (ok)
        printf("%s %d %d = %d\n", op, a, b, r);
    else
        printf("%s %d %d = overflow\n", op, a, b);
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

/* Краевые случаи. want — ожидаемый результат, ok — должна ли операция вообще
   получиться. Там, где ok == false, значение want не смотрится. */
static int self_check(void)
{
    static const struct { const char *op; int a, b; bool ok; int want; } cases[] = {
        { "add", 2, 3,                 true,  5 },
        { "add", INT_MAX, 0,           true,  INT_MAX },
        { "add", INT_MAX, 1,           false, 0 },
        { "add", INT_MIN, -1,          false, 0 },
        { "add", INT_MIN, INT_MAX,     true,  -1 },
        { "add", -1, INT_MIN,          false, 0 },
        { "add", INT_MAX, INT_MAX,     false, 0 },
        { "sub", 5, 3,                 true,  2 },
        { "sub", INT_MIN, 1,           false, 0 },
        { "sub", INT_MAX, -1,          false, 0 },
        { "sub", 0, INT_MIN,           false, 0 },
        { "sub", INT_MIN, INT_MIN,     true,  0 },
        { "sub", -1, INT_MAX,          true,  INT_MIN },
        { "mul", 6, 7,                 true,  42 },
        { "mul", 100000, 100000,       false, 0 },
        { "mul", INT_MIN, -1,          false, 0 },
        { "mul", -1, INT_MIN,          false, 0 },
        { "mul", INT_MIN, 0,           true,  0 },
        { "mul", 0, INT_MIN,           true,  0 },
        { "mul", INT_MIN, 1,           true,  INT_MIN },
        { "mul", INT_MAX, 1,           true,  INT_MAX },
        { "mul", 46341, 46341,         false, 0 },
        { "mul", 46340, 46340,         true,  2147395600 },
        { "mul", -46341, 46341,        false, 0 },
        { "mul", -46340, 46340,        true,  -2147395600 },
        { "mul", -1, INT_MAX,          true,  -INT_MAX },
    };
    char what[64];

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const char *op = cases[i].op;
        int a = cases[i].a, b = cases[i].b;
        int r = 0;
        bool ok;

        if (strcmp(op, "add") == 0)      ok = safe_add(a, b, &r);
        else if (strcmp(op, "sub") == 0) ok = safe_sub(a, b, &r);
        else                             ok = safe_mul(a, b, &r);

        snprintf(what, sizeof what, "%s %d %d: помещается", op, a, b);
        expect_ll(what, ok, cases[i].ok);
        if (cases[i].ok && ok) {
            snprintf(what, sizeof what, "%s %d %d", op, a, b);
            expect_ll(what, r, cases[i].want);
        }
    }

    /* Значение по адресу out при отказе трогать нельзя: условие обещает, что
       оно останется прежним. */
    int guard = 12345;
    safe_add(INT_MAX, 1, &guard);
    expect_ll("*out после отказа safe_add", guard, 12345);
    safe_sub(INT_MIN, 1, &guard);
    expect_ll("*out после отказа safe_sub", guard, 12345);
    safe_mul(INT_MIN, -1, &guard);
    expect_ll("*out после отказа safe_mul", guard, 12345);

    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    char op[16];
    int a, b;
    while (scanf("%15s %d %d", op, &a, &b) == 3)
        run(op, a, b);

    return 0;
}
