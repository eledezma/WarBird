# version 330 core

in vec4 color;
in vec3 fNormal; 
in vec3 fPosition;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {

  // Setting variables
  vec3 objColor = vec3(color);
    
  // Ambient
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * vec3(1.0f, 0.7f, 0.0f);

  // Diffuse
  vec3 norm = normalize(fNormal);
  vec3 lightDir = normalize(lightPos - fPosition);
  float diff = max(dot(fNormal, lightDir), 0.0f);
  vec3 diffuse = diff * objColor;


  // Specular
  float specularStrength = 0.5f;
  vec3 viewDir = normalize(viewPos - fPosition);
  vec3 reflectDir = reflect(-lightDir, norm);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * vec3(1.0f, 0.7f, 0.0f);  

  vec3 result = (ambient + diffuse + specular) * objColor;
  
  fragColor = vec4(result, 1.0f);

  }