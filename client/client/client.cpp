#include <GLFW/glfw3.h>
#include <GL/freeglut.h> // Include GLUT header
#include <iostream>
#include <string>
#include <unordered_set>

std::unordered_set<std::string> registered_users;
std::string current_user;
bool choosing_registration = false;
bool choosing_login = false;

enum class State {
    Registration,
    Login,
    Chat
};

State currentState;

void registerUser(const std::string& username) {
    if (registered_users.find(username) == registered_users.end()) {
        registered_users.insert(username);
        std::cout << "User " << username << " registered successfully!\n";
        currentState = State::Chat;
    }
    else {
        std::cout << "User " << username << " is already registered!\n";
    }
    choosing_registration = false;
}

void loginUser(const std::string& username) {
    if (registered_users.find(username) != registered_users.end()) {
        std::cout << "User " << username << " logged in successfully!\n";
        currentState = State::Chat;
        current_user = username;
    }
    else {
        std::cout << "User " << username << " is not registered!\n";
    }
    choosing_login = false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (!choosing_registration && !choosing_login) {
            switch (key) {
            case GLFW_KEY_R:
                std::cout << "Choose username for registration: ";
                choosing_registration = true;
                current_user.clear();
                break;
            case GLFW_KEY_L:
                std::cout << "Choose username for login: ";
                choosing_login = true;
                current_user.clear();
                break;
            default:
                break;
            }
        }
        else {
            switch (currentState) {
            case State::Registration:
                switch (key) {
                case GLFW_KEY_ENTER:
                    if (!current_user.empty()) {
                        registerUser(current_user);
                        current_user.clear();
                    }
                    break;
                case GLFW_KEY_BACKSPACE:
                    if (!current_user.empty()) {
                        current_user.pop_back();
                    }
                    break;
                default:
                    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                        current_user.push_back((char)key);
                    }
                    break;
                }
                break;
            case State::Login:
                switch (key) {
                case GLFW_KEY_ENTER:
                    if (!current_user.empty()) {
                        loginUser(current_user);
                        current_user.clear();
                    }
                    break;
                case GLFW_KEY_BACKSPACE:
                    if (!current_user.empty()) {
                        current_user.pop_back();
                    }
                    break;
                default:
                    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
                        current_user.push_back((char)key);
                    }
                    break;
                }
                break;
            case State::Chat:
                // Handle chat input here
                break;
            default:
                break;
            }
        }
    }
}

void renderScreen(GLFWwindow* window) { // Pass the window as an argument
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (!choosing_registration && !choosing_login) {
        // Render instructions
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.8f, 0.8f);
        std::string instructions = "Press 'R' to register, 'L' to login";
        for (char c : instructions) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c); // Call glutBitmapCharacter
        }
    }
    else {
        // Render registration/login text
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.8f, 0.8f);
        std::string text = currentState == State::Registration ? "Enter username for registration: " : "Enter username for login: ";
        text += current_user;
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c); // Call glutBitmapCharacter
        }
    }

    glfwSwapBuffers(window);
}

int main(int argc, char* argv[]) { // Update main signature to accept argc and argv
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Initialize GLUT
    glutInit(&argc, argv);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        renderScreen(window); // Pass the window to the render function
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
