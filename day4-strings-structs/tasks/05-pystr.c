/* Задача 5. Строка со счётчиком ссылок.  Условие — 05-pystr.md
 *
 *   make
 *   printf 'n a hello\n= b a\n?\n' | ./05-pystr
 *   ./05-pystr --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но объекты пока
 * не создаются. Ваша работа — пять мест с TODO; main, разбор команд и печать
 * таблицы менять не нужно.
 *
 * Пока str_new возвращает NULL, все команды печатают «объект не создан» — так
 * и должно быть до первой правки.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

/* Заголовок и байты строки лежат в одном блоке: data — не указатель, а само
   место, начинающееся сразу за заголовком. Так же устроен объект в CPython. */
struct Str {
    size_t refcnt;   /* сколько имён ссылается на объект */
    size_t len;      /* длина в байтах, без завершающего нуля */
    long   hash;     /* -1, пока не посчитан */
    char   data[];   /* len + 1 байт следом за заголовком */
};

/* Счётчики. Увеличивать их должны ваши функции, а не main. */
static size_t stat_created = 0;   /* сколько объектов создано */
static size_t stat_freed   = 0;   /* сколько освобождено */
static size_t stat_hashed  = 0;   /* сколько раз хеш действительно считался */

/* Новый объект из обычной строки. Счётчик ссылок нового объекта равен 1,
   хеш — -1. Владелец — вызывающий. */
static struct Str *str_new(const char *text)
{
    /* TODO
     * Один блок на заголовок и байты сразу:
     *     malloc(offsetof(struct Str, data) + len + 1)
     * Скопируйте текст вместе с завершающим нулём и увеличьте stat_created.
     */
    (void)text;
    return NULL;
}

/* Ещё одна ссылка на тот же объект. Возвращает его же. */
static struct Str *str_incref(struct Str *s)
{
    /* TODO */
    return s;
}

/* Ссылок стало на одну меньше. Когда их не осталось, блок освобождается и
   увеличивается stat_freed. Вызов с NULL законен и ничего не делает. */
static void str_decref(struct Str *s)
{
    /* TODO */
    (void)s;
}

/* Новый объект из байтов a и следом b. Ни a, ни b не меняются, их счётчики
   тоже. Владелец результата — вызывающий. */
static struct Str *str_concat(const struct Str *a, const struct Str *b)
{
    /* TODO */
    (void)a; (void)b;
    return NULL;
}

/* Хеш строки. Считается при первом обращении и запоминается в поле hash;
   при следующих обращениях возвращается сохранённое значение. */
static long str_hash(struct Str *s)
{
    /* TODO
     * Алгоритм выбирайте сами, но он обязан зависеть от всех байтов строки и
     * не должен давать -1: это значение означает «ещё не посчитан».
     * stat_hashed увеличивается только тогда, когда хеш действительно
     * считался.
     */
    (void)s;
    return -1;
}

/* ── выданное готовым ──────────────────────────────────────────────────── */

/* Имена a..z, каждое либо пусто, либо ссылается на объект. */
#define NAMES 26
static struct Str *names[NAMES];

static int name_index(char c)
{
    return (c >= 'a' && c <= 'z') ? c - 'a' : -1;
}

/* Связывает имя с объектом, отпуская то, на что оно ссылалось раньше. */
static void bind(int slot, struct Str *value)
{
    struct Str *old = names[slot];
    names[slot] = value;
    str_decref(old);
}

static void print_names(void)
{
    for (int i = 0; i < NAMES; i++) {
        if (names[i] == NULL)
            continue;
        printf("  %c -> %p  refcnt %zu, len %zu, hash %s, \"%s\"\n",
               'a' + i, (void *)names[i], names[i]->refcnt, names[i]->len,
               names[i]->hash == -1 ? "не посчитан" : "посчитан",
               names[i]->data);
    }
}

static bool step(char cmd)
{
    if (cmd == 'n') {              /* n a hello — создать объект */
        char target, text[256];
        if (scanf(" %c %255s", &target, text) != 2) return false;
        int slot = name_index(target);
        if (slot < 0) { printf("неизвестное имя\n"); return true; }
        struct Str *s = str_new(text);
        if (s == NULL) { printf("объект не создан\n"); return true; }
        bind(slot, s);
        printf("n %c %s\n", target, text);
    } else if (cmd == '=') {       /* = b a — ещё одно имя того же объекта */
        char target, source;
        if (scanf(" %c %c", &target, &source) != 2) return false;
        int to = name_index(target), from = name_index(source);
        if (to < 0 || from < 0 || names[from] == NULL) {
            printf("нет такого имени\n");
            return true;
        }
        bind(to, str_incref(names[from]));
        printf("= %c %c\n", target, source);
    } else if (cmd == '+') {       /* + c a b — склейка */
        char target, left, right;
        if (scanf(" %c %c %c", &target, &left, &right) != 3) return false;
        int to = name_index(target), l = name_index(left), r = name_index(right);
        if (to < 0 || l < 0 || r < 0 || names[l] == NULL || names[r] == NULL) {
            printf("нет такого имени\n");
            return true;
        }
        struct Str *s = str_concat(names[l], names[r]);
        if (s == NULL) { printf("объект не создан\n"); return true; }
        bind(to, s);
        printf("+ %c %c %c\n", target, left, right);
    } else if (cmd == 'h') {       /* h a — хеш */
        char target;
        if (scanf(" %c", &target) != 1) return false;
        int slot = name_index(target);
        if (slot < 0 || names[slot] == NULL) {
            printf("нет такого имени\n");
            return true;
        }
        printf("h %c\n  hash = %ld\n", target, str_hash(names[slot]));
    } else if (cmd == 'd') {       /* d a — удалить имя */
        char target;
        if (scanf(" %c", &target) != 1) return false;
        int slot = name_index(target);
        if (slot < 0) { printf("неизвестное имя\n"); return true; }
        bind(slot, NULL);
        printf("d %c\n", target);
    } else if (cmd == '?') {       /* ? — таблица имён */
        printf("?\n");
        print_names();
    } else if (cmd == 's') {       /* s — статистика */
        printf("s\n");
        printf("  создано объектов: %zu\n", stat_created);
        printf("  освобождено: %zu\n", stat_freed);
        printf("  хеш считался: %zu раз\n", stat_hashed);
    } else {
        printf("неизвестная команда: %c\n", cmd);
    }
    return true;
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

static void expect_str(const char *what, const char *got, const char *want)
{
    checks_run++;
    if (got != NULL && strcmp(got, want) == 0) {
        printf("  ok      %s = \"%s\"\n", what, got);
        return;
    }
    checks_failed++;
    printf("  ОШИБКА  %s = %s%s%s, ожидалось \"%s\"\n", what,
           got == NULL ? "" : "\"", got == NULL ? "NULL" : got,
           got == NULL ? "" : "\"", want);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

static void self_check_body(void)
{
    printf("  смещение data в заголовке: %zu\n",
           offsetof(struct Str, data));

    /* Создание. */
    struct Str *a = str_new("hello");
    expect_ll("str_new вернула объект", a != NULL, 1);
    if (a == NULL) {
        printf("  дальше проверять нечего: str_new не работает\n");
        return;
    }
    expect_str("содержимое", a->data, "hello");
    expect_ll("длина", (long long)a->len, 5);
    expect_ll("счётчик ссылок нового объекта", (long long)a->refcnt, 1);
    expect_ll("хеш ещё не посчитан", a->hash, -1);
    expect_ll("завершающий ноль на месте", a->data[a->len] == '\0', 1);
    expect_ll("байты лежат сразу за заголовком",
              (char *)a + offsetof(struct Str, data) == a->data, 1);
    expect_ll("создано объектов", (long long)stat_created, 1);

    /* Пустая строка. */
    struct Str *empty = str_new("");
    expect_ll("str_new(\"\") вернула объект", empty != NULL, 1);
    expect_ll("её длина", empty == NULL ? -1 : (long long)empty->len, 0);
    expect_str("и она пуста", empty == NULL ? NULL : empty->data, "");
    str_decref(empty);

    /* Ссылки. */
    struct Str *b = str_incref(a);
    expect_ll("str_incref вернула тот же адрес", b == a, 1);
    expect_ll("счётчик после incref", (long long)a->refcnt, 2);

    size_t freed_before = stat_freed;
    str_decref(b);
    expect_ll("счётчик после первого decref", (long long)a->refcnt, 1);
    expect_ll("объект ещё жив", (long long)(stat_freed - freed_before), 0);

    /* Хеш считается один раз. */
    size_t hashed_before = stat_hashed;
    long h1 = str_hash(a);
    long h2 = str_hash(a);
    expect_ll("два вызова str_hash дали одно число", h1 == h2, 1);
    expect_ll("хеш не равен -1", h1 != -1, 1);
    expect_ll("считался ровно один раз",
              (long long)(stat_hashed - hashed_before), 1);
    expect_ll("и сохранён в поле", a->hash == h1, 1);

    struct Str *same = str_new("hello");
    expect_ll("одинаковые строки — одинаковый хеш", str_hash(same) == h1, 1);
    struct Str *other = str_new("hellp");
    expect_ll("разные строки — разный хеш (иначе хеш смотрит не на все байты)",
              str_hash(other) != h1, 1);
    str_decref(same);
    str_decref(other);

    /* Склейка. */
    struct Str *tail = str_new(" world");
    struct Str *joined = str_concat(a, tail);
    expect_ll("str_concat вернула объект", joined != NULL, 1);
    expect_str("результат склейки", joined == NULL ? NULL : joined->data,
               "hello world");
    expect_ll("его длина", joined == NULL ? -1 : (long long)joined->len, 11);
    expect_ll("его счётчик ссылок",
              joined == NULL ? -1 : (long long)joined->refcnt, 1);
    expect_ll("его хеш ещё не посчитан", joined == NULL ? 0 : joined->hash, -1);
    expect_ll("счётчик первого слагаемого не изменился",
              (long long)a->refcnt, 1);
    expect_str("первое слагаемое не изменилось", a->data, "hello");
    str_decref(joined);
    str_decref(tail);

    /* Освобождение. */
    freed_before = stat_freed;
    str_decref(a);
    expect_ll("последний decref освободил объект",
              (long long)(stat_freed - freed_before), 1);
    str_decref(NULL);
    expect_ll("str_decref(NULL) ничего не сделал",
              (long long)(stat_freed - freed_before), 1);

    /* Длинная цепочка ссылок. */
    struct Str *many = str_new("x");
    for (int i = 0; i < 1000; i++)
        str_incref(many);
    expect_ll("счётчик после тысячи incref", (long long)many->refcnt, 1001);
    for (int i = 0; i < 1000; i++)
        str_decref(many);
    expect_ll("и после тысячи decref", (long long)many->refcnt, 1);
    freed_before = stat_freed;
    str_decref(many);
    expect_ll("а этот decref был последним",
              (long long)(stat_freed - freed_before), 1);

    expect_ll("создано и освобождено поровну",
              (long long)(stat_created - stat_freed), 0);
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

    char cmd;
    while (scanf(" %c", &cmd) == 1)
        if (!step(cmd))
            break;

    /* Имена исчезают вместе с программой, и объекты вместе с ними. */
    for (int i = 0; i < NAMES; i++)
        bind(i, NULL);

    return 0;
}
