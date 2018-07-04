include ../Make.defines

PROGS =	tcpserv01 \
	tcpcli01 \
	tcpserv02 \
	tcpcli02 \
	tcpcli03

all:	${PROGS}

tcpserv01:	tcpserv01.o
		${CC} ${CFLAGS} -o $@ tcpserv01.o ${LIBS}

tcpserv02:	tcpserv02.o
		${CC} ${CFLAGS} -o $@ tcpserv02.o ${LIBS}

tcpcli01:	tcpcli01.o
		${CC} ${CFLAGS} -o $@ tcpcli01.o ${LIBS}

tcpcli02:	tcpcli02.o
		${CC} ${CFLAGS} -o $@ tcpcli02.o ${LIBS}

tcpcli03:	tcpcli03.o
		${CC} ${CFLAGS} -o $@ tcpcli03.o ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}
