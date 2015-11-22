# version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;

uniform mat4 ModelViewProjection;  // = projection * view * model
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec3 fPosition;
out vec3 fNormal;

void main() {
  color = vColor;
  gl_Position = ModelViewProjection * vPosition;
  fPosition = vec3(model * vPosition);
  fNormal = vNormal;

  }