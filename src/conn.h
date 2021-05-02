/*
 * conn.h - connection module
 */

#ifndef CONN_H
#define CONN_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
	
	
/*
 * Connection context
 */

/* Connection context structure */
struct conn_ctx
{
    int sockfd;
    int sockdomain;
    struct sockaddr_in sockaddr;
};

/* Create a connection context structure */
struct conn_ctx *conn_ctx_create(void);
/* Destroy a connection context structure */
void conn_ctx_destroy(struct conn_ctx *ctx);


/*
 * Connection operations
 */

/* Initialize connection data to the target IPv4 host and port */
void conn_initialize_ipv4(struct conn_ctx *ctx, const char *ipv4, int port);
/* Attempt to connect to the target, return success status */
bool conn_connect(struct conn_ctx *ctx);
/* Disconnect from the target */
void conn_disconnect(struct conn_ctx *ctx);


#endif

