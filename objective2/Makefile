.PHONY: clean login_server_to_hack client

all: login_server_to_hack client

login_server_to_hack: client
	gcc -g -fno-stack-protector -Wno-stringop-overflow login_server_to_hack.c -o login_server_to_hack

client:
	gcc -g ../client/client.c -o client

clean:
	rm client login_server_to_hack