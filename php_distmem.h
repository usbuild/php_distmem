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


#ifndef PHP_DISTMEM_H
#define PHP_DISTMEM_H

extern zend_module_entry distmem_module_entry;
//user define function ----start----
ZEND_METHOD(Distmem, __construct);
ZEND_METHOD(Distmem, connect);
ZEND_METHOD(Distmem, use);
ZEND_METHOD(Distmem, set);
ZEND_METHOD(Distmem, get);
ZEND_METHOD(Distmem, delete);
//user define function ----end----

#define phpext_distmem_ptr &distmem_module_entry

#ifdef PHP_WIN32
#	define PHP_DISTMEM_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_DISTMEM_API __attribute__ ((visibility("default")))
#else
#	define PHP_DISTMEM_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(distmem);
PHP_MSHUTDOWN_FUNCTION(distmem);
PHP_RINIT_FUNCTION(distmem);
PHP_RSHUTDOWN_FUNCTION(distmem);
PHP_MINFO_FUNCTION(distmem);



/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(distmem)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(distmem)
*/

/* In every utility function you add that needs to use variables 
   in php_distmem_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as DISTMEM_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define DISTMEM_G(v) TSRMG(distmem_globals_id, zend_distmem_globals *, v)
#else
#define DISTMEM_G(v) (distmem_globals.v)
#endif

#endif	/* PHP_DISTMEM_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
#define PHP_DM_VERSION				"0.0.1"

#define DM_SOCK_STATUS_FAILED		0
#define DM_SOCK_STATUS_DISCONNECTED	1
#define DM_SOCK_STATUS_UNKNOWN		2
#define DM_SOCK_STATUS_CONNECTED	3
typedef struct DMSock_ {
	php_stream		*stream;
	char			*host;
	unsigned short 	port;
	long 			timeout;
	int				fail;
	int 			status;
} DMSock;

PHPAPI DMSock* dm_sock_create(char *host, int host_len, unsigned short port, long timeout);
PHPAPI int dm_sock_connect(DMSock *dm_sock TSRMLS_DC);
PHPAPI int dm_sock_disconnect(DMSock *dm_sock TSRMLS_DC);
PHPAPI int dm_sock_server_open(DMSock *dm_sock, int force_connect TSRMLS_DC);
PHPAPI char* dm_sock_read(DMSock *dm_sock, size_t buf_len TSRMLS_DC);
PHPAPI char* dm_sock_readln(DMSock *dm_sock);
PHPAPI int dm_sock_write(DMSock *dm_sock, char *cmd);
PHPAPI void dm_free_socket(DMSock *dm_sock);
PHPAPI int dm_sock_get(zval *id, DMSock **dm_sock TSRMLS_DC);
