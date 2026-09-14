# Общие правила сборки для всех задач курса.
#
# Режим 1 — «каждый .c это отдельная программа».
#   Makefile в каталоге задач состоит из одной строки:
#       include ../../common/task.mk
#   Тогда `make` соберёт 01-binary.c в ./01-binary, 02-popcount.c в ./02-popcount
#   и так далее. `make 01-binary` соберёт только одну.
#
# Режим 2 — «много файлов, одна программа».
#   Задайте BIN и SRC до include:
#       BIN := hashmap
#       SRC := main.c vector.c hashmap.c
#       include ../../common/task.mk
#
# Цели:
#   make          собрать с санитайзерами — так вы работаете каждый день
#   make release  собрать с -O2 и без санитайзеров — только для замеров времени
#   make clean

CC   := gcc
CSTD := -std=c11

# -Wall -Wextra    без них C молча разрешает писать ерунду
# -Wshadow         переменная, затенившая другую с тем же именем
# -Wvla            массив переменной длины на стеке (на курсе запрещён)
# -Wpointer-arith  арифметика над void *, которая в C формально вне стандарта
WARN := -Wall -Wextra -Wshadow -Wvla -Wpointer-arith

# ASan ловит выход за границы и работу с освобождённой памятью.
# UBSan ловит переполнение, кривые сдвиги, деление на ноль.
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer

CFLAGS_DEBUG   := $(CSTD) $(WARN) -g -O0 $(SAN)
CFLAGS_RELEASE := $(CSTD) $(WARN) -O2 -DNDEBUG

.PHONY: all release clean help

ifdef SRC
# ---- режим 2: одна программа из нескольких файлов ----
BIN ?= main

all: $(BIN)

$(BIN): $(SRC) $(wildcard *.h)
	$(CC) $(CFLAGS_DEBUG) -o $@ $(SRC) $(SAN) $(LDLIBS)

release: $(SRC) $(wildcard *.h)
	$(CC) $(CFLAGS_RELEASE) -o $(BIN)-release $(SRC) $(LDLIBS)

clean:
	rm -f $(BIN) $(BIN)-release

else
# ---- режим 1: каждый .c — отдельная программа ----
SOURCES := $(wildcard *.c)
TARGETS := $(basename $(SOURCES))

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS_DEBUG) -o $@ $< $(SAN) $(LDLIBS)

release: $(SOURCES)
	@for f in $(SOURCES); do \
	  echo "  CC(release) $$f"; \
	  $(CC) $(CFLAGS_RELEASE) -o $${f%.c}-release $$f $(LDLIBS); \
	done

clean:
	rm -f $(TARGETS) $(addsuffix -release,$(TARGETS))

endif

help:
	@echo "make          — собрать с санитайзерами"
	@echo "make release  — собрать с -O2, без санитайзеров"
	@echo "make clean    — удалить собранное"
