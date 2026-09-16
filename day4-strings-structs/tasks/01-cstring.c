/* Задача 1. Строковые функции своими руками.  Условие — 01-cstring.md
 *
 *   make
 *   printf 'hello\nworld\nhello\n' | ./01-cstring
 *   ./01-cstring --check      встроенные краевые случаи
 *
 * Это скелет: он собирается без предупреждений и запускается, но все пять
 * функций пока возвращают заведомо неверное. Ваша работа — их тела; main и
 * печать менять не нужно.
 *
 * <string.h> в этой задаче подключать нельзя: вы пишете то, что в нём лежит.
 */
#include <stdio.h>
#include <stdbool.h>

#define WORD_MAX 64

/* Длина строки: сколько байт до завершающего нуля, не считая его. */
static size_t my_strlen(const char *s)
{
    /* TODO */
    (void)s;
    return 0;
}

/* Копирование вместе с завершающим нулём. Места в dst достаточно. */
static void my_strcpy(char *dst, const char *src)
{
    /* TODO: не забудьте про байт, которого не видно. */
    (void)dst; (void)src;
}

/* Сравнение по кодам символов: 0 при совпадении, отрицательное, если a
   меньше b, положительное, если больше. */
static int my_strcmp(const char *a, const char *b)
{
    /* TODO: сравнивать надо unsigned char — почему, сказано в условии. */
    (void)a; (void)b;
    return 0;
}

/* Адрес первого вхождения c в строку, либо NULL, если его там нет. */
static char *my_strchr(const char *s, char c)
{
    /* TODO */
    (void)s; (void)c;
    return NULL;
}

/* Переворот строки на месте. Новой памяти не выделяется. */
static void my_reverse(char *s)
{
    /* TODO */
    (void)s;
}

/* ── работа программы ──────────────────────────────────────────────────── */

static void report(const char *word, const char *previous, bool have_previous)
{
    char copy[WORD_MAX];

    printf("%s\n", word);

    my_strcpy(copy, word);
    printf("  длина %zu, копия \"%s\"", my_strlen(word), copy);

    my_reverse(copy);
    printf(", наоборот \"%s\"\n", copy);

    char *found = my_strchr(word, 'l');
    if (found != NULL)
        printf("  'l' впервые на позиции %td\n", found - word);
    else
        printf("  'l' не встречается\n");

    if (have_previous)
        printf("  my_strcmp(\"%s\", \"%s\") = %d\n",
               word, previous, my_strcmp(word, previous));
    else
        printf("  предыдущего слова нет\n");
}

/* ── самопроверка ────────────────────────────────────────────────────────
   expect_* сравнивают то, что вернули ваши функции, с тем, что должно быть.
   Ожидаемые значения здесь — часть условия. Сравнение строк внутри проверок
   сделано своим циклом, а не вашей my_strcmp: иначе сломанная функция
   проверяла бы сама себя. */
static int checks_run = 0, checks_failed = 0;

/* Заполняет буфер мусором, оставляя его завершённым: если ваша my_strcpy не
   поставит ноль, проверка увидит мусор, а не выйдет за границу. */
static void fill_garbage(char *buffer, size_t size)
{
    for (size_t i = 0; i + 1 < size; i++)
        buffer[i] = '#';
    buffer[size - 1] = '\0';
}

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

static void expect_sign(const char *what, int got, int want_sign)
{
    int sign = (got > 0) - (got < 0);
    checks_run++;
    if (sign == want_sign) {
        printf("  ok      %s = %d (знак %+d)\n", what, got, want_sign);
        return;
    }
    checks_failed++;
    printf("  ОШИБКА  %s = %d, ожидался знак %+d\n", what, got, want_sign);
}

static void expect_str(const char *what, const char *got, const char *want)
{
    checks_run++;
    size_t i = 0;
    while (want[i] != '\0' && got[i] == want[i])
        i++;
    if (got[i] == want[i]) {
        printf("  ok      %s = \"%s\"\n", what, got);
        return;
    }
    checks_failed++;
    printf("  ОШИБКА  %s = \"%s\", ожидалось \"%s\"\n", what, got, want);
}

static int checks_total(void)
{
    printf("\nсошлось %d из %d\n", checks_run - checks_failed, checks_run);
    return checks_failed != 0;
}

static void self_check_body(void)
{
    char buffer[WORD_MAX];

    /* Длина. */
    expect_ll("my_strlen(\"\")", (long long)my_strlen(""), 0);
    expect_ll("my_strlen(\"a\")", (long long)my_strlen("a"), 1);
    expect_ll("my_strlen(\"hello\")", (long long)my_strlen("hello"), 5);
    expect_ll("my_strlen(\"привет\") в байтах",
              (long long)my_strlen("привет"), 12);

    /* Копирование. Буфер заранее заполнен мусором: если завершающий ноль не
       скопирован, это будет видно. */
    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "hello");
    expect_str("my_strcpy скопировал", buffer, "hello");
    expect_ll("и поставил ноль на место", buffer[5] == '\0', 1);

    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "");
    expect_ll("my_strcpy пустой строки", buffer[0] == '\0', 1);

    /* Сравнение. */
    expect_ll("my_strcmp(\"abc\", \"abc\")", my_strcmp("abc", "abc"), 0);
    expect_ll("my_strcmp(\"\", \"\")", my_strcmp("", ""), 0);
    expect_sign("my_strcmp(\"abc\", \"abd\")", my_strcmp("abc", "abd"), -1);
    expect_sign("my_strcmp(\"abd\", \"abc\")", my_strcmp("abd", "abc"), +1);
    expect_sign("my_strcmp(\"abc\", \"abcd\")", my_strcmp("abc", "abcd"), -1);
    expect_sign("my_strcmp(\"abcd\", \"abc\")", my_strcmp("abcd", "abc"), +1);
    expect_sign("my_strcmp(\"\", \"a\")", my_strcmp("", "a"), -1);
    expect_sign("my_strcmp(\"Zoo\", \"abc\")", my_strcmp("Zoo", "abc"), -1);
    /* Байт со старшим установленным битом: в char он отрицателен, и сравнение
       без приведения к unsigned char даст здесь неверный знак. */
    expect_sign("my_strcmp(\"я\", \"a\")", my_strcmp("я", "a"), +1);

    /* Поиск символа. */
    const char *text = "hello";
    expect_ll("my_strchr(\"hello\", 'l') — позиция",
              my_strchr(text, 'l') == NULL ? -1 : my_strchr(text, 'l') - text, 2);
    expect_ll("my_strchr(\"hello\", 'h') — позиция",
              my_strchr(text, 'h') == NULL ? -1 : my_strchr(text, 'h') - text, 0);
    expect_ll("my_strchr(\"hello\", 'z') == NULL", my_strchr(text, 'z') == NULL, 1);
    expect_ll("my_strchr(\"hello\", '\\0') — позиция нуля",
              my_strchr(text, '\0') == NULL ? -1 : my_strchr(text, '\0') - text, 5);

    /* Переворот. Строка кладётся в буфер вашей же my_strcpy: пока она не
       работает, эти пять проверок не сойдутся вместе с ней. */
    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "hello");   my_reverse(buffer);
    expect_str("my_reverse(\"hello\")", buffer, "olleh");
    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "abcd");    my_reverse(buffer);
    expect_str("my_reverse(\"abcd\")", buffer, "dcba");
    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "a");       my_reverse(buffer);
    expect_str("my_reverse(\"a\")", buffer, "a");
    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "");        my_reverse(buffer);
    expect_ll("my_reverse(\"\") оставила пустую строку", buffer[0] == '\0', 1);
    fill_garbage(buffer, WORD_MAX);
    my_strcpy(buffer, "hello");   my_reverse(buffer);   my_reverse(buffer);
    expect_str("два переворота подряд", buffer, "hello");
}

static int self_check(void)
{
    self_check_body();
    return checks_total();
}

int main(int argc, char **argv)
{
    if (argc > 1) {
        /* Сравнение без <string.h>: аргумент проверяется посимвольно. */
        const char *flag = "--check";
        size_t i = 0;
        while (flag[i] != '\0' && argv[1][i] == flag[i])
            i++;
        if (flag[i] == argv[1][i])
            return self_check() ? 1 : 0;
    }

    char word[WORD_MAX];
    char previous[WORD_MAX];
    bool have_previous = false;

    while (scanf("%63s", word) == 1) {
        report(word, previous, have_previous);
        my_strcpy(previous, word);
        have_previous = true;
    }

    return 0;
}
