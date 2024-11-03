uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = vec4(
        0.567 * color.r + 0.433 * color.g,
        0.558 * color.r + 0.442 * color.g,
        color.b,
        color.a
    );
}
