#pragma once

#include "Mesh.h"
#include "Datatypes/Vertex.h"

namespace af {
	class BufferedMeshOutput {
	private: 
		Mesh* backingMesh;
		uint currentVertexIndex, currentIndexIndex;

	public:
		int timesVertexThresholdReached = 0;
		int timesIndexThresholdReached = 0;

		~BufferedMeshOutput();

		inline Vertex getVertex(uint i) { return backingMesh->getVertices()[i]; }
		inline uint getIndex(uint i) { return backingMesh->getIndices()[i]; }
		inline uint currentV() { return currentVertexIndex; }
		inline uint currentI() { return currentIndexIndex; }

		void init(int vertexBufferSize, int indexBufferSize);

		uint addVertex(Vertex v);
		void makeTriangle(uint v1, uint v2, uint v3);

		void flush();
		bool flushIfRequired(int numIncomingVerts, int numIncomingIndices);
	};
}