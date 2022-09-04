#include<gl/glew.h>
#include<glm.hpp>
#include<vector>
#include "helpers.h"
#include "Datatypes/Vertex.h"

namespace af {
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
        inline uint getIndexCount() { return indexCount;  }
        inline uint getVertexCount() { return vertexCount; }

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
