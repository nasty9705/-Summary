#version 330

in vec4 point;
//in vec2 position;
//in vec4 variance;
out vec2 tmp1;
out vec4 send1;

uniform mat4 camera;
uniform samplerBuffer tboSampler;
uniform samplerBuffer tboSampler2;


void main() {   
	//vec2 bladePos = position;
	vec2 bladePos = texelFetch(tboSampler, gl_InstanceID).xy;
	vec4 var      = texelFetch(tboSampler2, gl_InstanceID);

	mat4 scaleMatrix = mat4(1.0);
	scaleMatrix[0][0] = 0.01;
	scaleMatrix[1][1] = 0.1;
    //scaleMatrix[1][1] = 0.05 + rand(bladePos)/10.0;
	mat4 positionMatrix = mat4(1.0);
	positionMatrix[3][0] = bladePos.x;
	positionMatrix[3][2] = bladePos.y;

	float a = 200*sqrt(dot(bladePos.xy, bladePos.xy));
    mat4 turnMatrix = mat4(0.0);
	turnMatrix[0][0] = cos(a);
    turnMatrix[0][2] = -sin(a);
    turnMatrix[1][1] = 1;
    turnMatrix[2][0] = sin(a);
    turnMatrix[2][2] = cos(a);
    turnMatrix[3][3] = 1;

    gl_Position = camera * (positionMatrix * turnMatrix * scaleMatrix * point + var * point.y * point.y);
	tmp1 = vec2(point.x, point.y);
	send1 = point;
}