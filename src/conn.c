/*
 * conn.c - connection module
 */

#include "conn.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct conn_ctx *conn_ctx_create(void)
{
    struct conn_ctx *ctx = malloc(sizeof(struct conn_ctx));
    ctx->sockfd = -1;
    ctx->sockfd_listen = -1;
    memset(&ctx->target_addr, 0, sizeof(struct sockaddr_in));
    memset(&ctx->local_addr, 0, sizeof(struct sockaddr_in));
    return ctx;
}

void conn_ctx_destroy(struct conn_ctx *ctx)
{
    if (ctx->sockfd != -1)
    {
        close(ctx->sockfd);
    }
    
    free(ctx);
}

void conn_out_init_ipv4(struct conn_ctx *ctx, const char *ipv4, int port)
{
    ctx->sockdomain = PF_INET;
    ctx->is_outbound = true;
    ctx->target_addr.sin_family = AF_INET;
    ctx->target_addr.sin_addr.s_addr = inet_addr(ipv4);
    ctx->target_addr.sin_port = htons(port);
}

bool conn_out_connect(struct conn_ctx *ctx)
{
    ctx->sockfd = socket(ctx->sockdomain, SOCK_STREAM, IPPROTO_TCP);
	if (ctx->sockfd == -1)
    {
        perror("socket");
        return false;
    }
    int res = connect(ctx->sockfd, (struct sockaddr *)&ctx->target_addr, sizeof(ctx->target_addr));
    if (res != 0)
    {
        perror("connect");
        close(ctx->sockfd);
    }
    return res == 0;
}

void conn_out_disconnect(struct conn_ctx *ctx)
{
    if (ctx->sockfd != -1)
    {
        close(ctx->sockfd);
        ctx->sockfd = -1;
    }
}

bool conn_in_init_ipv4(struct conn_ctx *ctx, int port)
{
    ctx->sockdomain = PF_INET;
    ctx->is_outbound = false;
    
    ctx->local_addr.sin_family = AF_INET;
    ctx->local_addr.sin_port = htons(port);
    
    ctx->sockfd_listen = socket(ctx->sockdomain, SOCK_STREAM, IPPROTO_TCP);
    if (ctx->sockfd_listen < 0)
    {
        perror("socket");
        return false;
    }
    
    if (bind(ctx->sockfd_listen, (struct sockaddr *)&ctx->local_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("bind");
        close(ctx->sockfd_listen);
        return false;
    }

    return true;
}

bool conn_in_listen(struct conn_ctx *ctx)
{
    if (listen(ctx->sockfd_listen, 1) < 0)
    {
        perror("listen");
        return false;
    }

    ctx->sockfd = accept(ctx->sockfd_listen, NULL, NULL);
    if (ctx->sockfd < 0)
    {
        perror("accept");
        return false;
    }

    return true;
}

void conn_in_disconnect(struct conn_ctx *ctx)
{
    if (ctx->sockfd != -1)
    {
        close(ctx->sockfd);
        ctx->sockfd = -1;
    }
}

