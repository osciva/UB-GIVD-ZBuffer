#include "GPUConnections/GPUCub.hh"

GPUCub::GPUCub() : GPUCub(10.0) {
    qDebug() <<"Estic en el constructor del GPUCub\n";
}

/* Constructora amb paràmetres de GPUCub, sabem que un cub té 36 punts */
GPUCub::GPUCub(float p) : GPUMesh(36) {
    qDebug() <<"Estic en el constructor parametritzat del GPUCub\n";

    vertexs.push_back(vec4(-p/2, -p/2,  p/2, 1.0));   /* v0 */
    vertexs.push_back(vec4(-p/2,  p/2,  p/2, 1.0));   /* v1 */
    vertexs.push_back(vec4( p/2,  p/2,  p/2, 1.0));   /* v2 */
    vertexs.push_back(vec4( p/2, -p/2,  p/2, 1.0));   /* v3 */

    vertexs.push_back(vec4(-p/2, -p/2, -p/2, 1.0));   /* v4 */
    vertexs.push_back(vec4(-p/2,  p/2, -p/2, 1.0));   /* v5 */
    vertexs.push_back(vec4( p/2,  p/2, -p/2, 1.0));   /* v6 */
    vertexs.push_back(vec4( p/2, -p/2, -p/2, 1.0));   /* v7 */

    initVertex();
    make();
}

void GPUCub::make() {
    Index = 0;

    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            Index++;
        }
    }
}

void GPUCub::initTextura() {
    qDebug() << "Initializing textures...\n";

    QImage img[6];

    for(GLuint i=0; i<faces.size(); i++) {
       img[i] = QImage(faces[i]).convertToFormat(QImage::Format_RGBA8888);
    }

    /* Carregar la textura */
    glActiveTexture(GL_TEXTURE0);

    texture = make_shared<QOpenGLTexture>(QOpenGLTexture::TargetCubeMap);

    /* Si la textura no s'ha iniciat, la creem */
    if(!texture->isCreated()) texture->create();

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->textureId());

    texture->setFormat(QOpenGLTexture::RGBAFormat);
    texture->setSize(img[0].width(), img[0].height(), img[0].depth());
    texture->generateMipMaps();
    texture->allocateStorage();

    /* Asignem les imatges a cada eix */
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGBA,
                     QOpenGLTexture::UInt8, (const void*)img[0].constBits(), 0);
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGBA,
                     QOpenGLTexture::UInt8, (const void*)img[1].constBits(), 0);
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGBA,
                     QOpenGLTexture::UInt8, (const void*)img[2].constBits(), 0);
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGBA,
                     QOpenGLTexture::UInt8, (const void*)img[3].constBits(), 0);
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGBA,
                     QOpenGLTexture::UInt8, (const void*)img[4].constBits(), 0);
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGBA,
                     QOpenGLTexture::UInt8, (const void*)img[5].constBits(), 0);

    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void GPUCub::toGPU(shared_ptr<QGLShaderProgram> pr) {
    qDebug() << "Passo el GPUCub a la GPU.....\n";

    /* S'activa la textura i es passa a la GPU */
    texture->bind(texture->textureId());
    pr->setUniformValue("texEnvironment", texture->textureId());

    /* Creació d'un vertex array object */
    glGenVertexArrays( 1, &vao );

    /* Creacio i inicialitzacio d'un vertex buffer object (VBO) */
    glGenBuffers( 1, &buffer );

    /* Activació a GL del Vertex Buffer Object */
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    /* Transferència dels punts, colors i coordenades de textura al vertex buffer object */
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*Index,
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*Index, points );

    /* Set up vertex arrays */
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);

    glEnable(GL_TEXTURE_CUBE_MAP);
}

void GPUCub::draw() {
    glBindVertexArray( vao );
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, Index);
    glDepthFunc(GL_LESS);

    glDisableVertexAttribArray(0);

    glDisable(GL_TEXTURE_CUBE_MAP);
}

void GPUCub::initVertex() {
    /* Bottom */
    Face *bottom1 = new Face(4, 0, 7, -1);
    Face *bottom2 = new Face(7, 0, 3, -1);

    /* Front */
    Face *front1 = new Face(0, 1, 3, -1);
    Face *front2 = new Face(3, 1, 2, -1);

    /* Top */
    Face *top1 = new Face(1, 5, 2, -1);
    Face *top2 = new Face(2, 5, 6, -1);

    /* Right */
    Face *right1 = new Face(3, 2, 7, -1);
    Face *right2 = new Face(7, 2, 6, -1);

    /* Left */
    Face *left1 = new Face(4, 5, 0, -1);
    Face *left2 = new Face(0, 5, 1, -1);

    /* Back */
    Face *back1 = new Face(7, 6, 4, -1);
    Face *back2 = new Face(4, 6, 5, -1);

    cares.push_back(*bottom1);
    cares.push_back(*bottom2);
    cares.push_back(*front1);
    cares.push_back(*front2);
    cares.push_back(*top1);
    cares.push_back(*top2);
    cares.push_back(*right1);
    cares.push_back(*right2);
    cares.push_back(*left1);
    cares.push_back(*left2);
    cares.push_back(*back1);
    cares.push_back(*back2);

    /* Les imatges que farà servir cada cara */
    faces.push_back(QImage("://resources/textures/skybox/front.jpg"));
    faces.push_back(QImage("://resources/textures/skybox/back.jpg"));
    faces.push_back(QImage("://resources/textures/skybox/bottom.jpg"));
    faces.push_back(QImage("://resources/textures/skybox/top.jpg"));
    faces.push_back(QImage("://resources/textures/skybox/right.jpg"));
    faces.push_back(QImage("://resources/textures/skybox/left.jpg"));
}
