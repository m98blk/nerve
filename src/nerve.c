/*
 * nerve.c - nerve main
 */

#include "conn.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Parse port number from arg */
int port_arg_to_portnum(const char *port_arg)
{
    char *endptr;
    int portnum = strtol(port_arg, &endptr, 10);
    if (*endptr != 0 || portnum > USHRT_MAX || portnum < 0)
    {
        fputs("error: invalid port number\n", stderr);
        exit(EXIT_FAILURE);
    }
    return portnum;
}

/* Print a usage string */
void usage(void)
{
    puts("usage: nerve [target_address] port\n"
         "        If target_address isn't specified, nerve will listen on the given port");
}

/* Program entry */
int main(int argc, char **argv)
{
	bool is_client_mode = argc == 3;
    if (!is_client_mode && argc != 2)
    {
        usage();
        exit(EXIT_SUCCESS);
    }

    struct conn_ctx *conn = conn_ctx_create();

	if (is_client_mode)
	{
        int portnum = port_arg_to_portnum(argv[2]);
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

    return EXIT_SUCCESS;
}

