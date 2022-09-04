#include<gl/glew.h>
#include<glm.hpp>
#include<vector>
#include "helpers.h"

namespace af {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        Vertex(vec3 pos, vec2 uv)
            : position(pos), uv(uv) {}
    };

    class Mesh {
    private:
        Vertex* vertices;
        GLuint* indices;
        uint indexCount;
        uint indexDrawCount;
        uint vertexCount;
        uint vertexDrawCount; // possibly redundant

        GLuint vbo;
        GLuint ebo;
        GLuint vao;
    public:
        inline int getHandle() { return vao; }
        inline Vertex* getVertices() { return vertices; }
        inline uint* getIndices() { return indices; }

        Mesh() : vertexCount(0), indexCount(0), vertexDrawCount(0), indexDrawCount(0), vertices(nullptr), 
            indices(nullptr), vbo(0), ebo(0), vao(0) {};

        /// <summary>
        /// This mesh will take ownership of the verts and indices you pass in.
        /// 
        /// Set stream=true if you are going to update the data every frame with UpdateBuffers
        /// </summary>
        Mesh(Vertex* data, uint numVerts, uint* indices, uint numIndices, bool stream = false);

        /// <summary>
        /// Uploads ALL the current verts and indices to the GPU. 
        /// </summary>
        void updateBuffers();

        /// <summary>
        /// Uploads the first however many verts and indices to the GPU, and sets new vertex/index counts
        ///
        /// newVertexCount and newIndexCount MUST be less than the total number of vertices and indices on this mesh object.
        /// </summary>
        void updateBuffers(uint newVertexCount, uint newIndexCount);

        void draw();

        ~Mesh();
    };
}
