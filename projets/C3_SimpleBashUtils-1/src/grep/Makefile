FLAGS = -std=c11 -Wall -Werror -Wextra
GREP = s21_grep
TFILES = *.txt

all:
	$(MAKE) $(GREP)

clean:
	rm -rf *.o
	rm -rf $(GREP)

rebuild: 
	$(MAKE) clean
	$(MAKE) $(GREP)

s21_grep:
	gcc -c $(FLAGS) grep.c
	gcc *.o -o $(GREP)
	rm -rf *.o

clang:
	clang-format -n *.c
	clang-format -n *.h
	clang-format -i *.c
	clang-format -i *.h

tests:
	$(MAKE) rebuild
	sh test_grep.sh
