#pragma once

#include <GL/glew.h>
#include "tree.hpp"
#include "shader.hpp"

namespace rmGUI
{
    class Instance
    {
        static constexpr GLuint SSBObinding = 0;
    protected:
        GLuint VAO = 0;
        GLuint VBO = 0; // in VBO we store draw-ordered indices to SSBO. Actual instance data is in SSBO
        GLuint SSBO = 0; // in SSBO we store actual unordered instance data
        ShaderProgram shader;
        virtual void InitVAO()
        {
            glCreateVertexArrays(1, &VAO);

            glEnableVertexArrayAttrib(VAO, 0);

            glVertexArrayAttribIFormat(VAO, 0, 1, GL_UNSIGNED_INT, 0);
            glVertexArrayAttribBinding(VAO, 0, 0);

            glVertexArrayBindingDivisor(VAO, 0, 1);
        }
        void InitBuffers()
        {
            glCreateBuffers(2, &VBO);
        }
        void BindVertexArrayVertexBuffer()
        {
            glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(GLuint));
        }
        void InitShader()
        {
            Shader vs = Shader::FromFile(GL_VERTEX_SHADER, "out/shaders/guiElem.vert.glsl");
            Shader fs = Shader::FromFile(GL_FRAGMENT_SHADER, "out/shaders/guiElem.frag.glsl");
            shader = ShaderProgram({vs, fs});
            //shader.debugProgram();
        }
    public:
        vec<int, 2> windowSize;
        void Init()
        {
            if(!VAO)
                InitVAO();
            InitBuffers();
            BindVertexArrayVertexBuffer();
            if(!shader.GetID())
                InitShader();
        }
        void DrawTree(GUITree& tree)
        {
            // set indexBufferData
            tree.PrepareRenderData();

            // set data in buffers
            const auto &nodeBufferData = tree.GetNodeBufferData();
            const auto &indexBufferData = tree.GetIndexBufferData();
            glNamedBufferData(SSBO, nodeBufferData.size() * sizeof(GraphicNodeProperties), nodeBufferData.data(), GL_DYNAMIC_DRAW);
            glNamedBufferData(VBO, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_DYNAMIC_DRAW);

            // setup draw
            glBindVertexArray(VAO);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBObinding, SSBO);
            shader.Use();

            // set window size uniform
            glUniform2i(0, windowSize.x, windowSize.y);


            // draw
            glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, indexBufferData.size());
        }
    };
}