#include "engine.h"
#include <iostream>
#include <source_location>
#include <variant>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "perspective_camera.h"

using namespace Eng;


struct Base::Reserved {
   // Flags:
   bool initFlag;
   
   Reserved() : initFlag{ false } 
   {}
};

Base::Base() : reserved_(std::make_unique<Reserved>()) {
#ifdef _DEBUG   
   std::cout << "[+] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}

Base::~Base() {
#ifdef _DEBUG
   std::cout << "[-] " << std::source_location::current().function_name() << " invoked" << std::endl;
#endif
}

Base& Base::getInstance() {
   static Base instance;
   return instance;
}


// static void reshapeCallback(int width, int height)
// {
//    std::cout << "[reshape func invoked]" << std::endl;
//
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 1.0f, 100.0f);
//    glLoadMatrixf(glm::value_ptr(projection));
//    glMatrixMode(GL_MODELVIEW);
// }


bool Base::init(int argc, char **argv, const std::string& title) {
    if (reserved_->initFlag)
        return false;

    // inizializzazione di freeglut e creazione finestra

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(getWidth(), getHeight());

    const int id = glutCreateWindow(title.c_str());
    setWindowId(id);

    // serve a glutMainLoopEvent() per ritornare quando la finestra viene chiusa
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);




    // impostazione base di OpenGL

   // attiva depth testing
    glEnable(GL_DEPTH_TEST);
   // abilita il lighting della fixed pipeline
    glEnable(GL_LIGHTING);
   // normalizza automaticamente le normali
    glEnable(GL_NORMALIZE);





    // impostazioni del modello di illuminazione globale

    glm::vec4 gAmbient(0.2f, 0.2f, 0.2f, 1.0f);
    // imposto modello più realistico per lo specular (viewer locale, di default V = (0, 0, 1))
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

    // imposto luce ambient globale, indipendente dalle singole luci
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(gAmbient));


    // attivazione e configurazione una luce di default (GL_LIGHT0)

    glEnable(GL_LIGHT0);

    // Componenti della luce
    constexpr float diffuse[4]  = {1.0f, 1.0f, 1.0f, 1.0f};
    constexpr float ambient[4]  = {0.1f, 0.1f, 0.1f, 1.0f};
    constexpr float specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    // Posizione della luce:
    // w = 0 → luce direzionale
    // w = 1 → luce puntiforme
    float pos[4] = {0.0f, 0.0f, 1.0f, 0.0f}; // direzionale che punta verso -Z
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    // registrazione callback di rendering
   glutDisplayFunc(displayCallback);
   // glutReshapeFunc(reshapeCallback);


    reserved_->initFlag = true;
    return true;
}


bool Base::update() const noexcept {
   // Processa eventi GLUT
   glutMainLoopEvent();

   // Se la finestra è stata chiusa:
   if (!glutGetWindow())
      return false;

   return true;
}

void Base::swap() noexcept {
   glutSwapBuffers();
}

void Base::begin3D(const std::shared_ptr<Camera>& camera) noexcept {

   glMatrixMode(GL_PROJECTION);
   glLoadMatrixf(glm::value_ptr(camera->getProjectionMatrix()));

   glMatrixMode(GL_MODELVIEW);
}


void Base::end3D() noexcept {}

void Base::clear() noexcept {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



bool Base::free() const {
   // Not initialized?
   if (!reserved_->initFlag)
   {
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   // Here you can properly dispose of any allocated resource (including third-party dependencies)...

   // Done:
   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved_->initFlag = false;
   return true;
}



void Base::displayCallback() {}



void Base::changeWindowSize(const int width, const int height) {

   glutReshapeWindow(width, height);
   setWidth(width);
   setHeight(height);
}


void Base::changeWindowTitle(const std::string &title) {
   glutSetWindowTitle(title.c_str());
}

void Base::changeBackgroundColor(const float r, const float g, const float b) {
   glClearColor(r, g, b, 1.0f);
}

ENG_API std::shared_ptr<Mesh> Base::createMesh(const std::vector<glm::vec3>& vertexes, const std::shared_ptr<Material>& material) {
   return std::make_shared<Mesh>(vertexes, material);
}


std::shared_ptr<Camera> Base::createPerspectiveCamera(float fov, float aspectRatio,
                                   float nearPlane, float farPlane) noexcept {

   return std::make_shared<Perspective_Camera>(fov, aspectRatio, nearPlane, farPlane);
}


std::shared_ptr<Node> Base::load(const std::string &path) const noexcept {

   std::vector<glm::vec3> cube = {
      // ===== FRONT (z = 0.5) =====
      {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, // Triangolo 1
      {-0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, // Triangolo 2

      // ===== BACK (z = -0.5) =====
      { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
      { 0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},

      // ===== LEFT (x = -0.5) =====
      {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
      {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},

      // ===== RIGHT (x = 0.5) =====
      { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
      { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},

      // ===== TOP (y = 0.5) =====
      {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f},
      {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},

      // ===== BOTTOM (y = -0.5) =====
      {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f},
      {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
  };


   std::vector<glm::vec3> cubeNormals;
   // 6 facce, 6 vertici per faccia
   // FRONT (Z+)
   for(int i=0; i<6; i++) cubeNormals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
   // BACK (Z-)
   for(int i=0; i<6; i++) cubeNormals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
   // LEFT (X-)
   for(int i=0; i<6; i++) cubeNormals.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
   // RIGHT (X+)
   for(int i=0; i<6; i++) cubeNormals.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
   // TOP (Y+)
   for(int i=0; i<6; i++) cubeNormals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
   // BOTTOM (Y-)
   for(int i=0; i<6; i++) cubeNormals.push_back(glm::vec3(0.0f, -1.0f, 0.0f));



   std::shared_ptr<Node> root = std::make_shared<Node>();

   // cubo
   std::shared_ptr<Material> material = std::make_shared<Material>(
      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), // emission
      glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), // ambient
      glm::vec4(0.5f, 0.2f, 0.8f, 1.0f), // diffuse
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // specular
      32.0f);


   std::shared_ptr<Mesh> first_cube = std::make_shared<Mesh>(cube, material);

   first_cube->setNormals(cubeNormals);

   // aggiungere cubo a root
   root->addChildren(first_cube);



   // creare e aggiungere camera al cubo

   // creare luce per cubo


   return root;
}



void Base::render(std::shared_ptr<Camera> camera, std::shared_ptr<List> renderList) noexcept {

   for (const auto& instance : renderList->getRenderList()) {

      glm::mat4 viewMatrix = camera->getViewMatrix();
      glm::mat4 modelViewMatrix = viewMatrix * instance.getNodeWorldMatrix();
      instance.getNodePtr()->render(modelViewMatrix);

   }


}