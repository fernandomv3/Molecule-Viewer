#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cassert>

#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

//#include "Molecule.h"
#include "object/Mesh.h"
#include "scene/Scene.h"
#include "render/Renderer.h"
#include "material/PhongMaterial.h"
#include "Molecule.h"

Renderer* renderer;
Scene* scene;
Molecule* mol;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
SDL_Window* window = NULL;
SDL_Surface* screenSurface;
SDL_GLContext context = NULL;

void initializeContext(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError());
	}
	else{
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		window = SDL_CreateWindow(
			"Molecule",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL 
		);
		if(window == NULL){
			printf("Window could not be created! SDL_Error: %s\n",SDL_GetError());
		}else{
            context = SDL_GL_CreateContext( window );
			if( context == NULL ){
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else{
				glewExperimental = GL_TRUE;
				GLenum err = glewInit();
				if (GLEW_OK != err)
				{
					fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
				}
				if(!GLEW_VERSION_4_0){
					fprintf(stderr,"OpenGL 4.0 not available\n");
					exit(-1);
				}
				const unsigned char* version = glGetString(GL_VERSION);
				if (version == NULL) {
					exit(-1);
				}
				printf("%s\n", version);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glFrontFace(GL_CCW);
				glEnable(GL_MULTISAMPLE);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POLYGON_SMOOTH);
				glEnable( GL_PROGRAM_POINT_SIZE);
			}
        }
	}
}

bool handleEvents(){
	SDL_Event event;
	while( SDL_PollEvent( &event ) ){
		if(event.type == SDL_KEYDOWN ){
			Camera* camera = scene->getCamera();	
			camera->getRotation()->setY(camera->getRotation()->getY()- 0.1);
			camera->getPosition()->setY(mol->getY());
    		camera->getPosition()->setX(12 * (sin(camera->getRotation()->getY()-0.1))+ mol->getX());
    		camera->getPosition()->setZ(12 * (cos(camera->getRotation()->getY()-0.1))+ mol->getZ());
		}
		if( event.type == SDL_QUIT ){
			return true;
		}
	}
	return false;
}

void render(){
	/*newTime = glutGet(GLUT_ELAPSED_TIME);
	int diff = newTime - oldTime;
	printf("%ld\n",diff);
	oldTime=newTime;*/
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer->render(scene);
    SDL_GL_SwapWindow(window);
}

void mainLoop(){
	bool quit = false;
	while(!quit){
		quit = handleEvents();
		render();
	}
}

void cleanUp(){
	delete scene;
	delete renderer;
	SDL_GL_DeleteContext(context);
    SDL_DestroyWindow( window );
    SDL_Quit();
}

int main(int argc, char** argv){
	initializeContext();
	scene = new Scene();
	mol = new Molecule("caffeine.pdb");
	mol->addToScene(scene);
	DirectionalLight* light1 = new DirectionalLight();
	light1->getPosition()->setX(2.0);
	light1->getPosition()->setY(4.0);
	light1->getPosition()->setZ(5.0);
	scene->addDirectionalLight(light1);

	renderer = new Renderer();
	mainLoop();
	cleanUp();
    return 0;
}