#version 330 core
layout(location = 0) in vec3 _position;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoord;
layout(location = 3) in mat4 _offset;

uniform mat4 V;
uniform mat4 P;
uniform float time;
uniform uint worldWidth;
uniform uint worldLength;
uniform uint worldHeight;
uniform float blockSizeOffset;

out vec3 position;
out vec3 normal;
out vec3 light;
out vec2 st;
out uint surfaceDirection;

uint checkSurface(vec3 normal);

void main()
{
    //Out variables
    position = vec3(_offset * vec4(_position * (1.0/blockSizeOffset), 1.0));
    light = normalize(vec3(worldWidth/100, 0, worldLength/100));
    normal = normalize(vec3(_offset * vec4(_normal * (1.0/blockSizeOffset), 1.0)));
    surfaceDirection = checkSurface(_normal);
    st = _texCoord;

    gl_Position = P*V*vec4(position, 1.0);
}

vec3 upVec = vec3(0.0, 1.0, 0.0);
vec3 sideVecX = vec3(1.0, 0.0, 0.0);
vec3 sideVecZ = vec3(0.0, 0.0, 1.0);

vec3 xFilter = vec3(1.0, 0.0, 0.0);
vec3 yFilter = vec3(0.0, 1.0, 0.0);
vec3 zFilter = vec3(0.0, 0.0, 1.0);

uint checkSurface(vec3 normal)
{
    if(dot(upVec, normal) > 0.5)
        return 0; //Top side
    else if(dot(upVec, normal) < -0.5)
        return 1; //Bottom side
    else
        return 2; //Middle side
}