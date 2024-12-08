#include "aeregui.h"
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "stdio.h"

#include "stb_image.h"

GLFWwindow* window;
using namespace AereGui;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    printf("%s \n", message);
}

int SCR_WIDTH;
int SCR_HEIGHT;

Screen* screen;
int main()
{

    glfwInit();
    // OpenGL version
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window 
    window = glfwCreateWindow(1920, 1080, "aeregui example 1", nullptr, nullptr);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    /*
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    */

    // Load OpenGL
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    glfwGetWindowSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Setup debug messages
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(message_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    UIContext uictx(window);
    uictx.loadFont("resources/fonts/unifont.ttf");
    uictx.preloadTextures("resources/sprites");

    Widget * button = new Button("play now!!!!", []()
        {
            printf("button pressed!!\n");
        }
    );
    button->assignTexture("button");
    button->setPos({200, 200});
    button->setSize({200, 50});
    uictx.addWidget(button);

    Widget * input = new TextInput("Enter text here :-)");
    input->assignTexture("textinput");
    input->setPos({500, 200});
    input->setSize({200, 32});
    input->setPadding(4);
    uictx.addWidget(input);

    Widget * awindow = new Window("My amazing gui window!!!!");
    awindow->assignTexture("window");
    awindow->setSize({400, 200});
    awindow->setPos({400, 500});
    awindow->setPadding(8);
    uictx.addWidget(awindow);

    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    printf("%f, %f\n", xscale, yscale);
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    screen = uictx.screenPtr();

    //background
    int width, height, channels;
    unsigned char* im = stbi_load("resources/worstwallpaper.png", &width, &height, &channels, 4);

    GLuint bg;
    glCreateTextures(GL_TEXTURE_2D, 1, &bg);

    glTextureStorage2D(bg, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(bg, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, im);

    stbi_image_free(im);

    GLuint vsh = glCreateShader(GL_VERTEX_SHADER); GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vsh_src = R"#(
#version 460 core
out vec2 uv;
void main() {
    ivec2 quad[6] = ivec2[6](
        ivec2(0, 0),
        ivec2(1, 0),
        ivec2(0, 1),
        ivec2(1, 1),
        ivec2(0, 1),
        ivec2(1, 0)
    ); 
    uv = quad[gl_VertexID];
    vec2 pos = quad[gl_VertexID] * 2 - 1;
    gl_Position = vec4(pos, 0.0, 1.0);
}
    )#";
    const char* fsh_src = R"#(
#version 460 core
out vec4 frag;
in vec2 uv;
layout (location = 0) uniform sampler2D bg;
void main() {
    frag = texture(bg, uv);
}
    )#";

    glShaderSource(vsh, 1, (const GLchar**)&vsh_src, NULL);
    glShaderSource(fsh, 1, (const GLchar**)&fsh_src, NULL);
    glCompileShader(vsh); glCompileShader(fsh);

    int logLength;
    glGetShaderiv(vsh, GL_INFO_LOG_LENGTH, &logLength);
    
    GLuint bg_shader = glCreateProgram();
    glAttachShader(bg_shader, vsh); glAttachShader(bg_shader, fsh);
    glLinkProgram(bg_shader);

    glDeleteShader(vsh); glDeleteShader(fsh);

    glfwSetFramebufferSizeCallback(window,
        [](GLFWwindow* window, int width, int height)
        {
            screen->framebufferSizeCallback(width, height);
        }
    );

    glfwSetCursorPosCallback(window,
        [](GLFWwindow* window, double x, double y)
        {
            screen->cursorPosCallback(x, y);
        }
    );

    glfwSetMouseButtonCallback(window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            screen->mouseButtonCallback(button, action);
        }
    );

    glfwSetKeyCallback(window,
        [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            screen->keyCallback(key, scancode, action, mods);
        }
    );

    glfwSetCharCallback(window,
        [](GLFWwindow* window, unsigned int codepoint)
        {
            screen->charCallback(codepoint);
        }
    );

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(bg_shader);
        glBindTextureUnit(0, bg);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        uictx.render();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
