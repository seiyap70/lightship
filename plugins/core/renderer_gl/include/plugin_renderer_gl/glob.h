#include "util/pstdint.h"
#include "util/unordered_vector.h"
#include "plugin_manager/game.h"

extern uint32_t global_hash;

struct plugin_t;

struct glob_t
{
    struct plugin_t* plugin;
};

void
glob_create(struct game_t* game);

void
glob_destroy(struct game_t* game);

#define get_global(game) ((struct glob_t*)game_get_global(game, global_hash))
