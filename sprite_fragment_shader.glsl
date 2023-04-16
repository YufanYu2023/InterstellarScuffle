// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;
//in float saturation

// Texture sampler
uniform sampler2D onetex;

void main()
{   
    // Sample texture
    vec4 color = texture2D(onetex, uv_interp);

    // Assign color to fragment
    //gl_FragColor = vec4(color.r, color.g, color.b, color.a);

    float saturation = 0;//0: full color, 1:greyscale
    float gray = 0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
    gl_FragColor = vec4(color.rgb * (1.0 - saturation) + (gray * saturation), color.a);

    // Check for transparency
    if(color.a < 1.0)
    {
         discard;
    }
}
