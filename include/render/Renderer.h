#ifndef RENDERER_H
#define RENDERER_H
#include "scene/Scene.h"
#include <GL/glew.h>
#include "light/DirectionalLight.h"
#include "material/Material.h"
#include "scene/OctreeNode.h"
#include <vector>
#include <list>

using namespace std;

struct dirLightsChunk{
  struct dirLight lights[10];
  GLint numDirLights;
};

struct pLightsChunk{
  struct pLight lights[10];
  GLint numPLights;
};

struct bufferIndices{
	GLuint materialIndex;
	GLuint visible;
	GLfloat distanceToCamera; 
	GLuint stride;
};

struct indirect{
    GLuint  count;
    GLuint  instanceCount;
    GLuint  firstIndex;
    GLint   baseVertex;
    GLuint  baseInstance;
};

struct bufferObjects{
	MaterialType type;
	GLuint bufferIndices;
	GLuint modelMatrices;
	GLuint indirectBuffer;
	GLuint drawIDBuffer;
};

struct globalBufferObjects{
    GLuint pointLights;
	GLuint directionalLights;
	GLuint globalMatrices;
	GLuint ambientLight;
	GLuint materials;
	GLuint vertexBuffer;
	GLuint elementBuffer;
	GLuint normalBuffer;
};

enum {VERTICES,NORMALS,ELEMENTS};
enum {MODEL_MATRIX,BUFFER_INDICES,INDIRECT,DRAWID};

typedef struct globalBufferObjects* GlobalBufferObjects;
typedef struct bufferObjects* BufferObjects;

class Renderer{
private:
	GLuint vao;
	BufferObjects buffers[NUM_MATERIAL_TYPES];
	GlobalBufferObjects globalBuffers;
	vector<list<Object3D*> > geometryGroups;
	GLuint calculateGlobalMatrices(Scene* scene);
	GLuint calculateDirectionalLights(Scene* scene);
	GLuint calculateAmbientLights(Scene* scene);
	GLuint calculatePointLights(Scene* scene);
	void setMaterialUniforms(Material* material, Scene* scene);
	BufferObjects createEmptyBufferObject();
public:
	Renderer();
	void render(Scene* scene);
	GLuint makeBuffer(GLenum target, void* bufferData, GLsizei bufferSize,GLenum usage = GL_STATIC_DRAW);
	GLuint makeUBO(void* bufferData, GLsizei bufferSize);
	GLuint makePointBuffer(GLenum target, void* bufferData, GLsizei bufferSize);
	void renderOctreeNode(OctreeNode* node,Scene* scene);
	BufferObjects* getBuffers();
	GLuint* createGeometryBuffers(Scene* scene);
	GLuint createMaterialBuffer(Scene* scene);
	GLuint* createObjectBuffers(list<Object3D*> objectList);
	void renderMultiDraw(Scene* scene);
	void updateModelMatrices(GLuint modelMatricesBuffer,GLuint indirectBuffer,GLuint bufferIndices,list<Object3D*> objects);
};

#endif