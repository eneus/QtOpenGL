uniform sampler2D u_texture;
varying highp vec2 v_texturCoord;

void main(void)
{
    gl_FragColor = texture2D(u_texture, v_texturCoord); //texture2D(u_texture, v_texturCoord); //vec4(1.0, 0.0, 0.0, 1.0); //
}
