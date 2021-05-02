/*
 * conn.c - connection module
 */

#include "conn.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct conn_ctx *conn_ctx_create(void)
{
    struct conn_ctx *ctx = malloc(sizeof(struct conn_ctx));
    ctx->sockfd = -1;
    memset(&ctx->sockaddr, 0, sizeof(struct sockaddr_in));
    return ctx;
}

void conn_ctx_destroy(struct conn_ctx *ctx)
{
    free(ctx);
}

void conn_initialize_ipv4(struct conn_ctx *ctx, const char *ipv4, int port)
{
    ctx->sockdomain = PF_INET;
    ctx->sockaddr.sin_family = AF_INET;
    ctx->sockaddr.sin_addr.s_addr = inet_addr(ipv4);
    ctx->sockaddr.sin_port = htons(port);
}

bool conn_connect(struct conn_ctx *ctx)
{
    ctx->sockfd = socket(ctx->sockdomain, SOCK_STREAM, IPPROTO_TCP);
	if (ctx->sockfd == -1)
    {
        perror("socket");
        return false;
    }
    int res = connect(ctx->sockfd, (struct sockaddr *)&ctx->sockaddr, sizeof(ctx->sockaddr));
    if (res != 0)
    {
        perror("connect");
        close(ctx->sockfd);
    }
    return res == 0;
}

void conn_disconnect(struct conn_ctx *ctx)
{
    if (ctx->sockfd != -1)
    {
        close(ctx->sockfd);
        ctx->sockfd = -1;
    }
}

