run: hangman
	@./hangman
compile hangman:hangman.o
	@gcc -o hangman hangman.o
hangman.o: hangman.c
	@gcc -c hangman.c
clean:
	rm hangman
	rm *.o