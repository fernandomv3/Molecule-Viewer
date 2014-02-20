#include "material/BasicMaterial.h"
#include <string.h>
 
BasicMaterial::BasicMaterial():Material(){
    this->vertexShaderSource= strdup(
        "#version 410\n\
        #define MAX_DIR_LIGHTS 10\n\
        #define MAX_P_LIGHTS 10\n\
        struct DirectionalLight{\n\
            vec4 color;\n\
            vec4 vectorToLight;\n\
            float intensity;\n\
        };\n\
        \n\
        struct PointLight{\n\
            vec4 color;\n\
            vec4 position;\n\
            float intensity;\n\
        };\n\
        \n\
        in vec3 position;\n\
        in vec3 normal;\n\
        smooth out vec4 color;\n\
        layout(std140) uniform globalMatrices{\n\
            mat4 worldMatrix;\n\
            mat4 projectionMatrix;\n\
        };\n\
        layout(std140) uniform directionalLights{\n\
            DirectionalLight dirLights[MAX_DIR_LIGHTS];\n\
            int numDirLights;\n\
        };\n\
        layout(std140) uniform pointLights{\n\
            PointLight pLights[MAX_P_LIGHTS];\n\
            int numPointLights;\n\
        };\n\
        layout(std140) uniform ambLight{\n\
            vec4 ambientLight;\n\
        };\n\
        uniform mat4 modelMatrix;\n\
        void main(){\n\
            vec4 pos = vec4(position,1.0);\n\
            vec4 modelSpace = modelMatrix *pos;\n\
            vec4 worldSpace = worldMatrix * modelSpace;\n\
            gl_Position = projectionMatrix * worldSpace;\n\
            vec4 diffuseColor = vec4(1.0,0.0,0.0,1.0);\n\
            color = vec4(0.0,0.0,0.0,1.0);\n\
            for(int i=0; i< numDirLights ;i++){\n\
                vec4 normCameraSpace = worldMatrix * dirLights[i].vectorToLight;\n\
                vec4 transfNormal = normalize(worldMatrix * vec4(normal,0.0));\n\
                float cosAngIncidence = dot( transfNormal , normalize(normCameraSpace));\n\
                cosAngIncidence = clamp(cosAngIncidence, 0, 1);\n\
                color = color + (dirLights[i].color * diffuseColor * cosAngIncidence);\n\
            }\n\
            for(int i=0; i< numPointLights ;i++){\n\
                vec4 normCameraSpace = worldMatrix * (modelSpace - pLights[i].position);\n\
                vec4 transfNormal = normalize(worldMatrix * vec4(normal,0.0));\n\
                float cosAngIncidence = dot( transfNormal , normalize(normCameraSpace));\n\
                cosAngIncidence = clamp(cosAngIncidence, 0, 1);\n\
                color = color + (pLights[i].color * diffuseColor * cosAngIncidence);\n\
            }\n\
            color = color + (diffuseColor * ambientLight);\n\
        }");
    this->fragmentShaderSource=strdup(
        "#version 410\n\
        in vec4 color;\n\
        out vec4 outputColor;\n\
        void main(){\n\
            outputColor = color;\n\
        }");
    this->program = new GLProgram();
    GLuint vertexShader = this->program->compileShader(GL_VERTEX_SHADER,this->vertexShaderSource);
    GLuint fragmentShader = this->program->compileShader(GL_FRAGMENT_SHADER,this->fragmentShaderSource);
    this->program->setVertexShader(vertexShader);
    this->program->setFragmentShader(fragmentShader);
    GLuint prog = this->program->linkProgram(vertexShader,fragmentShader);
    this->program->setProgram(prog);
    this->program->setAttrPosition(glGetAttribLocation(prog, "position"));
    this->program->setAttrNormal(glGetAttribLocation(prog, "normal"));
    this->program->getUniforms()->unifModelMatrix = glGetUniformLocation(prog,"modelMatrix");
    this->program->getUniforms()->unifBlockMatrices = glGetUniformBlockIndex(prog,"globalMatrices");
    glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockMatrices,0);
    this->program->getUniforms()->unifBlockDirectionalLights = glGetUniformBlockIndex(prog,"directionalLights");
    glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockDirectionalLights,1);
    this->program->getUniforms()->unifBlockAmbientLight = glGetUniformBlockIndex(prog,"ambLight");
    glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockAmbientLight,2);
    this->program->getUniforms()->unifBlockPointLights = glGetUniformBlockIndex(prog,"pointLights");
    glUniformBlockBinding(prog, this->program->getUniforms()->unifBlockPointLights,3);
}