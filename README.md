# ZBufferToy

### Team B07:
*   Oscar De Caralt Roy
*   Alejandro Guzman Requena
*   Adrià Alfonso Medero
*   Noah Márquez Vara

### Abstract

Aquest projecte implementa l'algoritme Zbuffer en un entorn de renderitzat 3D. Utilitzant shaders i tècniques que hem aprés a classe de teoria, s'han creat escenes personalitzades com visió nocturna o una tempesta inspirada en Fortnite. L'algoritme Zbuffer s'utilitza per realitzar el renderitzat en temps real i gestionar la visibilitat dels objectes a l'escena. Els shaders programats ja els havíem vist a la primera pràctica amb RayTracing però ara ens hem encarregat de realitzar-los amb GLSL. Aquest projecte combina conceptes de gràfics per computador, programació de shaders i disseny d'escenes per oferir una experiència visualment atractiva i immersiva.

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
        - [✅] Color (Adrià & Alejandro)
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
    - [ ] Èmfasi de siluetes 
    - [ ] Mapping indirecte de textures
    - [ ] Animacions amb dades temporals
    - [ ] Normal mapping 
    - [ ] Entorn amb textures
    - [ ] Reflexions
    - [ ] Transparències via objectes: 
    - [ ] Transparències via environmental mapping

### Preguntes de la pràctica

- Preguntes -> Fase 1:
    *   On es declara la llum? Quan es passa a la GPU?
        *   La llum es declara a LightFactory, al mètode createLight(). Es passa a la GPU amb el seu mètode toGPU()
    *   Què contindrà el "struct" de la GPU? Com l’estructurareu?
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
        * Per utilitzar diferents shaders en temps d'execució, primer es necessita inicialitzar els shaders que es vol utilitzar. Això es f fer al mètode initializeGL(), on es crida al mètode initShadersGPU(). Aquest mètode inicialitza diversos shaders amb codi vertex i fragment específic.

          Per controlar quin shader s'utilitza, es poden utilitzar mètodes separats que s'encarreguen d'activar un shader específic, com activaColorShader(), activaDepthShader(), activaNormalShader(), etc. Aquests mètodes primer estableixen la variable currentShader amb el tipus de shader desitjat i després criden a useShader(), que obté el programa de shader corr esponent de la llista de shaders i activa aquest programa.

          Un cop s'ha canviat de shader, no cal tornar a passar tota l'escena a la GPU. No obstant això, és possible que s'hagi de passar dades específiques a la GPU que el nou shader utilitza. En el nostre cas es veu que després de canviar el shader es crida a updateShader(), que passa les dades de llum a la GPU i actualitza l'escena a la GPU.
          
          ```glsl
          void GLWidget::updateShader() {
          Controller::getInstance()->getSetUp()->lightsToGPU(program);
          Controller::getInstance()->getScene()->toGPU(program);
          updateGL();
          }
          

    *  A més, si la teva escena està canviant (per exemple, si els objectes es mouen o canvien de color), hauràs de passar aquestes dades actualitzades a la GPU cada vegada que es produeixin aquests canvis, però          això és independent del canvi de shader.

    *  Prova a posar a la teva escena dos objectes amb materials diferents. Es pinta cadascun amb el seu material?
       * Sí, com es pot comprovar en la següent imatge, l'F16 es pinta de blanc i el cub es pinta d'un color vermellós. Hem utilitzat el color shader.
    
       <p align="center">
        <img width="300px" height="300px" src="https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/e977b9a3-9002-4e90-b782-3e86b77a1834" 
       </p>
    *  Fixa't que quan es llegeix un objecte, cada vèrtex ja té la seva normal. Com serà aquest valor de la normal? Uniform o no uniform? En la classe Camera utilitza el mètode toGPU per a passar l'observador als        shaders per a que es passi laposició de l'observador cada vegada que s'actualitza la posició de la càmera amb el ratolí. Com serà aquesta variable al shader? Uniform? O IN?
    
    *  Cal tenir un parell de vèrtex-fragment shader? O dos?
    
    *  Raona on s'inicialitzaran els shaders i com controlar quin shader s'usa. Cal tornar a passar l'escena a la GPU quan es canvia de shader? I també la càmera?
Quina diferència hi ha amb el Gouraud-shading? On l'has de codificar? Necessites uns nous vertex-shader i fragment-shader?

    *  On s'implementarà el càlcul del color per a tenir més trencament entre las games de colors? Necessites uns nous vertex-shader i fragment-shader?
       Raona on es calcula la il·luminació i modifica convenientment els fitxers de la pràctica.

- Preguntes -> Fase 2:
    *   Detalla on es faria el càlcul? Amb quines coordenades? Amb coordenades de món? De càmera? O de viewport?
        *   El càlcul es fa dins de la funció principal main() dins de l'if de "useNightVision". Es fa servir la variable gl_FragCoord, que conté les coordenades del fragment actual en el sistema de coordenades del viewport.
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
        *   Per aconseguir que els píxels de fons inclosos dins del cercle de visió nocturna es pintin de color verd, es pot utilitzar un pla ajustat (fitted plane)             de color verd com a fons.
    *   Considera quants parells de vèrtex-fragment shaders has d’usar, a on cal considerar el test amb l’esfera, etc.
        *   Només farem ús d'un parell de vèrtex-fragment shaders. El test amb l'esfera es considera de la següent manera a fphongshader.glsl:
        ```glsl
         /* Check if the fragment is inside the sphere */
        if (distanceToSphere <= 0.7) {
            /* Apply blue tint to fragments inside the sphere */
            Itotal.rgb *= vec3(0.6, 0.6, 1.5);
            colorOut = vec4(Itotal, 1.0);

### ZBufferToy outputs

| Color Shader | Normal Shader | Depth Shader |
|--------------|---------------|--------------|
| ![Color](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/3eceb573-c6e8-4105-a37c-2dba715994c8) | ![Normal](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/26bf241e-6e3d-4d6a-9653-3a04cca247c2) | ![Depth](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/4cfda1c0-fdaa-4c4f-bc4e-f10a63495626) |
| Gouraud-Phong Shader | Phong Shader | Gouraud-Blinn-Phong Shader |
|![Gouraud-Phong mono](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/38d30fa4-581d-4192-b6cd-86f8cf8c4165) | ![phong esfera](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/c633785b-0f05-4d2a-820c-af55135d02af) | ![Gouraud-Blinn-Phong mono](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/4a3c1cd9-1d11-4b30-9675-4758d2270a90)|
| Blinn-Phong Shader | Cell Shader |
| ![blinn phong esfera](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/29ba8e06-bb97-4176-88f8-a4a82348731f)| ![Toon](https://github.com/GiVD2022/p2-zbuffertoy-b07/assets/81873328/a85ce1e8-8d3e-416a-b6e1-365d922640bd) |
