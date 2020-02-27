#include <skybox.h>
#include <simpleobject3d.h>
#include <material.h>

SkyBox::SkyBox(float width, const QImage &texture)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    // Back
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, -1.0)));
    // Right
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(3.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(3.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(-1.0, 0.0, 0.0)));
    // Up
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(2.0f/4.0f, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f/4.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));
    // Front
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, 0.0, 1.0)));
    // Left
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 2.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(1.0, 0.0, 0.0)));
    // bottom
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f/4.0f, 0.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(2.0f/4.0f, 0.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(2.0f/4.0f, 1.0f/3.0f), QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        indexes.append(static_cast<uint>(i) + 0);
        indexes.append(static_cast<uint>(i) + 2);
        indexes.append(static_cast<uint>(i) + 1);
        indexes.append(static_cast<uint>(i) + 2);
        indexes.append(static_cast<uint>(i) + 3);
        indexes.append(static_cast<uint>(i) + 1);
    }

    Material *newMtl = new Material;
    newMtl->setDiffuseMap(texture);
    newMtl->setShines(96);
    newMtl->setDiffuseColor(QVector3D(1.0f,1.0f,1.0f));
    newMtl->setAmbienceColor(QVector3D(1.0f,1.0f,1.0f));
    newMtl->setSpecularColor(QVector3D(1.0f,1.0f,1.0f));

    m_cube = new SimpleObject3D(vertexes, indexes, newMtl);
}

SkyBox::~SkyBox()
{
    delete m_cube;
}

void SkyBox::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *functions)
{
    m_cube->draw(program, functions);
}

void SkyBox::rotate(const QQuaternion &r)
{
    (void)r;
}

void SkyBox::translate(const QVector3D &T)
{
    (void)T;
}

void SkyBox::scale(const float &s)
{
    (void)s;
}

void SkyBox::setGlobalTransform(const QMatrix4x4 &g)
{
    (void)g;
}
