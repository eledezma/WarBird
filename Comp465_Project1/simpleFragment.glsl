/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 color;
out vec4 fragColor;

void main() {

  float ambientStrength = 0.7f;
  vec3 ambient = ambientStrength * vec3(1.0f, 0.7f, 0.0f);
  vec3 result = ambient * vec3(color);
  
  fragColor = vec4(result, 1.0f);;

  }