dnl $Id$
dnl config.m4 for extension distmem

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(distmem, for distmem support,
dnl Make sure that the comment is aligned:
dnl [  --with-distmem             Include distmem support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(distmem, whether to enable distmem support,
dnl Make sure that the comment is aligned:
[  --enable-distmem           Enable distmem support])

if test "$PHP_DISTMEM" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-distmem -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/distmem.h"  # you most likely want to change this
  dnl if test -r $PHP_DISTMEM/$SEARCH_FOR; then # path given as parameter
  dnl   DISTMEM_DIR=$PHP_DISTMEM
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for distmem files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       DISTMEM_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$DISTMEM_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the distmem distribution])
  dnl fi

  dnl # --with-distmem -> add include path
  dnl PHP_ADD_INCLUDE($DISTMEM_DIR/include)

  dnl # --with-distmem -> check for lib and symbol presence
  dnl LIBNAME=distmem # you may want to change this
  dnl LIBSYMBOL=distmem # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $DISTMEM_DIR/lib, DISTMEM_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_DISTMEMLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong distmem lib version or lib not found])
  dnl ],[
  dnl   -L$DISTMEM_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(DISTMEM_SHARED_LIBADD)

  PHP_NEW_EXTENSION(distmem, distmem.c, $ext_shared)
fi
