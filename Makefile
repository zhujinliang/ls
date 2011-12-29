ls : ls.o libmyList.a
	cc -L ../lib -lmyList -o ls ls.o
ls.o : ls.c
	cc -c ls.c

.PHONY : clean
clean:
	rm ls *.o

