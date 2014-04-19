#include "render/Renderer.h"
#include <list>
#include "object/Object3D.h"
#include <cstdlib>
#include "object/Mesh.h"
#include "scene/Scene.h"
#include "material/PointMaterial.h"

Renderer::Renderer(){
	this->vao=0;
}

GLuint Renderer::makeBuffer(GLenum target, void* bufferData, GLsizei bufferSize){
	GLuint buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(target,buffer);
	glBufferData(target,bufferSize,bufferData, GL_STATIC_DRAW);
	return buffer;
}
GLuint Renderer::makeUBO(void* bufferData, GLsizei bufferSize){
	GLuint buf;
	glGenBuffers(1,&buf);
	glBindBuffer(GL_UNIFORM_BUFFER,buf);
	glBufferData(GL_UNIFORM_BUFFER,bufferSize,bufferData, GL_STREAM_DRAW);
	return buf;
}

GLuint Renderer::makePointBuffer(GLenum target, void* bufferData, GLsizei bufferSize){
	GLuint buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(target,buffer);
	glBufferData(target,bufferSize,bufferData, GL_STREAM_DRAW);
	return buffer;
}

void Renderer::calculateDirectionalLights(Scene* scene){
	int bufferSize = sizeof(struct dirLightsChunk);
	if(scene->getDirectionalLightsUBO() == 0){
		GLuint buf = makeUBO(NULL, bufferSize);
		scene->setDirectionalLightsUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			1,//binding point
			buf,//data
			0,//offset
			bufferSize//size in bytes
		);
	}
	GLint numLights = scene->getDirectionalLights().size();
	list<DirectionalLight*> lights = scene->getDirectionalLights();
	list<DirectionalLight*>::iterator itLights = lights.begin();
	list<DirectionalLight*>::iterator endLights = lights.end();
	struct dirLightsChunk chunk;
	chunk.numDirLights = numLights;
	for(int i=0;itLights != endLights && i<10 ;itLights++ , i++){
		DirLight light = (*itLights)->getAsStruct(scene->getCamera());
		memcpy(&(chunk.lights[i]),light,sizeof(struct dirLight));
		delete light;
	}

	glBindBuffer(GL_UNIFORM_BUFFER,scene->getDirectionalLightsUBO());
	glBufferSubData(GL_UNIFORM_BUFFER,0,bufferSize,&chunk);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::calculatePointLights(Scene* scene){
	//static GLuint bufPoint =0;
	int bufferSize = sizeof(struct pLightsChunk);
	if(scene->getPointLightsUBO() == 0){
		GLuint buf = makeUBO(NULL, bufferSize);
		scene->setPointLightsUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			3,//binding point
			buf,//data
			0,//offset
			bufferSize//size in bytes
		);
	}
	GLint numLights = scene->getPointLights().size();
	list<PointLight*> lights = scene->getPointLights();
	list<PointLight*>::iterator itLights = lights.begin();
	list<PointLight*>::iterator endLights = lights.end();
	struct pLightsChunk chunk;
	chunk.numPLights = numLights;

	/*PointMaterial pMat = PointMaterial();
	pMat.getDiffuseColor()->setRGB(0.0,0.0,0.0);
	if(bufPoint == 0){
		bufPoint = this->makePointBuffer(
			GL_ARRAY_BUFFER,
			NULL,
			3 * sizeof(GLfloat)
		);
	}
	glBindBuffer(GL_ARRAY_BUFFER,bufPoint);
	glVertexAttribPointer(
		pMat.getProgram()->getAttrPosition(),//attribute from prgram(position)
		3,//number of components per vertex
		GL_FLOAT,//type of data
		GL_FALSE,//normalized
		0,//separation between 2 values
		(void*)0 //offset
	);
	glEnableVertexAttribArray(pMat.getProgram()->getAttrPosition());
	glUseProgram(pMat.getProgram()->getProgram());
	GLfloat* diffuseColor = pMat.getDiffuseColor()->getAsArray();
	glUniform4fv(
		pMat.getProgram()->getUniforms()->unifDiffuseColor,
		1,
		diffuseColor
	);
	delete diffuseColor;
	GLfloat pos[3];*/

	for(int i=0;itLights != endLights && i<10 ;itLights++ , i++){
		PLight light = (*itLights)->getAsStruct(scene->getCamera());
		memcpy(&(chunk.lights[i]),light,sizeof(struct pLight));
		delete light;
		/*pos[0] = (*itLights)->getPosition()->getX();
		pos[1] = (*itLights)->getPosition()->getY();
		pos[2] = (*itLights)->getPosition()->getZ();
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(GLfloat)*3,&pos);
		glDrawArrays(GL_POINTS,0,1);*/
	}
	/*glDisableVertexAttribArray(pMat.getProgram()->getAttrPosition());
	delete pMat;
	delete[] pos;*/
	glBindBuffer(GL_UNIFORM_BUFFER,scene->getPointLightsUBO());
	glBufferSubData(GL_UNIFORM_BUFFER,0,bufferSize,&chunk);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::calculateAmbientLights(Scene* scene){
	if(scene->getAmbientLightUBO() == 0){
		GLfloat* data = scene->getAmbientLight()->getColor()->getAsArray();
		GLuint buf = makeUBO(data,sizeof(GLfloat)*4);
		scene->setAmbientLightUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			2,//binding point
			buf,//data
			0,//offset
			sizeof(GLfloat) * 4//size in bytes
		);
		delete[] data;
	}
}

void Renderer::calculateGlobalMatrices(Scene* scene){
	//update UBO TODO: update world matrix only as projection never changes
	scene->getCamera()->updateWorldMatrix();
	GLfloat* data = scene->getCamera()->getMatricesArray();
	if(scene->getCamera()->getMatricesUBO()==0){	
		GLuint buf = makeUBO(data,sizeof(GLfloat)*32);
		scene->getCamera()->setMatricesUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			0,//binding point
			buf,//data
			0,//offset
			sizeof(GLfloat) * 32//size in bytes
		);
	}else{
		glBindBuffer(GL_UNIFORM_BUFFER,scene->getCamera()->getMatricesUBO());
		glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(GLfloat)*32, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	delete[] data;
}

void Renderer::setMaterialUniforms(Material* material){
	//set diffuse color
	GLfloat* diffuseColor = material->getDiffuseColor()->getAsArray();
	glUniform4fv(
		material->getProgram()->getUniforms()->unifDiffuseColor,
		1,
		diffuseColor
	);
	delete diffuseColor;

	//set specular color
	GLfloat* specularColor = material->getSpecularColor()->getAsArray();
	glUniform4fv(
		material->getProgram()->getUniforms()->unifSpecularColor,
		1,
		specularColor
	);
	delete specularColor;

	//set shininess
	GLfloat shininess = material->getShininess();
	glUniform1fv(
		material->getProgram()->getUniforms()->unifShininess,
		1,
		&shininess
	);
}

void Renderer::render(Scene * scene){
	list<Object3D*> objects = scene->getObjects();
	list<Object3D*>::iterator it = objects.begin();
	list<Object3D*>::iterator end = objects.end();
	//vao initialization;
	if(this->vao == 0){
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
	}

	//AmbientLight UBO
	this->calculateAmbientLights(scene);

	//Camera UBO initialization
	this->calculateGlobalMatrices(scene);

	//DirectionalLightsUBO

	this->calculateDirectionalLights(scene);

	//PointLightsUBO

	this->calculatePointLights(scene);

	for(;it != end;it++){
		Mesh* mesh= (Mesh*)(*it);
		if(!mesh->getVisible()) continue;
		if(mesh->getGeometry()->getVertexBuffer() == 0 && mesh->getGeometry()->getVertices() != NULL){
			GLuint buf = this->makeBuffer(GL_ARRAY_BUFFER,
							mesh->getGeometry()->getVertices(),
							mesh->getGeometry()->getNumVertices() * sizeof(GLfloat)
							);
            mesh->getGeometry()->setVertexBuffer(buf);
		}
		if(mesh->getGeometry()->getElementBuffer() == 0 && mesh->getGeometry()->getElements() != NULL){
			GLuint buf = this->makeBuffer(GL_ELEMENT_ARRAY_BUFFER,
							mesh->getGeometry()->getElements(),
							mesh->getGeometry()->getNumElements() * sizeof(GLushort)
							);
            mesh->getGeometry()->setElementBuffer(buf);
		}
		if(mesh->getGeometry()->getNormalBuffer() == 0 && mesh->getGeometry()->getNormals() != NULL){
			GLuint buf = this->makeBuffer(GL_ELEMENT_ARRAY_BUFFER,
							mesh->getGeometry()->getNormals(),
							mesh->getGeometry()->getNumNormals() * sizeof(GLfloat)
							);
            mesh->getGeometry()->setNormalBuffer(buf);
		}
		//set vertex attribute
		glBindBuffer(GL_ARRAY_BUFFER,mesh->getGeometry()->getVertexBuffer());
		glVertexAttribPointer(
			mesh->getMaterial()->getProgram()->getAttrPosition(),//attribute from prgram(position)
			3,//number of components per vertex
			GL_FLOAT,//type of data
			GL_FALSE,//normalized
			0,//separation between 2 values
			(void*)0 //offset
		);
		glEnableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());

		//set normal attribute
		if(mesh->getGeometry()->getNormalBuffer() != 0){
			glBindBuffer(GL_ARRAY_BUFFER,mesh->getGeometry()->getNormalBuffer());
			glVertexAttribPointer(
				mesh->getMaterial()->getProgram()->getAttrNormal(),//attribute from prgram(position)
				3,//number of components per vertex
				GL_FLOAT,//type of data
				GL_FALSE,//normalized
				0,//separation between 2 values
				(void*)0 //offset
			);
			glEnableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrNormal());
		}
		
		glUseProgram(mesh->getMaterial()->getProgram()->getProgram());

		//set model matrix
		mesh->updateModelMatrix();
		glUniformMatrix4fv(
			mesh->getMaterial()->getProgram()->getUniforms()->unifModelMatrix,
			1,
			GL_TRUE,
			mesh->getModelMatrix()->getElements()
		);

		//set material uniforms
		setMaterialUniforms(mesh->getMaterial());
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->getGeometry()->getElementBuffer());
		glDrawElements(
			GL_TRIANGLES, //drawing mode
			mesh->getGeometry()->getNumElements(), //count
			GL_UNSIGNED_SHORT, //type,
			(void*)0 //offset
		);
		glDisableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());
	}
	this->renderOctreeNode(scene->getOctree());
}

void Renderer::renderOctreeNode(OctreeNode* node){

	Mesh* mesh = node->getBoundingBox();
	if(mesh->getGeometry()->getVertexBuffer() == 0 && mesh->getGeometry()->getVertices() != NULL){
		GLuint buf = this->makeBuffer(GL_ARRAY_BUFFER,
						mesh->getGeometry()->getVertices(),
						mesh->getGeometry()->getNumVertices() * sizeof(GLfloat)
						);
        mesh->getGeometry()->setVertexBuffer(buf);
	}

	if(mesh->getGeometry()->getElementBuffer() == 0 && mesh->getGeometry()->getElements() != NULL){
		GLuint buf = this->makeBuffer(GL_ELEMENT_ARRAY_BUFFER,
						mesh->getGeometry()->getElements(),
						mesh->getGeometry()->getNumElements() * sizeof(GLushort)
						);
        mesh->getGeometry()->setElementBuffer(buf);
	}

	if(mesh->getGeometry()->getNormalBuffer() == 0 && mesh->getGeometry()->getNormals() != NULL){
		GLuint buf = this->makeBuffer(GL_ELEMENT_ARRAY_BUFFER,
						mesh->getGeometry()->getNormals(),
						mesh->getGeometry()->getNumNormals() * sizeof(GLfloat)
						);
        mesh->getGeometry()->setNormalBuffer(buf);
	}
	//set vertex attribute

	glBindBuffer(GL_ARRAY_BUFFER,mesh->getGeometry()->getVertexBuffer());
	glVertexAttribPointer(
		mesh->getMaterial()->getProgram()->getAttrPosition(),//attribute from prgram(position)
		3,//number of components per vertex
		GL_FLOAT,//type of data
		GL_FALSE,//normalized
		0,//separation between 2 values
		(void*)0 //offset
	);
	glEnableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());

	//set normal attribute
	if(mesh->getGeometry()->getNormalBuffer() != 0){
		glBindBuffer(GL_ARRAY_BUFFER,mesh->getGeometry()->getNormalBuffer());
		glVertexAttribPointer(
			mesh->getMaterial()->getProgram()->getAttrNormal(),//attribute from prgram(position)
			3,//number of components per vertex
			GL_FLOAT,//type of data
			GL_FALSE,//normalized
			0,//separation between 2 values
			(void*)0 //offset
		);
		glEnableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrNormal());
	}

	glUseProgram(mesh->getMaterial()->getProgram()->getProgram());

	//set model matrix
	mesh->updateModelMatrix();
	glUniformMatrix4fv(
		mesh->getMaterial()->getProgram()->getUniforms()->unifModelMatrix,
		1,
		GL_TRUE,
		mesh->getModelMatrix()->getElements()
	);

	//set material uniforms
	setMaterialUniforms(mesh->getMaterial());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->getGeometry()->getElementBuffer());
	glDrawElements(
		GL_LINES, //drawing mode
		mesh->getGeometry()->getNumElements(), //count
		GL_UNSIGNED_SHORT, //type,
		(void*)0 //offset
	);
	glDisableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());
	if(!node->isDivided())return;
	OctreeNodeIterator it = node->children.begin();
	OctreeNodeIterator end = node->children.end();
	for(;it != end;it++){
		renderOctreeNode((*it));
	}
}
