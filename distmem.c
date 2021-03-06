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
static int le_dm_sock;
zend_class_entry *distmem_ce;

static zend_function_entry distmem_method[] = {
    ZEND_ME(Distmem, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    ZEND_ME(Distmem, connect, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(Distmem, use, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(Distmem, set, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(Distmem, get, NULL, ZEND_ACC_PUBLIC)
    ZEND_ME(Distmem, delete, NULL, ZEND_ACC_PUBLIC)
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

static void dm_destructor_redis_sock(zend_rsrc_list_entry * rsrc TSRMLS_DC)
{
    DMSock *dm_sock = (DMSock *) rsrc->ptr;
    dm_sock_disconnect(dm_sock TSRMLS_CC);
    dm_free_socket(dm_sock);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(distmem)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Distmem", distmem_method);
    distmem_ce = zend_register_internal_class(&ce TSRMLS_CC);
    //zend_declare_property_null(distmem_ce, "public_var", strlen("public_var"), ZEND_ACC_PUBLIC TSRMLS_CC);
    le_dm_sock = zend_register_list_destructors_ex(
        dm_destructor_redis_sock,
        NULL,
        dm_sock_name, module_number
    );

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

PHPAPI int str_count(char *haystack, char *needle) {/*{{{*/
    char *found = haystack;
    int needle_len = strlen(needle);
    int count = 0;
    while(found = strstr(found, needle)) {
        count++;
        found += needle_len;
    }
    return count;
}/*}}}*/

PHPAPI char* str_replace(char *haystack, char *search, char *replace) {/*{{{*/
    char *pos = strstr(haystack, search);
    int count = str_count(haystack, search);

    if(pos = NULL) {
        return strdup(haystack);
    }
    int search_len = strlen(search);
    int replace_len = strlen(replace);
    char *new_str = (char*) calloc(strlen(haystack) + count * (replace_len - search_len) + 2, sizeof(char));
    char *found = haystack;

    pos = haystack;
    while(found = strstr(found, search)) {
        strncat(new_str, pos, found - pos);
        strcat(new_str, replace);
        found += search_len;
        pos = found;
    }
    strcat(new_str, pos);
    return new_str;
}/*}}}*/

PHPAPI char *strdcat(const char *s1, const char *s2) {/*{{{*/
    char *new_str = (char*) calloc(strlen(s1) + strlen(s2) + 1, sizeof(char));
    strcpy(new_str, s1);
    strcat(new_str, s2);
    return new_str;
}/*}}}*/

PHPAPI DMSock* dm_sock_create(char *host, int host_len, unsigned short port, long timeout) {/*{{{*/
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
}/*}}}*/

PHPAPI int dm_sock_connect(DMSock *dm_sock TSRMLS_DC) {/*{{{*/
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
        STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT,
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
}/*}}}*/

PHPAPI int dm_sock_disconnect(DMSock *dm_sock TSRMLS_DC) {/*{{{*/
    int res = 0;

    if (dm_sock->stream != NULL) {
        dm_sock_write(dm_sock, "QUIT");

        dm_sock->status = DM_SOCK_STATUS_DISCONNECTED;
        php_stream_close(dm_sock->stream);
        dm_sock->stream = NULL;

        res = 1;
    }

    return res;

}/*}}}*/

PHPAPI int dm_sock_server_open(DMSock *dm_sock, int force_connect TSRMLS_DC) {/*{{{*/
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

}/*}}}*/

PHPAPI char *dm_sock_read(DMSock *dm_sock, size_t count TSRMLS_DC)/*{{{*/
{
    char * s = emalloc(sizeof(char) * count);
    php_stream_read(dm_sock->stream, s, count);
    return s;
}/*}}}*/

PHPAPI char *dm_sock_readln(DMSock *dm_sock)/*{{{*/
{
    char inbuf[1024], *s;
    s = php_stream_gets(dm_sock->stream, inbuf, 1024);
    return s;
}/*}}}*/

PHPAPI int dm_sock_write(DMSock *dm_sock, char *cmd)/*{{{*/
{
    php_stream_write(dm_sock->stream, cmd, strlen(cmd));

    return 0;
}/*}}}*/

PHPAPI int val_sep(const char *s, const char *str) {/*{{{*/
    if(*s != ',') return 0;
    if(s - 1 >= str && *(s - 1) != '\\') return 1;
    int i = 2;
    while(s - i >= str) {
        if(*(s - i) != '\\') {
            if(i % 2 == 0) return 0;
            return 1;
        }
        i++;
    }
}/*}}}*/
PHPAPI void parse_list(zval *array, char *str) {//only one level/*{{{*/
    char *s = str + 1, *old = s;
    zval *newarr;
    char *t = s;
    switch(str[0]) {
        case 's':
            t = str_replace(s, "\\,", ",");
            s = str_replace(t, "\\\\", "\\");
            free(t);
            add_next_index_string(array, s, 0);
            return;
        case 'i':
            add_next_index_long(array, atoi(s));
            return;
        case 'f':
            add_next_index_double(array, atof(s));
            return;
        case 'l':
            while(s = strchr(s, ',')) {
                if(val_sep(s, str)) {
                    parse_list(array, strndup(old, s - old));
                    old = s + 1;
                 }
                 s++;
             }
            parse_list(array, old);
            return;
        default:
            return;
    }
}/*}}}*/

PHPAPI char* array_to_string(zval *val) {/*{{{*/
    HashTable *arr_hash = Z_ARRVAL_P(val);
    zval tmp_data, **data;
    int array_count = zend_hash_num_elements(arr_hash);
    HashPosition pointer;
    char *final = strdup(" ");
    char *type;

    for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer); 
        zend_hash_get_current_data_ex(arr_hash, (void **)&data, &pointer)==SUCCESS;
        zend_hash_move_forward_ex(arr_hash,&pointer) ) {
        tmp_data = **data;

        switch(Z_TYPE_P(&tmp_data)) {
            case IS_ARRAY:type = 'l'; break;
            case IS_DOUBLE: type = 'f';break;
            case IS_LONG: type = 'i';break;
            default: type = 's';break;
        }

        zval_copy_ctor(&tmp_data);
        convert_to_string(&tmp_data);
        char *str = strndup(Z_STRVAL(tmp_data), Z_STRLEN(tmp_data));
        zval_dtor(&tmp_data);

        char *str1 = str_replace(str, "\\", "\\\\");
        char *str2 = str_replace(str1, ",", "\\,");
        char *tmp = malloc(sizeof(char) * (strlen(str1) + strlen(final) + 5));
        sprintf(tmp, "%s,%c%s", final, type, str2);
        free(final);
        final = tmp;
        free(str1);
        free(str2);
        free(str);
    }
    return final + 2;//for space and comma
}/*}}}*/

PHPAPI int dm_sock_get(zval *id, DMSock **dm_sock TSRMLS_DC)/*{{{*/
{
    zval **socket;
    int resource_type;

    if (Z_TYPE_P(id) != IS_OBJECT || zend_hash_find(Z_OBJPROP_P(id), "socket",
                                  sizeof("socket"), (void **) &socket) == FAILURE) {
        return -1;
    }

    *dm_sock = (DMSock *) zend_list_find(Z_LVAL_PP(socket), &resource_type);

    if (!*dm_sock || resource_type != le_dm_sock) {
        return -1;
    }

    return Z_LVAL_PP(socket);
}/*}}}*/

PHPAPI void dm_free_socket(DMSock *dm_sock)/*{{{*/
{
    efree(dm_sock->host);
    efree(dm_sock);
}/*}}}*/

ZEND_METHOD(Distmem, __construct) {/*{{{*/
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
		RETURN_FALSE;
	}
}/*}}}*/

/* {{{ proto boolean connect(string host, int port)
     */
PHP_METHOD(Distmem, connect)
{
    zval *object;
    int host_len, id;
    char *host = NULL;
    long port;

    struct timeval timeout = {5L, 0L};
    DMSock *dm_sock  = NULL;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Osl|l",
                                     &object, distmem_ce, &host, &host_len, &port,
                                     &timeout.tv_sec) == FAILURE) {
       RETURN_FALSE;
    }

    if (timeout.tv_sec < 0L || timeout.tv_sec > INT_MAX) {
        //zend_throw_exception(dm_exception_ce, "Invalid timeout", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    dm_sock = dm_sock_create(host, host_len, port, timeout.tv_sec);

    if (dm_sock_server_open(dm_sock, 1 TSRMLS_CC) < 0) {
        dm_free_socket(dm_sock);
        /*
        zend_throw_exception_ex(
            dm_exception_ce,
            0 TSRMLS_CC,
            "Can't connect to %s:%d",
            host,
            port
        );
        */
        RETURN_FALSE;
    }

    id = zend_list_insert(dm_sock, le_dm_sock);
    add_property_resource(object, "socket", id);

    RETURN_TRUE;
}
/* }}} */

PHP_METHOD(Distmem, use) {/*{{{*/
    zval *object;
    DMSock *dm_sock;
    char *domain = NULL, *cmd, *response;
    int domain_len, cmd_len, response_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os",
                                     &object, distmem_ce, &domain, &domain_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (dm_sock_get(object, &dm_sock TSRMLS_CC) < 0) {
        RETURN_FALSE;
    }

    cmd_len = spprintf(&cmd, 0, "*2\r\n$3\r\nuse\r\n$%d\r\n%s\r\n", strlen(domain), domain);

    if (dm_sock_write(dm_sock, cmd) < 0) {
        RETURN_FALSE;
    }

    if ((response = dm_sock_readln(dm_sock)) == NULL) {
        RETURN_FALSE;
    }

    if (response[0] == '+') {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}/*}}}*/

PHP_METHOD(Distmem, set)/*{{{*/
{
    zval *object;
    zval *val;
    DMSock *dm_sock;
    char *key = NULL, *valstr = NULL, *cmd, *response;
    int key_len, val_len, cmd_len, response_len;
    char type;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Osz",
                                     &object, distmem_ce, &key, &key_len,
                                     &val) == FAILURE) {
        RETURN_FALSE;
    }


    if (dm_sock_get(object, &dm_sock TSRMLS_CC) < 0) {
        RETURN_FALSE;
    }
    switch(Z_TYPE_P(val)) {
        case IS_ARRAY:type = 'l'; break;
        case IS_DOUBLE: type = 'f';break;
        case IS_LONG: type = 'i';break;
        default: type = 's';break;
    }
    if(Z_TYPE_P(val) != IS_ARRAY) {
        convert_to_string(val);
        valstr = Z_STRVAL_P(val);
        val_len = Z_STRLEN_P(val);
    } else {
        valstr = array_to_string(val);
        val_len = strlen(valstr);
    }
    cmd_len = spprintf(&cmd, 0, "*3\r\n$3\r\nset\r\n$%d\r\n%s\r\n$%d\r\n%c%s\r\n", strlen(key), key, val_len + 1, type, valstr);

    if (dm_sock_write(dm_sock, cmd) < 0) {
        RETURN_FALSE;
    }

    if ((response = dm_sock_readln(dm_sock)) == NULL) {
        RETURN_FALSE;
    }

    if (response[0] == '+') {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}/*}}}*/

PHP_METHOD(Distmem, get){/*{{{*/
    zval *object;
    DMSock *dm_sock;
    char *key = NULL, *cmd, *response;
    int key_len, cmd_len, response_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os",
                                     &object, distmem_ce, &key, &key_len) == FAILURE) {
        RETURN_NULL();
    }

    if (dm_sock_get(object, &dm_sock TSRMLS_CC) < 0) {
        RETURN_NULL();
    }
    cmd_len = spprintf(&cmd, 0, "*2\r\n$3\r\nget\r\n$%d\r\n%s\r\n", strlen(key), key);

    if (dm_sock_write(dm_sock, cmd) < 0) {
        RETURN_NULL();
    }

    if ((response = dm_sock_readln(dm_sock)) == NULL) {
        RETURN_NULL();
    }

    if (response[0] == '$') {

        response_len = atoi(response + 1);
        if(response_len == -1) {
            RETURN_NULL();
        }
        response = dm_sock_read(dm_sock, response_len);
        char *str;
        int result;
        double fres;
        zval * res_arr = emalloc(sizeof(zval));
        str = emalloc(sizeof(char) * (response_len + 1 ));
        strncpy(str, response, response_len);
        str[response_len] = 0;
        dm_sock_readln(dm_sock);//to skip next CRLF

        switch(response[0]) {
            case 's':
                RETURN_STRING(str + 1, 1);    
            case 'i':
                result = atoi(str + 1);
                RETURN_LONG(result);
                break;
            case 'f':
                fres = atof(str + 1);
                RETURN_DOUBLE(fres);
                break;
            case 'l':
                array_init(res_arr);
                parse_list(res_arr, str);
                RETURN_ZVAL(res_arr, 1, 1);
            default:
                break;
        };
        return 0;
    } else {
        RETURN_NULL();
    }
}/*}}}*/

PHP_METHOD(Distmem, delete) {
    zval *object;
    DMSock *dm_sock;
    char *domain = NULL, *cmd, *response;
    int domain_len, cmd_len, response_len;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os",
                                     &object, distmem_ce, &domain, &domain_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (dm_sock_get(object, &dm_sock TSRMLS_CC) < 0) {
        RETURN_FALSE;
    }

    cmd_len = spprintf(&cmd, 0, "*2\r\n$3\r\ndel\r\n$%d\r\n%s\r\n", strlen(domain), domain);

    if (dm_sock_write(dm_sock, cmd) < 0) {
        RETURN_FALSE;
    }

    if ((response = dm_sock_readln(dm_sock)) == NULL) {
        RETURN_FALSE;
    }

    if (response[0] == '+') {
        RETURN_TRUE;
    } else {
        RETURN_FALSE;
    }
}
