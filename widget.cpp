#include "widget.h"
#include <objectegine3d.h>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QtMath>
#include <group3d.h>
#include <camera3d.h>
#include <skybox.h>
#include <material.h>

#include <QDebug>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_camera = new Camera3D;
    m_camera->translate(QVector3D(0.0f, 0.0f, -15.0f));
}

Widget::~Widget()
{
    delete m_camera;

    for (int i = 0; i < m_objects.size(); ++i)
        delete m_objects[i];

    for (int i = 0; i < m_groups.size(); ++i)
        delete m_groups[i];
}

void Widget::initializeGL()
{
    glClearColor(15.0f/255.0f, 23.0f/255.0f, 61.0f/255.0f, 1.0f);

    glEnable(GL_DEPTH_TEST); // buffre depth
    glEnable(GL_CULL_FACE); // vidsichernnya zadnih graney

    initShaders();

    m_groups.append(new Group3D);
    initCuboid(0.01f,0.01f,100.0f);
    m_axis[m_axis.size() -1]->translate(QVector3D(0.0f,0.0f,0.0f));
    m_groups[0]->translate(QVector3D(0.0f, 0.0f, 0.0f));
    m_groups[0]->addOject(m_axis[m_axis.size() -1]);

    m_groups.append(new Group3D);
    initCuboid(100.01f,0.01f,0.0f);
    m_axis[m_axis.size() -1]->translate(QVector3D(0.0f,0.0f,0.0f));
    m_groups[0]->translate(QVector3D(0.0f, 0.0f, 0.0f));
    m_groups[0]->addOject(m_axis[m_axis.size() -1]);

//    m_groups.append(new Group3D);
//    initCude(10.0f); //, QImage(":/cubetexture.png")); // initCuboid(100.0f,0.01f,0.01f);
//    m_objects[m_objects.size() -1]->translate(QVector3D(0.0f,0.0f,0.0f));
//    m_groups[0]->addOject(m_objects[m_objects.size() -1]);
//    m_groups[0]->translate(QVector3D(0.0f, 0.0f, 0.0f));

    float step = 2.0f;

    m_groups.append(new Group3D);
    for (float x = -step; x <= step; x += step) {
        for (float y = -step; y <= step; y += step) {
            for (float z = -step; z <= step; z += step) {
                initCude(1.0f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addOject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[1]->translate(QVector3D(-8.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);
    for (float x = -step; x <= step; x += step) {
        for (float y = -step; y <= step; y += step) {
            for (float z = -step; z <= step; z += step) {
                initCude(1.0f);
                m_objects[m_objects.size() - 1]->translate(QVector3D(x, y, z));
                m_groups[m_groups.size() - 1]->addOject(m_objects[m_objects.size() - 1]);
            }
        }
    }
    m_groups[2]->translate(QVector3D(8.0f, 0.0f, 0.0f));

    m_groups.append(new Group3D);
    m_groups[3]->addOject(m_groups[0]);
    m_groups[3]->addOject(m_groups[1]);
    m_groups[3]->addOject(m_groups[2]);

    m_transformOjects.append(m_groups[3]);

    m_objects.append(new ObjectEgine3D);
    m_objects[m_objects.size() - 1 ]->loadObjectFromFile(":/monkey.obj");

    m_transformOjects.append(m_objects[m_objects.size() - 1]);

    m_groups[0]->addOject(m_camera);

    m_skybox = new SkyBox(200.0f, QImage(":/sky.png"));
    m_timer.start(30, this);
}

void Widget::resizeGL(int w, int h)
{
    float aspect = w / static_cast<float>(h);

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(60,aspect, 0.01f, 1000.0f); // angel of camera, aspect Ratio, Front,Back
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programSkyBox.bind();
    m_programSkyBox.setUniformValue("u_projectionMatrix", m_projectionMatrix);

    m_camera->draw(&m_programSkyBox);
    m_skybox->draw(&m_programSkyBox, context()->functions());
    m_programSkyBox.release();

    m_program.bind();
    m_program.setUniformValue("u_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("u_lightPosition", QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
    m_program.setUniformValue("u_lightPower", 3.0f);

    m_camera->draw(&m_program);
    for (int i = 0; i < m_transformOjects.size(); ++i) {
        m_transformOjects[i]->draw(&m_program, context()->functions());
    }
    m_program.release();

}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton){
        m_mousePosiotion = QVector2D(event->localPos());
    }
    event->accept();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton) return;

    QVector2D diff = QVector2D(event->localPos()) - m_mousePosiotion;
    m_mousePosiotion = QVector2D(event->localPos());

    float angel = diff.length() / 2.0f;

    QVector3D axis = QVector3D(diff.y(), diff.x(), 0.0);

    m_camera->rotate(QQuaternion::fromAxisAndAngle(axis, angel));

    update();
}

void Widget::weelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
        m_camera->translate(QVector3D(0.0f, 0.0f, 0.25f));
    } else if (event->delta() > 0) {
        m_camera->translate(QVector3D(0.0f, 0.0f, -0.25f));
    }
    update();
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    for (int i = 0 ; i < m_objects.size() - 1; ++i) {
        if (i % 2 == 0){
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, static_cast<float>(qSin(qDegreesToRadians(qreal(angelObject))))));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, static_cast<float>(qCos(qDegreesToRadians(qreal(angelObject))))));
        } else {
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, static_cast<float>(qSin(qDegreesToRadians(qreal(angelObject))))));
            m_objects[i]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, static_cast<float>(qCos(qDegreesToRadians(qreal(angelObject))))));
        }
    }

    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 0.0f, 1.0f, static_cast<float>(qSin(qDegreesToRadians(qreal(angelGroup1))))));
    m_groups[0]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f,  static_cast<float>(-qSin(qDegreesToRadians(qreal(angelGroup1))))));

    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, static_cast<float>(qCos(qDegreesToRadians(qreal(angelGroup2))))));
    m_groups[1]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, static_cast<float>(-qCos(qDegreesToRadians(qreal(angelGroup2))))));

    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, static_cast<float>(qSin(qDegreesToRadians(qreal(angelMain))))));
    m_groups[2]->rotate(QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, static_cast<float>(qCos(qDegreesToRadians(qreal(angelMain))))));

    angelObject += static_cast<float>(M_PI) / 180.0f;
    angelGroup1 += static_cast<float>(M_PI) / 360.0f;
    angelGroup2 -= static_cast<float>(M_PI) / 360.0f;
    angelMain += static_cast<float>(M_PI) / 720.0f;

    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        m_groups[0]->delOject(m_camera);
        m_groups[1]->addOject(m_camera);
        break;
    case Qt::Key_Right:
        m_groups[1]->delOject(m_camera);
        m_groups[0]->addOject(m_camera);
        break;
    case Qt::Key_Down:
        m_groups[0]->delOject(m_camera);
        m_groups[1]->addOject(m_camera);
        break;
    case Qt::Key_Up:
        m_groups[0]->delOject(m_camera);
        m_groups[1]->addOject(m_camera);
        QMatrix4x4 tmp;
        tmp.setToIdentity();
        m_camera->setGlobalTransform(tmp);
        break;
    }
    update();
}

void Widget::initShaders()
{
    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.vsh"))
        close();

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.fsh"))
        close();

    if (!m_program.link())
        close();

    if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/skybox.vsh"))
        close();

    if (!m_programSkyBox.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/skybox.fsh"))
        close();

    if (!m_programSkyBox.link())
        close();
}

void Widget::initCude(float width)
{
    float width_div_2 = width / 2.0f;
    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 0.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(1.0f, 1.0f), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, width_div_2), QVector2D(0.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-width_div_2, -width_div_2, -width_div_2), QVector2D(0.0f, 0.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, width_div_2), QVector2D(1.0f, 1.0f), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(width_div_2, -width_div_2, -width_div_2), QVector2D(1.0f, 0.0f), QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        indexes.append(static_cast<uint>(i) + 0);
        indexes.append(static_cast<uint>(i) + 1);
        indexes.append(static_cast<uint>(i) + 2);
        indexes.append(static_cast<uint>(i) + 2);
        indexes.append(static_cast<uint>(i) + 1);
        indexes.append(static_cast<uint>(i) + 3);
    }

    Material *newMtl = new Material;
    newMtl->setDiffuseMap(":/cubetexture.png");
    newMtl->setNormalMap(":/brick_normal1.png");
    newMtl->setShines(96);
    newMtl->setDiffuseColor(QVector3D(1.0f,1.0f,1.0f));
    newMtl->setAmbienceColor(QVector3D(1.0f,1.0f,1.0f));
    newMtl->setSpecularColor(QVector3D(1.0f,1.0f,1.0f));

    ObjectEgine3D *newObj = new ObjectEgine3D;
    newObj->calculateTBN(vertexes);
    newObj->addObject(new SimpleObject3D(vertexes, indexes, newMtl));

    m_objects.append(newObj);
}

void Widget::initCuboid(float width, float height, float thickness)
{
    float half_width = width / 2.0f;
    float half_height = height / 2.0f;
    float half_thickness = thickness / 2.0f;
    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-half_width, half_height, half_thickness), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-half_width, -half_height, half_thickness), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(half_width, half_height, half_thickness), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(half_width, -half_height, half_thickness), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(half_width, half_height, half_thickness), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(half_width, -half_height, half_thickness), QVector2D(0.0, 0.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(half_width, half_height, -half_thickness), QVector2D(1.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(half_width, -half_height, -half_thickness), QVector2D(1.0, 0.0), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(half_width, half_height, half_thickness), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(half_width, half_height, -half_thickness), QVector2D(0.0, 0.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-half_width, half_height, half_thickness), QVector2D(1.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-half_width, half_height, -half_thickness), QVector2D(1.0, 0.0), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(half_width, half_height, -half_thickness), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(half_width, -half_height, -half_thickness), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-half_width, half_height, -half_thickness), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-half_width, -half_height, -half_thickness), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-half_width, half_height, half_thickness), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-half_width, half_height, -half_thickness), QVector2D(0.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-half_width, -half_height, half_thickness), QVector2D(1.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-half_width, -half_height, -half_thickness), QVector2D(1.0, 0.0), QVector3D(-1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(-half_width, -half_height, half_thickness), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-half_width, -half_height, -half_thickness), QVector2D(0.0, 0.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(half_width, -half_height, half_thickness), QVector2D(1.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(half_width, -half_height, -half_thickness), QVector2D(1.0, 0.0), QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexes;
    for (int i = 0; i < 24; i += 4) {
        indexes.append(static_cast<uint>(i) + 0);
        indexes.append(static_cast<uint>(i) + 1);
        indexes.append(static_cast<uint>(i) + 2);
        indexes.append(static_cast<uint>(i) + 2);
        indexes.append(static_cast<uint>(i) + 1);
        indexes.append(static_cast<uint>(i) + 3);
    }

    Material *newCuboidMtl = new Material;
    newCuboidMtl->setDiffuseMap(QImage(":/sky.png").mirrored());
    newCuboidMtl->setShines(96);
    newCuboidMtl->setDiffuseColor(QVector3D(1.0,1.0,1.0));
    newCuboidMtl->setAmbienceColor(QVector3D(1.0,1.0,1.0));
    newCuboidMtl->setSpecularColor(QVector3D(1.0,1.0,1.0));

    ObjectEgine3D *newCuboidObj = new ObjectEgine3D;
    newCuboidObj->addObject(new SimpleObject3D(vertexes, indexes, newCuboidMtl));

    m_axis.append(newCuboidObj);
}
