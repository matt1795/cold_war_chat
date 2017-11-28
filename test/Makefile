# Makefile for test programs, runs them too

CC=gcc
CFLAGS=-I../include/ -g

all: fifo_test err_test

fifo_test:
	$(CC) fifo_test.c ../source/fifo.c -o fifo_test $(CFLAGS)

err_test:
	$(CC) err_test.c ../source/fifo.c ../source/err.c fake_phy.c -o err_test $(CFLAGS)
