#include <stdio.h>
#include <string.h>
#include "mongoose.h"

// This function will be called by mongoose on every new request.
static int begin_request_handler(struct mg_connection *conn) {
  const struct mg_request_info *request_info = mg_get_request_info(conn);
  char content[100];
  return 0;
}

int main(void) {
  struct mg_context *ctx = NULL;
  struct mg_callbacks callbacks;

  // List of options. Last element must be NULL.
  const char *options[] = {
    "listening_ports", "8080",
    "document_root", "C:\\html",
    "authentication_domain", "airphone",
    "cgi_pattern", "**.cgi$|**.exe$",
    NULL
  };

  // Prepare callbacks structure. We have only one callback, the rest are NULL.
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;

  // Start the web server.
  ctx = mg_start(&callbacks, NULL, options);

  // Wait until user hits "enter". Server is running in separate thread.
  // Navigating to http://localhost:8080 will invoke begin_request_handler().
  getchar();

  // Stop the server.
  if(ctx) mg_stop(ctx);

  return 0;
}
