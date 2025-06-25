#version 440

in vec2 TexCoord;
in vec4 fragPos;
in vec3 vNormals;
out vec4 FragColor;

uniform sampler2D colorTexture; // base texture (albedo/diffuse)
uniform sampler2D aoMap;// Ambient Occlusion (black = shadown)
uniform vec3 mainLightPosition;	//light position
uniform vec3 fillLightPosition; // 2nd light (fill)
uniform vec3 backLightPosition; // 3rd light (back)
uniform vec3 mainLightColor; // main light color (warm)
uniform vec3 fillLightColor; // fill light (cooler, bluish)
uniform vec3 backLightColor; // back light (neutral white)
uniform vec3 ka;		//environment light
uniform vec3 kd;		//difuse light
uniform vec3 ks;       // specular factor (ex: 0.2)
uniform vec3 viewPos;   // camera position (world space)
uniform float shininess;// brightness exponent (ex: 32.0)
uniform vec3 sunDirection; // direção da luz do sol (normalizada, vindo do céu)
uniform vec3 sunColor;     // cor da luz do sol

void main()
{	
    vec3 baseColor = texture(colorTexture, TexCoord).rgb;
    float ao = texture(aoMap, TexCoord).r; // use only R channel

    vec3 N = normalize(vNormals);
    vec3 V = normalize(viewPos - vec3(fragPos));

    // --- Ambient lighting ---
    float kaIntensity=0.5;
    vec3 ambient = ka * kaIntensity;

    // --- Main light ---
    vec3 L_main = normalize(mainLightPosition - vec3(fragPos));
    vec3 diffuseMain = kd * max(dot(L_main, N), 0.0f) * mainLightColor;
    vec3 R_main = reflect(-L_main, N);
    float specMain = pow(max(dot(V, R_main), 0.0), shininess);
    vec3 specularMain = ks * specMain * mainLightColor;

    // --- Fill light ---
    vec3 L_fill = normalize(fillLightPosition - vec3(fragPos));
    vec3 diffuseFill = kd * max(dot(L_fill, N), 0.0f) * fillLightColor;
    vec3 R_fill = reflect(-L_fill, N);
    float specFill = pow(max(dot(V, R_fill), 0.0), shininess);
    vec3 specularFill = ks * specFill * fillLightColor;

    // --- Back light ---
    vec3 L_back = normalize(backLightPosition - vec3(fragPos));
    vec3 diffuseBack = kd * max(dot(L_back, N), 0.0f) * backLightColor;
    vec3 R_back = reflect(-L_back, N);
    float specBack = pow(max(dot(V, R_back), 0.0), shininess);
    vec3 specularBack = ks * specBack * backLightColor;

    // Cálculo da luz do sol (direcional)
    vec3 norm = normalize(vNormals);

    // Atenção: geralmente usamos -sunDirection porque ela aponta DE onde vem a luz
    vec3 lightDir = normalize(-sunDirection);

    // Difuso (Lambert)
    float diffSun = max(dot(norm, lightDir), 0.0);
    vec3 diffuseSun = diffSun * sunColor;

    // (Opcional) Especular do sol
    float specularStrengthSun = 0.3; // ajuste à vontade
    vec3 viewDir = normalize(viewPos - fragPos.xyz);
    vec3 reflectDirSun = reflect(-lightDir, norm);
    float specSun = pow(max(dot(viewDir, reflectDirSun), 0.0), 32);
    vec3 specularSun = specularStrengthSun * specSun * sunColor;

    // Ambient (use sua estratégia: pode somar parte da sunColor)
    float ambientStrengthSun = 0.10;
    vec3 ambientSun = ambientStrengthSun * sunColor;

    vec3 finalColor = (ambient + 
                       diffuseMain + specularMain +
                       diffuseFill + specularFill +
                       diffuseBack + specularBack) * baseColor * ao;

    FragColor = vec4(finalColor, 1.0);

    /* 
                DEBUG
        O código abaixo é usado somente para debug.
    */


    //FragColor = vec4(TexCoord, 0, 1);    // para ver o UV
    //FragColor = vec4(vNormals * 0.5 + 0.5, 1); // para ver as normais
    //FragColor = vec4(vec3(1,0,0), 1.0);            // para ver só a textura
    //FragColor = vec4(ao, ao, ao, 1.0);       // para ver só o AO

    // Repete as UVs a cada 1.0 (para não ficar preto fora do [0,1])
    //vec2 uv = fract(TexCoord);

    // Mostra u como vermelho, v como verde. Azul = 0.
    //FragColor = vec4(uv, 0.0, 1.0);

    // Mostra cor por valor do UV.x e UV.y
    //FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);

    //vec2 uv = TexCoord;
    //uv.y = 1.0 - uv.y; // Inverte
    //float checker = mod(floor(uv.x * 10.0) + floor(uv.y * 10.0), 2.0);
    //FragColor = vec4(vec3(checker), 1.0);

    //vec2 uv = fract(TexCoord); // força range [0,1]
    //float checker = mod(floor(uv.x * 10.0) + floor(uv.y * 10.0), 2.0);
    //FragColor = vec4(vec3(checker), 1.0);

}
