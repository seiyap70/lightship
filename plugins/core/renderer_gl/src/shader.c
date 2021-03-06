#include "plugin_renderer_gl/config.h"
#include "plugin_renderer_gl/shader.h"
#include "util/file.h"
#include "util/memory.h"
#include "util/log.h"
#include "util/string.h"
#include <stdio.h>
#include <string.h>

#include "GL/glew.h"

/* ------------------------------------------------------------------------- */
void
check_shader(GLuint shader_ID)
{
    GLint result = GL_FALSE;
    int info_log_length;
    char* message = NULL;

    glGetShaderiv(shader_ID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_ID, GL_INFO_LOG_LENGTH, &info_log_length);
    message = (char*)MALLOC(info_log_length);
    glGetShaderInfoLog(shader_ID, info_log_length, NULL, message);
    if(result == GL_FALSE)
        llog(LOG_ERROR, PLUGIN_NAME, 1, message);
    FREE(message);
}

/* ------------------------------------------------------------------------- */
void
compile_shader(GLuint shader_ID, const char* file_name)
{
    GLchar* code;

    /* copy file into memory */
    file_load_into_memory(file_name, (void**)&code, 0);
    if(!code)
    {
        llog(LOG_ERROR, PLUGIN_NAME, 3, "failed to load file \"", file_name, "\"");
        return;
    }

    /* compile */
    llog(LOG_INFO, PLUGIN_NAME, 3, "compiling shader: \"", file_name, "\"");
    glShaderSource(shader_ID, 1, (const GLchar**)&code, NULL);
    glCompileShader(shader_ID);
    
    free_file(code);
}

/* ------------------------------------------------------------------------- */
void
check_program(GLuint program_ID)
{
    GLint result = GL_FALSE;
    int info_log_length;
    char* message = NULL;
    
    glGetProgramiv(program_ID, GL_LINK_STATUS, &result);
    glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &info_log_length);
    message = (char*)MALLOC(info_log_length);
    glGetProgramInfoLog(program_ID, info_log_length, NULL, message);
    if(result == GL_FALSE)
        llog(LOG_ERROR, PLUGIN_NAME, 1, message);
    FREE(message);
}

/* ------------------------------------------------------------------------- */
GLuint
shader_load(const char* name)
{
    char* vertex_shader;
    char* fragment_shader;
    GLuint result;

    vertex_shader = cat_strings(2, name, ".vsh");
    fragment_shader = cat_strings(2, name, ".fsh");
    result = load_shader_pair(vertex_shader, fragment_shader);
    
    free_string(vertex_shader);
    free_string(fragment_shader);
    return result;
}

/* ------------------------------------------------------------------------- */
GLuint
load_shader_pair(const char* vertex_shader, const char* fragment_shader)
{
    GLuint program_ID;
    GLuint vsh_ID;
    GLuint fsh_ID;
    
    /* compile shaders */
    vsh_ID = glCreateShader(GL_VERTEX_SHADER);
    fsh_ID = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(vsh_ID, vertex_shader);
    check_shader(vsh_ID);
    compile_shader(fsh_ID, fragment_shader);
    check_shader(fsh_ID);

    /* link program */
    llog(LOG_INFO, PLUGIN_NAME, 1, "linking program");
    program_ID = glCreateProgram();
    glAttachShader(program_ID, vsh_ID);
    glAttachShader(program_ID, fsh_ID);
    glLinkProgram(program_ID);
    check_program(program_ID);
    
    glDeleteShader(vsh_ID);
    glDeleteShader(fsh_ID);

    return program_ID;
}
