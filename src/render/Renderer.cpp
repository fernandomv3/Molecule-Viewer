#include "render/Renderer.h"
#include <list>
#include "object/Object3D.h"
#include <cstdlib>
#include "object/Mesh.h"
#include "scene/Scene.h"
#include "material/PointMaterial.h"
#include <cstdio>

Renderer::Renderer(){
	this->vao=0;
	this->buffers=NULL;
}

GLuint Renderer::makeBuffer(GLenum target, void* bufferData, GLsizei bufferSize, GLenum usage){
	GLuint buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(target,buffer);
	glBufferData(target,bufferSize,bufferData, usage);
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

GLuint Renderer::calculateDirectionalLights(Scene* scene){
	GLint numLights = scene->getDirectionalLights().size();
	if(numLights == 0) return 0;
	int bufferSize = sizeof(struct dirLight) * numLights;
	if(scene->getDirectionalLightsUBO() == 0){
		GLuint buf = makeUBO(NULL, bufferSize);
		scene->setDirectionalLightsUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			DIRLIGHTS_UBI,//binding point
			buf,//data
			0,//offset
			bufferSize//size in bytes
		);
	}
	
	list<DirectionalLight*> lightsList = scene->getDirectionalLights();
	list<DirectionalLight*>::iterator itLights = lightsList.begin();
	list<DirectionalLight*>::iterator endLights = lightsList.end();
	struct dirLight lights[numLights];
	for(int i=0;itLights != endLights && i<10 ;itLights++ , i++){
		DirLight light = (*itLights)->getAsStruct(scene->getCamera());
		lights[i]= *light;
		delete light;
	}

	glBindBuffer(GL_UNIFORM_BUFFER,scene->getDirectionalLightsUBO());
	glBufferSubData(GL_UNIFORM_BUFFER,0,bufferSize,&lights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return scene->getDirectionalLightsUBO();
}

GLuint Renderer::calculatePointLights(Scene* scene){
	GLint numLights = scene->getPointLights().size();
	if(numLights == 0) return 0;
	int bufferSize = sizeof(struct pLight)*numLights;
	if(scene->getPointLightsUBO() == 0){
		GLuint buf = makeUBO(NULL, bufferSize);
		scene->setPointLightsUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			PLIGHTS_UBI,//binding point
			buf,//data
			0,//offset
			bufferSize//size in bytes
		);
	}
	
	list<PointLight*> lightsList = scene->getPointLights();
	list<PointLight*>::iterator itLights = lightsList.begin();
	list<PointLight*>::iterator endLights = lightsList.end();
	struct pLight lights[numLights];

	for(int i=0;itLights != endLights && i<10 ;itLights++ , i++){
		PLight light = (*itLights)->getAsStruct(scene->getCamera());
		lights[i] = *light;
		delete light;
	}

	glBindBuffer(GL_UNIFORM_BUFFER,scene->getPointLightsUBO());
	glBufferSubData(GL_UNIFORM_BUFFER,0,bufferSize,&lights);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return scene->getPointLightsUBO();
}

GLuint Renderer::calculateAmbientLights(Scene* scene){
	if(scene->getAmbientLightUBO() == 0){
		GLfloat* data = scene->getAmbientLight()->getColor()->getAsArray();
		GLuint buf = makeUBO(data,sizeof(GLfloat)*4);
		scene->setAmbientLightUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			AMBLIGHT_UBI,//binding point
			buf,//data
			0,//offset
			sizeof(GLfloat) * 4//size in bytes
		);
		delete[] data;
	}
	return scene->getAmbientLightUBO();
}

GLuint Renderer::calculateGlobalMatrices(Scene* scene){
	//update UBO TODO: update world matrix only as projection never changes
	scene->getCamera()->updateWorldMatrix();
	GLfloat* data = scene->getCamera()->getMatricesArray();
	if(scene->getCamera()->getMatricesUBO()==0){	
		GLuint buf = makeUBO(data,sizeof(GLfloat)*32);
		scene->getCamera()->setMatricesUBO(buf);
		glBindBufferRange(
			GL_UNIFORM_BUFFER,//target
			GLOBAL_MATRICES_UBI,//binding point
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
	return scene->getCamera()->getMatricesUBO();
}

void Renderer::setMaterialUniforms(Material* material,Scene* scene){
	
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

		//compile the program if i'ts not compiled
		if(mesh->getMaterial()->getProgram() == NULL){
			mesh->getMaterial()->makePrograms(scene);
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
		GLfloat dist = mesh->getDistanceToCamera();
		glUniform1fv(
			mesh->getMaterial()->getProgram()->getUniforms()->unifDistanceToCamera,
			1,
			&dist
		);
		//set material uniforms
		setMaterialUniforms(mesh->getMaterial(),scene);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->getGeometry()->getElementBuffer());
		if(mesh->getMaterial()->getType() == TESS_MATERIAL){
			glPatchParameteri(GL_PATCH_VERTICES, 3);
			glDrawElements(
				GL_PATCHES, //drawing mode
				mesh->getGeometry()->getNumElements(), //count
				GL_UNSIGNED_SHORT, //type,
				(void*)0 //offset
			);
		}
		else{
			glDrawElements(
				GL_TRIANGLES, //drawing mode
				mesh->getGeometry()->getNumElements(), //count
				GL_UNSIGNED_SHORT, //type,
				(void*)0 //offset
			);
		}
		
		glDisableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());
	}
	//this->renderOctreeNode(scene->getOctree());
}

void Renderer::renderOctreeNode(OctreeNode* node, Scene* scene){

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
	setMaterialUniforms(mesh->getMaterial(),scene);
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
		renderOctreeNode((*it),scene);
	}
}

BufferObjects Renderer::getBuffers(){
	return this->buffers;
}

GLuint Renderer::createMaterialBuffer(Scene* scene){
	list<Material*> materials = scene->getMaterials();
	int numMaterials = materials.size();
	MaterialStruct materialList = new struct materialStruct[numMaterials];

	list<Material*>::iterator it = materials.begin();

	for(int i=0 ;it != materials.end(); it++ , i++){
		MaterialStruct mat = (*it)->getAsStruct(); 
		materialList[i] = *mat;
		delete mat;
		if ((*it)->getProgram() == NULL){
			(*it)->makePrograms(scene);
		}
	}
	/*for(int i=0; i < numMaterials;i++){
		printf("Material %d\n",i );
		printf("\tDiffuse color: %f %f %f\n",materialList[i].diffuseColor[0],materialList[i].diffuseColor[1],materialList[i].diffuseColor[2]);
		printf("\tSpecular color: %f %f %f\n",materialList[i].specularColor[0],materialList[i].specularColor[1],materialList[i].specularColor[2]);
		printf("\tShininess: %f\n",materialList[i].shininess);
	}*/
	GLuintubo = this->makeBuffer(
		GL_UNIFORM_BUFFER,
		materialList,
		numMaterials * sizeof(struct materialStruct),
	);
	glBindBufferRange(
		GL_UNIFORM_BUFFER,//target
		MATERIALS_UBI,//binding point
		ubo,//data
		0,//offset
		numMaterials * sizeof(struct materialStruct)//size in bytes
	);
	delete[] materialList;
	return ubo;
}

GLuint* Renderer::createGeometryBuffers(Scene* scene){
	GLuint* buffers = new GLuint[3];
	list<Geometry*> geometries= scene->getGeometries();
	//printf("Num geometries: %d\n", geometries.size());
	list<Geometry*>::iterator it = geometries.begin();
	int totalNumElements=0;
	int totalNumVertices=0;
	int totalNumNormals=0;
	for(int i=0;it != geometries.end();it++){
		(*it)->setSceneIndicesOffset(totalNumElements);
		(*it)->setSceneVerticesOffset(totalNumVertices);
		/*printf("Geomerty %d:\n",i);
		printf("\tnumElements: %d\n",(*it)->getNumElements());
		printf("\tElementsOffset: %d\n",totalNumElements);
		printf("\tnumVertices: %d\n",(*it)->getNumVertices());
		printf("\tVerticesOffset: %d\n",totalNumVertices);
		printf("\tnumNormals: %d\n",(*it)->getNumNormals());*/
		totalNumElements += (*it)->getNumElements();
		totalNumVertices += (*it)->getNumVertices();
		totalNumNormals += (*it)->getNumNormals();
		i++;
	}
	GLfloat* vertices = new GLfloat[totalNumVertices];
	GLfloat* normals = new GLfloat[totalNumNormals];
	GLushort* elements = new GLushort[totalNumElements];

	GLfloat* ptrVertices = vertices;
	GLfloat* ptrNormals = normals;
	GLushort* ptrElements = elements;

	for(it = geometries.begin(); it != geometries.end();it++){
		Geometry* geom = *it;
		memcpy(ptrVertices,geom->getVertices(),sizeof(GLfloat)*geom->getNumVertices());
		ptrVertices += geom->getNumVertices();
		memcpy(ptrNormals,geom->getNormals(),sizeof(GLfloat)*geom->getNumNormals());
		ptrNormals += geom->getNumNormals();
		memcpy(ptrElements,geom->getElements(),sizeof(GLushort)*geom->getNumElements());
		ptrElements += geom->getNumElements();
	}

	buffers[VERTICES] = this->makeBuffer(
		GL_ARRAY_BUFFER,
		vertices,
		sizeof(GLfloat)*totalNumVertices
	);
	buffers[NORMALS] = this->makeBuffer(
		GL_ARRAY_BUFFER,
		normals,
		sizeof(GLfloat)*totalNumNormals
	);
	buffers[ELEMENTS] = this->makeBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		elements,
		sizeof(GLushort)*totalNumElements
	);

	delete vertices;
	delete normals;
	delete elements;
	return buffers;
}

GLuint* Renderer::createObjectBuffers(Scene* scene){
	GLuint* buffers = new GLuint[4];
	list<Object3D*> objectList = scene->getObjects();
	list<Object3D*>::iterator it = objectList.begin();
	int size = objectList.size();
	GLushort * drawID = new GLushort[size];
	GLfloat* matrices = new GLfloat[size * 16];
	GLfloat* ptrMatrices = matrices;
	struct bufferIndices* indices = new struct bufferIndices[size];
	struct bufferIndices* ptrIndices = indices;
	struct indirect* indirects = new struct indirect[size];
	struct indirect* ptrIndirects = indirects;
	for(int i=0;it != objectList.end();it++){
		(*it)->updateModelMatrix();
		Mat4* traspose = (*it)->getModelMatrix()->getTraspose(); 
		GLfloat* mat = traspose->getElements();
		delete traspose;
		memcpy(ptrMatrices,mat, sizeof(GLfloat)*16);
		ptrMatrices += 16;
		delete[] mat;

		ptrIndices->materialIndex = ((Mesh*)(*it))->getMaterial()->getSceneIndex();
		ptrIndices->visible = (GLuint)(((Mesh*)(*it))->getOctreeNode()->isVisible());
		ptrIndices++;

		ptrIndirects->count = ((Mesh*)(*it))->getGeometry()->getNumElements();
		ptrIndirects->instanceCount =1;
		ptrIndirects->firstIndex = ((Mesh*)(*it))->getGeometry()->getSceneIndicesOffset();
		ptrIndirects->baseVertex = ((Mesh*)(*it))->getGeometry()->getSceneVerticesOffset()/3;
		ptrIndirects->baseInstance= i;
		ptrIndirects++;
		drawID[i]=i;
		i++;
	}
	/*for(int i = 0; i < size ; i++){
		printf("Object %d\n", i);
		printf("\tMatrix:\n");
		for (int j=0;j < 4 ;j++){
			printf("\t");
			for(int k =0; k < 4 ; k++){
				printf("%1.2f ",matrices[16*i + j*4 +k] );
			}
			printf("\n");
		}
		printf("index:\n");
		printf("\tmaterialIndex: %d\n",indices[i].materialIndex);
		printf("drawCommand:\n");
		printf("\tcount: %d\n",indirects[i].count);
		printf("\tinstanceCount: %d\n",indirects[i].instanceCount);
		printf("\tfirstIndex: %d\n",indirects[i].firstIndex);
		printf("\tbaseVertex: %d\n",indirects[i].baseVertex);
		printf("\tbaseInstance: %d\n",indirects[i].baseInstance);
	}*/

	buffers[MODEL_MATRIX] = this->makeBuffer(
		GL_SHADER_STORAGE_BUFFER,
		matrices,
		sizeof(GLfloat)*size*16,
		GL_STREAM_DRAW
	);
	glBindBufferRange(
		GL_SHADER_STORAGE_BUFFER,//target
		MODEL_MATRICES_UBI,//binding point
		buffers[MODEL_MATRIX],//data
		0,//offset
		sizeof(GLfloat)*size*16//size in bytes
	);
	buffers[BUFFER_INDICES] = this->makeBuffer(
		GL_UNIFORM_BUFFER,
		indices,
		sizeof(struct bufferIndices)*size
	);
	glBindBufferRange(
		GL_UNIFORM_BUFFER,//target
		INDICES_UBI,//binding point
		buffers[BUFFER_INDICES],//data
		0,//offset
		sizeof(struct bufferIndices)*size//size in bytes
	);
	buffers[INDIRECT] = this->makeBuffer(
		GL_DRAW_INDIRECT_BUFFER,
		indirects,
		sizeof(struct indirect)*size,
		GL_STREAM_DRAW
	);

	buffers[DRAWID] = this->makeBuffer(
		GL_ARRAY_BUFFER,
		drawID,
		sizeof(GLushort)*size
	);
	delete[] matrices;
	delete[] indirects;
	delete[] indices;
	return buffers;
}

void Renderer::renderMultiDraw(Scene* scene){
	//vao initialization;
	if(this->vao == 0){
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);
	}
	//should be using SSBO
	if(this->buffers == NULL){
		this->buffers = new struct bufferObjects;//must be an array for multimaterial rendering
		this->buffers->pointLights = 0;
		this->buffers->directionalLights = 0;
		this->buffers->ambientLight = 0;
		this->buffers->materials = 0;
		this->buffers->bufferIndices = 0;
		this->buffers->modelMatrices = 0;
		this->buffers->vertexBuffer = 0;
		this->buffers->elementBuffer = 0;
		this->buffers->normalBuffer = 0;
		this->buffers->globalMatrices =0;
		this->buffers->indirectBuffer = 0;
		this->buffers->drawIDBuffer =0;
	}

	this->buffers->ambientLight = this->calculateAmbientLights(scene);

	this->buffers->globalMatrices = this->calculateGlobalMatrices(scene);


	this->buffers->directionalLights = this->calculateDirectionalLights(scene);


	this->buffers->pointLights = this->calculatePointLights(scene);
	if(this->buffers->vertexBuffer == 0){
		GLuint* buf = this->createGeometryBuffers(scene);
		this->buffers->vertexBuffer = buf[VERTICES];
		this->buffers->elementBuffer = buf[ELEMENTS];
		this->buffers->normalBuffer = buf[NORMALS];
		delete[] buf;
	}

	if(this->buffers->indirectBuffer == 0 ||this->buffers->modelMatrices == 0 || this->buffers->bufferIndices == 0){
		GLuint* buf = this->createObjectBuffers(scene);
		this->buffers->bufferIndices = buf[BUFFER_INDICES];
		this->buffers->indirectBuffer = buf[INDIRECT];
		this->buffers->modelMatrices = buf[MODEL_MATRIX];
		this->buffers->drawIDBuffer = buf[DRAWID];
		delete[] buf;
	}

	if(this->buffers->materials == 0){
		this->buffers->materials = this->createMaterialBuffer(scene);
	}
	GLProgram* program = (*(scene->getMaterials().begin()))->getProgram();
	//set vertex attribute
	glBindBuffer(GL_ARRAY_BUFFER,this->buffers->vertexBuffer);
	glVertexAttribPointer(
		program->getAttrPosition(),//attribute from prgram(position)
		3,//number of components per vertex
		GL_FLOAT,//type of data
		GL_FALSE,//normalized
		0,//separation between 2 values
		(void*)0 //offset
	);
	glEnableVertexAttribArray(program->getAttrPosition());

	//set normal attribute

	glBindBuffer(GL_ARRAY_BUFFER,this->buffers->normalBuffer);
	glVertexAttribPointer(
		program->getAttrNormal(),//attribute from prgram(position)
		3,//number of components per vertex
		GL_FLOAT,//type of data
		GL_FALSE,//normalized
		0,//separation between 2 values
		(void*)0 //offset
	);
	glEnableVertexAttribArray(program->getAttrNormal());

	//set drawID attribute

	glBindBuffer(GL_ARRAY_BUFFER,this->buffers->drawIDBuffer);
	glVertexAttribIPointer(
		program->getAttrDrawID(),//attribute from prgram(position)
		1,//number of components per vertex
		GL_UNSIGNED_SHORT,//type of data
		sizeof(GLushort),//separation between two values
		(void*)0 //offset
	);
	glVertexAttribDivisor(program->getAttrDrawID(), 1);
	glEnableVertexAttribArray(program->getAttrDrawID());
	
	glUseProgram(program->getProgram());

	this->updateModelMatrices(this->buffers->modelMatrices,this->buffers->indirectBuffer,scene->getObjects());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->buffers->elementBuffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, this->buffers->indirectBuffer);
	glMultiDrawElementsIndirect(GL_TRIANGLES,GL_UNSIGNED_SHORT,(void*)0,scene->getObjects().size(),0);

	glDisableVertexAttribArray(program->getAttrPosition());
	glDisableVertexAttribArray(program->getAttrNormal());
}

void Renderer::updateModelMatrices(GLuint modelMatricesBuffer,GLuint indirectBuffer,list<Object3D*> objects){
	list<Object3D*>::iterator it = objects.begin();
	int size = objects.size();
	GLfloat* matrices = new GLfloat[size *16];
	struct indirect* indirects = new struct indirect[size];
	GLfloat* ptrMatrices= matrices;
	struct indirect* ptrIndirects= indirects;
	for(int i=0;it!= objects.end();it++,i++){
		if((*it)->getVisible()){
			ptrIndirects->count = ((Mesh*)(*it))->getGeometry()->getNumElements();
			ptrIndirects->instanceCount =1;
			(*it)->updateModelMatrix();
			Mat4* traspose = (*it)->getModelMatrix()->getTraspose();
			GLfloat* mat = traspose->getElements();
			delete traspose;
			memcpy(ptrMatrices,mat, sizeof(GLfloat)*16);
			delete[] mat;	
		}
		else{
			//ptrIndirects->count = 0;
			ptrIndirects->instanceCount =0;
		}
		ptrIndirects->firstIndex = ((Mesh*)(*it))->getGeometry()->getSceneIndicesOffset();
		ptrIndirects->baseVertex = ((Mesh*)(*it))->getGeometry()->getSceneVerticesOffset()/3;
		ptrIndirects->baseInstance= i;
		ptrIndirects++;
		ptrMatrices += 16;
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,modelMatricesBuffer);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER,0,size*sizeof(GLfloat)*16,matrices);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,0);
	delete[] matrices;
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER,indirectBuffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER,0,size*sizeof(struct indirect),indirects);
	delete[] indirects;

}