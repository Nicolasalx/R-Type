uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = vec4(
        0.625 * color.r + 0.375 * color.g,
        0.7 * color.r + 0.3 * color.g,
        color.b,
        color.a
    );
}
