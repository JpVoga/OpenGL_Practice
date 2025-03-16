#include "cg/vertex.hpp"
#include "cg/window.hpp"
#include "cg/vertexBuffer.hpp"
#include "cg/indexBuffer.hpp"
#include "cg/shader.hpp"
#include "cg/texture.hpp"
#include "cg/space.hpp"

// Settings:

constexpr bool usePerspectiveProjection = true;
constexpr float32_t zNear = 1.0;
constexpr float32_t zFar = 10000.0;

const std::vector<CG::Vertex> vertices = {
    {{-250.0f, +250.0f, +0.0f}, {0.0f, 1.0f}},
    {{+250.0f, +250.0f, +0.0f}, {1.0f, 1.0f}},
    {{-250.0f, -250.0f, +0.0f}, {0.0f, 0.0f}},
    {{+250.0f, -250.0f, +0.0f}, {1.0f, 0.0f}}
};

const Math::IndexList triangles = Math::IndexList::create<uint8_t>(
    0, 1, 2,
    1, 2, 3
);

Math::Vector3 position = Math::Vector3::zero;
Math::Vector3 rotation = Math::Vector3::zero;
Math::Vector3 scale    = Math::Vector3::one;

Math::Vector3 cameraPosition = Math::Vector3(0, 0, 1000);
Math::Vector3 cameraRotation = Math::Vector3::zero;
Math::Vector3 cameraScale    = Math::Vector3::one;

Math::Vector3 *boundInput = &position;

void processUserInput() {
    using namespace CG;

    const Window &window = *Window::getCurrentContext();
    const float32_t variation = ((boundInput == &rotation) || (boundInput == &cameraRotation))? 0.1:1;

    if (glfwGetKey(window.getWPtr(), GLFW_KEY_1) || glfwGetKey(window.getWPtr(), GLFW_KEY_KP_1)) boundInput = &position;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_2) || glfwGetKey(window.getWPtr(), GLFW_KEY_KP_2)) boundInput = &rotation;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_3) || glfwGetKey(window.getWPtr(), GLFW_KEY_KP_3)) boundInput = &scale;

    if (glfwGetKey(window.getWPtr(), GLFW_KEY_4) || glfwGetKey(window.getWPtr(), GLFW_KEY_KP_4)) boundInput = &cameraPosition;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_5) || glfwGetKey(window.getWPtr(), GLFW_KEY_KP_5)) boundInput = &cameraRotation;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_6) || glfwGetKey(window.getWPtr(), GLFW_KEY_KP_6)) boundInput = &cameraScale;

    if (glfwGetKey(window.getWPtr(), GLFW_KEY_A)) boundInput->x -= variation;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_D)) boundInput->x += variation;

    if (glfwGetKey(window.getWPtr(), GLFW_KEY_S)) boundInput->z += variation;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_W)) boundInput->z -= variation;

    if (glfwGetKey(window.getWPtr(), GLFW_KEY_E)) boundInput->y -= variation;
    if (glfwGetKey(window.getWPtr(), GLFW_KEY_Q)) boundInput->y += variation;
}

int main() {
    using namespace CG;

    Window window("Main Window", 1024, 512);
    Window window1("Other Window", 512, 512);
    std::vector<CG::Window *> toBeClosed;

    VertexBuffer vbo(vertices);
    IndexBuffer ibo(triangles);

    Shader shader("./assets/shaders/basic.shader");

    Texture texture("./assets/textures/container.jpg");
    texture.setMinificationMode(TextureMinificationMode::LinearMipmapLinear);
    texture.setMagnificationMode(TextureMagnificationMode::Linear);
    texture.setWrapModeU(TextureWrapMode::ClampToEdge);
    texture.setWrapModeV(TextureWrapMode::ClampToEdge);
    texture.slot(10);
    shader.setUniform<int32_t>("baseMap", texture.getSlot());

    const auto renderObj = [&]() -> void {
        processUserInput();

        const Window &window = *Window::getCurrentContext();

        const Matrix4 modelMatrix = model<float32_t>(position, rotationFromEulerDeg(rotation), scale);

        const Matrix4 viewMatrix = view<float32_t>(cameraPosition, rotationFromEulerDeg(cameraRotation), cameraScale);

        const Matrix4 projectionMatrix = usePerspectiveProjection?
            perspectiveDeg<float32_t>(60, window.getAspectRatio(), zNear, zFar):
            ortho<float32_t>(-window.getWidth()/2, +window.getWidth()/2, -window.getHeight()/2, +window.getWidth()/2, zNear, zFar)
        ;

        const Matrix4 mvp = projectionMatrix * viewMatrix * modelMatrix;

        shader.setUniform<Matrix4>("mvp", mvp);

        vbo.bind();
        ibo.bind();
        shader.bind();

        glDrawElements(GL_TRIANGLES, triangles.getMemorySize(), static_cast<GLenum>(triangles.getTypeEnum()), nullptr); // Render
    };

    window.setOnRenderLoop(renderObj);
    window1.setOnRenderLoop(renderObj);

    while (Window::getInstances().size() > 0) {
        for (const auto &window: Window::getInstances()) {
            window->renderLoop();
            if (window->shouldClose()) toBeClosed.push_back(window);
        }

        for (const auto &window: toBeClosed) {
            window->close();
        }
        toBeClosed.clear();
    }

    return 0;
}