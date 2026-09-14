/* Задача 2. Три алгоритма, один ответ.  Условие — 02-popcount.md
 *
 *   make
 *   printf '0\n37\n-1\n255\n' | ./02-popcount
 *   ./02-popcount --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все четыре
 * функции пока ничего не делают. Ваша работа — их тела; main и печать менять
 * не нужно.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Таблица ответов для всех 256 значений байта. Заполняется в build_table. */
static uint8_t byte_bits[256];

static void build_table(void)
{
    /* TODO
     * Для каждого i от 0 до 255 посчитайте количество единичных битов и
     * положите в byte_bits[i]. Чем считать — вашим же наивным способом.
     *
     * Пока таблица просто обнуляется, чтобы скелет собирался без
     * предупреждений; эту строку замените своим циклом.
     */
    for (int i = 0; i < 256; i++)
        byte_bits[i] = 0;
}

/* Цикл по всем 32 битам. Всегда ровно 32 итерации. */
static int popcount_naive(uint32_t v)
{
    /* TODO */
    (void)v;
    return -1;
}

/* v &= v - 1 в цикле. Итераций столько, сколько единиц. */
static int popcount_kernighan(uint32_t v)
{
    /* TODO */
    (void)v;
    return -1;
}

/* Четыре обращения к таблице. Достать i-й байт: (v >> (8 * i)) & 0xFFu */
static int popcount_table(uint32_t v)
{
    /* TODO */
    (void)v;
    return -1;
}

/* Печатает строку отчёта для одного числа. Возвращает 1, если три способа
   разошлись. */
static int report(int n)
{
    uint32_t v = (uint32_t)n;
    int a = popcount_naive(v);
    int b = popcount_kernighan(v);
    int c = popcount_table(v);
    int bad = (a != b || b != c);

    printf("%-11d naive=%-2d kernighan=%-2d table=%-2d  %s\n",
           n, a, b, c, bad ? "MISMATCH" : "OK");
    return bad;
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

/* Краевые случаи: ноль, одна единица, все единицы, границы знакового типа,
   степени двойки. Третье число в строке — правильный ответ. */
static int self_check(void)
{
    static const struct { int v; int bits; } cases[] = {
        { 0,           0 },
        { 1,           1 },
        { -1,          32 },
        { 255,         8 },
        { 256,         1 },
        { 65535,       16 },
        { 2147483647,  31 },
        { -2147483648, 1 },
        { -256,        24 },
        { 1073741824,  1 },
    };
    char what[64];

    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        uint32_t v = (uint32_t)cases[i].v;
        snprintf(what, sizeof what, "naive(%d)", cases[i].v);
        expect_ll(what, popcount_naive(v), cases[i].bits);
        snprintf(what, sizeof what, "kernighan(%d)", cases[i].v);
        expect_ll(what, popcount_kernighan(v), cases[i].bits);
        snprintf(what, sizeof what, "table(%d)", cases[i].v);
        expect_ll(what, popcount_table(v), cases[i].bits);
    }
    return checks_total();
}

int main(int argc, char **argv)
{
    build_table();

    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    int n, bad = 0;
    while (scanf("%d", &n) == 1)
        bad |= report(n);

    return bad ? 1 : 0;
}
