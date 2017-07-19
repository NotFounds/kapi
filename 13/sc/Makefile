sc : parser.o scanner.o main.o keywords.o constants.o symbols.o encoder.o misc.o
	gcc -o sc parser.o scanner.o main.o keywords.o constants.o symbols.o encoder.o misc.o -ll
parser.o : parser.y sc.h
	yacc -vd parser.y
	mv y.tab.c parser.c
	mv y.tab.h parser.h
	gcc -c parser.c
scanner.o : scanner.l parser.h sc.h
	lex scanner.l
	mv lex.yy.c scanner.c
	gcc -c scanner.c
main.o : main.c sc.h
	gcc -c main.c
keywords.o : keywords.c parser.h sc.h
	gcc -c keywords.c
constants.o : constants.c sc.h
	gcc -c constants.c
symbols.o : symbols.c sc.h
	gcc -c symbols.c
encoder.o : encoder.c sc.h
	gcc -c encoder.c
misc.o : misc.c sc.h
	gcc -c misc.c
clean :
	rm parser.c parser.h scanner.c y.output y.tab.h y.tab.c lex.yy.c sc *.o *.core
wc :
	wc main.c scanner.l keywords.c misc.c sc.h parser.y encoder.c constants.c symbols.c
