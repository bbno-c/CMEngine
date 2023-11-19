#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CMEngine {

    Camera::Camera(int screenWidth, int screenHeight)
        : position(glm::vec3(0.0f, 0.0f, 3.0f)),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)),
        yaw(-90.0f),
        pitch(0.0f),
        movementSpeed(0.1f),
        rotationSpeed(0.1f),
        screenWidth(screenWidth),
        screenHeight(screenHeight),
        fov(45.0f),
        nearPlane(0.1f),
        farPlane(100.0f) {
        UpdateProjectionMatrix();
        UpdateViewMatrix();
    }

    void Camera::SetPosition(const glm::vec3& newPosition) {
        position = newPosition;
        UpdateViewMatrix();
    }

    void Camera::SetDirection(const glm::vec3& newFront, const glm::vec3& newUp) {
        front = glm::normalize(newFront);
        up = glm::normalize(newUp);
        UpdateViewMatrix();
    }

    void Camera::SetPerspective(float newFov, float newNear, float newFar) {
        fov = newFov;
        nearPlane = newNear;
        farPlane = newFar;
        UpdateProjectionMatrix();
    }

    void Camera::MoveForward(float deltaTime) {
        position += front * movementSpeed /** deltaTime*/;
        UpdateViewMatrix();
    }

    void Camera::MoveBackward(float deltaTime) {
        position -= front * movementSpeed /** deltaTime*/;
        UpdateViewMatrix();
    }

    void Camera::MoveLeft(float deltaTime) {
        position -= glm::normalize(glm::cross(front, up)) * movementSpeed /** deltaTime*/;
        UpdateViewMatrix();
    }

    void Camera::MoveRight(float deltaTime) {
        position += glm::normalize(glm::cross(front, up)) * movementSpeed /** deltaTime*/;
        UpdateViewMatrix();
    }

    void Camera::Rotate(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= rotationSpeed;
        yoffset *= rotationSpeed;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        UpdateViewMatrix();
    }

    const glm::mat4& Camera::GetViewMatrix() const {
        return viewMatrix;
    }

    const glm::mat4& Camera::GetProjectionMatrix() const {
        return projectionMatrix;
    }

    void Camera::UpdateViewMatrix() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        viewMatrix = glm::lookAt(position, position + front, up);
    }

    void Camera::UpdateProjectionMatrix() {
        projectionMatrix = glm::perspective(glm::radians(fov),
            static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
            nearPlane, farPlane);
    }
}