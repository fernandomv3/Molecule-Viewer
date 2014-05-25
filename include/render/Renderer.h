#ifndef RENDERER_H
#define RENDERER_H
#include "scene/Scene.h"
#include <GL/glew.h>
#include "light/DirectionalLight.h"
#include "material/Material.h"
#include "scene/OctreeNode.h"

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
	GLuint stride[2];
};

struct indirect{
    uint  count;
    uint  instanceCount;
    uint  firstIndex;
    uint  baseVertex;
    uint  baseInstance;
};

struct bufferObjects{
	MaterialType type;
	GLuint pointLights;
	GLuint directionalLights;
	GLuint ambientLight;
	GLuint materials;
	GLuint bufferIndices;
	GLuint modelMatrices;
	GLuint vertexBuffer;
	GLuint elementBuffer;
	GLuint normalBuffer;
	GLuint indirectBuffer;
};

enum {VERTICES,NORMALS,ELEMENTS};
enum {MODEL_MATRIX,BUFFER_INDICES,INDIRECT};

typedef struct bufferObjects* BufferObjects;

class Renderer{
private:
	GLuint vao;
	BufferObjects buffers;
	void calculateGlobalMatrices(Scene* scene);
	void calculateDirectionalLights(Scene* scene);
	void calculateAmbientLights(Scene* scene);
	void calculatePointLights(Scene* scene);
	void setMaterialUniforms(Material* material, Scene* scene);
public:
	Renderer();
	void render(Scene* scene);
	GLuint makeBuffer(GLenum target, void* bufferData, GLsizei bufferSize);
	GLuint makeUBO(void* bufferData, GLsizei bufferSize);
	GLuint makePointBuffer(GLenum target, void* bufferData, GLsizei bufferSize);
	void renderOctreeNode(OctreeNode* node,Scene* scene);
	BufferObjects getBuffers();
	GLuint* createGeometryBuffers(Scene* scene);
	GLuint createModelMatrixBuffer(Scene* scene);
	GLuint createMaterialBuffer(Scene* scene);
	GLuint createBufferIndicesBuffer();
	GLuint createIndirectBuffer();
	GLuint* createObjectBuffers();
};

#endif