#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;   // Base texture (if any)
uniform vec4 colDiffuse;      // Color multiplier (if needed)

// Output fragment color
out vec4 finalColor;

// Customizable parameters for glow
uniform float outlineThickness = 0.001; // Thickness of the glow outline
uniform vec3 glowColor = vec3(1.0, 1.0, 1.0); // Color of the glow (white)

// Helper function to calculate outline intensity
float calculateGlow(vec2 offset) {
    vec4 sample = texture(texture0, fragTexCoord + offset);
return 1.0 - sample.a; // Detect edges based on alpha (1.0 means no alpha, i.e., edge)
}

void main() {
    // Sum glow intensity from multiple surrounding samples
    float glow = 0.0;
    glow += calculateGlow(vec2(-outlineThickness, 0.0));
    glow += calculateGlow(vec2(outlineThickness, 0.0));
    glow += calculateGlow(vec2(0.0, -outlineThickness));
    glow += calculateGlow(vec2(0.0, outlineThickness));
    glow += calculateGlow(vec2(-outlineThickness, -outlineThickness));
    glow += calculateGlow(vec2(outlineThickness, outlineThickness));
    glow += calculateGlow(vec2(-outlineThickness, outlineThickness));
    glow += calculateGlow(vec2(outlineThickness, -outlineThickness));

    // Normalize glow intensity to create a subtle effect
    glow = clamp(glow, 0.0, 1.0);

    // Get the base texture color
    vec4 baseColor = texture(texture0, fragTexCoord) * colDiffuse;

    // Combine the base color with the glowing outline
    vec3 finalGlowColor = mix(baseColor.rgb, glowColor, glow);

    // Blend the original shape with the glow outline based on alpha
    finalColor = vec4(finalGlowColor, baseColor.a);
}
