/*
 * pragha-radiobrowser-plugin.c
 *
 * Copyright 2021 Eric Vissers <https://github.com/emv-nl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * Based on pragha-tunein-plugin by matias
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#if defined(GETTEXT_PACKAGE)
#include <glib/gi18n-lib.h>
#else
#include <glib/gi18n.h>
#endif

#include <glib.h>
#include <glib-object.h>
#include <gmodule.h>
#include <gtk/gtk.h>

#include <libsoup/soup.h>

#include <libpeas/peas.h>

#include "pragha.h"
#include "pragha-app-notification.h"
#include "pragha-menubar.h"
#include "pragha-playlist.h"
#include "pragha-playlists-mgmt.h"
#include "pragha-musicobject-mgmt.h"
#include "pragha-hig.h"
#include "pragha-utils.h"
#include "pragha-window.h"
#include "pragha-background-task-bar.h"
#include "pragha-background-task-widget.h"
#include "xml_helper.h"

#include "plugins/pragha-plugin-macros.h"

#define PRAGHA_TYPE_RADIOBROWSER_PLUGIN         (pragha_radiobrowser_plugin_get_type ())
#define PRAGHA_RADIOBROWSER_PLUGIN(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), PRAGHA_TYPE_RADIOBROWSER_PLUGIN, PraghaRadiobrowserPlugin))
#define PRAGHA_RADIOBROWSER_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), PRAGHA_TYPE_RADIOBROWSER_PLUGIN, PraghaRadiobrowserPlugin))
#define PRAGHA_IS_RADIOBROWSER_PLUGIN(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), PRAGHA_TYPE_RADIOBROWSER_PLUGIN))
#define PRAGHA_IS_RADIOBROWSER_PLUGIN_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), PRAGHA_TYPE_RADIOBROWSER_PLUGIN))
#define PRAGHA_RADIOBROWSER_PLUGIN_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), PRAGHA_TYPE_RADIOBROWSER_PLUGIN, PraghaRADIOBROWSERPluginClass))

struct _PraghaRadiobrowserPluginPrivate {
	PraghaApplication          *pragha;

	PraghaBackgroundTaskWidget *task_widget;
	GtkWidget                  *name_entry;
	GtkActionGroup             *action_group_main_menu;
	guint                       merge_id_main_menu;
};

typedef struct _PraghaRadiobrowserPluginPrivate PraghaRadiobrowserPluginPrivate;

PRAGHA_PLUGIN_REGISTER (PRAGHA_TYPE_RADIOBROWSER_PLUGIN,
                        PraghaRadiobrowserPlugin,
                        pragha_radiobrowser_plugin)

/*
 * Prototypes
 */
static void pragha_radiobrowser_get_radio_dialog        (PraghaRadiobrowserPlugin *plugin);

/*
 * Popups
 */

static void
pragha_radiobrowser_plugin_get_radio_action (GtkAction *action, PraghaRadiobrowserPlugin *plugin)
{
	pragha_radiobrowser_get_radio_dialog (plugin);
}

static void
pragha_gmenu_radiobrowser_plugin_get_radio_action (GSimpleAction *action,
                                                   GVariant      *parameter,
                                                   gpointer       user_data)
{
	pragha_radiobrowser_get_radio_dialog (PRAGHA_RADIOBROWSER_PLUGIN(user_data));
}

static const GtkActionEntry main_menu_actions [] = {
	{"Search Radiobrowser", NULL, N_("Search radio on Radiobrowser"),
	 "", "Search Radiobrowser", G_CALLBACK(pragha_radiobrowser_plugin_get_radio_action)}
};

static const gchar *main_menu_xml = "<ui>						\
	<menubar name=\"Menubar\">									\
		<menu action=\"ToolsMenu\">								\
			<placeholder name=\"pragha-plugins-placeholder\">	\
				<menuitem action=\"Search radiobrowser\"/>		\
				<separator/>									\
			</placeholder>										\
		</menu>													\
	</menubar>													\
</ui>";

/*
 * Radiobrowser Handles
 */

