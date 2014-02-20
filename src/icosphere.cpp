#include <cstdio>
#include "object/Mesh.h"
#include "object/Geometry.h"
#include "material/PhongMaterial.h"
#include "material/Material.h"
#include "scene/Scene.h"
#include "render/Renderer.h"
#include "math.h"
#include <GL/glew.h>
#ifdef __APPLE__
#  include <glut/glut.h>
#else
#  include <GL/glut.h>
#endif

Renderer* renderer;
Scene* scene;
/*int newTime =0;
int oldTime =0;*/

void render(void){
		/*newTime = glutGet(GLUT_ELAPSED_TIME);
		int diff = newTime - oldTime;
		printf("%ld\n",diff);
		oldTime=newTime;*/
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer->render(scene);
        glutSwapBuffers();
        glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	Camera* camera = scene->getCamera();	
	camera->getRotation()->setY(camera->getRotation()->getY()- 0.1);
    camera->getPosition()->setX(10 * (sin(camera->getRotation()->getY()-0.1)));
    camera->getPosition()->setZ(10 * (cos(camera->getRotation()->getY()-0.1)));
    
}

void initializeContext(int argc, char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE| GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutCreateWindow("Icosaedro");
	glutDisplayFunc(&render);
	glutKeyboardFunc(&keyboard);
	glewExperimental = GL_TRUE;
	glewInit();
	if(!GLEW_VERSION_2_0){
		fprintf(stderr,"OpenGL 3.2 not available\n");
		return;
	}
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_MULTISAMPLE);
    glEnable( GL_PROGRAM_POINT_SIZE);
}

Mesh* createIcosphere(){
	Geometry* icosphereGeometry= new Geometry();
	icosphereGeometry->loadDataFromFile("highres-icosphere.mesh");
	Material* icosphereMaterial = (Material *)new PhongMaterial();
	icosphereMaterial->getDiffuseColor()->setRGB(1.0,0.0,0.0);
	icosphereMaterial->getSpecularColor()->setRGB(0.8,0.8,0.8);
	icosphereMaterial->setShininess(10);
	Mesh * icosphereMesh = new Mesh();
	icosphereMesh->setGeometry(icosphereGeometry);
	icosphereMesh->setMaterial(icosphereMaterial);
	return icosphereMesh;
}



int main(int argc, char** argv){
	int c;
	scanf("%d",&c);
	initializeContext(argc, argv);
	Mesh* icosphere = createIcosphere();

	Mesh* sphere = new Mesh();
	sphere->setGeometry(icosphere->getGeometry());
	Material* sphereMaterial = (Material *)new PhongMaterial();
	sphereMaterial->getDiffuseColor()->setRGB(0.75164,0.60648,0.22648);
	sphereMaterial->getSpecularColor()->setRGB(0.628281,0.555802,0.366065);
	sphereMaterial->setShininess(83.2);
	sphere->setMaterial(sphereMaterial);
	sphere->getPosition()->setX(-1.0);

	scene = new Scene();
	/*DirectionalLight* light = new DirectionalLight();
	light->getPosition()->setX(1.0);
	light->getPosition()->setY(1.0);
	light->getPosition()->setZ(-1.0);
	light->setIntensity(0.5);
	scene->addDirectionalLight(light);*/

	PointLight* light = new PointLight();
	light->getPosition()->setX(2.0);
	light->getPosition()->setY(2.0);
	light->getPosition()->setZ(2.0);
	light->setIntensity(0.5);
	scene->addPointLight(light);

	scene->addObject((Object3D*)sphere);
	icosphere->getPosition()->setX(1.0);
	scene->addObject((Object3D*)icosphere);
	renderer = new Renderer();
	glutMainLoop();
	return 0;
}