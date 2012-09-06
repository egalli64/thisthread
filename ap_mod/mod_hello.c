/**
 * mod_hello.c
 *
 * A minimal Hello World Apache module
 * Comments and info on http://thisthread.blogspot.com/2012/09/a-minimal-hello-world-apache-module.html
 */

#include <httpd.h>
#include <http_config.h>

static int handler(request_rec* r)
{
	ap_set_content_type(r, "text/plain");
	ap_rputs("Hello Apache httpd module", r);
	return OK;
}

static void hooks(apr_pool_t* p) 
{
	ap_hook_handler(handler, NULL, NULL, APR_HOOK_REALLY_FIRST);
}

module AP_MODULE_DECLARE_DATA hello_module = 
{ 
	STANDARD20_MODULE_STUFF, NULL, NULL, NULL, NULL, NULL, hooks
};

