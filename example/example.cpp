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
    root->gData().color = {1.f, 1.f, 1.f, 1.f};
    root->data.absPos = {0.f, 0.f};
    root->data.absSize = {1440, 810};

    auto child1 = root->CreateChild();

    child1->gData().color = {1.f, 0.f, 1.f, 1.f};
    child1->data.absPos = {10, 10};
    child1->data.relPos = {0.0f, 0.0f};
    child1->data.relSize = {0.25f, 1.0f};
    child1->data.absSize = {-15, -20};

    child1->data.absCornerSize[rmGUI::Node::TOP_LEFT] = {50, 50};
    child1->gData().SetCornerShape(rmGUI::Node::TOP_LEFT_MASK, rmGUI::CornerShape::ROUND);

    child1->data.absCornerSize[rmGUI::Node::BOTTOM_LEFT] = {50, 50};
    child1->gData().SetCornerShape(rmGUI::Node::BOTTOM_LEFT_MASK, rmGUI::CornerShape::ROUND);

    child1->data.absCornerSize[rmGUI::Node::BOTTOM_RIGHT] = {50, 50};
    child1->gData().SetCornerShape(rmGUI::Node::BOTTOM_RIGHT_MASK, rmGUI::CornerShape::NOTCH);

    auto child2 = root->CreateChild();

    child2->gData().color = {1.f, 0.f, 1.f, 1.f};
    child2->data.relPos = {0.25f, 0.2f};
    child2->data.absPos = {5, 5};
    child2->data.absSize = {-15, -15};
    child2->data.relSize = {0.75f, 0.8f};

    child2->data.absCornerSize[rmGUI::Node::BOTTOM_RIGHT] = {50, 50};
    child2->gData().SetCornerShape(rmGUI::Node::BOTTOM_RIGHT_MASK, rmGUI::CornerShape::ROUND);

    child2->data.absCornerSize[rmGUI::Node::TOP_LEFT] = {50, 50};
    child2->gData().SetCornerShape(rmGUI::Node::TOP_LEFT_MASK, rmGUI::CornerShape::SCOOP);
    
    child2->data.absCornerSize[rmGUI::Node::TOP_RIGHT] = {200, 50};
    child2->gData().SetCornerShape(rmGUI::Node::TOP_RIGHT_MASK, rmGUI::CornerShape::BEVEL);

    child2->data.absCornerSize[rmGUI::Node::BOTTOM_LEFT] = {50, 50};
    child2->gData().SetCornerShape(rmGUI::Node::BOTTOM_LEFT_MASK, rmGUI::CornerShape::NOTCH);

    auto child3 = root->CreateChild();

    child3->gData().color = {1.f, 0.f, 1.f, 1.f};
    child3->data.relPos = {0.25f, 0.0f};
    child3->data.absPos = {5, 10};
    child3->data.absSize = {-15, -15};
    child3->data.relSize = {0.75f, 0.2f};

    child3->data.absCornerSize[rmGUI::Node::TOP_RIGHT] = {50, 50};
    child3->gData().SetCornerShape(rmGUI::Node::TOP_RIGHT_MASK, rmGUI::CornerShape::ROUND);

    child3->data.absCornerSize[rmGUI::Node::BOTTOM_LEFT] = {50, 50};
    child3->gData().SetCornerShape(rmGUI::Node::BOTTOM_LEFT_MASK, rmGUI::CornerShape::SCOOP);

    child3->data.absCornerSize[rmGUI::Node::BOTTOM_RIGHT] = {200, 50};
    child3->gData().SetCornerShape(rmGUI::Node::BOTTOM_RIGHT_MASK, rmGUI::CornerShape::BEVEL);

    /*auto child4 = child2->CreateChild();
    child4->data.absPos = {50,50};
    child4->data.absSize = {200,200};
    child4->gData().color = {1,0,0,1};

    auto child5 = child2->CreateChild();
    child5->data.absPos = {300,50};
    child5->data.absSize = {200,200};
    child5->gData().color = {1,0,0,1};*/
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        guiInstance.DrawTree(tree);

        glfwSwapBuffers(window);
    }
    return 0;
}