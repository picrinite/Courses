all: Homework
Homework:BBoard.o Message.o User.o Topic.o Reply.o main.o
	g++ -W -Wall -Werror -pedantic -ansi BBoard.o Message.o User.o Topic.o Reply.o main.o -o Homework
main.o:main.cpp
	g++ -c -W -Wall -Werror -pedantic -ansi main.cpp
BBoard.o:BBoard.cpp BBoard.h
	g++ -c -W -Wall -Werror -pedantic -ansi BBoard.cpp
Message.o:Message.cpp Message.h
	g++ -c -W -Wall -Werror -pedantic -ansi Message.cpp
User.o:User.cpp User.h
	g++ -c -W -Wall -Werror -pedantic -ansi User.cpp
Topic.o:Topic.cpp Topic.h
	g++ -c -W -Wall -Werror -pedantic -ansi Topic.cpp
Reply.o:Reply.cpp Reply.h
	g++ -c -W -Wall -Werror -pedantic -ansi Reply.cpp
clean:
	rm -rf *o Homework
	rm -rf *gch Homework
run:
	./Homework users1.txt data1.txt