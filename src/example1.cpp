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
    /*
    button->setPos({200, 200});
    button->setSize({200, 50});
    uictx.addWidget(button);
    */

    Widget * input = new TextInput("Enter text here :-)");
    input->assignTexture("textinput");
    input->setPos({500, 200});
    input->setSize({200, 32});
    input->setPadding(4);
    uictx.addWidget(input);

    Widget * window1 = new Window("My amazing gui window!!!!");
    window1->assignTexture("window");
    window1->setSize({400, 200});
    window1->setPos({400, 600});
    window1->setPadding(8);
    uictx.addWidget(window1);

    Widget * cinput = new TextInput("Enter text:");
    cinput->assignTexture("textinput");
    cinput->setPadding(4);
    /*
    cinput->setPos({10, 40});
    cinput->setSize({250, 32});
    window1->addChild(cinput);
    */

    Widget * cb = new Button("send!! (not real)", [](){printf("sent lololo\n");});
    cb->assignTexture("button");
    /*
    cb->setPos({10, 80});
    cb->setSize({200, 32});
    window1->addChild(cb);
    */

    Row * row1 = new Row(50);
    window1->addChild(row1);
    row1->addCol(cb, 100);
    row1->addCol(cinput, 200);
    row1->addCol(button, 150);
    row1->setPos({10, 50});

    Widget * window2 = new Window("Some boxes");
    window2->assignTexture("window");
    window2->setSize({800, 400});
    window2->setPos({200, 100});
    window2->setPadding(8);

    Row * window2row = new Row(352);
    window2->addChild(window2row);
    window2row->setPos({0, 32});

    Box * box1 = new Box();
    box1->assignTexture("box1");
    window2row->addCol(box1, 300);

    Box * box2 = new Box();
    box2->assignTexture("box2");
    box2->setFlags(0);
    window2row->addCol(box2, 500);

    Row * row2 = new Row(40);
    box2->addChild(row2);
    for (int i = 0; i < 15; i++)
    {
        ListItem* item1 = new ListItem("lollipop", "A weird lollipop");
        item1->assignTexture("listitem");
        row2->addCol(item1, 72);

        ListItem* item2 = new ListItem("tennis", "A tennis ball");
        item2->assignTexture("listitem");
        row2->addCol(item2, 40);
    }
    //row2->setPos({10, 50}); // why does this fk it up?

    uictx.addWidget(window2);

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
    unsigned char* im = stbi_load("resources/windows.jpg", &width, &height, &channels, 4);

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
    pos.y *= -1;
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
