#include "plugin_menu/element.h"
#include "plugin_menu/services.h"
#include "plugin_menu/glob.h"
#include "plugin_manager/service_api.h"
#include "util/map.h"
#include "util/memory.h"
#include <string.h>

/* ------------------------------------------------------------------------- */
void
element_init(struct glob_t* g)
{
    g->element.guid = 1;
}

/* ------------------------------------------------------------------------- */
void
element_constructor(struct glob_t* g,
                    struct element_t* element,
                    element_destructor_func derived_destructor,
                    float x, float y,
                    float width, float height)
{
    unordered_vector_init_vector(&element->base.element.gl.shapes, sizeof(uint32_t));
    unordered_vector_init_vector(&element->base.element.gl.text, sizeof(struct element_font_text_id_pair_t));
    element->base.element.id = g->element.guid++;
    element->base.element.glob = g;
    element->base.element.pos.x = x;
    element->base.element.pos.y = y;
    element->base.element.size.x = width;
    element->base.element.size.y = height;
    element->base.element.derived_destructor = derived_destructor;
    element->base.element.visible = 1;
}

/* ------------------------------------------------------------------------- */
void
element_destructor(struct element_t* element)
{
    struct glob_t* g = element->base.element.glob;
    { UNORDERED_VECTOR_FOR_EACH(&element->base.element.gl.shapes, uint32_t, id)
    {
        SERVICE_CALL1(g->services.shapes_2d_destroy, SERVICE_NO_RETURN, id);
    }}
    { UNORDERED_VECTOR_FOR_EACH(&element->base.element.gl.text, struct element_font_text_id_pair_t, pair)
    {
        SERVICE_CALL2(g->services.text_destroy, SERVICE_NO_RETURN, pair->font_id,pair->text_id);
    }}
    unordered_vector_clear_free(&element->base.element.gl.shapes);
    unordered_vector_clear_free(&element->base.element.gl.text);
}

/* ------------------------------------------------------------------------- */
void
element_destroy(struct element_t* element)
{
    /* destruct derived */
    element->base.element.derived_destructor(element);
    /* destruct base */
    element_destructor(element);

    FREE(element);
}

/* ------------------------------------------------------------------------- */
void
element_add_text(struct element_t* element, uint32_t font_id, uint32_t text_id)
{
    struct element_font_text_id_pair_t* pair = unordered_vector_push_emplace(&element->base.element.gl.text);
    pair->font_id = font_id;
    pair->text_id = text_id;
}

/* ------------------------------------------------------------------------- */
void
element_add_shapes(struct element_t* element, uint32_t shapes_id)
{
    uint32_t* id = unordered_vector_push_emplace(&element->base.element.gl.shapes);
    *id = shapes_id;
}

/* ------------------------------------------------------------------------- */
void
element_show(struct element_t* element)
{
    struct glob_t* g = element->base.element.glob;

    { UNORDERED_VECTOR_FOR_EACH(&element->base.element.gl.shapes, uint32_t, id)
    {
        SERVICE_CALL1(g->services.shapes_2d_show, SERVICE_NO_RETURN, *id);
    }}
    { UNORDERED_VECTOR_FOR_EACH(&element->base.element.gl.text, struct element_font_text_id_pair_t, pair)
    {
        SERVICE_CALL1(g->services.text_show, SERVICE_NO_RETURN, pair->text_id);
    }}
    element->base.element.visible = 1;
}

/* ------------------------------------------------------------------------- */
void
element_hide(struct element_t* element)
{
    struct glob_t* g = element->base.element.glob;
    
    { UNORDERED_VECTOR_FOR_EACH(&element->base.element.gl.shapes, uint32_t, id)
    {
        SERVICE_CALL1(g->services.shapes_2d_hide, SERVICE_NO_RETURN, *id);
    }}
    { UNORDERED_VECTOR_FOR_EACH(&element->base.element.gl.text, struct element_font_text_id_pair_t, pair)
    {
        SERVICE_CALL1(g->services.text_hide, SERVICE_NO_RETURN, pair->text_id);
    }}
    element->base.element.visible = 0;
}
