// main.cpp
#include <gl.h>
#include <glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct ShaderSources {
  std::string Vertex;
  std::string Fragment;
};

enum ModoShader { None = -1, Vertex = 0, Fragment = 1 };

// Parsea el shader
ShaderSources parseShader(const std::string& ruta) {
  std::ifstream stream(ruta);
  if (!stream) {
    std::cerr << "Error al abrir el archivo: " << ruta << std::endl;
    exit(0);
  }

  std::string line;
  std::stringstream ss[2];
  ModoShader modo = ModoShader::None;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        modo = ModoShader::Vertex;
      else if (line.find("fragment") != std::string::npos)
        modo = ModoShader::Fragment;
    } else {
      if (modo != ModoShader::None) ss[(int)modo] << line << '\n';
    }
  }
  return {ss[0].str(), ss[1].str()};
}

const unsigned int WIDTH = 500, HEIGHT = 500;
int main(int argc, char* argv[]) {
  if (!glfwInit()) {
    std::cout << "No se pudo inicializar glfw\n";
    exit(0);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 1);

  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hola wenas", 0, 0);

  if (window == NULL) {
    std::cout << "No se pudo crear la ventana\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  int version = gladLoadGL(glfwGetProcAddress);
  if (version == 0) {
    std::cout << "Fallo al cargar glad\n";
  }

  float triangulo[] = {0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};

  unsigned int VAO;
  glCreateVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glCreateBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangulo), triangulo, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  ShaderSources sources = parseShader("/home/alumno/Escritorio/OPENGLTest/src/shader/shader.glsl");
  const char* vsource = sources.Vertex.c_str();
  const char* fsource = sources.Fragment.c_str();

  unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader, 1, &vsource, NULL);
  glCompileShader(vshader);

  unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader, 1, &fsource, NULL);
  glCompileShader(fshader);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vshader);
  glAttachShader(shaderProgram, fshader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  while (!glfwWindowShouldClose(window)) {
    // render here
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  return 0;
}
