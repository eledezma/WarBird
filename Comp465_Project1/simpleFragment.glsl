# version 330 core

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 HeadLightPosition;
uniform vec3 HeadLightIntensity;
uniform vec3 PointLightPosition;
uniform vec3 PointLightIntensity;
uniform bool IsTexture; 

uniform bool HeadLightOn; // toggles set in application
uniform bool PointLightOn;
uniform bool DebugOn; 

out vec4 fragColor;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


vec3 vLight(vec3 LightPosition, vec3 LightIntensity, bool directional) {
    float ambient = 0.2f;
    // scale directional ambient
	float diffuse = 0.0f;
    // compute diffuse in all cases
	vec3 n, s;
    // normal, light source
	if (directional)
	{ 
		s = normalize(LightPosition); 
	}
    else {
    // point light has no ambient
		s = normalize(LightPosition - fPosition);
		ambient = 0.0f;

	}
	n = normalize(fNormal);
    diffuse = max(dot(s, n), 0.0);

    // reflected light
	return ambient * LightIntensity + diffuse * LightIntensity;
}



void main() {
	vec3 tempColor = vec3(color) * 0.1f;
	//initial value
	if (HeadLightOn){
		tempColor += vLight(HeadLightPosition,HeadLightIntensity, true);
	}
	if (PointLightOn) 
	{
		tempColor += vLight(PointLightPosition, PointLightIntensity, false);
	}

	if (IsTexture){   // use texture on surface
		
		float x = rand(fPosition.xy);
        fragColor = vec4(x/6.0, x/4.0, x/3.0, 1.0);
	}
	else{
		fragColor = vec4(tempColor, 1.0);
	}
	
}


