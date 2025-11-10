#include <cstdio>
#include "instance.hpp"
#include <GLFW/glfw3.h>
static void glfw_error_callback(int error, const char* description)
{
    (void)error;
	std::fprintf(stderr, "GLFW error: %s\n", description);
}
static void GLAPIENTRY gl_error_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
{
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
	if(type == GL_DEBUG_TYPE_ERROR)
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
}
int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
    {
        const char *errorString;
        glfwGetError(&errorString);
        printf("GLFW initialization failed: %s\n", errorString);
        return 1;
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1440, 810, "rmGUI example", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    GLenum glewInitCode = glewInit();
    if(glewInitCode != GLEW_OK)
    {
        printf("GLEW initialization failed: %s\n", glewGetErrorString(glewInitCode));
        return 1;
    }
    glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, nullptr);

    rmGUI::Instance guiInstance;
    rmGUI::GUITree tree;

    guiInstance.windowSize = {1440, 810};
    guiInstance.Init();

    auto root = tree.GetRoot();
    root->data().color = {1.f, 1.f, 1.f, 1.f};
    root->data().pos = {0.f, 0.f};
    root->SetSizePixels({1440, 810});

    auto child1 = root->CreateChild();

    child1->data().color = {1.f, 0.f, 1.f, 1.f};
    child1->data().pos = {15, 15};
    child1->data().size = {150, 100};

    auto child2 = root->CreateChild();

    child2->data().color = {1.f, 0.f, 1.f, 1.f};
    child2->data().pos = {180, 15};
    child2->data().size = {150, 100};
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        guiInstance.DrawTree(tree);

        glfwSwapBuffers(window);
    }
    return 0;
}