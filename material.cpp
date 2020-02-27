#include "material.h"

Material::Material()
{
    m_isUsingDiffuseMap = false;
    m_isUsingNormalMap = false;
}

void Material::setName(const QString &mtlName)
{
    m_mtlName = mtlName;
}

const QString &Material::mtlName() const
{
    return m_mtlName;
}

void Material::setDiffuseColor(const QVector3D &diffuseColor)
{
    m_diffuseColor = diffuseColor;
}

const QVector3D &Material::diffuseColor() const
{
    return m_diffuseColor;
}

void Material::setAmbienceColor(const QVector3D &ambienceColor)
{
    m_ambienceColor = ambienceColor;
}

const QVector3D &Material::ambienceColor() const
{
    return m_ambienceColor;
}

void Material::setSpecularColor(const QVector3D &specularColor)
{
    m_specularColor = specularColor;
}

const QVector3D &Material::specularColor() const
{
    return m_specularColor;
}

void Material::setShines(const float &shines)
{
    m_shines = shines;
}

const float &Material::shines() const
{
    return m_shines;
}

void Material::setDiffuseMap(const QString &filename)
{
    m_diffuseMap = QImage(filename);
    m_isUsingDiffuseMap = true;
}

void Material::setDiffuseMap(const QImage &imageFile)
{
    m_diffuseMap = imageFile;
    m_isUsingDiffuseMap = true;
}

const QImage &Material::diffuseMap() const
{
    return m_diffuseMap;
}

bool Material::isUsingDiffuseMap() const
{
    return m_isUsingDiffuseMap;
}

void Material::setNormalMap(const QString &filename)
{
    m_normalMap = QImage(filename);
    m_isUsingNormalMap = true;
}

void Material::setNormalMap(const QImage &imageFile)
{
    m_normalMap = imageFile;
    m_isUsingNormalMap = true;
}

const QImage &Material::normalMap() const
{
    return m_normalMap;
}

bool Material::isUsingNormalMap() const
{
    return m_isUsingNormalMap;
}
