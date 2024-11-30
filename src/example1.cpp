#include "aeregui.h"
#include "GLFW/glfw3.h"
#include "glad/gl.h"
#include "stdio.h"

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
    uictx.addWidget(input);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    screen = uictx.screenPtr();
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

        uictx.render();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
