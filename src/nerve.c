/*
 * nerve.c - nerve main
 */

#include "conn.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	bool is_client_mode = argc == 3;
    puts("Hello world!");

    struct conn_ctx *conn = conn_ctx_create();

	if (is_client_mode)
	{
        char *endptr;
        int portnum = strtol(argv[2], &endptr, 10);
        if (*endptr != 0)
        {
            fputs("Invalid port number", stderr);
            exit(EXIT_FAILURE);
        }

		conn_initialize_ipv4(conn, argv[1], portnum);
        if (!conn_connect(conn))
        {
            exit(EXIT_FAILURE);
        }

        char buffer[128] = "Test message!\n";
        write(conn->sockfd, buffer, 14);

        conn_disconnect(conn);
	}

    conn_ctx_destroy(conn);

    return 0;
}

