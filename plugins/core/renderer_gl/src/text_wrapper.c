#include "plugin_renderer_gl/text_wrapper.h"
#include "plugin_renderer_gl/text.h"
#include "plugin_renderer_gl/text_manager.h"
#include "util/map.h"

static struct map_t g_text_groups;
static struct map_t g_texts;
static uint32_t guid = 1;

/* ------------------------------------------------------------------------- */
char
text_wrapper_init(void)
{
    map_init_map(&g_texts);
    
    return 1;
}

/* ------------------------------------------------------------------------- */
void
text_wrapper_deinit(void)
{
    /* text objects are cleaned up automatically when all groups get destroyed */
    
    map_clear_free(&g_texts);
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
const struct map_t*
text_group_get_all(void)
{
    return &g_text_groups;
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
SERVICE(text_group_create_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT_PTR(0, file_name, const char*);
    SERVICE_EXTRACT_ARGUMENT(1, char_size, uint32_t, uint32_t);

    SERVICE_RETURN(text_group_create(file_name, char_size), uint32_t);
    
}

/* ------------------------------------------------------------------------- */
SERVICE(text_group_destroy_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT(0, id, uint32_t, uint32_t);

    text_group_destroy(id);
}

/* ------------------------------------------------------------------------- */
SERVICE(text_group_load_character_set_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT(0, id, uint32_t, uint32_t);
    SERVICE_EXTRACT_ARGUMENT_PTR(1, characters, wchar_t*);

    text_group_load_character_set(id, characters);
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

SERVICE(text_create_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT(0, group_id, uint32_t, uint32_t);
    SERVICE_EXTRACT_ARGUMENT(1, centered, char, char);
    SERVICE_EXTRACT_ARGUMENT(2, x, float, GLfloat);
    SERVICE_EXTRACT_ARGUMENT(3, y, float, GLfloat);
    SERVICE_EXTRACT_ARGUMENT_PTR(4, string, wchar_t*);
    
    struct text_group_t* group = text_group_get(group_id);
    struct text_t* text = text_create(group, centered, x, y, string);
    uint32_t text_id = guid++;
    map_insert(&g_texts, text_id, text);
    
    SERVICE_RETURN(text_id, uint32_t);
}

/* ------------------------------------------------------------------------- */
SERVICE(text_destroy_wrapper)
{
}

/* ------------------------------------------------------------------------- */
SERVICE(text_set_centered_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT(0, text_id, uint32_t, uint32_t);
    SERVICE_EXTRACT_ARGUMENT(1, is_centered, char, char);
    
    struct text_t* text = map_find(&g_texts, text_id);
    if(!text)
        return;
    
    text_set_centered(text, is_centered);
}

/* ------------------------------------------------------------------------- */
SERVICE(text_set_position_wrapper)
{
}

/* ------------------------------------------------------------------------- */
SERVICE(text_set_string_wrapper)
{
}

/* ------------------------------------------------------------------------- */
SERVICE(text_show_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT(0, text_id, uint32_t, uint32_t);
    struct text_t* text = map_find(&g_texts, text_id);
    if(!text)
        return;
    
    text_show(text);
}

/* ------------------------------------------------------------------------- */
SERVICE(text_hide_wrapper)
{
    SERVICE_EXTRACT_ARGUMENT(0, text_id, uint32_t, uint32_t);
    struct text_t* text = map_find(&g_texts, text_id);
    if(!text)
        return;
    
    text_hide(text);
}
