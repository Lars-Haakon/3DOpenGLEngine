#version 120

varying vec2 texCoord0;

uniform vec3 ambientIntensity;
uniform sampler2D colorMap;

void main()
{
    gl_FragColor = texture2D(colorMap, texCoord0) * vec4(ambientIntensity, 0.0);
}