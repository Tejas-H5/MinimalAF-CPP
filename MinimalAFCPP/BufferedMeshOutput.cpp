#include "BufferedMeshOutput.h"

af::BufferedMeshOutput::BufferedMeshOutput(int vertexBufferSize, int indexBufferSize) :
	currentIndexIndex(0), currentVertexIndex(0) {
	// they must be multiples of 3
	vertexBufferSize = (1 + vertexBufferSize / 3) * 3;
	indexBufferSize = (1 + indexBufferSize / 3) * 3;

	backingMesh = new Mesh(new Vertex[vertexBufferSize], vertexBufferSize, new uint[indexBufferSize], indexBufferSize, true);
}

af::BufferedMeshOutput::~BufferedMeshOutput() {
	delete backingMesh;
}

uint af::BufferedMeshOutput::addVertex(Vertex v) {
	backingMesh->getVertices()[currentVertexIndex] = v;
	return currentVertexIndex++;
}

void af::BufferedMeshOutput::makeTriangle(uint v1, uint v2, uint v3) {
	backingMesh->getIndices()[currentIndexIndex] = v1;
	backingMesh->getIndices()[currentIndexIndex + 1] = v2;
	backingMesh->getIndices()[currentIndexIndex + 2] = v3;

	currentIndexIndex += 3;
}

void af::BufferedMeshOutput::flush() {
	if (currentIndexIndex == 0)
		return;

	// actually draw what we have so far
	backingMesh->updateBuffers(currentVertexIndex, currentIndexIndex);
	backingMesh->draw();

	currentVertexIndex = 0;
	currentIndexIndex = 0;
}

bool af::BufferedMeshOutput::flushIfRequired(int numIncomingVerts, int numIncomingIndices) {
	if (currentIndexIndex + numIncomingIndices >= backingMesh->getIndexCount()  ||
		currentVertexIndex + numIncomingVerts >= backingMesh->getVertexCount()) {
#ifdef DEBUG
		if (currentIndexIndex + numIncomingIndices >= backingMesh.Indices.Length) {
			TimesIndexThresholdReached++;
		}
		else if (currentVertexIndex + numIncomingVerts >= backingMesh.Vertices.Length) {
			TimesVertexThresholdReached++;
		}
#endif

		flush();
		return true;
	}

	return false;
	
}
