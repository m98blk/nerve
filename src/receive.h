/*
 * receive.h - reading and handling of incoming messages on the socket
 */

#ifndef RECEIVE_H
#define RECEIVE_H

#include "conn.h"


// Start the reader thread
void reader_start(struct conn_ctx *ctx);
// Stop the reader thread
void reader_stop(void);


#endif
