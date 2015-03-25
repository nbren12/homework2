FIXME_SRCS = mpi_bug1.c mpi_bug2.c mpi_bug3.c mpi_bug4.c mpi_bug5.c mpi_bug6.c mpi_bug7.c
FIXME_EXEC = $(FIXME_SRCS:.c=)


all : $(FIXME_EXEC)

% : %.c
	mpicc -o $@ $^
