struct materialProperty {
    vec3 diffuseColor;
    vec3 ambienceColor;
    vec3 specularColor;
    float shines;
};

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
uniform materialProperty u_materialProperty;
uniform bool u_isUsingDiffuseMap;
uniform bool u_isUsingNormalMap;
varying highp vec4 v_position;
varying highp vec2 v_texturCoord;
varying highp vec3 v_normal;
varying highp mat3 v_tbnMatrix;

void main(void)
{
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 diffMatColor = texture2D(u_diffuseMap, v_texturCoord);
    vec3 usingNormal = v_normal;
    if (u_isUsingNormalMap) usingNormal = normalize(texture2D(u_normalMap, v_texturCoord).rgb * 2.0 - 1.0);
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    if (u_isUsingNormalMap) eyeVect = normalize(v_tbnMatrix * eyeVect);
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition.xyz);
    if (u_isUsingNormalMap) lightVect = normalize(v_tbnMatrix * lightVect);
    vec3 reflectionLight = normalize(reflect(lightVect, usingNormal));

    float len = length(v_position.xyz - eyePosition.xyz);
    float specularFactor = u_materialProperty.shines;
    float ambientFactor = 0.1;

    if (u_isUsingDiffuseMap == false) diffMatColor = vec4(u_materialProperty.diffuseColor, 1.0);
    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(usingNormal, -lightVect)) ;
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffColor;
    resultColor += ambientColor * vec4(u_materialProperty.ambienceColor, 1.0);
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow(max(0.0, dot(reflectionLight, -eyeVect)), specularFactor);
    resultColor += specularColor * vec4(u_materialProperty.specularColor, 1.0);

    gl_FragColor = resultColor;
}
