/*
 * nerve.c - nerve main
 */

#include "conn.h"
#include "receive.h"
#include <histedit.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 256


// Parse port number from arg
int port_str_to_int(const char *port_arg)
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

const char *readline_prompt(EditLine *el)
{
    (void)el;
    return "> ";
}

// Print a usage string
void usage(void)
{
    puts("usage: nerve [target_address] port\n"
         "        If target_address isn't specified, nerve will listen on the given port");
}

int main(int argc, char **argv)
{
	bool is_outbound_mode = argc == 3;
    if (!is_outbound_mode && argc != 2)
    {
        usage();
        exit(EXIT_FAILURE);
    }
    int portnum = port_str_to_int(is_outbound_mode ? argv[2] : argv[1]);

    struct conn_ctx *conn = conn_ctx_create();
	if (is_outbound_mode)
	{
		conn_out_init_ipv4(conn, argv[1], portnum);
        if (!conn_out_connect(conn))
        {
            exit(EXIT_FAILURE);
        }
	}
    else
    {
        if (!conn_in_init_ipv4(conn, portnum))
        {
            exit(EXIT_FAILURE);
        }
        
        if (!conn_in_listen(conn))
        {
            exit(EXIT_FAILURE);
        }
    }
    
    reader_start(conn);
    EditLine *el = el_init("nerve", stdin, stdout, stderr);
    el_set(el, EL_PROMPT, readline_prompt);
    const char *line;
    int len;
    
    while (true)
    {
        line = el_gets(el, &len);
        if (line == NULL) break;

        int bytes = conn_write(conn, line, len + 1);
        if (bytes < 0)
        {
            perror("write");
            break;
        }
    }
    
    el_end(el);
    reader_stop();
    if (is_outbound_mode)
    {
        conn_out_disconnect(conn);
    }
    else
    {
        conn_in_disconnect(conn);
    }
    conn_ctx_destroy(conn);

    return EXIT_SUCCESS;
}

