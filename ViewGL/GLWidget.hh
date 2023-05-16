#pragma once

#define GLWIDGET_H

#include <math.h>

#include <QtWidgets>
#include <QGLWidget>
#include <QtOpenGL>
#include <QTime>
#include <QtDebug>
#include <QString>

#include "Controller.hh"

#include "ViewGL/GLShader.hh"

#include "GPUConnections/GPUScene.hh"
#include "GPUConnections/GPUCamera.hh"

#include "Model/Modelling/Lights/LightFactory.hh"
#include "Model/Modelling/Animation.hh"

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

const int numShaders = 10;

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    GLWidget(const QGLFormat &glf, QWidget *parent=0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

public slots:

    void updateObject(shared_ptr<GPUMesh> obj);
    void updateScene();

    void saveImage();
    void saveAnimation();

    void activaColorShader();
    void activaDepthShader();
    void activaNormalShader();
    void activaGouraudShader();
    void activaPhongShader();
    void activaGouraudBlinnShader();
    void activaBlinnPhongShader();
    void activaToonShader();


    void activaEnvMapping();
    void activaReflection();
    void activaTransparency();

    void setPerspective(float nearPlane, float farPlane);
    void setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up);
    void setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                    const QVector3D &Is, const QVector3D &coefs);

    // Acció per activar a cada update del timer
    void setCurrentFrame();

signals:
    void ObsCameraChanged(shared_ptr<GPUCamera> cam);
    void FrustumCameraChanged(shared_ptr<GPUCamera> cam);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void setXRotation(int angle);
    void setYRotation(int angle);

    void qNormalizeAngle(double &angle);

    void Zoom (int positiu);
    void saveFrame();

private:
    QTimer *timer;
    unsigned int currentFrame;
    unsigned int currentImage;

    GLShader::SHADER_TYPES currentShader;

    QPoint lastPos;   /* per interactuar amb la camera */

    // TO DO: Pràctica 2: Fase 1: Per ara nomes es té un parell vertex-fragment
    // i cal estendre-ho a tenir mé parells
    shared_ptr<QGLShaderProgram> program;

    shared_ptr<GLShader> shaderList[numShaders];
    void initShader(GLShader::SHADER_INDEX index, const char* vertexFile, const char* fragmentFile);
    void initShadersGPU();
    void updateShader();
    void useShader(GLShader::SHADER_TYPES s);
};

