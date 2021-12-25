`config-manager` allows a unified management of a configuration
distributed among different components.

All configuration directories are viewed as unique one, allowing
overloading configuration options in multiple levels.

== Use case ==

Or a web server with plugins providing optional services. In the
configuration of nginx, we need to declare backends for each plugin
and map URLs to those backends.

* Main application configuration:
  ```
  # /path/to/application/config/main.ini
  [nginx]
  port = 8080
  # ...
  ```

* First plugin:
  ```
  # /path/to/first/plugin/config/plugins/first_plugin.ini
  [plugin]
  enabled = true
  name = my_first_plugin
  [nginx]
  socket = /var/first_plugin.sock
  urls = ^/
  ```

* Local instance configuration:
  ```
  # /path/to/local/config/plugins/first_plugin.ini
  [plugin]
  # To deactivate this plugin on this instance
  enabled = false
  ```

* And the nginx configuration
  ```
  http {
      listen {{settings.main.nginx.port}};

      {% for plugin in settings.plugins %}
      {% if plugin.plugin.enabled %}
      upstream {{plugin.plugin.name}} {
          server unix:{{plugin.nginx.socket}};
      }
      {% endif %}
      {% endfor %}

      server {
          {% for plugin in settings.plugins %}
          {% if plugin.plugin.enabled %}
          {% for location in plugin.nginx.locations.split() %}
          location ~ {{location}} {
              proxy_pass http://{{plugin.plugin.name}};
          }
          {% endfor %}
          {% endif %}
          {% endfor %}
      }
    }


== Commands ==

=== get key ===

=== render template ===

=== doc key ===


== Use cases ==


== Install ==

=== Requirements ===

=== Building ===


== License ==
