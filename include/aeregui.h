#pragma once

#include <string>
#include <memory>
#include "glm/glm.hpp"
#include <vector>
#include <unordered_map>

#if !defined(NAMESPACE_BEGIN) || defined(DOXYGEN_DOCUMENTATION_BUILD)
    #define NAMESPACE_BEGIN(name) namespace name {
#endif

#if !defined(NAMESPACE_END) || defined(DOXYGEN_DOCUMENTATION_BUILD)
    #define NAMESPACE_END(name) }
#endif

NAMESPACE_BEGIN(AereGui)

// each character's rendering information to put in the buffer
struct alignas(16) Character
{
    glm::vec4 vector; //xpos, ypos, width, height
    glm::vec4 col;
    int layer;
    float scale;
};

// font information
struct CharInfo
{
    int layer;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

struct gShader;

class GuiHelper
{
    public:
        // Initialise GUI.
        // The following ....Idx arguments are used to determine which index
        // each respective buffer will be bound.
        GuiHelper(const char* font,
                  uint32_t font_idx,
                  uint32_t screenSz_idx,
                  uint32_t text_idx);

        // Binds each respective buffer at their assigned index.
        // Only use when necessary.
        void bind();

        // Renders all GUI elements in 'data'.
        void render();

        // Clears render data.
        void clear();

        // functions for GLFW callbacks.
        void setMousePos(glm::vec2 pos);
        void setScreenSz(glm::ivec2 size);

        // Adds a text object.
        void addText(const char* text,
                     glm::vec2 pos, float scale, glm::vec4 col, bool center);

        // Adds a clickable text object.
        void addTextButton(const char* text, void(*f)(),
                           glm::vec2 pos, float scale, glm::vec4 col, bool center);
    private:
        // Render data. Holds all GUI elements in GPU-friendly form.
        struct {
            std::vector<AereGui::Character> text;
        } data;

        glm::vec2 mousePos;
        glm::ivec2 screenSz;

        // Buffer name/Buffer binding pair
        struct nameIdx
        {
            uint32_t buf;
            uint32_t bind;
        };

        // Uniform buffers
        struct
        {
            nameIdx screenSz;
        } ub;

        // Shader storage buffers
        struct
        {
            nameIdx text;
        } ssb;

        // Texture arrays
        struct
        {
            nameIdx font;
        } ta;

        struct
        {
            std::unique_ptr<gShader> text;
        } shaders;

        std::unordered_map<char, AereGui::CharInfo> charMap;

        float fontPx;

        // loads font. #TODO: only does ASCII characters, 0-128
        inline void loadFont(const char* font);
};


struct uniformInfo
{ 
    char * key;

    struct Value
    {
	    int32_t location;
	    uint32_t count;
    };
    Value value;
};

struct gShader
{
    uint32_t id;
    uniformInfo * uniforms;

    void use();
    uint32_t getLocation(const char * uniform_name);

    uint32_t fontPx = -1;
};

void createShader(gShader* shader, const std::string& vertexShader, const std::string& fragmentShader);

NAMESPACE_END(AereGui);
