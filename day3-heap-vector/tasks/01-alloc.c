/* Задача 1. Массив, длину которого узнали во время работы.
 * Условие — 01-alloc.md
 *
 *   make
 *   printf '5\n10 20 30 40 50\n' | ./01-alloc
 *   ./01-alloc --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но четыре
 * функции пока ничего не делают. Ваша работа — их тела; main трогать не нужно.
 *
 * Одна оговорка про незаконченный скелет: `make release` на нём выдаёт
 * -Wmaybe-uninitialized. С -O2 компилятор видит, что read_array ничего не
 * записала, и предупреждает о чтении незаполненной памяти — совершенно
 * справедливо. Как только read_array будет написана, предупреждение исчезнет.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Печатает подпись и n элементов через пробел. Готова, править не нужно. */
static void print_array(const char *label, const int *a, size_t n)
{
    printf("%s", label);
    for (size_t i = 0; i < n; i++)
        printf(" %d", a[i]);
    printf("\n");
}

/* Выделяет блок на n элементов типа int. Возвращает NULL, если выделить не
   удалось или если n таково, что размер в байтах не поместится в size_t. */
static int *alloc_array(size_t n)
{
    /* TODO
     * Ниже malloc без единой проверки. Добавьте две.
     * Первая — про переполнение: произведение считается в size_t и может
     * переполниться. Сравнивать надо ДО умножения, с SIZE_MAX.
     * Вторая — что делать при n == 0. Решение за вами, но оно должно быть
     * согласовано с тем, как main отличает пустой массив от нехватки памяти.
     */
    int *a = malloc(n * sizeof *a);
    return a;
}

/* Читает со входа n чисел в готовый блок. false, если чисел не хватило. */
static bool read_array(int *a, size_t n)
{
    /* TODO
     * Цикл и scanf.
     * Пока функция ничего не читает, и программа печатает то, что оказалось
     * в свежем блоке. Соберите её через make и посмотрите на эти числа: они
     * все одинаковые, и это тот самый мусор из демонстрации 02-uninitialized.
     */
    (void)a; (void)n;
    return true;
}

/* Сумма n элементов. Тип результата шире int намеренно. */
static long long sum(const int *a, size_t n)
{
    /* TODO */
    (void)a; (void)n;
    return 0;
}

/* Выделяет новый блок и переносит в него n элементов из a.
   Возвращает NULL, если выделить не удалось. */
static int *copy_array(const int *a, size_t n)
{
    /* TODO
     * Выделять здесь заново незачем: alloc_array уже написана.
     * Не забудьте проверить её результат и перенести элементы — сейчас
     * возвращается блок, в который никто ничего не положил.
     */
    (void)a;
    return alloc_array(n);
}

/* Один прогон: выделить, прочитать, посчитать, показать копию и алиас. */
static int run(size_t n)
{
    int *a = alloc_array(n);
    if (n > 0 && a == NULL) {
        fprintf(stderr, "память не выделена\n");
        return 1;
    }
    printf("n = %zu, запрошено байт: %zu\n", n, n * sizeof *a);

    if (!read_array(a, n)) {
        fprintf(stderr, "не хватило чисел\n");
        free(a);
        return 1;
    }
    print_array("элементы:", a, n);
    printf("сумма = %lld\n", sum(a, n));
    printf("sizeof a = %zu (это указатель, а не массив)\n\n", sizeof a);

    if (n == 0) {
        free(a);
        printf("массив пуст, показывать нечего\n");
        return 0;
    }

    /* Копия: отдельный блок с теми же числами. */
    int *copy = copy_array(a, n);
    if (copy == NULL) {
        fprintf(stderr, "память не выделена\n");
        free(a);
        return 1;
    }
    copy[0] = 999;
    printf("copy_array, затем copy[0] = 999:\n");
    print_array("  оригинал:", a, n);
    print_array("  копия:   ", copy, n);

    /* Алиас: скопирован один адрес, блок остался один. */
    int *alias = a;
    alias[n - 1] = -1;
    printf("\nint *alias = a, затем alias[%zu] = -1:\n", n - 1);
    print_array("  оригинал:", a, n);
    print_array("  алиас:   ", alias, n);

    free(copy);
    free(a);
    printf("\nосвобождено два блока\n");
    return 0;
}

/* ── самопроверка ────────────────────────────────────────────────────────
   expect_* сравнивают то, что вернули ваши функции, с тем, что должно быть.
   Ожидаемые значения здесь — часть условия.*/
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

static void expect_arr(const char *what, const int *got, const int *want, size_t n)
{
    checks_run++;
    if (got == NULL) {
        checks_failed++;
        printf("  ОШИБКА  %s: вернулся NULL\n", what);
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (got[i] == want[i])
            continue;
        checks_failed++;
        printf("  ОШИБКА  %s\n", what);
        printf("            получилось:");
        for (size_t j = 0; j < n; j++) printf(" %d", got[j]);
        printf("\n            ожидалось: ");
        for (size_t j = 0; j < n; j++) printf(" %d", want[j]);
        printf("\n");
        return;
    }
    printf("  ok      %s\n", what);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

/* Краевые случаи. Про malloc(0) стандарт не обещает ничего определённого,
   поэтому здесь проверяется не сам ответ, а то, что программа его переживает. */
static void self_check_body(void)
{
    /* Нулевая длина: годятся оба ответа, лишь бы не падало. */
    int *zero = alloc_array(0);
    printf("  (alloc_array(0) вернул %s — годится любой ответ)\n",
           zero == NULL ? "NULL" : "не NULL");
    free(zero);

    /* Произведение переполняет size_t и становится крошечным: без проверки
       malloc выдаст на это n блок в четыре байта, и запись во второй элемент
       уйдёт за его границу. Это то самое n, которое разобрано в условии. */
    size_t huge = (size_t)-1 / 4 + 2;
    int *over = alloc_array(huge);
    expect_ll("alloc_array при переполнении вернул NULL", over == NULL, 1);
    free(over);

    /* Обычная длина: блок обязан выделиться. */
    int *a = alloc_array(4);
    expect_ll("alloc_array(4) выделил блок", a != NULL, 1);
    if (a == NULL)
        return;
    for (size_t i = 0; i < 4; i++)
        a[i] = (int)(i + 1);

    expect_ll("sum 1..4", sum(a, 4), 10);
    expect_ll("sum нулевой длины", sum(a, 0), 0);

    int *c = copy_array(a, 4);
    expect_ll("copy_array выделил блок", c != NULL, 1);
    if (c != NULL) {
        static const int same[] = { 1, 2, 3, 4 };
        expect_arr("copy_array перенёс числа", c, same, 4);
        expect_ll("copy_array вернул другой блок", c != a, 1);
        c[0] = 999;
        expect_arr("после c[0] = 999 оригинал не тронут", a, same, 4);
        expect_ll("после c[0] = 999 копия изменилась", c[0], 999);
        free(c);
    }

    /* Алиас — тот же блок: запись через него видна в оригинале. */
    int *alias = a;
    alias[3] = -1;
    expect_ll("после alias[3] = -1 оригинал изменился", a[3], -1);

    free(a);
}

static int self_check(void)
{
    self_check_body();
    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    /* Длина читается в знаковый тип: в size_t отрицательное число превратится
       в огромное положительное, и проверка потеряет смысл. */
    long long got;
    if (scanf("%lld", &got) != 1 || got < 0) {
        fprintf(stderr, "нужна неотрицательная длина\n");
        return 1;
    }

    return run((size_t)got);
}
