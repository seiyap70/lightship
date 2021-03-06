#ifndef LIGHTSHIP_SERVICES_H
#define LIGHTSHIP_SERVICES_H

#include "plugin_manager/service_api.h"

C_HEADER_BEGIN

struct plugin_t;
struct ordered_vector_t;

typedef enum service_script_type_e
{
    SERVICE_SCRIPT_TYPE_UNKNOWN,
    SERVICE_SCRIPT_TYPE_NONE,

    SERVICE_SCRIPT_TYPE_INT8,   /* NOTE: Interleaving signed and unsigned-ness so */
    SERVICE_SCRIPT_TYPE_UINT8,  /*       a signed type can be set to an unsigned */
    SERVICE_SCRIPT_TYPE_INT16,  /*       type, simply by adding 1. */
    SERVICE_SCRIPT_TYPE_UINT16,
    SERVICE_SCRIPT_TYPE_INT32,
    SERVICE_SCRIPT_TYPE_UINT32,
    SERVICE_SCRIPT_TYPE_INT64,
    SERVICE_SCRIPT_TYPE_UINT64,

    SERVICE_SCRIPT_TYPE_INTPTR,

    SERVICE_SCRIPT_TYPE_FLOAT,
    SERVICE_SCRIPT_TYPE_DOUBLE,

    SERVICE_SCRIPT_TYPE_STRING,
    SERVICE_SCRIPT_TYPE_WSTRING
} service_script_type_e;

/*!
 * @brief Initialises the service system. This must be called before calling any
 * other service related functions.
 */
PLUGIN_MANAGER_PUBLIC_API void
services_init(struct game_t* game);

/*!
 * @brief De-initialises the service system. This must be called to clean up
 * any memory allocated by the system before shutdown.
 */
PLUGIN_MANAGER_PUBLIC_API void
services_deinit(void);

/*!
 * @brief Registers a service to the global service directory.
 * @param[in] plugin The plugin the service belongs to. The plugin name is used
 * to create the namespace under which the service name is registered.
 * @param[in] name The name of the service. This should be unique within the
 * plugin, but can have the same name as other services in different plugins.
 * @param[in] exec A function pointer to the service function.
 */
PLUGIN_MANAGER_PUBLIC_API char
service_register(struct game_t* game,
                 const struct plugin_t* plugin,
                 const char* name,
                 const service_callback_func exec,
                 const char* ret_type,
                 const int argc,
                 const char** argv);

/*!
 * @brief Frees the specified service object's contents and object itself.
 * @param service The service object to free
 */
PLUGIN_MANAGER_PUBLIC_API void
service_free(struct service_t* service);

/*!
 * @brief Unregisters a service from the global service directory.
 * @param[in] plugin The plugin the service belongs to. The plugin name is used
 * to create the namespace under which the service is registered.
 * @param[in] name The name of the service to unregister.
 */
PLUGIN_MANAGER_PUBLIC_API char
service_unregister(struct game_t* game,
                   const struct plugin_t* plugin,
                   const char* name);

/*!
 * @brief Unregisters all services that were previously registered by the
 * specified plugin.
 * @param[in] plugin The plugin to unregister all services.
 */
PLUGIN_MANAGER_PUBLIC_API char
service_unregister_all(const struct plugin_t* plugin);

/*!
 * @brief Retrieves the specified service from the global service directory.
 * @param[in] name The global name, including namespace, of the service to get.
 * This follows the naming convention *plugin_name.service_name*.
 * @return The return value is an integer representing a function pointer to the
 * service. It must be cast to the exact function signature of the service
 * registered by the plugin. If the service does not exist, 0 is returned.
 */
PLUGIN_MANAGER_PUBLIC_API struct service_t*
service_get(struct game_t* game, const char* name);

PLUGIN_MANAGER_PUBLIC_API char
service_do_typecheck(const struct service_t* service, const char* ret_type, uint32_t argc, const char** argv);

PLUGIN_MANAGER_PUBLIC_API void**
service_create_argument_list_from_strings(struct service_t* service, struct ordered_vector_t* argv);

PLUGIN_MANAGER_PUBLIC_API void
service_destroy_argument_list(struct service_t* service, void** argv);

C_HEADER_END

#endif /* LIGHTSHIP_SERVICES_H */
