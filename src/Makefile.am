plugindir = $(libdir)/pragha/plugins/radiobrowser
plugin_LTLIBRARIES = libradiobrowser.la

libsample_la_SOURCES =      \
	pragha-radiobrowser-plugin.c

libsample_la_CFLAGS = \
	$(PRAGHA_PLUGINS_CFLAGS)

libsample_la_LIBADD = \
	$(PRAGHA_PLUGINS_LIBS)


plugin_DATA = radiobrowser.plugin
EXTRA_DIST = $(plugin_DATA)
