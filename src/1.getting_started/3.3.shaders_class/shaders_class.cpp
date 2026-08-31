#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <iostream>
#include <vector>
#include <random>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int RotateAngle = 0;

int count = 360;
//float test = 0.0f;
float shadeR, shadeB, shadeG;
std::vector<float> baseColors;

struct Particle { float x, y; };
std::vector<Particle> particles;
float PARTICLE_SPEED = 0.005f;
float PARTICLE_SIZE = 0.01f;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    srand(time(NULL));
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    /*
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };
    */

    float r = 0.5f;
    float pi = 3.14159265f;

    shadeR = 0.0f; shadeB = 0.0f; shadeG = 1.0f;//default


    int colorPhase = 0;
    //count = 20;
    std::vector<float> vertices;

    for (int i = 0; i < count; ++i)
    {
        int color = RotateAngle + i;
        float angle0 = 2.0f * pi * (float)i / count;
        float angle1 = 2.0f * pi * (float)(i + 1) / count;

        // center
        vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f });

        baseColors.push_back(shadeR);
        baseColors.push_back(shadeG);
        baseColors.push_back(shadeB);

        // edge 1
        vertices.insert(vertices.end(), { r * cos(angle0), r * sin(angle0), 0.0f,
            shadeR, shadeG, shadeB });

        // edge 2
        vertices.insert(vertices.end(), { r * cos(angle1), r * sin(angle1), 0.0f,
            shadeR, shadeG, shadeB });

        if (color < count / 3) {
            shadeR += 1.0 / 120.0f;
            shadeG -= 1.0 / 120.0f;
        }
        if (color > count / 3 && color < (count / 3) * 2) {
            shadeR -= 1.0 / 120.0f;
            shadeB += 1.0 / 120.0f;
        }
        if (color > (count / 3) * 2) {
            shadeG += 1.0 / 120.0f;
            shadeB -= 1.0 / 120.0f;
        }
        //std::cout<<i<<color<< "\n";
    }

    std::vector<float> arrow = {
        0.65f * cos(0.0f), 0.65f * sin(0.0f), 0.0f,     1.0f, 0.0f, 0.0f, // top
        0.55f * cos(-0.05f), 0.55f * sin(-0.1f), 0.0f, 0.0f, 1.0f, 0.0f, // left
        0.55f * cos(0.05f), 0.55f * sin(0.1f), 0.0f,   0.0f, 0.0f, 1.0f  // right
    };

    unsigned int VBO, VAO;
    unsigned int arrowVBO, arrowVAO;
    unsigned int particleVBO, particleVAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &arrowVAO);
    glGenBuffers(1, &arrowVBO);

    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);

    // circle
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // arrow
    glBindVertexArray(arrowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);
    glBufferData(GL_ARRAY_BUFFER, arrow.size() * sizeof(float), arrow.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //particle
    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        //std::cout << "RotateAngle: " << RotateAngle << std::endl;

        //particle
        float markerAngle = 2.0f * pi * RotateAngle / count;

        if (rand() % 10 == 0) {
            float spawnAngle = ((float)rand() / RAND_MAX) * 2.0f * pi;
            float radius = 1.0f + ((float)rand() / RAND_MAX) * 0.3f;
            particles.push_back({ radius * cos(spawnAngle), radius * sin(spawnAngle) });
        }

        for (auto it = particles.begin(); it != particles.end(); ) {
            it->x += cos(markerAngle) * PARTICLE_SPEED;
            it->y += sin(markerAngle) * PARTICLE_SPEED;
            if (it->x > 2.0f || it->x < -2.0f || it->y > 2.0f || it->y < -2.0f)
                it = particles.erase(it);
            else
                ++it;
        }
        //circle
        for (int i = 0; i < count; ++i)
        {

            int srcIndex = ((i - RotateAngle) % count + count) % count;

            int base = i * 18;//x,y,z,r,g,b

            // edge 1
            vertices.at(base + 9) = baseColors.at(srcIndex * 3 + 0);
            vertices.at(base + 10) = baseColors.at(srcIndex * 3 + 1);
            vertices.at(base + 11) = baseColors.at(srcIndex * 3 + 2);

            // edge 2
            vertices.at(base + 15) = baseColors.at(srcIndex * 3 + 0);
            vertices.at(base + 16) = baseColors.at(srcIndex * 3 + 1);
            vertices.at(base + 17) = baseColors.at(srcIndex * 3 + 2);


        }
        //arrow.at(3) = 1.0f;
        //arrow.at(4) = 0.0f;
        //arrow.at(5) = 0.0f;

        //arrow.at(9) = 0.0f;
        //arrow.at(10) = 1.0f;
        //arrow.at(11) = 0.0f;

        //arrow.at(15) = 0.0f;
        //arrow.at(16) = 0.0f;
        //arrow.at(17) = 1.0f;
        //RGB triangle (arrow head)
        if (colorPhase == 0) {
            arrow.at(3) -= 0.001f;
            arrow.at(4) += 0.001f;
            arrow.at(10) -= 0.001f;
            arrow.at(11) += 0.001f;
            arrow.at(17) -= 0.001f;
            arrow.at(15) += 0.001f;
            if (arrow.at(4) >= 1.0f) {
                colorPhase = 1;
            }
        }
        else if (colorPhase == 1) {
            arrow.at(4) -= 0.001f;
            arrow.at(5) += 0.001f;
            arrow.at(11) -= 0.001f;
            arrow.at(9) += 0.001f;
            arrow.at(15) -= 0.001f;
            arrow.at(16) += 0.001f;
            if (arrow.at(5) >= 1.0f) {
                colorPhase = 2;
            }
        }
        else if (colorPhase == 2) {
            arrow.at(5) -= 0.001f;
            arrow.at(3) += 0.001f;
            arrow.at(9) -= 0.001f;
            arrow.at(10) += 0.001f;
            arrow.at(16) -= 0.001f;
            arrow.at(17) += 0.001f;
            if (arrow.at(3) >= 1.0f) {
                colorPhase = 0;
            }
        }
        std::cout << vertices.size() << "\n";
        markerAngle = 2.0f * pi * RotateAngle / count;

        //top left right (arrow pos)
        arrow.at(0) = 0.65f * cos(markerAngle);
        arrow.at(1) = 0.65f * sin(markerAngle);

        arrow.at(6) = 0.55f * cos(markerAngle - 0.1f);
        arrow.at(7) = 0.55f * sin(markerAngle - 0.1f);

        arrow.at(12) = 0.55f * cos(markerAngle + 0.1f);
        arrow.at(13) = 0.55f * sin(markerAngle + 0.1f);

        std::vector<float> particleVerts;
        for (auto& p : particles) {
            particleVerts.insert(particleVerts.end(), 
                {
                    p.x + cos(markerAngle) * PARTICLE_SIZE,   p.y + sin(markerAngle) * PARTICLE_SIZE,   0.0f, 1.0f , 0.5f, 1.0f,
                    p.x - sin(markerAngle) * PARTICLE_SIZE,   p.y + cos(markerAngle) * PARTICLE_SIZE,   1.0f, 1.0f, 1.0f, 1.0f,
                    p.x + sin(markerAngle) * PARTICLE_SIZE,   p.y - cos(markerAngle) * PARTICLE_SIZE,   1.0f, 1.0f, 1.0f, 1.0f
                });
        }

        glBindVertexArray(particleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
        glBufferData(GL_ARRAY_BUFFER, particleVerts.size() * sizeof(float), particleVerts.data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(arrowVAO);
        glBindBuffer(GL_ARRAY_BUFFER, arrowVBO);
        glBufferData(GL_ARRAY_BUFFER, arrow.size() * sizeof(float), arrow.data(), GL_STATIC_DRAW);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        ourShader.use();
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

        glBindVertexArray(arrowVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, count * 3);

        glBindVertexArray(particleVAO);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)particles.size() * 3);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &arrowVAO);
    glDeleteBuffers(1, &arrowVBO);
    glDeleteVertexArrays(1, &particleVAO);
    glDeleteBuffers(1, &particleVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        //std::cout << shadeR << shadeG << shadeB << "\n";
        RotateAngle++;
        //std::cout << "RotateAngle: " << RotateAngle << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        RotateAngle--;
        //std::cout << "RotateAngle: " << RotateAngle << std::endl;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
