# src/mybash/Makefile

CC = gcc
CFLAGS = -Wall -I../../include

TARGET := mybash
BINDIR ?= ./bin
OBJDIR ?= ./obj
LINKLIBS ?= -L../../obj/mysed -L../../obj/myfile -L../../obj/mywc -L../../obj/mytrans

SRCS := main.c
OBJS := $(SRCS:.c=.o)
OBJS := $(patsubst %.o,$(OBJDIR)/%.o,$(OBJS))

TARGET_PATH := $(BINDIR)/$(TARGET)

.PHONY: all clean

all: $(TARGET_PATH)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_PATH): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LINKLIBS) -lmysed -lmyfile -lmywc -lmytrans

clean:
	@rm -rf $(OBJDIR) $(TARGET_PATH)
