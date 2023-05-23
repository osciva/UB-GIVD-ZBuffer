#version 330

in vec3 v_texcoord;
out vec4 colorOut;

uniform vec2 viewportSize;

uniform samplerCube texMap;

/* Night vision model switch */
uniform bool useNightVision;

void main()
{
    vec4 texColor = texture(texMap, v_texcoord.xyz);
    vec3 Itotal = texColor.rgb;

    if (useNightVision) {
        vec2 viewportCenter = viewportSize / 2.0;
        float pixelDist = distance(gl_FragCoord.xy, viewportCenter);

        if (pixelDist <= viewportSize.y / 4.0) {
            Itotal.r = 0.0;
            Itotal.b = 0.0;
        } else {
            Itotal = vec3(0.0, 0.0, 0.0);
        }
    }

    colorOut = vec4(Itotal, 1.0f);
}
