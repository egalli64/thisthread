/**
 * mod_hello2.c
 *
 * A simple Apache Web Server (httpd) module
 * More info: http://thisthread.blogspot.com/2012/09/simple-apache-module.html
 */

#include <httpd.h>
#include <http_config.h>
#include <http_core.h>
#include <http_log.h>

static const char* MOD_HANDLER = "hello";

static int handler(request_rec* r)
{
	if(r->handler == NULL || strcmp(r->handler, MOD_HANDLER))
	{
		ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "handling declined: %s", r->handler);
		return DECLINED;
	}

	ap_log_rerror(APLOG_MARK, APLOG_DEBUG, 0, r, "hello handler processes the request");

	ap_set_content_type(r, "text/html");
	ap_rputs("<html><head><title>Hello Module</title></head><body>", r);
	ap_rprintf(r, "handler: %s<br />\n", r->handler);
	ap_rprintf(r, "filename: %s<br />\n", r->filename);
	ap_rprintf(r, "the_request: %s<br />\n", r->the_request);
	ap_rprintf(r, "header_only: %d<br />\n", r->header_only);
	ap_rprintf(r, "hostname: %s<br />\n", r->hostname);
	ap_rputs("</body></html>", r);

	return OK;
}

static void hooks(apr_pool_t* p) 
{
	ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
}

module AP_MODULE_DECLARE_DATA hello_module = 
{ 
	STANDARD20_MODULE_STUFF, NULL, NULL, NULL, NULL, NULL, hooks
};

