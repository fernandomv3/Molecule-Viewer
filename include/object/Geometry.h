#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <GL/glew.h>
//Geometry owns Buffers (Vertex, Normal, UVs)
//Future add -> facesArray, bounding box, normals array
struct bounds{
	GLfloat x[2];
	GLfloat y[2];
	GLfloat z[2];
};

typedef struct bounds* BoundingBox;

class Geometry{
private:
	GLfloat* vertices;
	int numVertices;
	GLushort* elements;
	int numElements;
	GLfloat* normals;
	int numNormals;
	int numMeshes;
	GLuint vertexBuffer;
	GLuint elementBuffer;
	GLuint normalBuffer;
	int sceneIndex;
	BoundingBox boundingBox;
	~Geometry();
public:
	Geometry();
	int getNumElements();
	GLushort* getElements();
	void setElements(GLushort* elements, int numElements);
	GLfloat* getVertices();
	void setVertices(GLfloat* vertices, int numVertices);
	int getNumVertices();
	GLfloat* getNormals();
	void setNormals(GLfloat* normals, int numNormals);
	int getNumNormals();
	void setNumMeshes(int numMeshes);
	int getNumMeshes();
	void requestDelete();
	GLuint getVertexBuffer();
	void setVertexBuffer(GLuint vertexBuffer);
	GLuint getElementBuffer();
	GLuint getNormalBuffer();
	void setNormalBuffer(GLuint normalBuffer);
	void setElementBuffer(GLuint elementBuffer);
	void loadDataFromFile(const char* filename);
	BoundingBox getBoundingBox();
	static Geometry* generateCubeGeometry(float size);
	static Geometry* generateCubeWireframe(float size);
	int getSceneIndex();
	void setSceneIndex(int index);
};

#endif
 