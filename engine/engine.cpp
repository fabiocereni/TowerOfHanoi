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
   if (reserved_->initFlag)
      return false;


   /// @brief inizializzazione di FreeImage
   FreeImage_Initialise();




   ///@brief Inizializzazione di FreeGLUT e della finestra
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(getWidth(), getHeight());
   const int id = glutCreateWindow(title.c_str());
   setWindowId(id);
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   // @brief Impostazioni OpenGL
   /// @details attivazione dello z-buffer
   glEnable(GL_DEPTH_TEST);

   /// @details normalizzazione automatica
   /// delle normali delle mesh
   glEnable(GL_NORMALIZE);


   /// @brief Abilita il sistema di illuminazione
   glEnable(GL_LIGHTING);
   // Modello speculare realistico
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);

   glEnable(GL_CULL_FACE);


   glm::vec4 globalAmbient(0.1f, 0.1f, 0.1f, 1.0f);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glm::value_ptr(globalAmbient));


   /// @brief Callbacks
   glutDisplayFunc(displayCallback);
   glutReshapeFunc(reshapeCallback);
   glutTimerFunc(1000, timerCallback, 0);
   glutKeyboardFunc(useCustomKeyboardCallback);
   glutSpecialFunc(specialCallback);

   /// @brief definizione color "ombra"
   auto black = glm::vec4(0.0f);
   shadowMaterial_ = std::make_shared<Material>(black, black, black,black, 0.0f, "");

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
   // Processa eventi GLUT
   glutMainLoopEvent();

   // Se la finestra è stata chiusa:
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
   /// @brief attiva la modalità matrice di model view
   glMatrixMode(GL_MODELVIEW);
}


/**
 * @brief De-inizializza il contesto di rendering 3D
 */

void Base::end3D() noexcept {
   glMatrixMode(GL_MODELVIEW);
   auto identity = glm::mat4(1.0f);
   /// @details carica una matrice neutra
   glLoadMatrixf(glm::value_ptr(identity));
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
   if (!reserved_->initFlag) {
      std::cout << "ERROR: engine not initialized" << std::endl;
      return false;
   }

   FreeImage_DeInitialise();

   std::cout << "[<] " << LIB_NAME << " deinitialized" << std::endl;
   reserved_->initFlag = false;
   return true;
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

std::shared_ptr<Mesh> Base::createMesh(const std::vector<glm::vec3>& vertexes,
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
 * @brief Innesca la catena di rendering
 * @param camera
 * @param renderList
 * @param excludedList
 */
void Base::render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList, const std::list<std::string>& excludedList) const {

    for (const auto& instance : renderList->getRenderList()) {
        glm::mat4 viewMatrix = camera->getViewMatrix();
        // @brief carichiamo la matrice di modelView
        glm::mat4 modelViewMatrix = viewMatrix * instance.getNodeWorldMatrix();
        instance.getNodePtr()->render(modelViewMatrix);
    }

    glDepthFunc(GL_LEQUAL);

    // Crea una matrice per ridurre l'altezza delle ombre (appiattimento sul piano, z' = 0.05 * z)
    const glm::mat4 shadowModelScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.05f, 1.0f));

    for (const auto& instance : renderList->getRenderList())
    {

        // @brief Tenta il cast a Mesh
        std::shared_ptr<Mesh> mesh = std::dynamic_pointer_cast<Mesh>(instance.getNodePtr());


        if(mesh)
            for (const auto& name : excludedList)
            {
                if (mesh->getName() == name)
                    mesh->setShadow(false);
            }


        // @brief Se è un Mesh e può proiettare ombre
        if (mesh && mesh->getShadow()) {
            const std::shared_ptr<Material> originalMaterial = mesh->getMaterial();

            mesh->setMaterial(shadowMaterial_);

            const glm::mat4 shadow_matrix = shadowModelScaleMatrix * instance.getNodeWorldMatrix();

            glm::mat4 shadowModelViewMatrix = camera->getViewMatrix() * shadow_matrix;


           /// @details Renderizziamo l'ombra
            mesh->render(shadowModelViewMatrix);

            /// @details rimettiamo il materiale originale
            mesh->setMaterial(originalMaterial);
        }
    }

    glDepthFunc(GL_LESS);

    glClear(GL_DEPTH_BUFFER_BIT);


    // --- Switch a 2D (per FPS/InfoPrinter) ---
    // Questo è il codice per l'Orthographic Projection che hai nella tua funzione Base::render
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, static_cast<float>(getWidth()),
                    0.0f, static_cast<float>(getHeight()), -1.0f, 1.0f)));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
}


/**
 * @brief Cerca un nodo nel grafo della scena tramite il nome
 * @param node Nodo di partenza per la ricerca
 * @param name Nome del nodo da cercare
 * @return std::shared_ptr<Node> Il nodo trovato o nullptr se non esiste
 */
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


/**
 * @brief Callback per la gestione del rendering
 */
void Base::displayCallback() {
   frames_++;
   glutPostRedisplay();
}

/**
 * @brief Mostra il conteggio degli FPS a schermo
 */
void Base::showFps() {
    // 1. Disabilita luce, non altera il colore del testo
    glDisable(GL_LIGHTING);

    // Il contesto di proiezione (Ortho) e ModelView (Identità) è già impostato
    char text[64]; sprintf(text, "FPS: %.1f", fps);
    glColor3f(1.0f, 1.0f, 1.0f); // Colore bianco

    glRasterPos2f(10.0f, static_cast<float>(getHeight()) - 20.0f);

    // display the string
    glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<unsigned char*>(text));

    //Riabilita l'illuminazione
    glEnable(GL_LIGHTING);
}


/**
 * @brief Stampa informazioni testuali a schermo
 * @param info Stringa contenente le informazioni da visualizzare
 */
void Base::infoPrinter(const std::string& info) {

    // 1. Disabilita luce, non altera il colore del testo
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 1.0f, 1.0f);

    glRasterPos2f(10.0f, (float)getInstance().getHeight()-35.0f);

    getInstance().setInfo(info);

    // display the string
    if (!info.empty()) {
        glutBitmapString(GLUT_BITMAP_8_BY_13, reinterpret_cast<const unsigned char*>(info.c_str()));
    }

    //Riabilita l'illuminazione
    glEnable(GL_LIGHTING);
}

/**
 * @brief Pulisce le informazioni visualizzate dall'infoPrinter
 */
void Base::clearInfoPrinter() {
   getInstance().infoClear();
}

/**
 * @brief Callback del timer per il calcolo degli FPS
 * @param value Valore intero passato dal timer (non utilizzato)
 */
void Base::timerCallback(int value) {
   fps = getFrames();
   setFrames(0);

   // ogni secondo
   glutTimerFunc(1000, timerCallback, 0);
}

/**
 * @brief Callback per la gestione del ridimensionamento della finestra
 * @param width Nuova larghezza della finestra
 * @param height Nuova altezza della finestra
 */
void Base::reshapeCallback(const int width, const int height) {

   // aggiorno view port
   glViewport(0, 0, width, height);

   if (const auto cam = getInstance().getActiveCamera()) {
      // aggiorno in base alla camera attiva
      cam->onResize(width, height);
   }
}

/**
 * @brief Callback per la gestione degli input da tastiera speciale
 * @param key Codice del tasto premuto
 * @param mouseX Posizione X del mouse
 * @param mouseY Posizione Y del mouse
 */
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

/**
 * @brief Carica una texture da file utilizzando FreeImage
 * @param path Percorso del file immagine
 * @return std::shared_ptr<Texture> La texture caricata o nullptr in caso di errore
 */
std::shared_ptr<Texture> Base::loadTextureFromFile(const std::string& path) const noexcept {

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
    if (format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(path.c_str());

    if (format == FIF_UNKNOWN) {
        std::cout << "ERROR: Unknown image format " << path << '\n';
        return nullptr;
    }

    FIBITMAP* bitmap = FreeImage_Load(format, path.c_str());
    if (!bitmap) {
        std::cout << "ERROR: Failed to load image " << path << '\n';
        return nullptr;
    }

    FreeImage_FlipVertical(bitmap);

    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    BYTE* data = FreeImage_GetBits(bitmap);

    /// @details dividendo per 8 ottengo il numero di canali
    int bpp = FreeImage_GetBPP(bitmap);
    int channels = bpp / 8;

    auto texture = std::make_shared<Texture>(data, width, height, channels);

    size_t p = path.find_last_of(pathSeparator);
    texture->setName(path.substr(p + 1));

    FreeImage_Unload(bitmap);
    return texture;
}

/**
 * @brief Associa le texture caricate ai materiali delle mesh nel grafo della scena
 * @param root Nodo radice da cui iniziare la ricerca
 * @param textures Vettore di texture disponibili
 */
void Base::bindTexturesToMaterials(const std::shared_ptr<Node>& root, const std::vector<std::shared_ptr<Texture>>& textures) const noexcept {

    if (!root || textures.empty()) {
        return;
    }

    for (const auto& childNode : root->getChildren()) {

        auto mesh = std::dynamic_pointer_cast<Mesh>(childNode);

        if (mesh) {


            auto material = mesh->getMaterial();

            if (material && !material->getTextureName().empty()) {

                for (const auto& texture : textures) {

                    if (texture->getName() == material->getTextureName()) {
                        material->setTexture(texture);
                    }
                }
            }
        }

        // Ricorsione
        this->bindTexturesToMaterials(childNode, textures);
    }
}

/**
 * @brief Abilita o disabilita la modalità wireframe
 * @param enable True per abilitare, False per disabilitare
 */
void Base::setWireframe(const bool enable) {
   if (enable)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
 * @brief Abilita o disabilita l'illuminazione
 * @param enable True per abilitare, False per disabilitare
 */
void Base::setLighting(const bool enable) {
   if (enable)
      glEnable(GL_LIGHTING);
   else
      glDisable(GL_LIGHTING);
}

