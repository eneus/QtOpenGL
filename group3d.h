#ifndef GROUP3D_H
#define GROUP3D_H
#include <transformational.h>
#include <QVector>
#include <QQuaternion>
#include <QMatrix4x4>

class Group3D : public Transformational
{
public:
    Group3D();
    void draw(QOpenGLShaderProgram *program,QOpenGLFunctions *functions);
    void rotate(const QQuaternion &r);
    void translate(const QVector3D &t);
    void scale(const float &s);
    void setGlobalTransform(const QMatrix4x4 &g);

    void addOject(Transformational *obj);
    void delOject(Transformational *obj);
    void delOject(const int &index);

private:
    QQuaternion m_rotate;
    QVector3D m_translate;
    float m_scale;
    QMatrix4x4 m_globalTransform;

    QVector<Transformational*> m_objects;
};

#endif // GROPU3D_H
