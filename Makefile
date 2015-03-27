FIXME_SRCS = mpi_bug1.c mpi_bug2.c mpi_bug3.c mpi_bug4.c mpi_bug5.c mpi_bug6.c mpi_bug7.c
FIXME_EXEC = $(FIXME_SRCS:.c=)

CC = mpicc
CFLAGS = -O3

all: $(FIXME_EXEC) ssort

run: ssort
	mpirun -np 4 ./ssort

verify: run
	./verify.sh

clean: 
	rm -f $(FIXME_EXEC) ssort
	rm -f out.???.txt out.txt
	rm -rf *.dSYM

.PHONY: verify clean all run
