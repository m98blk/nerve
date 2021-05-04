/*
 * conn.h - connection module
 */

#ifndef CONN_H
#define CONN_H

#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdbool.h>


/*
 * Connection context
 */

// Connection context structure
struct conn_ctx
{
    int sockfd;
    int sockfd_listen;
    int sockdomain;
    pthread_mutex_t sockmtx;
    bool is_outbound;
    struct sockaddr_in target_addr;
    struct sockaddr_in local_addr;
};

// Create a connection context structure
struct conn_ctx *conn_ctx_create(void);
// Destroy a connection context structure
void conn_ctx_destroy(struct conn_ctx *ctx);


/*
 * Outbound connection operations
 */

// Initialize connection data for an outgoing IPv4 connection
void conn_out_init_ipv4(struct conn_ctx *ctx, const char *ipv4, int port);
// Attempt to connect to the target, return success status
bool conn_out_connect(struct conn_ctx *ctx);
// Disconnect from the target
void conn_out_disconnect(struct conn_ctx *ctx);


/*
 * Incoming connection operations
 */

// Initialize connection data for an incoming IPv4 connection
bool conn_in_init_ipv4(struct conn_ctx *ctx, int port);
// Listen for an incoming connection (blocking), return success status
bool conn_in_listen(struct conn_ctx *ctx);
// End the connection
void conn_in_disconnect(struct conn_ctx *ctx);


/*
 * General connection operations
 */

// Write n bytes of data
int conn_write(struct conn_ctx *ctx, const char *data, int n);
// Read up to n bytes into buffer, return number of bytes read
int conn_read(struct conn_ctx *ctx, char *buffer, int n);


#endif

