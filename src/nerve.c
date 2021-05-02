/*
 * nerve.c - nerve main
 */

#include "conn.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 256


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
	bool is_outbound_mode = argc == 3;
    if (!is_outbound_mode && argc != 2)
    {
        usage();
        exit(EXIT_SUCCESS);
    }

    struct conn_ctx *conn = conn_ctx_create();
    
	if (is_outbound_mode)
	{
        int portnum = port_arg_to_portnum(argv[2]);
		conn_out_init_ipv4(conn, argv[1], portnum);
        if (!conn_out_connect(conn))
        {
            exit(EXIT_FAILURE);
        }

        const char msg[] = "Hello world!\n";
        char buffer[BUFSIZE];
        strcpy(buffer, msg);
        write(conn->sockfd, buffer, sizeof(msg));

        conn_out_disconnect(conn);
	}
    else
    {
        int portnum = port_arg_to_portnum(argv[1]);
        if (!conn_in_init_ipv4(conn, portnum))
        {
            exit(EXIT_FAILURE);
        }
        
        if (!conn_in_listen(conn))
        {
            exit(EXIT_FAILURE);
        }

        char buffer[BUFSIZE];
        ssize_t len = 0;
        while ((len = read(conn->sockfd, buffer, BUFSIZE - 1)) > 0)
        {
            buffer[len] = 0;
            printf("%s", buffer);
        }

        conn_in_disconnect(conn);
    }

    conn_ctx_destroy(conn);

    return EXIT_SUCCESS;
}

