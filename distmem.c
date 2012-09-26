/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Usbuild <njuzhangqichao@gmail.com>                           |
  +----------------------------------------------------------------------+
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_distmem.h"

/* If you declare any globals in php_distmem.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(distmem)
*/

/* True global resources - no need for thread safety here */
static int le_distmem;
zend_class_entry *distmem_ce;

static zend_function_entry distmem_method[] = {
    ZEND_ME(Distmem, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Distmem, connect, NULL, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
/* {{{ distmem_functions[]
 *
 * Every user visible function must have an entry in distmem_functions[].
 */
const zend_function_entry distmem_functions[] = {
	PHP_FE_END	/* Must be the last line in distmem_functions[] */
};
/* }}} */

/* {{{ distmem_module_entry
 */
zend_module_entry distmem_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"distmem",
	distmem_functions,
	PHP_MINIT(distmem),
	PHP_MSHUTDOWN(distmem),
	PHP_RINIT(distmem),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(distmem),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(distmem),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_DISTMEM
ZEND_GET_MODULE(distmem)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("distmem.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_distmem_globals, distmem_globals)
    STD_PHP_INI_ENTRY("distmem.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_distmem_globals, distmem_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_distmem_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_distmem_init_globals(zend_distmem_globals *distmem_globals)
{
	distmem_globals->global_value = 0;
	distmem_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(distmem)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Distmem", distmem_method);
    distmem_ce = zend_register_internal_class(&ce TSRMLS_CC);
    //zend_declare_property_null(distmem_ce, "public_var", strlen("public_var"), ZEND_ACC_PUBLIC TSRMLS_CC);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(distmem)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(distmem)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(distmem)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(distmem)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Distmem Support", "enabled");
	php_info_print_table_row(2, "Version", PHP_DM_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

PHPAPI DMSock* dm_sock_create(char *host, int host_len, unsigned short port, long timeout) {
    DMSock *dm_sock;
    dm_sock         = emalloc(sizeof *dm_sock);
    dm_sock->host   = emalloc(host_len + 1);
    dm_sock->stream = NULL;
    dm_sock->status = DM_SOCK_STATUS_DISCONNECTED;

    memcpy(dm_sock->host, host, host_len);
    dm_sock->host[host_len] = '\0';

    dm_sock->port    = port;
    dm_sock->timeout = timeout;

    return dm_sock;
}

PHPAPI int dm_sock_connect(DMSock *dm_sock TSRMLS_DC) {
    struct timeval tv;
    char *host = NULL, *hash_key = NULL, *errstr = NULL;
    int host_len, err = 0;

    if (dm_sock->stream != NULL) {
        dm_sock_disconnect(dm_sock TSRMLS_CC);
    }

    tv.tv_sec  = dm_sock->timeout;
    tv.tv_usec = 0;

    host_len = spprintf(&host, 0, "%s:%d", dm_sock->host, dm_sock->port);

    dm_sock->stream = php_stream_xport_create(host, host_len, ENFORCE_SAFE_MODE,
        STREAM_XPORT_CLIENT
        | STREAM_XPORT_CONNECT,
        hash_key, &tv, NULL, &errstr, &err
        );

    efree(host);

    if (!dm_sock->stream) {
        efree(errstr);
        return -1;
    }

    php_stream_auto_cleanup(dm_sock->stream);

    php_stream_set_option(dm_sock->stream,
        PHP_STREAM_OPTION_READ_TIMEOUT,
        0, &tv);
    php_stream_set_option(dm_sock->stream,
        PHP_STREAM_OPTION_WRITE_BUFFER,
        PHP_STREAM_BUFFER_NONE, NULL);
    dm_sock->status = DM_SOCK_STATUS_CONNECTED;
    return 0;
}

PHPAPI int dm_sock_disconnect(DMSock *dm_sock TSRMLS_DC) {
    int res = 0;

    if (dm_sock->stream != NULL) {
        dm_sock_write(dm_sock, "QUIT");

        dm_sock->status = DM_SOCK_STATUS_DISCONNECTED;
        php_stream_close(dm_sock->stream);
        dm_sock->stream = NULL;

        res = 1;
    }

    return res;

}

PHPAPI int dm_sock_server_open(DMSock *dm_sock, int force_connect TSRMLS_DC) {
    int res = -1;

    switch (dm_sock->status) {
    case DM_SOCK_STATUS_DISCONNECTED:
        return dm_sock_connect(dm_sock TSRMLS_CC);
    case DM_SOCK_STATUS_CONNECTED:
        res = 0;
        break;
    case DM_SOCK_STATUS_UNKNOWN:
        if (force_connect > 0 && dm_sock_connect(dm_sock TSRMLS_CC) < 0) {
            res = -1;
        } else {
            res = 0;
            dm_sock->status = DM_SOCK_STATUS_CONNECTED;
        }
        break;
    }

    return res;

}

ZEND_METHOD(Distmem, __construct) {
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		RETURN_FALSE;
	}
}

/* {{{ proto boolean connect(string host, int port)
     */
ZEND_METHOD(Distmem, connect) {
	zval *object;
	char *host;
	long port;
	int host_len;
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Osl", &object, distmem_ce, &host, &host_len, &port) == FAILURE) {
		RETURN_FALSE;
	}
}
/* }}} */
