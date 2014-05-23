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

class Renderer{
private:
	GLuint vao;
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
};

#endif