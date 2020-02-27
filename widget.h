#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QBasicTimer>

class ObjectEgine3D;
class Transformational;
class Group3D;
class Camera3D;
class SkyBox;

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void weelEvent(QWheelEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);

    void initShaders();
    void initCude(float width);
    void initCuboid(float width, float height, float thickness);

private:
    QMatrix4x4 m_projectionMatrix;
    QOpenGLShaderProgram m_program;
    QOpenGLShaderProgram m_programSkyBox;
    QVector2D m_mousePosiotion;

    QVector<ObjectEgine3D *> m_objects;
    QVector<ObjectEgine3D *> m_axis;
    QVector<Group3D *> m_groups;
    QVector<Transformational *> m_transformOjects;

    QBasicTimer m_timer;

    float angelObject;
    float angelGroup1;
    float angelGroup2;
    float angelMain;

    Camera3D *m_camera;
    SkyBox *m_skybox;
};

#endif // WIDGET_H
