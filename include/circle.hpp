#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<glm/mat3x3.hpp>
#include<glm/gtx/matrix_transform_2d.hpp>
class Circle {

    public:
        enum DIRECTION
         {
            DIR_RIGHT   =  1,
            DIR_LEFT    = -1,
            DIR_UP      =  2,
            DIR_DOWN    = -2
         };

        Circle(float  x,float y,float length);

        glm::mat3 mtxTransform();
        glm::vec3 getPosition();
        glm::vec4 getColor();
        glm::mat3 getTransform();
        float angle;
        
        DIRECTION getDirection();

        void setDirection(DIRECTION dir);

        void move();           

        void Trotate(glm::mat3& mtxTransform); 

    private:
        glm::vec4   m_Color;
        glm::vec3   m_Position;
        glm::mat3   mTxTransform;
        float       m_Length;
        

    DIRECTION   m_Direction;

};
#endif