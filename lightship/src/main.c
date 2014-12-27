#include <stdio.h>
#include "lightship/plugin_manager.h"
#include "util/services.h"
#include "util/events.h"
#include "lightship/api.h"
#include "util/plugin.h"
#include "util/unordered_vector.h"
#include "util/memory.h"
#include "util/log.h"

struct plugin_t* plugin_main_loop = NULL;
struct plugin_t* plugin_yaml = NULL;
struct plugin_t* plugin_renderer = NULL;
struct plugin_t* plugin_input = NULL;

typedef void (*start_loop_func)(void); start_loop_func start;

void load_core_plugins(void)
{
    struct plugin_info_t target;
    
    /* 
     * Main loop plugin.
     * Dependencies: None.
     */
    target.name = "main_loop";
    target.version.major = 0;
    target.version.minor = 0;
    target.version.patch = 1;
    plugin_main_loop = plugin_load(&target, PLUGIN_VERSION_MINIMUM);
    
    /*!
     * Yaml plugin.
     * Dependencies: None.
     */
    target.name = "yaml";
    target.version.major = 0;
    target.version.minor = 0;
    target.version.patch = 1;
    plugin_yaml = plugin_load(&target, PLUGIN_VERSION_MINIMUM);

    /*
     * Graphics plugin.
     * Dependencies: main_loop
     */
    target.name = "renderer_gl";
    target.version.major = 0;
    target.version.minor = 0;
    target.version.patch = 1;
    plugin_renderer = plugin_load(&target, PLUGIN_VERSION_MINIMUM);

    /* 
     * Input plugin.
     * Dependencies: main_loop, graphics, yaml
     */
    target.name = "input";
    target.version.major = 0;
    target.version.minor = 0;
    target.version.patch = 1;
    plugin_input = plugin_load(&target, PLUGIN_VERSION_MINIMUM);
}

char start_core_plugins(void)
{
    if(!plugin_main_loop || plugin_start(plugin_main_loop) == PLUGIN_FAILURE)
        return 0;
    if(!plugin_yaml      || plugin_start(plugin_yaml) == PLUGIN_FAILURE)
        return 0;
    if(!plugin_renderer  || plugin_start(plugin_renderer) == PLUGIN_FAILURE)
        return 0;
    if(!plugin_input     || plugin_start(plugin_input) == PLUGIN_FAILURE)
        return 0;

    return 1;
}

void init(void)
{
    /*
     * Services and events should be initialised before anything else, as they
     * register built-in mechanics that are required throughout the rest of the
     * program (such as the log event).
     */
    services_init();
    events_init();
    
    /*
     * Enable logging as soon as possible (right after enabling services and
     * events)
     */
    llog_init();
    
    /*
     * The lightship API must be initialised before any plugins can be
     * loaded, so an api struct can be passed to each plugin.
     */
    api_init();
    
    /*
     * The plugin manager must be initialised before being able to load
     * plugins.
     */
    plugin_manager_init();

    /*
     * Try to load and start the core plugins. If that fails, bail out.
     */
    load_core_plugins();
    if(!start_core_plugins())
        return;

    typedef uint32_t(*open_func)(const char*);
    open_func open_s = (open_func)service_get("yaml.open");
    uint32_t x = open_s("test.yml");

    typedef void(*close_func)(uint32_t);
    close_func close_s = (close_func)service_get("yaml.close");
    close_s(x);
    
    /* 
     * Try to get the main loop service and start running the game
     *
    start = (start_loop_func)service_get("main_loop.start");
    if(start)
        start();
    else
        llog(LOG_FATAL, 1, "Failed to find service \"main_loop.start\". Cannot start.");*/
}

void deinit(void)
{
    plugin_manager_deinit();
    events_deinit();
}

int main(int argc, char** argv)
{
    /* first thing - initialise memory management */
    memory_init();

    /* initialise everything else */
    init();

    /* clean up */
    deinit();

    /* de-init memory management and print report */
    memory_deinit();

    return 0;
}
