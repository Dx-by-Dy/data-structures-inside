/* Задача 3. Функции, которые отдают память.  Условие — 03-owner.md
 *
 *   make
 *   printf '5\n1 2 3 4 5\n1 3 2\n' | ./03-owner
 *   ./03-owner --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все три
 * функции пока возвращают NULL. Ваша работа — их тела; main менять не нужно.
 *
 * Над каждой функцией комментарием сказано, кто становится владельцем
 * возвращённого блока. На защите этот комментарий будут читать.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Возвращает новый блок с count элементами, начиная с позиции from.
   ВЛАДЕЛЕЦ — вызывающий. NULL, если участок за границами или нет памяти. */
static int *slice(const int *a, size_t n, size_t from, size_t count)
{
    /* TODO
     * Проверка границ без вычитания в size_t: from > n || count > n - from.
     */
    (void)a; (void)n; (void)from; (void)count;
    return NULL;
}

/* Возвращает новый блок из na + nb элементов: сначала a, потом b.
   ВЛАДЕЛЕЦ — вызывающий. */
static int *concat(const int *a, size_t na, const int *b, size_t nb)
{
    /* TODO */
    (void)a; (void)na; (void)b; (void)nb;
    return NULL;
}

/* Возвращает новый блок, в котором a повторён times раз.
   ВЛАДЕЛЕЦ — вызывающий. */
static int *repeat(const int *a, size_t n, size_t times)
{
    /* TODO
     * n * times считается в size_t и может переполниться. Проверка до
     * умножения.
     */
    (void)a; (void)n; (void)times;
    return NULL;
}

static void print_array(const char *label, const int *a, size_t n)
{
    printf("%s", label);
    if (n == 0)
        printf(" (пусто)");
    for (size_t i = 0; i < n; i++)
        printf(" %d", a[i]);
    printf("\n");
}

/* Один прогон. Все выделенные блоки освобождаются здесь же — санитайзер при
   завершении должен молчать. */
static void run(const int *a, size_t n, size_t from, size_t count, size_t times)
{
    char label[64];

    print_array("исходный:", a, n);

    int *part = slice(a, n, from, count);
    if (part == NULL) {
        printf("slice(%zu, %zu): участок за границами или нет памяти\n",
               from, count);
    } else {
        snprintf(label, sizeof label, "slice(%zu, %zu):", from, count);
        print_array(label, part, count);

        int *joined = concat(a, n, part, count);
        if (joined == NULL) {
            printf("concat: памяти нет\n");
        } else {
            print_array("concat:", joined, n + count);
            free(joined);
        }
        free(part);
    }

    int *many = repeat(a, n, times);
    if (many == NULL) {
        printf("repeat(%zu): памяти нет или переполнение\n", times);
    } else {
        snprintf(label, sizeof label, "repeat(%zu):", times);
        print_array(label, many, n * times);
        free(many);
    }

    printf("все блоки освобождены\n");
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

/* Краевые случаи. Каждый выделенный здесь блок освобождается тут же:
   санитайзер при завершении обязан молчать. */
static void self_check_body(void)
{
    static const int a5[] = { 1, 2, 3, 4, 5 };
    static const int want_slice[] = { 2, 3, 4 };
    static const int want_concat[] = { 1, 2, 3, 4, 5, 2, 3, 4 };
    static const int want_repeat[] = { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 };

    int *part = slice(a5, 5, 1, 3);
    expect_arr("slice(1, 3)", part, want_slice, 3);
    if (part != NULL) {
        int *joined = concat(a5, 5, part, 3);
        expect_arr("concat исходного с участком", joined, want_concat, 8);
        free(joined);
        free(part);
    }

    int *many = repeat(a5, 5, 2);
    expect_arr("repeat(2)", many, want_repeat, 10);
    free(many);

    /* Участок нулевой длины и участок в самом конце — законные запросы. */
    int *empty = slice(a5, 5, 0, 0);
    expect_ll("slice(0, 0) не считается ошибкой", empty != NULL, 1);
    free(empty);
    int *at_end = slice(a5, 5, 5, 0);
    expect_ll("slice(5, 0) не считается ошибкой", at_end != NULL, 1);
    free(at_end);

    /* А эти — за границами, и отличить их от нехватки памяти вызывающий
       обязан по NULL. */
    expect_ll("slice(3, 5) за границами -> NULL", slice(a5, 5, 3, 5) == NULL, 1);
    expect_ll("slice(6, 0) за границами -> NULL", slice(a5, 5, 6, 0) == NULL, 1);
    expect_ll("slice(0, 6) за границами -> NULL", slice(a5, 5, 0, 6) == NULL, 1);

    /* Повторение ноль раз даёт пустой результат, а не ошибку. */
    int *zero = repeat(a5, 5, 0);
    expect_ll("repeat(0) не считается ошибкой", zero != NULL, 1);
    free(zero);

    /* А вот здесь произведение n * times переполняет size_t. */
    expect_ll("repeat при переполнении -> NULL",
              repeat(a5, 5, (size_t)-1 / 4) == NULL, 1);

    /* Соединение с пустым участком: длина не меняется. */
    int *same = concat(a5, 5, NULL, 0);
    expect_arr("concat с пустым", same, a5, 5);
    free(same);
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

    long long got;
    if (scanf("%lld", &got) != 1 || got <= 0) {
        fprintf(stderr, "нужна положительная длина\n");
        return 1;
    }
    size_t n = (size_t)got;

    int *a = malloc(n * sizeof *a);
    if (a == NULL)
        return 1;
    for (size_t i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1) {
            fprintf(stderr, "не хватило чисел\n");
            free(a);
            return 1;
        }
    }

    long long f, c, t;
    if (scanf("%lld %lld %lld", &f, &c, &t) != 3 || f < 0 || c < 0 || t < 0) {
        fprintf(stderr, "нужны три неотрицательных числа\n");
        free(a);
        return 1;
    }

    run(a, n, (size_t)f, (size_t)c, (size_t)t);
    free(a);
    return 0;
}
