CFLAGS = -Wall -Wextra -pedantic -Iinclude
LDFLAGS = -Wall -Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all: LinkChat

LinkChat: main.o menu.o p2p.o server.o login.o networkmanager.o button.o textbox.o textfield.o
	@echo "Linking and building final project"
	gcc -o LinkChat main.o menu.o p2p.o server.o login.o networkmanager.o button.o textbox.o textfield.o $(LDFLAGS)

main.o: main.c
	@echo "Building main.o"
	gcc $(CFLAGS) -c main.c

menu.o: menu.c
	@echo "Building menu.o"
	gcc $(CFLAGS) -c menu.c

p2p.o: p2p.c
	@echo "Building p2p.o"
	gcc $(CFLAGS) -c p2p.c

server.o: server.c
	@echo "Building server.o"
	gcc $(CFLAGS) -c server.c

login.o: login.c
	@echo "Building login.o"
	gcc $(CFLAGS) -c login.c

networkmanager.o: networkmanager.c
	@echo "Building networkmanager.o"
	gcc $(CFLAGS) -c networkmanager.c

button.o: button.c
	@echo "Building button.o"
	gcc $(CFLAGS) -c button.c

textbox.o: textbox.c
	@echo "Building textbox.o"
	gcc $(CFLAGS) -c textbox.c

textfield.o: textfield.c
	@echo "Building textfield.o"
	gcc $(CFLAGS) -c textfield.c

clean:
	@echo "recoving everything but source files"
	rm main.o menu.o button.o p2p.o server.o login.o networkmanager.o textbox.o textfield.o LinkChat
