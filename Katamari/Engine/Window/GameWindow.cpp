#include "GameWindow.h"

#include "..\Math\Algebra.h"
#include "..\Rendering\RenderManager.h"     // TODO should not have dependency here

// TODO move this somewhere better (InputManager)
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_callback(GLFWwindow* window, double x, double y);

// TODO temp hacky stuff, move it!
void MoveCamera(Vector3 localSpaceOffset)
{
    Matrix4x4 view = RenderManager::Singleton().GetView();
    view = Translation(localSpaceOffset)*view;
    RenderManager::Singleton().SetView(view);
}
// TODO temp hacky stuff, move it!
void RotateCamera(eAXIS axis, float degrees)
{
    Matrix4x4 view = RenderManager::Singleton().GetView();
    view = Rotation(degrees, axis)*view;
    RenderManager::Singleton().SetView(view);
}

void GameWindow::Setup(string name, int width, int height)
{
    m_name = name;
    m_width = width;
    m_height = height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetMouseButtonCallback(m_window, mouse_callback);
    glfwSetCursorPosCallback(m_window, cursor_callback);
}

void GameWindow::Destroy()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void GameWindow::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

bool GameWindow::ShouldClose()
{
    return glfwWindowShouldClose(m_window);
}

// TODO temp hacky stuff, move it!
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float rotAmt = 1.0f;
    float transAmt = 0.2f;
    switch (key)
    {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_UP:
        RotateCamera(AXIS_X, -rotAmt);
        break;
    case GLFW_KEY_DOWN:
        RotateCamera(AXIS_X, rotAmt);
        break;
    case GLFW_KEY_LEFT:
        RotateCamera(AXIS_Y, -rotAmt);
        break;
    case GLFW_KEY_RIGHT:
        RotateCamera(AXIS_Y, rotAmt);
        break;
    case GLFW_KEY_W:
        MoveCamera(Vector3(0, 0, transAmt));
        break;
    case GLFW_KEY_S:
        MoveCamera(Vector3(0, 0, -transAmt));
        break;
    case GLFW_KEY_A:
        MoveCamera(Vector3(transAmt, 0, 0));
        break;
    case GLFW_KEY_D:
        MoveCamera(Vector3(-transAmt, 0, 0));
        break;
    };
}

bool mouseDragging = false;
double prevX;
double prevY;

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    printf("Button: %d   Actions: %d\n", button, action);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            mouseDragging = true;
            glfwGetCursorPos(window, &prevX, &prevY);
        }
        else
        {
            mouseDragging = false;
        }
    }
}

static void cursor_callback(GLFWwindow* window, double x, double y)
{
    double rotAmt = 0.07;
    if (mouseDragging)
    {
        double deltaX = x - prevX;
        double deltaY = y - prevY;
        RotateCamera(AXIS_Y, deltaX*rotAmt);
        RotateCamera(AXIS_X, deltaY*rotAmt);
    }
    prevX = x;
    prevY = y;
}