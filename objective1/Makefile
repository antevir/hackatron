.PHONY: clean

all: msg_server_to_hack client

msg_server_to_hack: client
	gcc -g -fno-stack-protector  msg_server_to_hack.c -o msg_server_to_hack

client:
	gcc -g ../client/client.c -o client

clean:
	rm client msg_server_to_hack