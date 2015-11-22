# version 330 core

in vec4 color;
in vec3 fNormal; 
in vec3 fPosition;

uniform vec3 lightPos;

out vec4 fragColor;

void main() {

  // Setting variables
  vec3 objColor = vec3(color);
    
  // Ambient
  float ambientStrength = 0.7f;
  vec3 ambient = ambientStrength * vec3(1.0f, 0.7f, 0.0f);

  // Diffuse
 
  vec3 norm = normalize(fNormal);
  vec3 lightDir = normalize(lightPos - fPosition);
  float diff = max(dot(fNormal, lightDir), 0.0f);
  vec3 diffuse = diff * objColor;

  vec3 result = (ambient + diffuse) * objColor;
  
  fragColor = vec4(result, 1.0f);;

  }