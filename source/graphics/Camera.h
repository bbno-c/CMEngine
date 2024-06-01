#pragma once

#include <glm/glm.hpp>

namespace CMEngine {

    constexpr const float CAM_NEAR = 0.1f;
    constexpr const float CAM_FAR = 100.0f;

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
        const glm::vec3& GetFront() const {
            return front;
        }

        std::string Serialize() const;
        void Deserialize(const std::string& data);

        void SaveCurrentState();
        void LoadState(int index);
        void DeleteState(int index); // New method to delete a state
        const std::vector<std::string>& GetStates() const;

        void SaveStatesToFile(const std::string& filename);
        void LoadStatesFromFile(const std::string& filename);

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

        std::vector<std::string> cameraStates;

    };
}