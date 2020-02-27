#ifndef OBJECTEGINE3D_H
#define OBJECTEGINE3D_H

#include <QVector>
#include <simpleobject3d.h>
#include <materiallibrary.h>

class ObjectEgine3D : public Transformational
{
public:
    ObjectEgine3D();
    void loadObjectFromFile(const QString &filename);
    void addObject(SimpleObject3D *object);
    SimpleObject3D *getObject(quint32 index);
    void calculateTBN(QVector<VertexData> &vertData);
    void draw(QOpenGLShaderProgram *program,QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

private:
    QVector<SimpleObject3D *> m_objects;
    MaterialLibrary m_materialLibrary;

};

#endif // OBJECTEGINE3D_H
