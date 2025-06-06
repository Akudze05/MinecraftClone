//
// Created by Asg on 06.06.2025.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\glm.hpp>
#include <C:\Users\Asg\CLionProjects\MineEngine\dependencies\glm\gtc\matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 5.0f); // Отодвинули камеру назад
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f); // Смотрит вдоль оси Z
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    float Speed = 2.5f;
    float Sensitivity = 0.1f;
    float Yaw = -90.0f;
    float Pitch = 0.0f;

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
};



#endif //CAMERA_H
