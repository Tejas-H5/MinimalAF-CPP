#include<gl/glew.h>
#include<glm.hpp>
#include<vector>
#include "helpers.h"

namespace af {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;
    };

    class Mesh {
    private:
        Vertex* vertices;
        GLuint* indices;
        uint indexCount;
        uint indexDrawCount;
        uint vertexCount;
        uint vertexDrawCount;

        GLuint vbo;
        GLuint ebo;
        GLuint vao;
    public:
        inline int getHandle() { return vao; }
        inline Vertex* getVertices() { return vertices; }
        inline uint* getIndices() { return indices; }

        /// <summary>
        /// Set stream=true if you are going to update the data every frame with UpdateBuffers
        /// </summary>
        Mesh(Vertex* data, uint numVerts, uint* indices, uint numIndices, bool stream = false);

        /// <summary>
        /// Uploads ALL the current verts and indices to the GPU. 
        /// </summary>
        void UpdateBuffers();

        /// <summary>
        /// Uploads the first however many verts and indices to the GPU, and sets new vertex/index counts
        ///
        /// newVertexCount and newIndexCount MUST be less than the total number of vertices and indices on this mesh object.
        /// </summary>
        void UpdateBuffers(uint newVertexCount, uint newIndexCount);

        void Draw();

        ~Mesh();
    };
}
