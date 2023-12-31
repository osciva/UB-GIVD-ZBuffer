# ZBufferToy

### Team B07:
*   Oscar De Caralt Roy
*   Alejandro Guzman Requena
*   Adrià Alfonso Medero
*   Noah Márquez Vara

### Abstract

Aquest projecte implementa l'algoritme ZBuffer en un entorn de renderitzat 3D. Utilitzant shaders i tècniques que hem aprés a classes de teoria, i també s'han creat escenes personalitzades com la visió nocturna o una tempesta inspirada en Fortnite. L'algoritme ZBuffer s'utilitza per realitzar el renderitzat en temps real i gestionar la visibilitat dels objectes a l'escena. Els shaders programats ja els havíem vist a la primera pràctica amb RayTracing però ara ens hem encarregat de realitzar-los amb GLSL. Aquest projecte combina conceptes de gràfics per computador, programació de shaders i disseny d'escenes per oferir una experiència visualment atractiva i immersiva.

NOTES IMPORTANTS:
- Night Vision està parcialment implementada, es veu el cercle amb els objectes de dins amb només el canal verd, però el fons es continua veient en negre.
- S'ha intentat implementar les textures en els objectes mitjançant fitxers, però només hem aconseguit que els objectes canviïn la seva Kd segons la textura. Per tal de provar-ho s'ha d'accedir al menú de Texture, seleccionar un fitxer de Textura, i llavors seleccionar Active Texture. Posteriorment escollim Phong o Blinn-Phong shading per veure-ho en funcionament. Creiem que segurament sigui un tema de com ho estem enviant a la GPU.
- FittedPlane parcialment implementat. Compila sense cap error però no hem aconseguit visualitzar-ho a l'escena.
- Hem implementat l'environmental mapping, però hem comprovat que només ens funcionava amb MacOS o bé establint al màxim les característiques de la màquina virtual.

### Features

A continuació s'indica quines parts s'han fet i qui les ha implementat:
- Fase 1
    - Lights
        - [✅] Ambient Global (Adrià)
        - [✅] Puntual (Noah & Alejandro) 
        - [✅] Direccional (Noah)
        - [✅] Spotlight (Noah)
    - Materials: 
       - [✅] Pas a la GPU (Oscar & Noah)
       - [✅] Lectura de fitxers .json (Oscar & Noah)
    - Shading
        - [✅] Color (Noah & Adrià & Alejandro)
        - [✅] Normal (Alejandro)
        - [✅] Depth (Adrià)
        - [✅] Phong-Gouraud (Alejandro)
        - [✅] Phong-Phong (Adrià & Noah)
        - [✅] BlinnPhong-Gouraud (Noah)
        - [✅] BlinnPhong-Phong (Adrià & Noah)
        - [✅] Cel-shading (Noah)
    - Textures
        - [✅] Textura com material en un objecte (Oscar)
        - [ ] Textura al pla base 
    - Adaptació a la lectura de fitxers de dades
        - [✅] Escenes virtuals (Noah)
        - [✅] SetUp (Noah)
        - [✅] Escenes de dades (Noah)

- Fase 2 
    - [✅] Visió nocturna (Noah)
    - [✅] La Tempesta de Fornite (Adrià & Alejandro)
    - [✅] Èmfasi de siluetes 
    - [ ] Mapping indirecte de textures
    - [ ] Animacions amb dades temporals
    - [ ] Normal mapping 
    - [✅] Enviromental mapping

### Preguntes de la pràctica

- Preguntes -> Fase 1:
    *   On es declara la llum? Quan es passa a la GPU?
        *   La llum es declara al mètode initializeGL() de la classe GLWidget, nosaltres creem una llum de tipus POINTLIGHT. Es passa a la GPU amb el seu mètode toGPU(), cridant-lo a través del mètode toGPU de la classe GPUSetUp, que a la vegada fa un set de la llum ambient i crida al mètode toGPU de cada llum guardada.
    *   Què contindrà el "struct" de la GPU relacionat amb la llum? Com l’estructurareu?
        *   L'estructura per passar l'informació relativa a les llums cap a la GPU, serà la següent:
            ```glsl
            struct lights_id{
                GLuint type;
                GLuint ia;
                GLuint id;
                GLuint is;
                GLuint coeficients;
                GLuint position;       
            }
    *   Si vols utilitzar diferents shaders en temps d'execució raona on s'inicialitzaran els shaders i com controlar quin shader s'usa? Cal tornar a passar l'escena a la GPU quan es canvia de shader?
        * Per utilitzar diferents shaders en temps d'execució, primer es necessita inicialitzar els shaders que es vol utilitzar. Això es fa mitjançant un mètode anomenat initShadersGPU(), que es crida al mètode initializeGL(). Aquest mètode inicialitza diversos shaders amb codi vertex i fragment específic.

          Per controlar quin shader s'utilitza, es poden utilitzar mètodes separats que s'encarreguen d'activar un shader específic, com activaColorShader(), activaDepthShader(), activaNormalShader(), etc. Aquests mètodes primer estableixen la variable currentShader amb el tipus de shader desitjat i després criden a useShader(), que obté el programa de shader corresponent de la llista de shaders i activa aquest programa.

          Un cop s'ha canviat de shader, no cal tornar a passar tota l'escena a la GPU. No obstant això, és possible que s'hagi de passar dades específiques a la GPU que el nou shader utilitza. En el nostre cas es veu que després de canviar el shader es crida a updateShader(), que passa les dades de llum a la GPU, i també crida al mètode toGPU de l'escena.
          
          ```glsl
          void GLWidget::updateShader() {
          Controller::getInstance()->getSetUp()->lightsToGPU(program);
          Controller::getInstance()->getScene()->toGPU(program);
          updateGL();
          }

    *  Prova a posar a la teva escena dos objectes amb materials diferents. Es pinta cadascun amb el seu material?
       * Sí, com es pot comprovar en la següent imatge, l'F16 es pinta de blanc i el cub es pinta d'un color vermellós. Hem utilitzat el color shader.
    
       <p align="center">
        <img width="300px" height="300px" src="https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/e977b9a3-9002-4e90-b782-3e86b77a1834">
       </p>

    *  Fixa't que quan es llegeix un objecte, cada vèrtex ja té la seva normal. Com serà aquest valor de la normal? Uniform o no uniform? En la classe Camera utilitza el mètode toGPU per a passar l'observador als shaders per a que es passi la posició de l'observador cada vegada que s'actualitza la posició de la càmera amb el ratolí. Com serà aquesta variable al shader? Uniform? O IN?
        *   El valor de la normal no ha de ser uniform ja que es diferent a cada vèrtex, en canvi l'observador es comú, per això l'observador als shaders sí que és uniform.
    
    *  Cal tenir un parell de vèrtex-fragment shader? O dos? (Utilització del Gouraud-Phong i Gouraud-Blinn Phong)
        * No cal tenir dos parell de vèrtex-fragment, amb un encarregat del Gouraud (tant Gouraud-Phong com Gouraud-Blinn Phong) és suficient. Només hem de configurar una variable booleana uniform per saber si utilitzem Blinn Phong o, en cas de no estar activada, utilitzar Phong. El càlcul ho realitzem en el vertex shading, ja que el Gouraud ho calcula en aquest shader.
        
          ```glsl
            /* Comprovem si s'ha activat BlinnPhong */
            if (useBlinnPhong) {
                Is = lights[i].is * material.Ks * pow(max(dot(N,H), 0.0), material.shininess);
            } else {
                R = reflect(-L, N);
                Is = lights[i].is * material.Ks * pow(max(dot(V,R), 0.0), material.shininess);
            }
      
    *  Raona on s'inicialitzaran els shaders i com controlar quin shader s'usa. Cal tornar a passar l'escena a la GPU quan es canvia de shader? I també la càmera?
        *   Com hem mencionat anteriorment, un cop s'ha canviat de shader, no cal tornar a passar la càmara a la GPU. No obstant això, és possible que s'hagi de passar dades específiques a la GPU que el nou shader utilitza. En el nostre cas es veu que després de canviar el shader es crida a updateShader(), que passa les dades de llum a la GPU i actualitza l'escena a la GPU.
        
    * Implementa el Phong-shading per a les dues versions de la fórmula de càlcul de la il·luminació. Quina diferència hi ha amb el Gouraud-shading? On l'has de codificar? Necessites uns nous vertex-shader i fragment-shader?
        * En el Gouraud-Shading l'intensitat de la llum es calcula als vèrtexs dels triangles que formen la superfície. Llavors, aquestes intensitats s'interpolen linealment a través de les cares del triangle per obtenir el color de cada píxel. Aquesta tècnica és ràpida, però pot produir resultats no realistes quan es tracta de llum especular intensa o quan el model geomètric utilitza polígons grans. En canvi en el Phong Shading l'interpolació es realitza en els vectors normals als vèrtexs, no en les intensitats de la llum. Llavors, es calcula la intensitat de la llum a cada píxel utilitzant la normal interpolada. Aquesta tècnica és més costosa en termes de recursos computacionals, però produeix resultats més realistes, en especial quan es tracta de llum especular. Nosaltres per calcular el Gouraud-Shading, codifiquem el càlcul del color en el vertex shader, en canvi el mateix càlcul ho codifiquem en el fragment quan es tracta de les dues versions del Phong-shading. Per tant si que necessitem d'un nou parell de shaders per tal de codificar el càlcul del color o en el vertex o en el fragment.

    *  On s'implementarà el càlcul del color per a tenir més trencament entre las games de colors? Necessites uns nous vertex-shader i fragment-shader? 
       Raona on es calcula la il·luminació i modifica convenientment els fitxers de la pràctica. (Toon-shading)
       * El càlcul del color s'implementarà en el fragment-shader, degut a que la intensitat es calcula amb la normal en aquell punt, llavors si utilitzem la normal interpolada en el fragment la precisió augmentarà. Calculem el color tal i com mostra el següent codi:
       
         ```glsl
            float intensity, silueta;
            vec4 color;

            /* Calculem la direcció de la llum segons el tipus de llum */
            vec4 lightDirection = normalize(lights[0].direction);

            if (lights[0].type == 0) /* Point light */
            {
            /* Suposem llavors que la direcció de la llum és 1.0. */
            lightDirection = vec4(1.0);
            }

            /* Calculem la intensitat del toon entre 4 intervalss */
            intensity = dot(lightDirection, fNormal);

            if (intensity > 0.95)
                color = vec4(1.0, 0.5, 0.5, 1.0);
            else if (intensity > 0.5)
                color = vec4(0.6, 0.3, 0.3, 1.0);
            else if (intensity > 0.25)
                color = vec4(0.4, 0.2, 0.2, 1.0);
            else
                color = vec4(0.2, 0.1 ,0.1, 1.0)

- Preguntes -> Fase 2:
    *   Detalla on es faria el càlcul? Amb quines coordenades? Amb coordenades de món? De càmera? O de viewport?
        *   El càlcul es fa dins de la funció principal main() (del fragment shader) dins de l'if de "useNightVision". Es fa servir la variable gl_FragCoord, que conté les coordenades del fragment actual en el sistema de coordenades del viewport.
        
            ```glsl
            if (useNightVision) {
                /* Calculem la distància des del gl_FragCoordCalculate distance al centre del viewport */
                vec2 viewportCenter = viewportSize / 2.0;
                float pixelDist = distance(gl_FragCoord.xy, viewportCenter);

                /* Comprovem si està dins del radi */
                if (pixelDist <= viewportSize.y / 4.0) {
                    /* Calculem Phong com abans però només considerant el canal verd */
                    /* Posem a 0 els canals vermell i blau */
                    Itotal.r = 0.0;
                    Itotal.b = 0.0;

                } else {
                    /* La resta de color negre */
                    Itotal = vec3(0.0, 0.0, 0.0);
                }
            } 
    *   Com aconseguiries que els píxels de fons inclosos en el cercle de visió nocturna es pintessin també de color verd? 
        *   Per aconseguir que els píxels de fons inclosos dins del cercle de visió nocturna es pintin de color verd, es pot utilitzar un pla acotat (fitted plane) de color verd com a fons. Una altre implementació podría ser col·locar un cub l'origen del qual es troba situat en (0,0,0) i augmentar el volum fins a contenir l'objecte al seu interior, de forma que en rotar l'objecte el fons sigui sent verd independentment de la posició de la càmera.
    *   Considera quants parells de vèrtex-fragment shaders has d’usar, a on cal considerar el test amb l’esfera, etc.
        *   Només farem ús d'un parell de vèrtex-fragment shaders (els que hem fet servir per Phong i Blinn-Phong, però ara afegim una variable booleana per saber si s'ha activat la tempesta de Fortnite). El test amb l'esfera es considera de la següent manera a fphongshader.glsl:
        
            ```glsl
             /* Check if the fragment is inside the sphere */
            if (distanceToSphere <= 0.7) {
                /* Apply blue tint to fragments inside the sphere */
                Itotal.rgb *= vec3(0.6, 0.6, 1.5);
                colorOut = vec4(Itotal, 1.0);

### Parts opcionals realitzades

#### Lectura de material .mtl juntament amb un fitxer .obj
Durant la càrrega de l'objecte Mesh, el codi verifica si cada línia del fitxer conté la paraula clau "mtllib", que indica un fitxer de material (.mtl). Si es troba "mtllib", agafa el nom del fitxer .mtl associat, crea la ruta completa del fitxer i crida a la funció "loadMaterial" amb aquesta ruta. Aquesta funció carrega les propietats del material a partir del fitxer .mtl i les aplica a l'objecte Mesh. Així es pot renderitzar l'objecte amb el material correcte tal com es defineix en el fitxer .mtl. Es pot observar a continuació la part del codi del mètode load que llegeix el material (en cas de que el fitxer -obj el contingui):
  
             else if(lineParts.at(0).compare("mtllib", Qt::CaseInsensitive) == 0)
                    {
                        materialFileName = lineParts.at(1);
                        QString fullPath = basePath + QDir::separator() + materialFileName;
                        loadMaterial(fullPath);
                    }
                    
#### Èmfasi de siluetes
Una de les dues parts opcionals que hem programat ha estat l'èmfasi de siluetes, que permet en el ToonShading ressaltar les siluetes dels objectes renderitzats, donant-los un aspecte més estilitzat i definint millor els contorns. La part del codi que implementa aquest opcional és la següent (toon shading fragment):
                
                silueta = dot(fNormal, normalize(obs - fPosition));

                if (silueta < 0.1)
                    color *= silueta;
                  


<p align="center">
 <img width="300px" height="300px" src="https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/593679e8-b645-47eb-bf33-2b2d2b497137">
</p>

#### Environmental mapping:
L'environmental Mapping és una tècnica utilitzada per a la decoració de l'entorn d'una escena gràfica mitjançant textures. L'objectiu és decorar l'entorn de la escena carregant un cub prou gran que envolti tota l'escena i aplicar una textura a aquest cub.
Per realitzar aquesta tasca hem creat una classe cub que hereda de la clase Mesh i té els següents mètodes:
*  Un constructor per defecte que crida al constructor GPUCub::GPUCub(float p) amb un valor de 10.0 com a paràmetre.
*  Un constructor amb paràmetres que rep un valor p per determinar la mida del cub. Defineix els vèrtexs del cub i crida a les funcions initVertex() i make() per inicialitzar les dades del cub.
*  Un mètode make() que genera els punts del cub a partir dels vèrtexs definits a initVertex(). Recorre les cares del cub i assigna els punts corresponents als vèrtexs de cada cara a l'array points.
*  Un mètode initTextura() que Inicialitza les textures del cub. Carrega les imatges de textura per a cada cara del cub i les assigna al cub utilitzant un objecte QOpenGLTexture. Estableix el format de la textura, genera mipmaps i assigna les dades de la imatge a cada cara del cub. També configura els paràmetres d'embolcall i filtratge de la textura.
*   Un mètode toGPU() que transfereix les dades del cub a la GPU. Aquest mètode assigna la textura del cub i crea un array de vèrtexs i un buffer per emmagatzemar les dades del cub. També especifica els formats dels atributs de vèrtex i habilita els atributs corresponents.
*   Un mètode draw() que dibuixa el cub a l'escena. Enllaça l'array i el buffer de vèrtexs, estableix el mode de dibuix de polígons i fa la crida a glDrawArrays per renderitzar el cub.
*   Un mètode initVertex() que inicialitza les cares i les imatges de textura del cub. Crea objectes Face per representar cada cara del cub i emmagatzema les imatges de textura corresponents al vector faces.

A més a la classe Widget.cpp hem afegit codi al mètode initializeGL, que s'executa durant la inicialització de l'OpenGL i que configura el renderitzat de l'escena per a l'Environmental Mapping. Hem afegit codi que verifica si s'ha activat el mode CUBEMAP a la configuració de l'escena. Si és així, es canvia a l'shader del CUBEMAP, s'inicialitza la textura del cub, s'envia la càmera i el cub a la GPU, i finalment es dibuixa el cub. A més, hem implementat el mètode activaEnvMapping s'executa quan es vol activar l'Environmental Mapping. En aquest cas, es crea una instància de GPUCub si el mode CUBEMAP està desactivat, es configura el shader del CUBEMAP i es fan les actualitzacions necessàries.

### ZBufferToy outputs
| Color Shader | Normal Shader | Depth Shader |
|--------------|---------------|--------------|
| ![Color](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/3eceb573-c6e8-4105-a37c-2dba715994c8) | ![Normal](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/26bf241e-6e3d-4d6a-9653-3a04cca247c2) | ![Depth](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/4cfda1c0-fdaa-4c4f-bc4e-f10a63495626) |
| Gouraud-Phong Shader | Phong Shader | Gouraud-Blinn-Phong Shader |
|![Gouraud-Phong mono](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/38d30fa4-581d-4192-b6cd-86f8cf8c4165) | ![phong esfera](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/c633785b-0f05-4d2a-820c-af55135d02af) | ![Gouraud-Blinn-Phong mono](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/4a3c1cd9-1d11-4b30-9675-4758d2270a90)|
| Blinn-Phong Shader | Cell Shader | Environmental Mapping |
| ![blinn phong esfera](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/29ba8e06-bb97-4176-88f8-a4a82348731f)| ![Toon](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/a85ce1e8-8d3e-416a-b6e1-365d922640bd) | ![Environmental Mapping](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/47271218/c411b288-c34c-4215-a88f-bca54ceefa67) |
| Fornite Storm | Night Vision |
![ForniteStorm](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/1eaaf8d6-00a8-4260-97ef-9a838733fa20) | ![Night Vision](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/47271218/07fdf88a-7b7a-4ed8-8796-b83e9c4c19fa)

