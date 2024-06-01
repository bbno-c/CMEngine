#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace CMEngine {

    Camera::Camera(int screenWidth, int screenHeight)
        : position(glm::vec3(0.0f, 0.0f, 0.0f)),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)),
        yaw(-90.0f),
        pitch(0.0f),
        movementSpeed(0.1f),
        rotationSpeed(0.1f),
        screenWidth(screenWidth),
        screenHeight(screenHeight),
        fov(45.0f),
        nearPlane(CAM_NEAR),
        farPlane(CAM_FAR) {
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

    void Camera::MoveUp(float deltaTime) {
        position += up * movementSpeed /** deltaTime*/;
        UpdateViewMatrix();
    }

    void Camera::MoveDown(float deltaTime) {
        position -= up * movementSpeed /** deltaTime*/;
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

    std::string Camera::Serialize() const {
        std::ostringstream oss;
        oss << position.x << " " << position.y << " " << position.z << " "
            << front.x << " " << front.y << " " << front.z << " "
            << up.x << " " << up.y << " " << up.z << " "
            << yaw << " " << pitch;
        return oss.str();
    }

    void Camera::Deserialize(const std::string& data) {
        std::istringstream iss(data);
        iss >> position.x >> position.y >> position.z
            >> front.x >> front.y >> front.z
            >> up.x >> up.y >> up.z
            >> yaw >> pitch;
        UpdateViewMatrix();
    }

    void Camera::SaveCurrentState() {
        cameraStates.push_back(Serialize());
        SaveStatesToFile("camera_states.txt");
    }

    void Camera::LoadState(int index) {
        if (index >= 0 && index < cameraStates.size()) {
            Deserialize(cameraStates[index]);
        }
    }

    void Camera::DeleteState(int index) {
        if (index >= 0 && index < cameraStates.size()) {
            cameraStates.erase(cameraStates.begin() + index);
        }
    }

    const std::vector<std::string>& Camera::GetStates() const {
        return cameraStates;
    }

    void Camera::SaveStatesToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& state : cameraStates) {
                file << state << "\n";
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file for writing: " << filename << "\n";
        }
    }

    void Camera::LoadStatesFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            cameraStates.clear();
            std::string line;
            while (std::getline(file, line)) {
                cameraStates.push_back(line);
            }
            file.close();
        }
        else {
            std::cerr << "Unable to open file for reading: " << filename << "\n";
        }
    }

}