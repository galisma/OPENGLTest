// main.cpp
#include <gl.h>
#include <glfw3.h>

#include <iostream>

const unsigned int WIDTH = 500, HEIGHT = 500;
int main(int argc, char* argv[]) {
  if (!glfwInit()) {
    std::cout << "No se pudo inicializar glfw\n";
    exit(0);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 1);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hola wenas", 0, 0);

  if (window == NULL) {
    std::cout << "No se pudo crear la ventana\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0){
    std::cout << "Fallo al cargar glad\n";
  }

  float triangulo[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
  };

  unsigned int VBO;
  glCreateBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo), triangulo, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  unsigned int VAO;
  glCreateVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  while (!glfwWindowShouldClose(window)) {
    // render here
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  return 0;
}
