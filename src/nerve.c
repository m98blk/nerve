/*
 * nerve.c - nerve main
 */

#include "conn.h"
#include <stdio.h>

int main(void)
{
	struct conn_ctx *conn = conn_ctx_create();

	puts("Hello world!");

	conn_ctx_destroy(conn);

	return 0;
}
