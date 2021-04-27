#ifndef _SHADER_PROGRAM_INCLUDE
#define _SHADER_PROGRAM_INCLUDE

#include "Shader.h"
#include <GL/gl.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

// Using the Shader class ShaderProgram can link a vertex and a fragment shader
// together, bind input attributes to their corresponding vertex shader names,
// and bind the fragment output to a name from the fragment shader

class ShaderProgram {
public:
  ShaderProgram();

  void initShaders(const std::string &vertShader,
                   const std::string &fragShader);
  GLuint getProgram();
  void addShader(const Shader &shader);
  void bindFragmentOutput(const string &outputName);
  GLint bindVertexAttribute(const string &attribName, GLint size,
                            GLsizei stride, GLvoid *firstPointer);
  void link();
  void free();

  void use();

  // Pass uniforms to the associated shaders
  void setUniform1i(const string &uniformName, int v);
  void setUniform2f(const string &uniformName, float v0, float v1);
  void setUniform3f(const string &uniformName, float v0, float v1, float v2);
  void setUniform4f(const string &uniformName, float v0, float v1, float v2,
                    float v3);

  void setUniformMatrix3f(const string &uniformName, glm::mat3 &mat);
  void setUniformMatrix4f(const string &uniformName, glm::mat4 &mat);

  bool isLinked();
  const string &log() const;

private:
  void init();
  GLuint programId;
  bool linked;
  string errorLog;
};

#endif // _SHADER_PROGRAM_INCLUDE