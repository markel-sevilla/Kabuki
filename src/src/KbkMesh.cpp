/**
 * @file    KTexture.h
 * @brief   wrap up for the texture 
 *
 * @details  Has basic texture info
 *          
 *
 * @author  Markel Sevilla
 * @date    2026-07-25
 * @version 1.0
 */
#include "KbkTexture.h"

namespace Graphics
{
   
    CS562::Mesh::Mesh()
    {
        mVertices.clear();
        mIndices.clear();
        mTex = nullptr;

        // Delete the VAO
        glDeleteVertexArrays(1, &mVAO);

        // Delete the VBO and EBO
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);

    }

    CS562::Mesh::~Mesh()
    {
        delete mTex;
        mVertices.clear();
        mIndices.clear();
    }

    void CS562::Mesh::GenerateMesh()

    {
        //vertex array bind
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        //vertex buffer bind
        glGenBuffers(1, &mVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

        ////element buffer objects
        glGenBuffers(1, &mEBO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, mNormal)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, mTangent)));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, mBiTan)));
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, mUV)));
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, mColor)));
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, mAvgNormal)));
        glEnableVertexAttribArray(6);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

}