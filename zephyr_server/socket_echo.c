/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifndef __ZEPHYR__

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/__assert.h>

#else

#include <zephyr/net/socket.h>
#include <zephyr/kernel.h>

#endif

#include <zephyr/sys/reboot.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_ctrl.h>

#define PORT 4242

typedef struct {
	uint16_t magic_value;
	uint16_t payload_length;
} __packed protocol_header_t;
static int gClient;

#define GRN "\e[0;32m"
#define CRESET "\e[0m"
#define MSG "Congrats, you managed to modify the return address!\n"

__attribute__ ((section (".fixed_func.A")))
void hacker_message(char *pMsg)
{
	printf(GRN MSG CRESET);
	send(gClient, MSG, strlen(MSG), 0);
	while (1) {
		k_sleep(K_MSEC(1000));
	}
}

__attribute__((optimize("-O0"),section(".fixed_func.B"),noinline))
int receive_header(int client)
{
	protocol_header_t header;
	int len = recv(client, &header, sizeof(header), 0);
	if (len != sizeof(header)) {
		printf("Invalid header length: %d\n", len);
		return -1;
	}
	if (header.magic_value != 0x1234) {
		printf("Invalid header magic: %d\n", header.magic_value);
		return -1;
	}
	return header.payload_length;
}

void socket_handler(int client)
{
	char buf[64];
	int payload_len = receive_header(client);
	if (payload_len > 0) {
		int len = recv(client, &buf, payload_len, 0);
		printf("Received %d bytes for buffer: %p\n", len, buf);
	}
}

int main(void)
{
	int serv;
	struct sockaddr_in bind_addr;
	static int counter;

	serv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serv < 0) {
		printf("error: socket: %d\n", errno);
		exit(1);
	}

	bind_addr.sin_family = AF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind_addr.sin_port = htons(PORT);

	if (bind(serv, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0) {
		printf("error: bind: %d\n", errno);
		exit(1);
	}

	if (listen(serv, 5) < 0) {
		printf("error: listen: %d\n", errno);
		exit(1);
	}

	printf("Single-threaded TCP echo server waits for a connection on port %d...\n", PORT);

	volatile bool loop = 1;
	while (loop) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		char addr_str[32];
		int client = accept(serv, (struct sockaddr *)&client_addr,
				    &client_addr_len);

		if (client < 0) {
			printf("error: accept: %d\n", errno);
			continue;
		}

		inet_ntop(client_addr.sin_family, &client_addr.sin_addr,
			  addr_str, sizeof(addr_str));
		printf("Connection #%d from %s\n", counter++, addr_str);

		gClient = client;

		socket_handler(client);

		close(client);
		printf("Connection from %s closed\n", addr_str);
	}

	hacker_message("foo");
}


void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf)
{
	LOG_PANIC();
	sys_reboot(SYS_REBOOT_WARM);
}
