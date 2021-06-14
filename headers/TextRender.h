#ifndef _TEXTDISPLAY_INCLUDE
#define _TEXTDISPLAY_INCLUDE

#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

class TextRender {
public:
  bool init();
  void renderText(std::string text, float x, float y, float scale,
                  glm::vec3 color);

private:
  struct Character {
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size;        // Size of glyph
    glm::ivec2 bearing;     // Offset from baseline to left/top of glyph
    unsigned int advance;   // Offset to advance to next glyph
  };

  std::map<char, Character> characters;
  unsigned int VAO, VBO;
  inline static unsigned int size = 40;
  inline static glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
  ShaderProgram textProgram;
};

#endif // ifndef _TEXTDISPLAY_INCLUDE
