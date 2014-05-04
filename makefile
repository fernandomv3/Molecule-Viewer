OBJS = $(BUILDDIR)/Vec3.o \
	   $(BUILDDIR)/Mat4.o \
       $(BUILDDIR)/Geometry.o \
       $(BUILDDIR)/GLProgram.o \
       $(BUILDDIR)/Material.o \
       $(BUILDDIR)/BasicMaterial.o \
       $(BUILDDIR)/GouraudMaterial.o \
       $(BUILDDIR)/PhongMaterial.o \
       $(BUILDDIR)/CelMaterial.o \
       $(BUILDDIR)/LineMaterial.o \
       $(BUILDDIR)/TessMaterial.o \
       $(BUILDDIR)/Object3D.o \
       $(BUILDDIR)/Mesh.o \
       $(BUILDDIR)/Scene.o \
       $(BUILDDIR)/OctreeNode.o \
       $(BUILDDIR)/Renderer.o \
       $(BUILDDIR)/Euler.o \
       $(BUILDDIR)/Quaternion.o \
       $(BUILDDIR)/Camera.o \
       $(BUILDDIR)/Color.o \
       $(BUILDDIR)/Light.o \
       $(BUILDDIR)/DirectionalLight.o \
       $(BUILDDIR)/PointLight.o \
       $(BUILDDIR)/PointMaterial.o \
       $(BUILDDIR)/AtomMaterialPool.o \
       $(BUILDDIR)/AtomRadiusTable.o \
       $(BUILDDIR)/SphericalCoord.o \
       $(BUILDDIR)/Atom.o \
       $(BUILDDIR)/Molecule.o \
       $(BUILDDIR)/main.o

INCDIR = include
SRCDIR = src
BUILDDIR = build
BINDIR = bin
CC = g++
DEBUG = -g -Wall
IFLAGS = -I $(INCDIR) -Ilib
SDLFLAGS = -Llib -lSDL2main -lSDL2 
CFLAGS = -c $(DEBUG) $(IFLAGS)
GLEWFLAGS = -Llib -lglew32 -lglew32mx
OPENGLFLAGS = -lopengl32 
LFLAGS = $(DEBUG) $(GLEWFLAGS) $(SDLFLAGS) $(OPENGLFLAGS) 

vpath %.cpp $(SRCDIR)
vpath %.cpp $(SRCDIR)/material
vpath %.cpp $(SRCDIR)/math
vpath %.cpp $(SRCDIR)/object
vpath %.cpp $(SRCDIR)/render
vpath %.cpp $(SRCDIR)/scene
vpath %.cpp $(SRCDIR)/light

vpath %.h $(INCDIR)
vpath %.h $(INCDIR)/material
vpath %.h $(INCDIR)/math
vpath %.h $(INCDIR)/object
vpath %.h $(INCDIR)/render
vpath %.h $(INCDIR)/scene
vpath %.h $(INCDIR)/light

$(BINDIR)/molecule : $(OBJS)
	@echo generating executable...
	$(CC) -o $(BINDIR)/molecule $(OBJS) $(LFLAGS)

$(BUILDDIR)/%.o : %.cpp %.h
	@echo compiling $<
	$(CC) -o $(BUILDDIR)/$*.o $< $(CFLAGS) 

Euler.h : Mat4.h

Material.h : GLProgram.h Color.h

BasicMaterial.h : Material.h

GouraudMaterial.h : Material.h

PhongMaterial.h : Material.h

CelMaterial.h : Material.h

LineMaterial.h : Material.h

TessMaterial.h : Material.h

Object3D.h : Vec3.h Mat4.h Euler.h Quaternion.h

Mesh.h : Object3D.h Material.h  Geometry.h

Scene.h : Object3D.h Camera.h OctreeNode.h

Renderer.h : Scene.h Mesh.h OctreeNode.h

Camera.h : Object3D.h

Light.h : Color.h

SphericalCoord.h : Vec3.h

DirectionalLight.h : Light.h

PointLight.h : Light.h

PointMaterial.h : Material.h

Atom.h : Mesh.h

AtomMaterialPool.h : PhongMaterial.h

Molecule.h : Atom.h Scene.h

OctreeNode.h : Object3D.h

$(BUILDDIR)/main.o : $(SRCDIR)/main.cpp $(INCDIR)/object/Mesh.h $(INCDIR)/object/Geometry.h $(INCDIR)/object/Object3D.h $(INCDIR)/math/Vec3.h $(INCDIR)/math/Mat4.h $(INCDIR)/material/Material.h $(INCDIR)/render/GLProgram.h $(INCDIR)/material/BasicMaterial.h $(INCDIR)/scene/Scene.h $(INCDIR)/render/Renderer.h
	@echo compiling molecule
	$(CC) -o $(BUILDDIR)/main.o $(CFLAGS) $(SRCDIR)/main.cpp
	
clean:
	@echo cleaning project...
	rm $(BUILDDIR)/*.o $(BINDIR)/molecule
	
