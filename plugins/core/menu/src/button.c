#include "plugin_menu/button.h"
#include "plugin_menu/services.h"
#include "plugin_menu/events.h"
#include "util/unordered_vector.h"
#include "util/memory.h"
#include "util/string.h"
#include <string.h>
#include <wchar.h>

static struct unordered_vector_t g_buttons;
static uint32_t guid_counter = 1;
static uint32_t font_id = -1;

#ifdef _DEBUG
static const char* ttf_filename = "../../plugins/core/menu/ttf/DejaVuSans.ttf";
#else
static const char* ttf_filename = "ttf/DejaVuSans.ttf";
#endif

void button_init(void)
{
    /* load font and characters */
    font_id = text_load_font(ttf_filename, 9);
    text_load_characters(font_id, NULL);
    
    unordered_vector_init_vector(&g_buttons, sizeof(struct button_t));
}

void button_deinit(void)
{
    button_destroy_all();
    unordered_vector_clear_free(&g_buttons);
    
    text_destroy_font(font_id);;
}

static struct button_t* button_get(uint32_t ID)
{
    UNORDERED_VECTOR_FOR_EACH(&g_buttons, struct button_t, button)
    {
        if(button->ID == ID)
            return button;
    }
    return NULL;
}

uint32_t button_create(const char* text, float x, float y, float width, float height)
{
    uint32_t len;
    struct button_t* btn = (struct button_t*)unordered_vector_push_emplace(&g_buttons);
    btn->ID = guid_counter++;
    btn->pos.x = x;
    btn->pos.y = y;
    btn->size.x = width;
    btn->size.y = height;
    
    /* copy wchar_t string into button object */
    len = (strlen(text)+1) * sizeof(wchar_t);
    btn->text = (wchar_t*)MALLOC(len);
    swprintf(btn->text, len, L"%s", text);

    /* draw box */
    shapes_2d_begin();
        box_2d(x-width*0.5, y-height*0.5, x+width*0.5, y+height*0.5, BUTTON_COLOUR_NORMAL);
    btn->shapes_normal_ID = shapes_2d_end();

    /* add text to button */
    /* TODO centering code for text */
    /* TODO instead of passing the raw string, add way to pass a "string instance"
     * which can specify the font and size of the string. */
    btn->text_ID = text_add_static_center_string(font_id, x, y+0.02, btn->text);

    return btn->ID;
}

void button_destroy(uint32_t ID)
{
    UNORDERED_VECTOR_FOR_EACH(&g_buttons, struct button_t, btn)
    {
        if(btn->ID == ID)
        {
            FREE(btn->text);
            shapes_2d_destroy(btn->shapes_normal_ID);
            unordered_vector_erase_element(&g_buttons, btn);
            return;
        }
    }
    
}

void button_destroy_all(void)
{
    UNORDERED_VECTOR_FOR_EACH(&g_buttons, struct button_t, btn)
    {
        FREE(btn->text);
    }
    unordered_vector_clear(&g_buttons);
}

struct button_t* button_collision(struct button_t* button, float x, float y)
{

    /* test specified button */
    if(button)
    {
        if(x > button->pos.x - button->size.x*0.5 && x < button->pos.x + button->size.x*0.5)
            if(y > button->pos.y - button->size.y*0.5 && y < button->pos.y + button->size.y*0.5)
                return button;
        return NULL;
    }
    
    /* test all buttons */
    {
        UNORDERED_VECTOR_FOR_EACH(&g_buttons, struct button_t, cur_btn)
        {
            if(x > cur_btn->pos.x - cur_btn->size.x*0.5 && x < cur_btn->pos.x + cur_btn->size.x*0.5)
                if(y > cur_btn->pos.y - cur_btn->size.y*0.5 && y < cur_btn->pos.y + cur_btn->size.y*0.5)
                    return cur_btn;
        }
    }
    return NULL;
}

EVENT_LISTENER3(on_mouse_clicked, char mouse_btn, double x, double y)
{
    struct button_t* button = button_collision(NULL, (float)x, (float)y);
    if(button)
    {
        printf("button \"%S\" clicked at %f,%f\n", button->text, x, y);
        EVENT_FIRE1(evt_button_clicked, button->text);
    }
}
