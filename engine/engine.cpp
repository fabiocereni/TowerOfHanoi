#include "engine.h"
#include <iostream>
#include <source_location>
#include <variant>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "filesystem"

#define FREEIMAGE_LIB

#include "ovoreader.h"
#include "perspective_camera.h"
#include <FreeImage.h>


constexpr char pathSeparator =
                              #ifdef _WIN32
                                 '\\';
                              #else
                                 '/';
                              #endif


using namespace Eng;


struct Base::Reserved {
   // Flags:
   bool initFlag;

   Reserved() : initFlag{ false } {}
};

// inizializzazione variabili statiche
float Base::frames_ = 0.0f;
Base::KeyboardCallback Base::customKeyboardCallbackVar_ = nullptr;
Base::SpecialKeyAction Base::up_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::down_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::left_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::right_arrow_key_ = nullptr;
Base::SpecialKeyAction Base::F1_key = nullptr;
Base::SpecialKeyAction Base::F2_key = nullptr;
Base::SpecialKeyAction Base::F3_key = nullptr;
Base::SpecialKeyAction Base::F4_key = nullptr;



// serve per il calcolo del framerate
float fps = 0.0f;


// servono per salvare le matrici
glm::mat4 perspective;
glm::mat4 ortho;

//shadowMatrix
std::shared_ptr<Material> shadowMaterial;


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


   FreeImage_Initialise();


   // 1. Inizializzazione GLUT
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(getWidth(), getHeight());
   const int id = glutCreateWindow(title.c_str());
   setWindowId(id);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   // 2. Impostazioni OpenGL Base (CORRETTO)
   glEnable(GL_DEPTH_TEST); // Fondamentale per il 3D
   glEnable(GL_NORMALIZE);  // Utile se scali le mesh, ma costa performance

   // ILLUMINAZIONE: Abilita il sistema
   glEnable(GL_LIGHTING);
   // Modello speculare realistico
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

   // 3. CULLING: Disabilitalo per sicurezza all'inizio
   glDisable(GL_CULL_FACE);


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

   //Shadow Matrix
   glm::vec4 black = glm::vec4(0.0f);
   shadowMaterial = std::make_shared<Material>(black, black, black,black, 0.0f, "");


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

      FreeImage_DeInitialise();
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


std::shared_ptr<Camera> Base::createOrthographicCamera(float left,
                                                       float right,
                                                       float top,
                                                       float bottom,
                                                       float nearPlane,
                                                       float farPlane) noexcept
{
   return std::make_shared<Orthographic_Camera>(left, right, top, bottom, nearPlane, farPlane);
}


std::shared_ptr<OmnidirectionalLight> Base::createOmnidirectionalLight() {
   return OmnidirectionalLight::createOmnidirectionalLight();
}


std::shared_ptr<Spotlight> Base::createSpotlight() {
   return Spotlight::createSpotLight();
}


std::shared_ptr<DirectionalLight> Base::createDirectionalLight() {
   return DirectionalLight::createDirectionalLight();
}



std::shared_ptr<Node> Base::load(const std::string& path) const noexcept {
   return OvoReader::load(std::filesystem::absolute(path).string());
}



void Base::render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList, const std::list<std::string>& excludedList) const {

    // 1. Rendering della scena (come già implementato)
    for (const auto& instance : renderList->getRenderList()) {
        glm::mat4 viewMatrix = camera->getViewMatrix();
        glm::mat4 modelViewMatrix = viewMatrix * instance.getNodeWorldMatrix();
        instance.getNodePtr()->render(modelViewMatrix);
    }

    // --- Fake Shadow (Planar Shadow semplificata) ---

    // Ottiene l'inversa della camera matrix per la trasformazione
    // (Nel tuo codice 'Engine' la ricavi da Engine::activeCamera->getInverseMatrix())
    // Qui usiamo l'inversa della View Matrix della camera, che è la stessa cosa
    const glm::mat4 inverseViewMatrix = camera->getViewMatrix();

    // Modifica la funzione di confronto del buffer di profondità per permettere la scrittura anche per i pixel
    // con la stessa profondità (per disegnare l'ombra sullo stesso piano dell'oggetto)
    glDepthFunc(GL_LEQUAL);

    // Crea una matrice per ridurre l'altezza delle ombre (appiattimento sul piano, z' = 0.05 * z)
    const glm::mat4 shadowModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.0f));

    for (const auto& instance : renderList->getRenderList())
    {

        // 1. Tenta il cast a Mesh
        std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(instance.getNodePtr());


        if(mesh!=nullptr)
            for (const auto& name : excludedList)
            {
                if (mesh->getName() == name)
                    mesh->setShadow(false);
            }


        // 2. Se è un Mesh e può proiettare ombre
        if (mesh != nullptr && mesh->getShadow())
        {
            // Salva il materiale originale
            const std::shared_ptr<Material> originalMaterial = mesh->getMaterial();

            // Cambia temporaneamente il materiale della mesh con quello dell'ombra (scuro, non illuminato)
            mesh->setMaterial(shadowMaterial);

            // Calcola la matrice di trasformazione dell'ombra:
            // Scalatura dell'ombra * Matrice mondiale dell'oggetto
            const glm::mat4 shadow_matrix = shadowModelScaleMatrix * instance.getNodeWorldMatrix();

            // Calcola la Model-View Matrix per l'ombra:
            // Inversa della View Matrix * Matrice di trasformazione dell'ombra
            // Nota: Se la renderizzazione del mesh avviene con la Model-View Matrix (View * Model)
            // allora l'ombra deve essere renderizzata con l'inversa della View Matrix (Model * View^-1)
            // in modo che 'mesh->render' applichi (View * (Model * View^-1)) * Model_ombra = View * Model_ombra
            // Semplicisticamente, la Model-View dell'ombra è View * shadow_matrix
            glm::mat4 shadowModelViewMatrix = camera->getViewMatrix() * shadow_matrix;


            // Si renderizza l'ombra.
            mesh->render(shadowModelViewMatrix);

            // Si rimette il materiale originale della mesh.
            mesh->setMaterial(originalMaterial);
        }
    }

    // 3. Ripristina l'ambiente
    // Ripristina la funzione di confronto del buffer di profondità originale.
    glDepthFunc(GL_LESS);

    // Pulisce il buffer di profondità per il rendering 2D/Testo (così appare sopra la scena)
    glClear(GL_DEPTH_BUFFER_BIT);

    // --- Switch a 2D (per FPS/InfoPrinter) ---
    // Questo è il codice per l'Orthographic Projection che hai nella tua funzione Base::render

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, (float)Base::getWidth(), 0.0f, (float)Base::getHeight(), -1.0f, 1.0f)));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
}

std::shared_ptr<Node> Base::findByName(const std::shared_ptr<Node>& node, const std::string& name) const {

    // @brief iterazione su tutti i figli diretti del nodo corrente
    for (const auto& childNode : node->getChildren()) {

        // @details caso base: se il figlio corrente ha il nome cercato, lo restituiamo subito
        if (childNode->getName() == name)
        {
            return childNode;
        }

        // @brief chiamata ricorsiva per cercare nei discendenti del figlio corrente
        // @details è fondamentale salvare il risultato: se non è nullo, abbiamo trovato il nodo in profondità
        std::shared_ptr<Node> foundNode = this->findByName(childNode, name);

        if (foundNode != nullptr) {
            // @details propagazione del risultato verso l'alto nello stack delle chiamate
            return foundNode;
        }
    }

    // @details se il ciclo termina senza ritorni, il nodo non esiste in questo ramo
    return nullptr;
}

void Base::displayCallback() {
   frames_++;
   glutPostRedisplay();
}

void Base::showFps() {
    // 1. Disabilita luce, non altera il colore del testo
    glDisable(GL_LIGHTING);

    // Il contesto di proiezione (Ortho) e ModelView (Identità) è già impostato
    char text[64]; sprintf(text, "FPS: %.1f", fps);
    glColor3f(1.0f, 1.0f, 1.0f); // Colore bianco

    glRasterPos2f(10.0f, (float)getHeight() - 20.0f);

    // display the string
    glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<unsigned char*>(text));

    //Riabilita l'illuminazione
    glEnable(GL_LIGHTING);
}


void Base::infoPrinter(const std::string& info) {

    // 1. Disabilita luce, non altera il colore del testo
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(10.0f, (float)getInstance().getHeight()-35.0f);

    Base::getInstance().setInfo(info);

    // display the string
    if (!info.empty()) {
        glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<const unsigned char*>(info.c_str()));
    }

    //Riabilita l'illuminazione
    glEnable(GL_LIGHTING);
}

void Base::clearInfoPrinter() {
   Base::getInstance().infoClear();
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

   case GLUT_KEY_F1:
      if (F1_key)
         F1_key();
      break;

   case GLUT_KEY_F2:
      if (F2_key)
         F2_key();
      break;

   case GLUT_KEY_F3:
      if (F3_key)
         F3_key();
      break;

   case GLUT_KEY_F4:
      if (F4_key)
         F4_key();
      break;

   default: std::cout << "UNKNOWN" << std::endl ;
   }
}

ENG_API std::shared_ptr<Texture> Base::loadTextureFromFile(const std::string& path) const noexcept {

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(path.c_str());

    if (format == FIF_UNKNOWN) {
        std::cout << "ERROR: Unknown image format " << path << '\n';
        return nullptr;
    }

    // Load image (can be 24 or 32 bits)
    FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
    if (!bitmap) {
        std::cout << "ERROR: Failed to load image " << path << '\n';
        return nullptr;
    }

    // Flip Y like screenshot
    FreeImage_FlipVertical(bitmap);

    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    BYTE* data = FreeImage_GetBits(bitmap);

    // Determine bit depth (24 = RGB/BGR, 32 = RGBA/BGRA)
    int bpp = FreeImage_GetBPP(bitmap);  // bits per pixel
    int channels = bpp / 8;              // 3 or 4

    auto texture = std::make_shared<Texture>(data, width, height, channels);

    size_t p = path.find_last_of(pathSeparator);
    texture->setName(path.substr(p + 1));

    FreeImage_Unload(bitmap);
    return texture;
}
void Base::bindTexturesToMaterials(const std::shared_ptr<Node>& root, const std::vector<std::shared_ptr<Texture>>& textures) const noexcept {

    if (!root || textures.empty()) {
        return;
    }

    // Nota: .get() non è necessario con l'operatore ->
    for (const auto& childNode : root->getChildren()) {

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
