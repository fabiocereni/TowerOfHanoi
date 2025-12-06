#include "engine.h"
#include <iostream>
#include <source_location>
#include <variant>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

/*
#include <FreeImage.h>
*/

#include "ovoreader.h"
#include "perspective_camera.h"

using namespace Eng;


struct Base::Reserved {
   // Flags:
   bool initFlag;

   Reserved() : initFlag{ false }
   {}
};

// inizializzazione variabili statiche
float Base::frames_ = 0.0f;
Base::KeyboardCallback Base::customKeyboardCallbackVar_ = nullptr;
Base::SpecialKeyAction Base::up_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::down_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::left_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::right_arrow_key_ = nullptr;



// serve per il calcolo del framerate
float fps = 0.0f;



// servono per salvare le matrici
glm::mat4 perspective;
glm::mat4 ortho;


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

bool Base::init(int argc, char **argv, const std::string& title) {
   if (reserved_->initFlag) return false;

   /*
   FreeImage_Initialise();
   */

   // 1. Inizializzazione GLUT e Finestra (CORRETTO)
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(getWidth(), getHeight());
   const int id = glutCreateWindow(title.c_str());
   setWindowId(id);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   // 2. Impostazioni OpenGL Base (CORRETTO)
   glEnable(GL_DEPTH_TEST); // Fondamentale per il 3D
   glEnable(GL_NORMALIZE);  // Utile se scali le mesh, ma costa performance

   // ILLUMINAZIONE: Abilita il sistema, ma non singole luci
   glEnable(GL_LIGHTING);
   // Modello speculare realistico (utile tenerlo)
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

   // 3. CULLING: Disabilitalo per sicurezza all'inizio (o lascialo commentato)
   glDisable(GL_CULL_FACE);
   // Se decidi di usarlo, assicurati che i modelli siano perfetti:
   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);

   // 4. LUCI DI DEFAULT: RIMOSSE ❌
   // Lasciamo che sia la scena (tramite OvoReader o codice main) a creare le luci.
   // Se vuoi una luce ambientale di base minima per non vedere tutto nero assoluto:
   glm::vec4 globalAmbient(0.1f, 0.1f, 0.1f, 1.0f);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(globalAmbient));

   // 5. Callbacks (CORRETTO)
   glutDisplayFunc(displayCallback);
   glutReshapeFunc(reshapeCallback);
   glutTimerFunc(1000, timerCallback, 0);
   glutKeyboardFunc(useCustomKeyboardCallback);
   glutSpecialFunc(specialCallback);

   reserved_->initFlag = true;
   return true;
}


void Base::useCustomKeyboardCallback(const unsigned char key, const int mouseX, const int mouseY) noexcept {
   if (customKeyboardCallbackVar_) {
      customKeyboardCallbackVar_(key, mouseX, mouseY);
   }
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

void Base::end3D() noexcept {
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void Base::clear() noexcept {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Base::free() const {
   // Not initialized?
   if (!reserved_->initFlag)
   {
      
      //FreeImage_DeInitialise();
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   // Here you can properly dispose of any allocated resource (including third-party dependencies)...

   // Done:
   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved_->initFlag = false;
   return true;
}

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

ENG_API std::shared_ptr<Mesh> Base::createMesh(const std::vector<glm::vec3>& vertexes,
                                               const std::string& materialName,
                                               const std::shared_ptr<Material>& material) {
   return std::make_shared<Mesh>(vertexes, materialName, material);
}

std::shared_ptr<Camera> Base::createPerspectiveCamera(float fov, float aspectRatio,
                                                      float nearPlane, float farPlane) noexcept {

   return std::make_shared<Perspective_Camera>(fov, aspectRatio, nearPlane, farPlane);
}


std::shared_ptr<Node> Base::load(const std::string& path) const noexcept {
   return OvoReader::load(path);
}

void Base::render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList) noexcept {

   for (const auto& instance : renderList->getRenderList()) {

      glm::mat4 viewMatrix = camera->getViewMatrix();
      glm::mat4 modelViewMatrix = viewMatrix * instance.getNodeWorldMatrix();
      instance.getNodePtr()->render(modelViewMatrix);
   }


}

void Base::displayCallback() {
   frames_++;
   glutPostRedisplay();
}

void Base::showFps() {

   // disabilita luce, non altera il colore del testo
   glDisable(GL_LIGHTING);

   // salva proiezione
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();


   // salva modelview
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));


   char text[64]; sprintf(text, "FPS: %.1f", fps);
   glColor3f(1, 1, 1);
   // string position
   glRasterPos2f(-0.95f, 0.85f);

   // display the string
   glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<unsigned char*>(text));
   // ripristina
   glPopMatrix();

   // MODELVIEW
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glEnable(GL_LIGHTING);

}

void Base::timerCallback(int) {
   fps = getFrames();
   setFrames(0);

   // ogni secondo
   glutTimerFunc(1000, timerCallback, 0);
}

void Base::reshapeCallback(const int width, const int height) {

   // aggiorno view port
   glViewport(0, 0, width, height);

   if (const auto cam = getInstance().getActiveCamera()) {
      // aggiorno in base alla camera attiva
      cam->onResize(width, height);
   }

   ortho = glm::ortho(0.0f, static_cast<float>(width), 0.0f,
                      static_cast<float>(height), -1.0f, 1.0f);
}

void Base::specialCallback(int key, int mouseX, int mouseY) {

   switch (key) {
   case GLUT_KEY_UP:
      if (up_arrow_key_)
         up_arrow_key_();
      break;

   case GLUT_KEY_DOWN:
      if (down_arrow_key_)
         down_arrow_key_();
      break;

   case GLUT_KEY_LEFT:
      if (left_arrow_key_)
         left_arrow_key_();
      break;

   case GLUT_KEY_RIGHT:
      if (right_arrow_key_)
         right_arrow_key_();
      break;

   default: std::cout << "UNKNOWN" << std::endl ;
   }
}

ENG_API std::shared_ptr<Texture> Base::loadTextureFromFile(const std::string& path) const noexcept {

   // 1. Determina il formato del file
   /*
   FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
   if (format == FIF_UNKNOWN) {
      format = FreeImage_GetFIFFromFilename(path.c_str());
   }
   if (format == FIF_UNKNOWN) {
      std::cout << "ERROR: Unknown image format for " << path << std::endl;
      return nullptr;
   }

   // 2. Carica l'immagine
   FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
   if (!bitmap) {
      std::cout << "ERROR: Unable to load image " << path << std::endl;
      return nullptr;
   }

   FreeImage_FlipVertical(bitmap);

   // 3. Converti in 32 bit (RGBA) per uniformità
   // Questo passaggio è CRUCIALE per evitare problemi tra RGB/BGR/RGBA
   FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
   FreeImage_Unload(bitmap); // Scarica l'originale
   bitmap = bitmap32;

   // 4. Inverti verticalmente (OpenGL ha l'origine in basso a sinistra)
   // FreeImage_FlipVertical(bitmap); // Nota: gluBuild2DMipmaps potrebbe non richiedere il flip, prova con e senza.

   // 5. Estrai info
   int width = FreeImage_GetWidth(bitmap);
   int height = FreeImage_GetHeight(bitmap);
   unsigned char* data = FreeImage_GetBits(bitmap);

   // ATTENZIONE: FreeImage usa BGR per default, OpenGL vuole RGB o RGBA.
   // Nel costruttore della Texture (texture.cpp) usi GL_RGB.
   // Per semplicità qui invieremo i dati così come sono, ma dovrai aggiornare texture.cpp
   // per gestire il canale Alpha se vuoi la trasparenza.

   // Crea la texture usando il tuo costruttore
   auto texture = std::make_shared<Texture>(data, width, height);

   const size_t lastSlashPos = path.find_last_of('/');


   if (lastSlashPos == std::string::npos) {
      texture->setName(path);
   } else {
      texture->setName(path.substr(lastSlashPos + 1));
   }

   // 6. Pulisci memoria RAM
   FreeImage_Unload(bitmap);
   
   return texture;*/
   return nullptr;
}



void Base::bindTexturesToMaterials(const std::shared_ptr<Node>& root, const std::vector<std::shared_ptr<Texture>>& textures) const noexcept {

   if (!root || textures.empty()) {
      return;
   }

   // Nota: .get() non è necessario con l'operatore ->
   for (const auto& childNode : root->getChildrens()) {

      // 1. Tenta il cast
      auto mesh = std::dynamic_pointer_cast<Mesh>(childNode);

      // 2. VERIFICA SE IL CAST È RIUSCITO
      if (mesh) {

         // 3. Recupera il material e VERIFICA SE ESISTE
         auto material = mesh->getMaterial();

         if (material && !material->getTextureName().empty()) {

            for (const auto& texture : textures) {
               // Nota: texture è già shared_ptr<Texture>, il cast qui è inutile/ridondante

               if (texture->getName() == material->getTextureName()) {
                  material->setTexture(texture);
                  // Opzionale: break; // Se supporti solo 1 texture per materiale
               }
            }
         }
      }

      // Ricorsione
      this->bindTexturesToMaterials(childNode, textures);
   }
}






void Base::setWireframe(const bool enable) {
   if (enable)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Base::setLighting(const bool enable) {
   if (enable)
      glEnable(GL_LIGHTING);
   else
      glDisable(GL_LIGHTING);
}

void Base::setCulling(const bool enable) {
   if (enable)
      glEnable(GL_CULL_FACE);
   else
      glDisable(GL_CULL_FACE);
}