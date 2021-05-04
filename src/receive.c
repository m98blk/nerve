/*
 * receive.c - reading and handling of incoming messages on the socket
 */

#include "receive.h"
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>

#define POLL_TIMEOUT_MS 1000
#define READ_BUF_SIZE 1024


/*
 * Statics
 */

// reader thread
static pthread_t g_thread;
// connection context structure
static struct conn_ctx *g_conn = NULL;
// true if reader should shut down
static bool g_shutdown = false;
// read buffer 
static char g_readbuf[READ_BUF_SIZE];


// Reader thread entry function
static void *reader_thread_entry(void *args)
{
    (void)args;
    const char *peername = inet_ntoa(g_conn->target_addr.sin_addr);
    struct pollfd pfd = { .fd = g_conn->sockfd, .events = POLLIN, .revents = 0 };
    while (!g_shutdown)
    {
        int p = poll(&pfd, 1, POLL_TIMEOUT_MS);
        if (pfd.revents & POLLHUP)
        {
            puts("peer disconnected");
            break;
        }
        if (p > 0)
        {
            int bytes = conn_read(g_conn, g_readbuf, READ_BUF_SIZE);
            if (bytes > 0) printf("%s says: %s", peername, g_readbuf);
            else if (bytes < 0)
            {
                perror("read");
                break;
            }
        }
        else if (p < 0)
        {
            perror("poll");
        }
    }
    return NULL;
}


void reader_start(struct conn_ctx *conn)
{
    g_shutdown = false;
    g_conn = conn;
    pthread_create(&g_thread, NULL, reader_thread_entry, NULL);
}

void reader_stop(void)
{
    g_shutdown = true;
    pthread_join(g_thread, NULL);
}
