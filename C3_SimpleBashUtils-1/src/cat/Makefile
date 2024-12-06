FLAGS = -std=c11 -Wall -Werror -Wextra
CAT = s21_cat

all:
	$(MAKE) $(CAT)

clean:
	rm -rf *.o
	rm -rf $(CAT)

rebuild: 
	$(MAKE) clean
	$(MAKE) $(CAT)

s21_cat:
	gcc -c $(FLAGS) cat.c
	gcc *.o -o $(CAT)
	rm -rf *.o

clang:
	clang-format -n *.c
	clang-format -n *.h
	clang-format -i *.c
	clang-format -i *.h

tests:
	$(MAKE) rebuild
	sh tests.shFLAGS = -std=c11 -Wall -Werror -Wextra
