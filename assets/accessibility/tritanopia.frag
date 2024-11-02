uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = vec4(
        color.r,
        0.95 * color.g + 0.05 * color.b,
        0.433 * color.g + 0.567 * color.b,
        color.a
    );
}
