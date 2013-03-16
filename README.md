
php_distmem
===============

This extension provides an API for communicating with Distmem database, a persistent
key-value database with built-in net interface written in ANSI-C for Posix systems.

IMPORTANT
=========

This extension is experimental, it's still under development and it's not safe
enough for production use. The method names may change in future versions.

INSTALLING
==========
Make sure you have php5-dev installed on your system
phpize
./configure
make && make install

USING
=========
```php
$dm = new	 Distmem();
$dm->connect("localhost", 4327);
$dm->use("test");
$dm->set("name", "hello");
$dm->get("name");
$dm->del("name");
```

CONTACT
============
* GitHub     [http://github.com/usbuild](http://github.com)
* Sina-Weibo [http://weibo.com/usbuild](http://weibo.com/usbuild)
* Email      [njuzhangqichao@gmail.com](mailto:njuzhangqichao@gmail.com)
