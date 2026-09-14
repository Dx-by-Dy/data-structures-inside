/* Задача 5. Список ссылок.  Условие — 05-refs.md
 *
 *   make
 *   printf '10 20 30 40 50\n' | ./05-refs
 *   ./05-refs --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все три
 * функции пока ничего не делают. Ваша работа — их тела; main менять не нужно.
 *
 * Устройство такое. Сами числа лежат в обычном массиве int values[CAP].
 * «Список» — это отдельный массив int *original[CAP], в котором лежат адреса
 * элементов values.
 *
 * Одна оговорка про сборку. Флагами курса (make) файл собирается без
 * предупреждений, а `make release` выдаёт -Wmaybe-uninitialized на original:
 * с -O2 компилятор не может доказать, что print_list читает только первые n
 * элементов, хотя заполнены именно они. Предупреждение остаётся и на готовом
 * решении, чинить его не нужно.
 */
#include <stdio.h>
#include <string.h>

#define CAP 8

/* Печатает подпись и n чисел, разыменовывая каждый элемент списка. */
static void print_list(const char *label, int *const *list, size_t n)
{
    /* TODO
     * Элемент списка — это адрес. Напечатать надо то, что лежит по нему.
     */
    (void)list; (void)n;
    printf("%s\n", label);
}

/* Копирует n адресов из src в dst. Сами числа не трогает. */
static void copy_pointers(int **dst, int *const *src, size_t n)
{
    /* TODO */
    (void)dst; (void)src; (void)n;
}

/* Копирует n чисел в storage, а в dst кладёт адреса копий. */
static void copy_values(int **dst, int *const *src, size_t n, int *storage)
{
    /* TODO */
    (void)dst; (void)src; (void)n; (void)storage;
}

/* Один прогон: строит список ссылок на values и показывает разницу между
   копированием адресов и копированием значений. */
static void run(int *values, size_t n)
{
    int *original[CAP];
    int *shallow[CAP];
    int *deep[CAP];
    int  storage[CAP];

    for (size_t i = 0; i < n; i++)
        original[i] = &values[i];

    print_list("original: ", original, n);
    printf("sizeof original    = %zu (весь массив, %zu ячеек)\n",
           sizeof original, sizeof original / sizeof original[0]);
    printf("sizeof original[0] = %zu (одна ячейка — это указатель)\n",
           sizeof original[0]);

    if (n == 0) {
        printf("\nсписок пуст, показывать нечего\n");
        return;
    }

    copy_pointers(shallow, original, n);
    if (shallow[0] != NULL)
        *shallow[0] = 999;
    printf("\nпосле copy_pointers и *shallow[0] = 999:\n");
    print_list("  original:", original, n);
    print_list("  shallow: ", shallow, n);

    copy_values(deep, original, n, storage);
    size_t j = (n > 1) ? 1 : 0;
    if (deep[j] != NULL)
        *deep[j] = 777;
    printf("\nпосле copy_values и *deep[%zu] = 777:\n", j);
    print_list("  original:", original, n);
    print_list("  deep:    ", deep, n);
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

/* Краевые случаи. Главное различие двух функций видно по одному числу: после
   записи через shallow исходное значение меняется, после записи через deep —
   нет. */
static int self_check(void)
{
    int values[CAP] = { 10, 20, 30, 40, 50 };
    int *original[CAP];
    int *shallow[CAP];
    int *deep[CAP];
    int  storage[CAP];

    /* Обнуляются, чтобы незаполненный скелет печатал ОШИБКА, а не падал. */
    for (size_t i = 0; i < CAP; i++)
        original[i] = shallow[i] = deep[i] = NULL;
    for (size_t i = 0; i < 5; i++)
        original[i] = &values[i];

    /* Список ссылок собран правильно: каждый элемент — адрес своего числа. */
    expect_ll("*original[0]", *original[0], 10);
    expect_ll("*original[4]", *original[4], 50);

    copy_pointers(shallow, original, 5);
    expect_ll("shallow[0] — тот же адрес", shallow[0] == original[0], 1);
    expect_ll("shallow[3] — тот же адрес", shallow[3] == original[3], 1);

    if (shallow[0] != NULL)
        *shallow[0] = 999;
    expect_ll("после *shallow[0] = 999: values[0]", values[0], 999);
    expect_ll("после *shallow[0] = 999: *original[0]", *original[0], 999);

    copy_values(deep, original, 5, storage);
    expect_ll("deep[1] — другой адрес", deep[1] != original[1], 1);
    expect_ll("deep[1] показывает в storage", deep[1] == &storage[1], 1);
    if (deep[1] == NULL) {
        expect_ll("deep[1] заполнен", 0, 1);
        return checks_total();
    }
    expect_ll("*deep[1] до записи", *deep[1], 20);

    *deep[1] = 777;
    expect_ll("после *deep[1] = 777: *deep[1]", *deep[1], 777);
    expect_ll("после *deep[1] = 777: values[1] не тронут", values[1], 20);
    expect_ll("после *deep[1] = 777: *original[1] не тронут", *original[1], 20);

    /* Остальные значения глубокая копия перенесла как есть. */
    expect_ll("*deep[0]", *deep[0], 999);
    expect_ll("*deep[4]", *deep[4], 50);

    /* Пустой список: обе функции обязаны отработать и ничего не тронуть. */
    int guard[CAP];
    for (size_t i = 0; i < CAP; i++)
        guard[i] = 12345;
    int *dst_empty[CAP];
    for (size_t i = 0; i < CAP; i++)
        dst_empty[i] = NULL;
    copy_pointers(dst_empty, original, 0);
    copy_values(dst_empty, original, 0, guard);
    expect_ll("copy_* с n == 0 не тронули storage", guard[0], 12345);
    expect_ll("copy_* с n == 0 не тронули dst", dst_empty[0] == NULL, 1);

    /* sizeof массива указателей не зависит от того, сколько чисел введено. */
    expect_ll("sizeof original", (long long)sizeof original, (long long)(CAP * 8));
    expect_ll("sizeof original[0]", (long long)sizeof original[0], 8);

    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    int values[CAP];
    size_t n = 0;
    int v;
    while (n < CAP && scanf("%d", &v) == 1)
        values[n++] = v;

    run(values, n);
    return 0;
}
