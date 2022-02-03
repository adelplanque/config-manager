What is it ?
============

`config-manager` manages the configuration files of several
independent applications in a global way, allowing to model other
files.

Different configuration variants can be defined, as well as a local
overloading of an application instance.


Use case
========

Consider a web server with plugins providing optional services. In the
configuration of nginx, we need to declare backends for each plugin
and map URLs to those backends.

* Main application, installed in `/path/to/application`:
  ```ini
  # /path/to/application/config/application.ini
  [nginx]
  port = 8080
  # ...
  ```

* First plugin, installed in `/path/to/first/plugin`:
  ```ini
  # /path/to/first/plugin/config/plugins/first_plugin.ini
  [main]
  enabled = true
  name = my_first_plugin
  [nginx]
  socket = /var/first_plugin.sock
  urls = ^/
  ```

* Maybe other plugins are installed in their own directory.

* The manager of a local instance overrides the configuration in a separate directory:
  ```ini
  # /path/to/local/config/plugins/first_plugin.ini
  [main]
  # To deactivate this plugin on this instance
  enabled = false
  ```

* And the nginx configuration
  ```
  http {
      listen {{settings.application.nginx.port}};

      {% for plugin in settings.plugins %}
      {% if plugin.main.enabled %}
      upstream {{plugin.main.name}} {
          server unix:{{plugin.nginx.socket}};
      }
      {% endif %}
      {% endfor %}

      server {
          {% for plugin in settings.plugins %}
          {% if plugin.main.enabled %}
          {% for url in plugin.nginx.urls.split() %}
          location ~ {{url}} {
              proxy_pass http://{{plugin.main.name}};
          }
          {% endfor %}
          {% endif %}
          {% endfor %}
      }
    }

We can render the nginx configuration with:

```bash
config-manager -p /path/to/local/config -p /path/to/first/plugin/config \
    -p /path/to/application/config render nginx.conf
```

Or:

```bash
export CONFIG_PATH=/path/to/local/config:/path/to/first/plugin/config:/path/to/application/config
config-manager render nginx.conf
```


Configuration file format
=========================

Each config file is an ini file.


Commands
========

* `config-manager get [key]`: Return the value of a key
* `config-manager render [template]`: Formats the template with
  configuration values using jinja2 syntax.
* `config-manager doc [key]`: Shows the documentation of the key as
  well as its various value overloads.  render template


Configuration overload
======================

Internal to a config file
-------------------------

It is possible to define variants to the configuration. A value that
should apply to a specific variant is shown in square brackets.

Exemple:

```ini
# config.ini
[main]
debug = 0
debug[DEV] = 1
debug[DEV_JHON] = 2
```

```bash
$ config-manager get config.main.debug
0
$ config-manager get -c DEV config.main.debug
1
$ config-manager get -c DEV_JHON config.main.debug
2
$ config-manager get -c DEV_PETER config.main.debug
1
```

Note: `DEV_PETER` has no specific configuration, closest is `DEV`
configuration.

External to a configuration file
--------------------------------

If the same key is defined in several files, the value retained
corresponds to that of the first configuration directory in the order
defined, either by the environment variable `CONFIG_PATH`, or by the
parameters of the command line.


Other features
==============

Shell call
----------

It is possible to call system commands an insert stdout in templates:

`{{shell("commande")}}`

Récursive key definition
------------------------

It is allowed to write

```ini
# first.ini
[main]
key1 = value
```

```ini
# second.ini
[main]
key1 = {{settings.first.main.key1}}
key2 = {{settings.second.main.key1}}
```


Configuration documentation
===========================


Installation
============

Requirements
------------

Building
--------


License
=======

config-manager is under LGPL-3.0-or-later.

config-manager embded various third-parties softwares:

* [Jinja2CPP](https://github.com/jinja2cpp/Jinja2Cpp): Mozilla Public License Version 2.0
* [fmt](https://github.com/fmtlib/fmt): MIT License
* [CLI11](https://github.com/CLIUtils/CLI11)
* [subprocess](https://github.com/benman64/subprocess): MIT License
