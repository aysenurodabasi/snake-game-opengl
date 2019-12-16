#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "circle.hpp"
#include "shaderprogram.hpp"
#include<glm/mat3x3.hpp>

float length = 0.18;

std::vector<glm::vec3>      vertices;


std::vector<Circle*>    snakeList;



unsigned int VBO;
unsigned int VAO;

void buildCircle(float radius,int vertexCount)
{
    float angle = 360.0f/vertexCount;

    int triangleCount = vertexCount-2;
    
    std::vector<glm::vec3>   tempVertices;

    for(int i=0;i<vertexCount;i++)
    {
        float newAngle = angle*i;  

        float x= radius*cos(glm::radians(newAngle));

        float y= radius*sin(glm::radians(newAngle));
        
        float z = 1.0f;

       
        tempVertices.push_back(glm::vec3(x,y,z));

    }

    
    for(int i=0;i<triangleCount;i++)
    {
        vertices.push_back(tempVertices[0]);
        vertices.push_back(tempVertices[i+1]);
        vertices.push_back(tempVertices[i+2]);
    }

}


void moveSnake()
{
    for(auto next:snakeList)
    {
        next->move();
  
    }

    for(int i=snakeList.size()-1;i>0;i--)
    {
        snakeList[i]->setDirection(snakeList[i-1]->getDirection());
    }

}




void drawSnake(ShaderProgram& program)
{

    for(auto next:snakeList)
    {
        program.setVec4("uColor",next->getColor());

        glm::mat3 rot = next->getTransform();
        program.setMat3("uMtxTransform",&rot);
        
        glDrawArrays(GL_TRIANGLES, 0,vertices.size());
    }


}


void addToSnakeTail()
{
    int elementCount = snakeList.size();


    if(elementCount==0)
    {
        snakeList.push_back(new Circle(0.0f,0.0f,length));
    }
    else
    {
        Circle* lastCircle = snakeList[elementCount-1];

        glm::vec3 pos = lastCircle->getPosition();

        Circle::DIRECTION dir = lastCircle->getDirection();

       

        switch(dir)
        {
            case Circle::DIR_RIGHT:
                pos-=glm::vec3(length,0.0f,0.0f);
            break;
            case  Circle::DIR_LEFT:
                pos+=glm::vec3(length,0.0f,0.0f);
                break;
            case  Circle::DIR_UP:
                pos-=glm::vec3(0.0f,length,0.0f);
                break;    
            case  Circle::DIR_DOWN:
                pos+=glm::vec3(0.0f,length,0.0f);
                break;              
        }
        
        Circle* newCircle = new Circle(pos.x,pos.y,length);

        newCircle->setDirection(dir);

        snakeList.push_back(newCircle);

    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE)
        glfwTerminate();   
    if(action==GLFW_PRESS)
    {
        if(snakeList.size()!=0)
        {
            Circle* first = snakeList[0];

            if(key==GLFW_KEY_LEFT)  
            {
                first->setDirection(Circle::DIR_LEFT);
            }
            if(key==GLFW_KEY_RIGHT) 
            {
                first->setDirection(Circle::DIR_RIGHT);
            }    
            if(key==GLFW_KEY_UP)  
            {
                first->setDirection(Circle::DIR_UP);
            }
            if(key==GLFW_KEY_DOWN) 
            {
                first->setDirection(Circle::DIR_DOWN);
            }                
        }
        
        if(key==GLFW_KEY_SPACE)
            addToSnakeTail();
           
    }  
           
}

int main(int argc,char** argv)
{
    if(!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(800,800,"Snake Game",NULL,NULL);
    if(window==NULL)
    {
        std::cout<<"Pencere Olusturulamadi"<<std::endl;

        glfwTerminate();

        return -1;
    }    
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 


     buildCircle(0.09,36);


    ShaderProgram program;

    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    addToSnakeTail();
    

    program.attachShader("./shaders/simplevs.glsl",GL_VERTEX_SHADER);
    program.attachShader("./shaders/simplefs.glsl",GL_FRAGMENT_SHADER);
    program.link();

    
    program.addUniform("uColor");
    program.addUniform("uMtxTransform");


    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),&vertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

   
    while(!glfwWindowShouldClose(window))
    {
       
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        
      
        program.use();
      
                
        glBindVertexArray(VAO);
      
        drawSnake(program);
        
        moveSnake();

        
        program.setVec4("uColor",glm::vec4(1.0f,0.0f,0.0f,1.0f));
        

        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

}