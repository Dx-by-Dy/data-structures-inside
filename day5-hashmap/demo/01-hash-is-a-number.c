/* Демо 1. Хеш — это число, посчитанное по байтам строки.
 *
 *   gcc -std=c11 -Wall -Wextra 01-hash-is-a-number.c -o 01-hash-is-a-number
 *   ./01-hash-is-a-number
 *
 * До запуска: какие две строки из списка обязаны дать одно и то же число?
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>
#include <stdint.h>

/* FNV-1a, 32 бита. Два числа-константы взяты из описания алгоритма. */
static uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    for (const unsigned char *p = (const unsigned char *)s; *p != '\0'; p++) {
        h ^= *p;
        h *= 16777619u;
    }
    return h;
}

static int bits_set(uint32_t v)
{
    int n = 0;
    for (; v != 0; v >>= 1)
        n += (int)(v & 1u);
    return n;
}

int main(void)
{
    const char *words[] = {"", "a", "b", "c", "hello", "hello", "Hello", "hell"};
    const size_t count = sizeof words / sizeof words[0];

    printf("  key              hash   hex\n");
    for (size_t i = 0; i < count; i++)
        printf("  %-10s %10u   0x%08X\n", words[i], fnv1a(words[i]), fnv1a(words[i]));

    /* Строка из русских букв — те же байты, никакого особого случая. */
    printf("  %s     %10u   0x%08X\n", "привет", fnv1a("привет"), fnv1a("привет"));

    printf("\nсоседние строки:\n");
    printf("  fnv1a(\"a\") ^ fnv1a(\"b\") = 0x%08X, различается бит: %d из 32\n",
           fnv1a("a") ^ fnv1a("b"), bits_set(fnv1a("a") ^ fnv1a("b")));
    printf("  fnv1a(\"hello\") ^ fnv1a(\"hellp\") = 0x%08X, различается бит: %d из 32\n",
           fnv1a("hello") ^ fnv1a("hellp"), bits_set(fnv1a("hello") ^ fnv1a("hellp")));

    printf("\nодна и та же строка, посчитанная дважды: %u и %u\n",
           fnv1a("hello"), fnv1a("hello"));

    return 0;
}
