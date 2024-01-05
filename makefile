run: hangman
	@./hangman
compile hangman: main.o hangman.o
	@gcc -o hangman main.o hangman.o
main.o: main.c hangman.h
	@gcc -c main.c
hangman.o: hangman.c
	@gcc -c hangman.c
clean:
	rm hangman
	rm *.o