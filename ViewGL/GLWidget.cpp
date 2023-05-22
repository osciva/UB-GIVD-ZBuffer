#include "library/Common.h"
#include "ViewGL/GLWidget.hh"


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

    setFocusPolicy( Qt::StrongFocus );


}

GLWidget::GLWidget(const QGLFormat &glf, QWidget *parent) : QGLWidget(glf, parent) {

    setFocusPolicy( Qt::StrongFocus );
    auto cameraGPU = make_shared<GPUCamera>(this->size().width(), this->size().height());
    Controller::getInstance()->getSetUp()->setCamera(cameraGPU);
}


GLWidget::~GLWidget() {

}


/* Interacció amb GL i inicialització dels programes a la GPU */

/**
 * @brief GLWidget::initializeGL()
 */
void GLWidget::initializeGL() {

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);

    /* Inicialitzem la llista de shaders */
    initShadersGPU();

    /* Indiquem quin shader volem fer servir */
    useShader(GLShader::DEFAULT);

    /* Creacio d'una Light per a poder modificar el seus valors amb la interficie */
    std::vector<shared_ptr<GPULight>> lights;
    auto l = GPULightFactory::getInstance().createLight(LightFactory::POINTLIGHT);
    lights.push_back(l);

    /* Call setAmbientGlobalToGPU function */
    Controller::getInstance()->getSetUp()->setAmbientGlobalToGPU(program);

    Controller::getInstance()->getSetUp()->setLights(lights);

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();
    auto scene = Controller::getInstance()->getScene();

    camera->init(this->size().width(), this->size().height(), scene->capsaMinima);
    emit ObsCameraChanged(camera);
    emit FrustumCameraChanged(camera);
    Controller::getInstance()->getSetUp()->toGPU(program);
    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);

}

/**
 * @brief GLWidget::paintGL()
 */
void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    auto scene = Controller::getInstance()->getScene();
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (scene->CUBEMAP) {
        useShader(GLShader::CUBE_MAP);

        if (cubeTexture) {
            /* Fem set de la textura del cub un únic cop */
            scene->cub->initTextura();
            cubeTexture = false;
        }
        camera->toGPU(program);
        scene->cub->toGPU(program);
        scene->cub->draw();

        useShader(currentShader);
    }

    Controller::getInstance()->getSetUp()->toGPU(program);
    scene->toGPU(program);
    camera->toGPU(program);
    scene->draw();
}

/**
 * @brief GLWidget::resizeGL()
 */
void GLWidget::resizeGL(int width, int height) {
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->vp.a = width;
    camera->vp.h = height;
    auto scene = Controller::getInstance()->getScene();
    camera->CalculaWindow(scene->capsaMinima);
    glViewport(camera->vp.pmin[0], camera->vp.pmin[1], camera->vp.a, camera->vp.h);
    emit FrustumCameraChanged(camera);
    updateGL();
}

/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){
    initShader(GLShader::DEFAULT_SHADER, "://resources/GPUshaders/vshader1.glsl", "://resources/GPUshaders/fshader1.glsl");
    initShader(GLShader::COLOR_SHADER, "://resources/GPUshaders/vColorShader.glsl", "://resources/GPUshaders/fColorShader.glsl");
    initShader(GLShader::DEPTH_SHADER, "://resources/GPUshaders/vdepthshader.glsl", "://resources/GPUshaders/fdepthshader.glsl");
    initShader(GLShader::NORMAL_SHADER, "://resources/GPUshaders/vnormalshader.glsl", "://resources/GPUshaders/fnormalshader.glsl");
    initShader(GLShader::PHONG_SHADER, "://resources/GPUshaders/vphongshader.glsl", "://resources/GPUshaders/fphongshader.glsl");
    initShader(GLShader::GOURAUDPHONG_SHADER, "://resources/GPUshaders/vgouraudphongshader.glsl", "://resources/GPUshaders/fgouraudphongshader.glsl");
    initShader(GLShader::TOON_SHADER, "://resources/GPUshaders/vtoonshader.glsl", "://resources/GPUshaders/ftoonshader.glsl");
    initShader(GLShader::CUBE_MAP_SHADER, "://resources/GPUshaders/vCubeShader.glsl", "://resources/GPUshaders/fCubeShader.glsl");
}

/**
 * @brief GLWidget::initShader
 * Compiles and links vertex & fragment shader
 */
void GLWidget::initShader(GLShader::SHADER_INDEX index, const char *vertexFile, const char *fragmentFile) {
    shaderList[index] = make_shared<GLShader>(vertexFile, fragmentFile);
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

/** Gestio de les animacions i la gravació d'imatges ***/
void GLWidget::setCurrentFrame(){
    auto scene = Controller::getInstance()->getScene();
    scene->update(currentFrame);
    updateGL();
    this->saveFrame();
    currentFrame++;

    if (currentFrame == MAXFRAMES)
        timer->stop();
}

void GLWidget::saveFrame(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentFrame).toString()+".jpg");
}

void GLWidget::saveImage(){
    this->grabFrameBuffer().save("screen_output_"+QVariant(currentImage).toString()+".jpg");
    currentImage++;
}

/** Metodes SLOTS que serveixen al builder per a actualitzar l'escena i els objectes */
void GLWidget::updateObject(shared_ptr<GPUMesh> obj) {
    obj->toGPU(program);
    updateGL();
}

void GLWidget::updateScene() {
    // Control de la nova escena a la GPU
    auto sc = Controller::getInstance()->getScene();
    sc->toGPU(program);
    Controller::getInstance()->setScene(sc);
    // Podeu utilitzar l'altre constructor de Camera per inicialitzar els
    // parametres de la camera.
    auto camera = make_shared<GPUCamera>(this->size().width(), this->size().height());
    Controller::getInstance()->getSetUp()->setCamera(camera);

    camera->actualitzaCamera(sc->capsaMinima);

    emit ObsCameraChanged(camera);
    emit FrustumCameraChanged(camera);

    updateGL();
}

/** Metodes que es criden des dels menús */

void GLWidget::saveAnimation() {
    // Comença el timer de l'animació
    timer = new QTimer(this);
    currentFrame=0;
    currentImage=0;
    connect(timer, SIGNAL(timeout()), this, SLOT(setCurrentFrame()));
    timer->start(1000);

}

void GLWidget::activaColorShader() {
    qDebug()<<"Estic a Color Shader";
    currentShader = GLShader::COLOR;
    useShader(currentShader);
    updateShader();
}

void GLWidget::activaDepthShader() {
    qDebug()<<"Estic a Depth Shader";
    currentShader = GLShader::DEPTH;
    useShader(currentShader);
    updateShader();
}

void GLWidget::activaNormalShader() {
    qDebug()<<"Estic a Normal Shader";
    currentShader = GLShader::NORMAL;
    useShader(currentShader);
    updateShader();
}

void GLWidget::activaGouraudShader() {
    qDebug()<<"Estic a Gouraud - Phong shader";
    currentShader = GLShader::GOURAUDPHONG;
    useShader(currentShader);

    /* Set the useBlinnPhong uniform variable */
    GLint useBlinnPhongLocation = program->uniformLocation("useBlinnPhong");
    glUniform1i(useBlinnPhongLocation, false);  // Use the OpenGL function through glFuncs
    updateShader();
}

void GLWidget::activaPhongShader() {
    qDebug()<<"Estic a Phong Shader";
    currentShader = GLShader::PHONG;
    useShader(currentShader);

    /* Set the useBlinnPhong uniform variable */
    GLint useBlinnPhongLocation = program->uniformLocation("useBlinnPhong");
    glUniform1i(useBlinnPhongLocation, false);  // Use the OpenGL function through glFuncs

    /* Set the useNightVision uniform variable */
    GLint useNightVisionLocation = program->uniformLocation("useNightVision");
    glUniform1i(useNightVisionLocation, false);

    /* Set the useNightVision uniform variable */
    GLint useForniteStormLocation = program->uniformLocation("useForniteStorm");
    glUniform1i(useForniteStormLocation, false);

    updateShader();
}

void GLWidget::activaGouraudBlinnShader() {
    qDebug()<<"Estic a Gouraud - Blinn-Phong shader";
    currentShader = GLShader::GOURAUDPHONG;
    useShader(currentShader);

    /* Set the useBlinnPhong uniform variable */
    GLint useBlinnPhongLocation = program->uniformLocation("useBlinnPhong");
    glUniform1i(useBlinnPhongLocation, true);  // Use the OpenGL function through glFuncs

    updateShader();
}

void GLWidget::activaBlinnPhongShader() {
    qDebug()<<"Estic a Phong Blinn-Phong Shader";
    currentShader = GLShader::PHONG;
    useShader(currentShader);

    /* Set the useBlinnPhong uniform variable */
    GLint useBlinnPhongLocation = program->uniformLocation("useBlinnPhong");
    glUniform1i(useBlinnPhongLocation, true);  // Use the OpenGL function through glFuncs

    /* Set the useNightVision uniform variable */
    GLint useNightVisionLocation = program->uniformLocation("useNightVision");
    glUniform1i(useNightVisionLocation, false);

    /* Set the useNightVision uniform variable */
    GLint useForniteStormLocation = program->uniformLocation("useForniteStorm");
    glUniform1i(useForniteStormLocation, false);

    updateShader();
}

void GLWidget::activaToonShader() {
    qDebug()<<"Estic a Toon";
    currentShader = GLShader::TOON;
    useShader(currentShader);
    updateShader();
}

void GLWidget::activaNightVision() {
     qDebug()<<"Estic a Night Vision";

     currentShader = GLShader::PHONG;
     useShader(currentShader);

     /* Set the useBlinnPhong uniform variable */
     GLint useBlinnPhongLocation = program->uniformLocation("useBlinnPhong");
     glUniform1i(useBlinnPhongLocation, false);

     /* Set the useNightVision uniform variable */
     GLint useNightVisionLocation = program->uniformLocation("useNightVision");
     glUniform1i(useNightVisionLocation, true);

     /* Set the useNightVision uniform variable */
     GLint useForniteStormLocation = program->uniformLocation("useForniteStorm");
     glUniform1i(useForniteStormLocation, false);

     updateShader();
}

void GLWidget::activaForniteStorm() {
     qDebug()<<"Estic a Fornite Storm";

     currentShader = GLShader::PHONG;
     useShader(currentShader);
     /* Set the useBlinnPhong uniform variable */
     GLint useBlinnPhongLocation = program->uniformLocation("useBlinnPhong");
     glUniform1i(useBlinnPhongLocation, false);

     /* Set the useNightVision uniform variable */
     GLint useForniteStormLocation = program->uniformLocation("useForniteStorm");
     glUniform1i(useForniteStormLocation, true);

     /* Set the useNightVision uniform variable */
     GLint useNightVisionLocation = program->uniformLocation("useNightVision");
     glUniform1i(useNightVisionLocation, false);

     updateShader();
}

void GLWidget::activaReflection() {
   //TO DO: Pràctica 2:  implementar a la fase 2
     qDebug()<<"Estic a Reflection";
}

void GLWidget::activaEnvMapping() {
    qDebug()<<"Estic a Environmental Mapping";

    auto scene = Controller::getInstance()->getScene();

    if (scene->CUBEMAP == false) {
        scene->cub = make_shared<GPUCub>();
        scene->CUBEMAP = true;
    }

    //currentShader = GLShader::CUBE_MAP;
    useShader(currentShader);
    updateShader();
}

void GLWidget::activaTransparency() {
    //TO DO: Pràctica 2:  implementar a la fase 2
    qDebug()<<"Estic a Transparencia";
}

void GLWidget::useShader(GLShader::SHADER_TYPES s) {
    switch (s) {
        case GLShader::COLOR:
            program = shaderList[GLShader::COLOR_SHADER]->getProgram();
            shaderList[GLShader::COLOR_SHADER]->activateShader(program);
            break;
        case GLShader::DEPTH:
            program = shaderList[GLShader::DEPTH_SHADER]->getProgram();
            shaderList[GLShader::DEPTH_SHADER]->activateShader(program);
            break;
        case GLShader::NORMAL:
            program = shaderList[GLShader::NORMAL_SHADER]->getProgram();
            shaderList[GLShader::NORMAL_SHADER]->activateShader(program);
            break;
        case GLShader::GOURAUDPHONG:
            program = shaderList[GLShader::GOURAUDPHONG_SHADER]->getProgram();
            shaderList[GLShader::PHONG_SHADER]->activateShader(program);
            break;
        case GLShader::PHONG:
            program = shaderList[GLShader::PHONG_SHADER]->getProgram();
            shaderList[GLShader::PHONG_SHADER]->activateShader(program);
            break;
        case GLShader::TOON:
            program = shaderList[GLShader::TOON_SHADER]->getProgram();
            shaderList[GLShader::TOON_SHADER]->activateShader(program);
            break;
        case GLShader::CUBE_MAP:
            program = shaderList[GLShader::CUBE_MAP_SHADER]->getProgram();
            shaderList[GLShader::CUBE_MAP_SHADER]->activateShader(program);
            break;
        default:
            program = shaderList[GLShader::DEFAULT_SHADER]->getProgram();
            shaderList[GLShader::DEFAULT_SHADER]->activateShader(program);
            break;
    }
}

void GLWidget::updateShader() {
    Controller::getInstance()->getSetUp()->lightsToGPU(program);
    Controller::getInstance()->getScene()->toGPU(program);
    updateGL();
}

void GLWidget::setPerspective(float nearPlane, float farPlane)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->setFrustum(camera->vfovRadians DEGREES, nearPlane, farPlane, TipProj::PERSPECTIVA);

    updateGL();
}

void GLWidget::setLookAt(const QVector3D &eye, const QVector3D &center, const QVector3D& up)
{
    vec4 lookfrom(eye[0], eye[1], eye[2], 1.0);
    vec4 lookat(center[0], center[1], center[2], 1.0);
    vec4 vup(up[0], up[1], up[2], 1.0);

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->setObservador(lookfrom, lookat, vup);

    updateGL();
}

/* Hem de passar les dades de llum a la GPU sempre que canvïin les propietats de la llum o s'actualitzi
   l'escena. No cal passar les dades de llum a la GPU al principi ni cada vegada que es visualitzi l'escena.*/
void GLWidget::setLighting(const QVector3D &lightPos, const QVector3D &Ia, const QVector3D &Id,
                           const QVector3D &Is, const QVector3D &coefs)
{
    vec4 lightPosition(lightPos[0],lightPos[1], lightPos[2], 1.0) ;
    vec3 intensityA( Ia[0], Ia[1], Ia[2]);
    vec3 intensityD( Id[0], Id[1], Id[2]);
    vec3 intensityS( Is[0], Is[1], Is[2]);

    auto lights = Controller::getInstance()->getSetUp()->getLights();
    lights[0]->setIa(intensityA);
    lights[0]->setId(intensityD);
    lights[0]->setIs(intensityS);
    lights[0]->setLightPosition(lightPosition);

    /* Cridem a updateGL() per assegurar-nos que l'escena es torna a dibuixar amb la nova configuració de
       la llum. */
    updateGL();
}

/**  Mètodes d'interacció amb el ratolí */
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton)
    {
        if(lastPos.y()!= event->y() && lastPos.x()!= event->x()) {
           setXRotation(dy);
           setYRotation(dx);
        } else if(lastPos.y()!= event->y()) {// rotar la camera
            setXRotation(dy);
        } else if (lastPos.x()!= event->x()) {
            setYRotation(dx);
        }

    } else if (event->buttons() & Qt::RightButton) {
       // Zoom: canviar la mida de la window
        if(lastPos.y()> event->y())
             Zoom(-1);
        else
             Zoom(1);
    }

    lastPos = event->pos();
}


void GLWidget::setXRotation(int angle)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (angle >0) {
        camera->angX += 5;
    } else if (angle<0)
        camera->angX -= 5;
    qNormalizeAngle(camera->angX);

    camera->rotaCamera();

    emit ObsCameraChanged(camera);

    updateGL();
}

void GLWidget::setYRotation(int angle)
{
    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    if (angle >0) {
        camera->angY += 5;
    } else if (angle<0)
        camera->angY-= 5;
    qNormalizeAngle(camera->angY);

    camera->rotaCamera();

    emit ObsCameraChanged(camera);

    updateGL();
}

void GLWidget::qNormalizeAngle(double &angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360)
        angle -= 360*16;
}

void GLWidget::Zoom (int positiu) {

    shared_ptr<GPUCamera> camera = Controller::getInstance()->getSetUp()->getCamera();

    camera->AmpliaWindow(positiu*(0.005));

    emit FrustumCameraChanged(camera);

    updateGL();
}

