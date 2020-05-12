uniform vec3 lightDir;
varying vec3 normal;


void main()
{
	float intensity;	
	vec4 color;	
	intensity = dot(lightDir, normal);

	if (intensity > 0.9)
		color = vec4(1.0,0.9,0.0,0.9);

	else if (intensity > 0.8)
		color = vec4(0.9,0.8,0.0,0.8);

	else if (intensity > 0.7)
		color = vec4(0.8,0.7,0.0,0.8);

	else if (intensity > 0.6)
		color = vec4(0.7,0.6,0.0,0.7);

	else if (intensity > 0.5)
		color = vec4(0.6,0.5,0.0,0.6);

		else if (intensity > 0.4)
		color = vec4(0.5,0.4,0.0,0.5);

	else if (intensity > 0.3)
		color = vec4(0.4,0.3,0.0,0.4);

	else if (intensity > 0.2)
		color = vec4(0.3,0.2,0.0,0.3);

	else if (intensity > 0.2)
		color = vec4(0.2,0.1,0.0,0.2);

	else
		color = vec4(0.1,0.0,0.0,0.1);

	gl_FragColor = color;
}