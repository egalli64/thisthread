/**
 * mod_info.cpp
 *
 * An Apache2 module printing the request headers.
 * Described and commented: http://thisthread.blogspot.com/2012/11/iterating-over-apache-aprtablet.html 
 */

#include <httpd.h>
#include <http_config.h>
#include <http_core.h>
#include <http_log.h>
#include <http_protocol.h>

namespace
{
    int print(void* rec, const char* key, const char* value)
    {
        request_rec* r = static_cast<request_rec*>(rec);
        ap_rprintf(r, "%s: %s<br />\n", key, value);

        return 1;
    }

    int handler(request_rec* r)
    {
        if(!r->handler || (strcmp(r->handler, "info") != 0))
            return DECLINED ;

        ap_set_content_type(r, "text/html;charset=ascii");
        ap_rputs("<html><head><title>Apache Info</title></head>"
            "<body><h2>Request Headers</h2>", r);

        apr_table_do(print, r, r->headers_in, NULL);
        ap_rputs("</body></html>", r);
        return OK;
    }

    void hooks(apr_pool_t* p)
    {
        ap_hook_handler(handler, NULL, NULL, APR_HOOK_MIDDLE);
    }
}

module AP_MODULE_DECLARE_DATA info_module =
{
    STANDARD20_MODULE_STUFF, NULL, NULL, NULL, NULL, NULL, hooks
};
