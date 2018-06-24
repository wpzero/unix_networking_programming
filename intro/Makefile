include ../Make.defines

PROGS =	seq_daytimesrv

all:	${PROGS}

seq_daytimesrv:	seq_daytimesrv.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
