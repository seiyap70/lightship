#include "plugin_manager/plugin.h"   /* plugin api */
#include "plugin_main_loop/config.h"  /* configurations for this plugin */
#include "plugin_main_loop/services.h"
#include "plugin_main_loop/events.h"
#include "plugin_main_loop/glob.h"
#include <stdio.h>

/* ------------------------------------------------------------------------- */
PLUGIN_MAIN_LOOP_PUBLIC_API PLUGIN_INIT()
{
    struct plugin_t* plugin;
    struct glob_t* g;
    
    /* init global data */
    glob_create(game);

    /* init plugin */
    plugin = plugin_create(game);
    g = get_global(game);
    g->plugin = plugin;
    
    /* set plugin information */
    plugin_set_info(plugin,
            PLUGIN_NAME,            /* name */
            PLUGIN_CATEGORY,        /* category */
            PLUGIN_AUTHOR,          /* author */
            PLUGIN_DESCRIPTION,     /* description */
            PLUGIN_WEBSITE          /* website */
    );
    plugin_set_programming_language(plugin,
            PLUGIN_PROGRAMMING_LANGUAGE_C
    );
    plugin_set_version(plugin,
            PLUGIN_VERSION_MAJOR,
            PLUGIN_VERSION_MINOR,
            PLUGIN_VERSION_PATCH
    );

    register_services(plugin);
    register_events(plugin);
    main_loop_init(&get_global(game)->main_loop);

    return plugin;
}

/* ------------------------------------------------------------------------- */
PLUGIN_MAIN_LOOP_PUBLIC_API PLUGIN_START()
{
    register_event_listeners(get_global(game)->plugin);
    
    return PLUGIN_SUCCESS;
}

/* ------------------------------------------------------------------------- */
PLUGIN_MAIN_LOOP_PUBLIC_API PLUGIN_STOP()
{
}

/* ------------------------------------------------------------------------- */
PLUGIN_MAIN_LOOP_PUBLIC_API PLUGIN_DEINIT()
{
    plugin_destroy(get_global(game)->plugin);
    glob_destroy(game);
}
