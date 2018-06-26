include ../Make.defines

PROGS =	seq_daytimesrv \
	seq_daytimesrvconcurrent \
	daytimetcpcli

all:	${PROGS}

seq_daytimesrv:	seq_daytimesrv.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}
seq_daytimesrvconcurrent: seq_daytimesrvconcurrent.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}
daytimetcpcli: daytimetcpcli.o
		${CC} ${CFLAGS} -o $@ $< ${LIBS}
clean:
		rm -f ${PROGS} ${CLEANFILES}
