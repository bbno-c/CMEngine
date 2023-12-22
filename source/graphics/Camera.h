#pragma once

#include <glm/glm.hpp>

namespace CMEngine {

    class Camera {
    public:
        Camera(int screenWidth, int screenHeight);
        void SetPosition(const glm::vec3& newPosition);
        void SetDirection(const glm::vec3& newFront, const glm::vec3& newUp);
        void SetPerspective(float newFov, float newNear, float newFar);
        void MoveForward(float deltaTime);
        void MoveBackward(float deltaTime);
        void MoveLeft(float deltaTime);
        void MoveRight(float deltaTime);
        void MoveUp(float deltaTime);
        void MoveDown(float deltaTime);
        void Rotate(float xoffset, float yoffset, bool constrainPitch = true);
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetProjectionMatrix() const;

        const glm::vec3& GetPosition() const {
            return position;
        }

    private:
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();

        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        float yaw;
        float pitch;
        float movementSpeed;
        float rotationSpeed;
        int screenWidth;
        int screenHeight;
        float fov;
        float nearPlane;
        float farPlane;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };
}