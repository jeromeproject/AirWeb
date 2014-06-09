#include <iostream>
#include <map>
#include <string>
using namespace std;

#include <stdio.h>
#include <string.h>
#include "mongoose.h"

// This function will be called by mongoose on every new request.
static int begin_request_handler(struct mg_connection *conn) {
	const struct mg_request_info *request_info = mg_get_request_info(conn);
	char content[100];
	if (!strcmp(request_info->uri, "/UploadPic.exe"))
	{
		char path[200];
		char result[128] = "Bad request";
		char output_page[] = "HTTP/1.0 200 OK\n\n<script>function goBack() {  window.history.back()  } </script> <body> %s <button onclick='goBack()'>Go Back</button></body>";
		int num = mg_upload(conn, ".");
		if (num > 0) {
			_snprintf(result, sizeof(result), "Upload %d files", num);
		}
		else if(num == -2)
		{
			_snprintf(result, sizeof(result), "The size of upload file is too large (Max: %sKB)", mg_conn_get_config(conn, MAX_UPLOAD_PICTURE_SIZE));
		}
		else
		{
			_snprintf(result, sizeof(result), "No files upload");
		}
		mg_printf(conn, output_page, result);
		return 1;
	}
	return 0;
}

void http_upload_files(struct mg_connection *conn, const char *file_name);

int main(int argc, char *argv[]) {
  struct mg_context *ctx = NULL;
  struct mg_callbacks callbacks;

  // List of options. Last element must be NULL.
  const char *options[] = {
    "listening_ports", "8080",
    "document_root", "C:\\html",
    "max_upload_picture_size", "2048",	// unit: kb
    "authentication_domain", "AirWeb",
    "cgi_pattern", "**.cgi$|**.exe$",
    NULL
  };

  // Prepare callbacks structure. We have only one callback, the rest are NULL.
  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;
  callbacks.upload = http_upload_files;

  // Start the web server.
  ctx = mg_start(&callbacks, NULL, options);

  // Wait until user hits "enter". Server is running in separate thread.
  // Navigating to http://localhost:8080 will invoke begin_request_handler().
  getchar();

  // Stop the server.
  if(ctx) mg_stop(ctx);

  return 0;
}
