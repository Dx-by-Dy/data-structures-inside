/* Демо 6. Запись в строковый литерал.
 *
 *   gcc -std=c11 -Wall -Wextra 06-literal-write.c -o 06-literal-write
 *   ./06-literal-write
 *   echo $?
 *
 * До запуска: обе строки состоят из одних и тех же букв, и обе меняют первую
 * из них на 'H'. Предскажите, чем закончится программа.
 *
 * Остальные вопросы — в README.md.
 */
#include <stdio.h>

int main(void)
{
    char  array[] = "hello";   /* массив, заполненный буквами литерала */
    char *literal = "hello";   /* указатель на сам литерал */

    printf("array   = %s, адрес %p\n", array, (void *)array);
    printf("literal = %s, адрес %p\n\n", literal, (void *)literal);

    array[0] = 'H';
    printf("array после записи: %s\n\n", array);

    printf("сейчас будет запись по адресу %p\n", (void *)literal);
    fflush(stdout);   /* иначе строка потеряется при падении */

    literal[0] = 'H';

    printf("literal после записи: %s\n", literal);
    return 0;
}
