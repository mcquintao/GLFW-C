#include "main.h"
#include <math.h>
#include <stdio.h>

static void error_callback(int e, const char *d) {
  printf("Error %d: %s\n", e, d);
}
void processInput(GLFWwindow *window);
char *lerArquivo(const char *nomeArquivo);

// float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

unsigned int VBO;

int main(void) {
  /* Platform */
  struct nk_glfw glfw = {0};
  static GLFWwindow *win;
  int width = 0, height = 0;
  struct nk_context *ctx;
  struct nk_colorf bg;

  /* GLFW */
  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    fprintf(stdout, "[GFLW] failed to init!\n");
    exit(1);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  const  char * vertexShaderSource =
      lerArquivo("../shaders/triangle.vert");
  const char * fragmentShaderSource =
      lerArquivo("../shaders/triangle.frag");

  // printf("%s", vertexShaderSource);
  // printf("%s", fragmentShaderSource);

  GLfloat vertices[] = {

      -0.5f, -0.5f * (sqrt(3)) / 3,    0.0f,

      0.5f,  -0.5f * (sqrt(3)) / 3,    0.0f,

      0.0f,  0.5f * (sqrt(3)) * 2 / 3, 0.0f,

  };

  win = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
  glfwMakeContextCurrent(win);

  /* OpenGL */

  // For GLEW :( (Need to be installed on system)
  //  glewExperimental = 1;
  //  if (glewInit() != GLEW_OK) {
  //      fprintf(stderr, "Failed to setup GLEW\n");
  //      exit(1);
  //  }

  // ---

  // For GLAD :) (../include/glad/glad.h)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("failed initializing glad");
    return -1;
  }
  // ---

  ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
  struct nk_font_atlas *atlas;
  nk_glfw3_font_stash_begin(&glfw, &atlas);
  nk_glfw3_font_stash_end(&glfw);
  bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  GLuint shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(win)) {
    processInput(win);
    glfwGetFramebufferSize(win, &width, &height);

    glViewport(0, 0, width, height);

    glfwPollEvents();
    nk_glfw3_new_frame(&glfw);

    /* GUI */
    if (nk_begin(ctx, "UI Window", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                     NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
      nk_layout_row_static(ctx, 30, 80, 1);

      nk_layout_row_dynamic(ctx, 20, 1);
      nk_label(ctx, "Hello World", NK_TEXT_LEFT);
    }
    nk_end(ctx);

    // DRAW SHADERS //

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER,
                    MAX_ELEMENT_BUFFER);
    glfwSwapBuffers(win);
  }


  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  nk_glfw3_shutdown(&glfw);
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

char *lerArquivo(const char *nomeArquivo) {
  // GERADO PELO CHAT GPT <3 //
  // Abra o arquivo em modo de leitura
  FILE *arquivo = fopen(nomeArquivo, "r");

  // Verifique se o arquivo foi aberto com sucesso
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return NULL;
  }

  // Determine o tamanho do arquivo
  fseek(arquivo, 0, SEEK_END); // Move o cursor para o fim do arquivo
  long tamanho = ftell(
      arquivo); // Pega a posição atual do cursor, que é o tamanho do arquivo
  fseek(arquivo, 0,
        SEEK_SET); // Move o cursor de volta para o início do arquivo

  // Aloque memória para armazenar o conteúdo do arquivo
  char *conteudo = (char *)malloc(
      tamanho + 1); // +1 para o caractere nulo no final da string

  // Verifique se a alocação de memória foi bem-sucedida
  if (conteudo == NULL) {
    printf("Erro ao alocar memória.\n");
    fclose(arquivo);
    return NULL;
  }

  // Leia o conteúdo do arquivo para a variável conteudo
  fread(conteudo, 1, tamanho, arquivo);

  // Adicione um caractere nulo ao final para formar uma string C válida
  conteudo[tamanho] = '\0';

  // Feche o arquivo
  fclose(arquivo);

  return conteudo;
}
