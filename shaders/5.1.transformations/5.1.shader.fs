    #version 330 core 
    out vec4 FragColor;

    in vec3 ourColor;
    in vec2 TexCoord;

    uniform sampler2D texture01;
    uniform sampler2D texture02;

    void main() { 
    FragColor = mix(texture(texture01, TexCoord),texture(texture02,TexCoord),0.2);
    };