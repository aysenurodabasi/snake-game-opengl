#include "circle.hpp"
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <cstdlib>


Circle::Circle(float x,float y,float length)
{
   
    m_Position  = glm::vec3(x,y,0.0f);
    angle=0.0f;
    
    m_Color.r   = (10+std::rand()%15)/25.0;
    m_Color.g   = (10+std::rand()%15)/25.0;  
    m_Length    = length;
    m_Direction = DIR_RIGHT; 
    
}
glm::vec3 Circle::getPosition()
{
    return m_Position;
}
glm::vec4 Circle::getColor()
{
    return m_Color;
}

Circle::DIRECTION Circle::getDirection()
{
    return m_Direction;
}
 glm::mat3 Circle::getTransform()
 {
    glm::mat3 mtxTransform;
    Trotate(mtxTransform);
    glm::mat3 mtxTranslate = glm::translate(glm::mat3(1),glm::vec2(m_Position.x,m_Position.y));
    mtxTransform= mtxTranslate*mtxTransform;
    return mtxTransform;
 }
void Circle::setDirection(Circle::DIRECTION dir)
{
    m_Direction = dir;
}
void Circle::Trotate( glm::mat3& mtxTransform){
    mtxTransform = glm::rotate(glm::mat3(1),glm::radians(angle));
    angle+=2.0f;
}
void Circle::move()
{
    switch(m_Direction)
    {
        case DIR_LEFT:
            m_Position-= glm::vec3(m_Length,0.0f,0.0f);
        break;
        case DIR_RIGHT:
            m_Position+= glm::vec3(m_Length,0.0f,0.0f);
        break;
        case DIR_UP:
            m_Position+= glm::vec3(0.0f,m_Length,0.0f);
        break;
        case DIR_DOWN:
            m_Position-= glm::vec3(0.0f,m_Length,0.0f);
        break;                        
    }
}