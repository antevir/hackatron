.PHONY: clean

all: login_server_to_hack client

msg_server_to_hack: client
	gcc -g -fno-stack-protector  login_server_to_hack.c -o login_server_to_hack

client:
	gcc -g ../client/client.c -o client

clean:
	rm client login_server_to_hack