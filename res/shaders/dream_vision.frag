#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Customizable blending factor for effect intensity
uniform float effectStrength = 0.5; // Value between 0.0 (no effect) to 1.0 (full effect)

void main() {
    vec4 originalColor = texture(texture0, fragTexCoord);
    vec4 color = originalColor;

    // Reduced number of offset samples for less intense effect
    color += texture(texture0, fragTexCoord + 0.001);
    color += texture(texture0, fragTexCoord + 0.003);
    color += texture(texture0, fragTexCoord + 0.005);

    color += texture(texture0, fragTexCoord - 0.001);
    color += texture(texture0, fragTexCoord - 0.003);
    color += texture(texture0, fragTexCoord - 0.005);

    // Average the color channels for a grayscale-like effect
    color.rgb = vec3((color.r + color.g + color.b) / 3.0);
    color = color / 7.0; // Adjusted divisor to reduce intensity

    // Blend with original color for reduced effect strength
    color = mix(originalColor, color, effectStrength);

    fragColor = color;
}
