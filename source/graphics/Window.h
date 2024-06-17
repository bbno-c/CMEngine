#pragma once

#include <SDL.h>
#include <GL/glew.h>
#include "Camera.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>




namespace CMEngine {
    class MyWidget : public QWidget {
        Q_OBJECT

    public:
        explicit MyWidget(QWidget* parent = nullptr);

    signals:
        void sendDataToEngine(const QString& data);

    private slots:
        void onButtonClicked();

    private:
        QLineEdit* inputField;
        QPushButton* sendButton;
    };

    class Window {
    public:
        Window(int width, int height, const char* title);
        ~Window();

        void Update(const float deltaTime, std::shared_ptr<Camera> camera);
        bool IsClosed() const;

        inline SDL_Window* GetWindow() const {
            return m_window;
        }
        inline SDL_GLContext GetContext() const {
            return m_glContext;
        }

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        bool m_isClosed;
        bool firstClick = true;

        uint16_t ScreenWidth;
        uint16_t ScreenHeight;

    };
}