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

/**
 * @brief Path separator
 * @details Permette di usare il corretto
 * separatore di percorsi in base al sistema operativo
 */
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

/**
 * @brief Inizializzazione dei
 * componenti statici
 */
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



/// @brief Variabile per il calcolo degli fps
float fps = 0.0f;


/// @brief Variabili per salvare le matrici
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


/**
 * @brief Si occupa dell'inizializzazione del motore
 * @param argc
 * @param argv
 * @param title
 * @return Se riesce a inizializzare il motore
 * torna true
 */
bool Base::init(int argc, char **argv, const std::string& title) {
   if (reserved_->initFlag) return false;

  /// @brief inizializzazione di FreeImage
   FreeImage_Initialise();


   /// @brief Inizializzazione di FreeGLUT
   /// e della finestra
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(getWidth(), getHeight());
   const int id = glutCreateWindow(title.c_str());
   setWindowId(id);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   /// @brief Impostazioni OpenGL
   /// @details attivazione dello z-buffer
   glEnable(GL_DEPTH_TEST);
   /// @details normalizzazione automatica
   /// delle normali delle mesh
   glEnable(GL_NORMALIZE);

   /// @brief Abilita il sistema di illuminazione
   glEnable(GL_LIGHTING);
   /// @bried Abilita il modello speculare realistico
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

   /// @brief Culling disabilitalo all'inizio
   glDisable(GL_CULL_FACE);



   /// @brief Callbacks
   glutDisplayFunc(displayCallback);
   glutReshapeFunc(reshapeCallback);
   glutTimerFunc(1000, timerCallback, 0);
   glutKeyboardFunc(useCustomKeyboardCallback);
   glutSpecialFunc(specialCallback);

   /// @brief motore inizializzato
   reserved_->initFlag = true;
   return true;
}

/**
 * @brief Registrazione callback di tasti "normali"
 * @details Permette una registrazione esterna
 * delle callbacks su tasti "normali"
 * @param key
 * @param mouseX
 * @param mouseY
 */
void Base::useCustomKeyboardCallback(const unsigned char key, const int mouseX, const int mouseY) noexcept {
   if (customKeyboardCallbackVar_) {
      customKeyboardCallbackVar_(key, mouseX, mouseY);
   }
}


/**
 * @brief Richiama il main loop
 * @return Ritorna true se la finestra
 * non è stata chiusa
 */
bool Base::update() const noexcept {
   glutMainLoopEvent();

   if (!glutGetWindow())
      return false;

   return true;
}


/**
 * @brief Eseguo lo swap dei buffer (front e back)
 */
void Base::swap() noexcept {
   glutSwapBuffers();
}


/**
 * @brief Inizializza il contesto di rendering 3D
 * @param camera La telecamera da cui prendere la matrice di proiezione
 */
void Base::begin3D(const std::shared_ptr<Camera>& camera) noexcept {
   /// @brief attiva la modalità matrice di proiezione
   glMatrixMode(GL_PROJECTION);
   /// @brief attiva la modalità matrice di proiezione
   glLoadMatrixf(glm::value_ptr(camera->getProjectionMatrix()));
   /// @brief attiva la modalità matrice di proiezione
   glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief De-inizializza il contesto di rendering 3D
 */
void Base::end3D() noexcept {
   glMatrixMode(GL_MODELVIEW);
   /// @details carica una matrice neutra
   glLoadIdentity();
}

/**
 * @brief Pulisce i buffer per preparare il nuovo frame
 */
void Base::clear() noexcept {
   /// @brief pulisce sia il buffer di colore che il depth buffer (z-buffer)
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Si occupa del rilascio delle risorse
 * @return Se il motore era stato inizializzato torna true
 */
bool Base::free() const {
   if (!reserved_->initFlag)
   {

      FreeImage_DeInitialise();
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }


   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved_->initFlag = false;
   return true;
}

/**
 * @details Gestisce il resize della finestra
 * @param width
 * @param height
 */
void Base::changeWindowSize(const int width, const int height) {

   glutReshapeWindow(width, height);
   setWidth(width);
   setHeight(height);
}


/**
 * @brief Si occupa di cambiare il nome della finestra
 * @param title
 */
void Base::changeWindowTitle(const std::string &title) {
   glutSetWindowTitle(title.c_str());
}


/**
 * @brief Metodo statico per creazione di una mesh
 * @param vertexes
 * @param materialName
 * @param material
 * @return Ritorna la mesh
 */
ENG_API std::shared_ptr<Mesh> Base::createMesh(const std::vector<glm::vec3>& vertexes,
                                               const std::string& materialName,
                                               const std::shared_ptr<Material>& material) {
   return std::make_shared<Mesh>(vertexes, materialName, material);
}

/**
 * @brief Metodo statico per creazione della telecamera prospettica
 * @param fov
 * @param aspectRatio
 * @param nearPlane
 * @param farPlane
 * @return Ritorna una telecamera prospettica
 */
std::shared_ptr<Camera> Base::createPerspectiveCamera(float fov, float aspectRatio,
                                                      float nearPlane, float farPlane) noexcept {

   return std::make_shared<Perspective_Camera>(fov, aspectRatio, nearPlane, farPlane);
}


/**
 * @brief Metodo statico per creazione della telecamera ortografica
 * @param left
 * @param right
 * @param top
 * @param bottom
 * @param nearPlane
 * @param farPlane
 * @return Ritorna una telecamera ortografica
 */
std::shared_ptr<Camera> Base::createOrthographicCamera(float left,
                                                       float right,
                                                       float top,
                                                       float bottom,
                                                       float nearPlane,
                                                       float farPlane) noexcept
{
   return std::make_shared<Orthographic_Camera>(left, right, top, bottom, nearPlane, farPlane);
}

/// @brief Metodo statico per la creazione di una luce omnidirezionale
std::shared_ptr<OmnidirectionalLight> Base::createOmnidirectionalLight() {
   return OmnidirectionalLight::createOmnidirectionalLight();
}

/// @brief Metodo statico per la creazione di una luce spotlight
std::shared_ptr<Spotlight> Base::createSpotlight() {
   return Spotlight::createSpotLight();
}

/// @brief Metodo statico per la creazione di una luce direzionale
std::shared_ptr<DirectionalLight> Base::createDirectionalLight() {
   return DirectionalLight::createDirectionalLight();
}



/**
 * @brief Permette il caricamento di una scena
 * @param path Path al percorso del file OVO
 * @return Ritorna la root con la gerarchia completa
 */
std::shared_ptr<Node> Base::load(const std::string& path) const noexcept {
   return OvoReader::load(std::filesystem::absolute(path).string());
}

/**
 * @brief
 * @param camera
 * @param renderList
 */
void Base::render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList) noexcept {

   for (const auto& instance : renderList->getRenderList()) {

      glm::mat4 viewMatrix = camera->getViewMatrix();
      glm::mat4 modelViewMatrix = viewMatrix * instance.getNodeWorldMatrix();
      instance.getNodePtr()->render(modelViewMatrix);
   }

}



void Base::renderPlanarShadows(const std::shared_ptr<Camera>& camera,
    const std::shared_ptr<List>& renderList)
{
    // --- 1. CERCA LA LUCE E IL PIANO NELLA LISTA ---

    glm::vec4 lightPos(0.0f, 100.0f, 0.0f, 1.0f); // Default se non trova luci
    glm::vec4 planeEquation(0.0f, 1.0f, 0.0f, 0.0f); // Default: Piano Y=0 (Pavimento)

    bool lightFound = false;

    for (const auto& instance : renderList->getRenderList()) {
        auto node = instance.getNodePtr();

        // A. Cerca una luce (Light)
        // Usiamo dynamic_pointer_cast per vedere se il nodo è una luce
        if (!lightFound) {
            auto light = std::dynamic_pointer_cast<Light>(node);
            if (light) {
                // Trovata! Prendiamo la sua posizione World dalla matrice dell'istanza
                auto pos = glm::vec3(instance.getNodeWorldMatrix()[3]);
                lightPos = glm::vec4(pos, 1.0f);
                lightFound = true;
            }
        }

        // B. (Opzionale) Cerca l'altezza del tavolo
        // Se il tuo tavolo si chiama "baseTavolo", possiamo prendere la sua Y
        if (node->getName() == "pavimento") {
            float tableHeight = instance.getNodeWorldMatrix()[3].y;
            // Aggiorna l'equazione del piano: Y = tableHeight -> 0x + 1y + 0z - tableHeight = 0
            // Usiamo un piccolo offset (+0.1) per evitare z-fighting
            planeEquation.w = -(tableHeight + 0.1f);
        }
    }

    // --- 2. CALCOLO MATRICE (Come prima) ---

    glm::mat4 shadowMat(0.0f);
    float dot = planeEquation.x * lightPos.x +
        planeEquation.y * lightPos.y +
        planeEquation.z * lightPos.z +
        planeEquation.w * lightPos.w;

    // Matrice di proiezione planare standard
    shadowMat[0][0] = dot - lightPos.x * planeEquation.x;
    shadowMat[1][0] = 0.f - lightPos.x * planeEquation.y;
    shadowMat[2][0] = 0.f - lightPos.x * planeEquation.z;
    shadowMat[3][0] = 0.f - lightPos.x * planeEquation.w;

    shadowMat[0][1] = 0.f - lightPos.y * planeEquation.x;
    shadowMat[1][1] = dot - lightPos.y * planeEquation.y;
    shadowMat[2][1] = 0.f - lightPos.y * planeEquation.z;
    shadowMat[3][1] = 0.f - lightPos.y * planeEquation.w;

    shadowMat[0][2] = 0.f - lightPos.z * planeEquation.x;
    shadowMat[1][2] = 0.f - lightPos.z * planeEquation.y;
    shadowMat[2][2] = dot - lightPos.z * planeEquation.z;
    shadowMat[3][2] = 0.f - lightPos.z * planeEquation.w;

    shadowMat[0][3] = 0.f - lightPos.w * planeEquation.x;
    shadowMat[1][3] = 0.f - lightPos.w * planeEquation.y;
    shadowMat[2][3] = 0.f - lightPos.w * planeEquation.z;
    shadowMat[3][3] = dot - lightPos.w * planeEquation.w;

    // --- 3. RENDERIZZAZIONE ---

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Colore ombra (nero trasparente)
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Applichiamo la matrice d'ombra alla View Matrix corrente
    // (Simuliamo l'effetto duplicando la logica di render base)
    glm::mat4 shadowView = camera->getViewMatrix() * shadowMat;

    for (const auto& instance : renderList->getRenderList()) {
        // NON disegnare l'ombra della base del tavolo o del pavimento (si auto-ombreggerebbero male)
        // Filtra per nome o tipo
        std::string name = instance.getNodePtr()->getName();
        if (name == "pavimento") continue;

        // Disegna solo gli oggetti che proiettano ombra (es. dischi, pali)
        glm::mat4 modelViewMatrix = shadowView * instance.getNodeWorldMatrix();
        instance.getNodePtr()->render(modelViewMatrix);
    }

    glPopMatrix();
    glPopAttrib();
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

void Base::infoPrinter(const std::string& info) {

   // disabilita luce, non altera il colore del testo
   glDisable(GL_LIGHTING);

   // salva proiezione
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();


   // salva modelview
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));


   glColor3f(1, 1, 1);
   // string position
   glRasterPos2f(-0.95f, 0.80f);

   Base::getInstance().setInfo(info);

   // display the string
   if (!info.empty()) {
      glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<const unsigned char*>(info.c_str()));
   }   // ripristina

   glPopMatrix();

   // MODELVIEW
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
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


std::shared_ptr<Node> Base::findByName(const std::shared_ptr<Node>& node, const std::string& name) const {
   for (const auto& childNode : node.get()->getChildren()) {
      if (childNode->getName() == name)
      {
         return childNode;
      }
      this->findByName(childNode, name);
   }
   return nullptr;
}