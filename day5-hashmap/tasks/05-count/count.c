/* Задача 5. Частотный словарь.  Условие — ../05-count.md
 *
 *   make
 *   ./count < текст.txt
 *   ./count --check      встроенные краевые случаи
 *
 * Программа собирается из трёх файлов: этого, а также hashmap.c и vector.c из
 * соседнего каталога. Словарь берётся оттуда же — тот самый, который вы
 * написали в задаче 4.
 *
 * Это скелет: он собирается без предупреждений и запускается, но словарь
 * остаётся пустым. Ваша работа — три функции, помеченные TODO; печать и
 * разбор аргументов менять не нужно.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../04-hashmap/hashmap.h"

#define WORD_MAX 64
#define TOP_N    10

/* Читает следующее слово из in в buf. Словом считается непустая цепочка
   латинских букв; всё остальное — разделители. Буквы приводятся к нижнему
   регистру. Слово длиннее buf обрезается до size - 1 байта, но разделителем
   от этого не становится.
 *
 * Возвращает false, когда слов больше нет.
 */
static bool next_word(FILE *in, char *buf, size_t size)
{
    /* TODO
     * Читать посимвольно: getc(in) возвращает EOF в конце ввода.
     * Проверка «это буква» пишется руками, без <ctype.h>: коды латинских
     * букв идут подряд, и это разбиралось в четвёртом дне.
     */
    (void)in; (void)size;
    buf[0] = '\0';
    return false;
}

/* d[word] = d.get(word, 0) + 1 */
static bool add_word(struct Map *m, const char *word)
{
    /* TODO */
    (void)m; (void)word;
    return false;
}

/* Заполняет out адресами живых записей словаря, от самых частых к самым
   редким. При равных счётчиках раньше идёт тот, кто встретился раньше.
   В out места ровно на map_len(m) указателей.
 *
 * Возвращает, сколько указателей положено.
 */
static size_t collect_sorted(const struct Map *m, const struct Entry **out)
{
    /* TODO
     * Записи лежат в m->entries.data, их m->entries.len штук; у удалённых
     * key равен NULL.
     *
     * Сортировать n записей за n^2 здесь допустимо: слов в тексте на
     * порядки меньше, чем букв. Порядок при равных счётчиках задан условием,
     * и из него следует ограничение на способ сортировки.
     */
    (void)m; (void)out;
    return 0;
}

/* ── печать ─────────────────────────────────────────────────────────────── */

static void report(struct Map *m, size_t total_words)
{
    printf("всего слов:   %zu\n", total_words);
    printf("разных слов:  %zu\n", map_len(m));

    const struct Entry **sorted = malloc((map_len(m) + 1) * sizeof *sorted);
    if (sorted == NULL)
        return;

    size_t count = collect_sorted(m, sorted);
    size_t shown = count < TOP_N ? count : TOP_N;

    printf("\nсамые частые:\n");
    for (size_t i = 0; i < shown; i++)
        printf("  %4ld  %s\n", sorted[i]->value, sorted[i]->key);

    printf("\nработа словаря:\n");
    printf("  просмотрено корзин:  %zu\n", map_stat_probes);
    printf("  сравнений ключей:    %zu\n", map_stat_compares);
    printf("  перестроек таблицы:  %zu\n", map_stat_rehashes);
    printf("  корзин сейчас:       %zu\n", m->index_cap);

    free(sorted);
}

/* ── самопроверка ───────────────────────────────────────────────────────── */

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
    printf("  ОШИБКА  %s = \"%s\", ожидалось \"%s\"\n",
           what, got == NULL ? "(ничего)" : got, want);
}

/* Слова из строки, склеенные через пробел: по ней видно и разбиение, и
   порядок. */
static void words_of(const char *text, char *out, size_t size)
{
    out[0] = '\0';
    FILE *in = tmpfile();
    if (in == NULL)
        return;
    fputs(text, in);
    rewind(in);

    char word[WORD_MAX];
    size_t used = 0;
    while (next_word(in, word, sizeof word)) {
        int written = snprintf(out + used, size - used,
                               used == 0 ? "%s" : " %s", word);
        if (written < 0 || (size_t)written >= size - used)
            break;
        used += (size_t)written;
    }
    fclose(in);
}

static int self_check(void)
{
    char line[256];

    /* Разбиение на слова. */
    words_of("Hello, world!", line, sizeof line);
    expect_str("два слова через запятую", line, "hello world");
    words_of("  \n\t one   two \n", line, sizeof line);
    expect_str("разделителей может быть сколько угодно", line, "one two");
    words_of("C99 and x86-64", line, sizeof line);
    expect_str("цифры — разделители", line, "c and x");
    words_of("", line, sizeof line);
    expect_str("пустой ввод", line, "");
    words_of("123 456", line, sizeof line);
    expect_str("ввод без единой буквы", line, "");
    words_of("MiXeD", line, sizeof line);
    expect_str("регистр", line, "mixed");

    /* Подсчёт. */
    struct Map m;
    map_init(&m);
    long value = 0;

    expect_ll("add_word нового слова", add_word(&m, "cat"), 1);
    expect_ll("счётчик после первого раза", map_get(&m, "cat", &value) ? value : -1, 1);
    add_word(&m, "cat");
    add_word(&m, "cat");
    expect_ll("счётчик после третьего раза", map_get(&m, "cat", &value) ? value : -1, 3);
    expect_ll("разных слов", (long long)map_len(&m), 1);

    /* Сортировка. Счётчики подобраны так, что есть и совпадающие. */
    const char *words[] = {"a", "b", "c", "d"};
    const long counts[] = {3, 1, 3, 2};
    map_free(&m);
    map_init(&m);
    for (size_t i = 0; i < 4; i++)
        map_put(&m, words[i], counts[i]);

    const struct Entry *sorted[8];
    size_t n = collect_sorted(&m, sorted);
    expect_ll("сколько записей вернула collect_sorted", (long long)n, 4);
    if (n == 4) {
        expect_str("первое место", sorted[0]->key, "a");
        expect_str("второе место при равных счётчиках", sorted[1]->key, "c");
        expect_str("третье место", sorted[2]->key, "d");
        expect_str("последнее место", sorted[3]->key, "b");
    }

    map_del(&m, "a");
    n = collect_sorted(&m, sorted);
    expect_ll("удалённое слово не попало в список", (long long)n, 3);
    if (n == 3)
        expect_str("теперь первый", sorted[0]->key, "c");

    map_free(&m);
    map_init(&m);
    expect_ll("пустой словарь", (long long)collect_sorted(&m, sorted), 0);
    map_free(&m);

    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

int main(int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "--check") == 0)
        return self_check() ? 1 : 0;

    struct Map m = {{NULL, 0, 0}, NULL, 0, 0};
    map_init(&m);

    char word[WORD_MAX];
    size_t total = 0;
    while (next_word(stdin, word, sizeof word)) {
        total++;
        if (!add_word(&m, word)) {
            printf("не хватило памяти\n");
            break;
        }
    }

    report(&m, total);
    map_free(&m);
    return 0;
}
