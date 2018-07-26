CC	= g++

BOOSTDIR = /home/software/boost_1_55_0
OPT_FLAGS  = -O3
CFLAGS	= -Wall $(DBG_FLAGS) $(OPT_FLAGS) -I$(BOOSTDIR) -fopenmp -DNDEBUG
LIBS	= -lm

SRCS0	= main.c unixtimer.c err.c dist.c mst1.c\
          neighbors.c heap.c mst2.c\
          sd2.c mst8.c
OBJS0	= $(SRCS0:.c=.o)
MAIN0	= rmst

SRCS1	= gen.c
OBJS1	= $(SRCS1:.c=.o)
MAIN1	= gen

SRCS3	= main.cpp mymeasure.cpp argument.cpp mystring.cpp graph.cpp
OBJS3	= $(SRCS3:.c=.o)
MAIN3	= pd_rev

pd_rev:	${OBJS3}
	$(CC) -g ${CFLAGS} -o ${MAIN3} ${OBJS3} $(LIBS) &> make_op.log

pd_rev_heap:	${OBJS2}
	$(CC) ${CFLAGS} -o ${MAIN2} ${OBJS2} $(LIBS)

rmst: 	${OBJS0}
	$(CC) ${CFLAGS} -o ${MAIN0} ${OBJS0} $(LIBS)

gen: 	${OBJS1}
	$(CC) ${CFLAGS} -o ${MAIN1} ${OBJS1} $(LIBS)

new:    clean rmst gen

clean:
	rm -f *.o
	rm -f ${MAIN0}
	rm -f ${MAIN1}
	rm -f core

