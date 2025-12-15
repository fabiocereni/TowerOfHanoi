#pragma once

#include <memory>


#include <functional>
#include <filesystem>

#include "directionallight.h"
#include "list.h"
#include "mesh.h"
#include "node.h"
#include "omnidirectionallight.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"
#include "spotlight.h"

namespace Eng
{
   /**
     * @class Base
     * @brief Base engine main class. This class is a singleton.
     */
   class ENG_API Base final
   {
   public:
      /// @brief Alias per definire le funzioni di callback
      using KeyboardCallback = std::function<void(unsigned char key, int mouseX, int mouseY)>;
      using SpecialKeyAction = std::function<void()>;


      /// @brief Costruttori, distruttori, operatore di copia
      Base(Base const&) = delete;
      ~Base();

      void operator=(Base const&) = delete;

      /// @brief Singleton
      static Base& getInstance();

      /// @brief Init e free del motore
      bool init(int argc, char** argv, const std::string& title = "Engine Window");
      [[nodiscard]] bool free() const;

      /// @brief Getter e setter del WindowID
      void setWindowId(const int windowId) noexcept { windowId_ = windowId; };
      [[nodiscard]] int getWindowId() const noexcept { return windowId_; };

      /// @brief Richiama il main loop
      [[nodiscard]] bool update() const noexcept;

      /// @brief Pulisce i buffer per preparare il nuovo frame
      void clear() noexcept;

      /// @brief Inizializza il contesto di rendering 3D
      void begin3D(const std::shared_ptr<Camera>& camera) noexcept;

      /// @brief De-inizializza il contesto di rendering 3D
      void end3D() noexcept;

      /// @brief Inizializza il contesto di rendering 2D per stampare
      void begin2D(const std::shared_ptr<Camera>& camera) noexcept;

      /// @brief De-inizializza il contesto di rendering 2D
      void end2D() noexcept;


      /// @brief Eseguo lo swap dei buffer (front e back)
      void swap() noexcept;

      /// @brief Permette il caricamento di una scena
      [[nodiscard]] std::shared_ptr<Node> load(const std::string& path) noexcept;

      /// @brief Permette il caricamento di una texture
      [[nodiscard]] static std::shared_ptr<Texture> loadTextureFromFile(const std::string& path) noexcept;

      /// @brief Permette il binding delle texture a tutti i nodi della scena
      void bindTexturesToMaterials(const std::shared_ptr<Node>& root,
                                                 const std::vector<std::shared_ptr<Texture>>& textures) const noexcept;

      /// @brief Carica le texture dalla cartella e le applica ai materiali
      void loadAndBindTextures(const std::filesystem::path& projectDir_,
                                             const std::shared_ptr<Eng::Node>& root) const;


      /// @brief Callbacks
      static void displayCallback();
      static void timerCallback(int value);
      static void reshapeCallback(int width, int height);
      static void specialCallback(int key, int mouseX, int mouseY);

      /// @brief Si occupa di cambiare il nome della finestra
      void changeWindowTitle(const std::string& title);

      /// @brief Metodi statici per creazione delle telecamere
      std::shared_ptr<Camera> createPerspectiveCamera(float fov, float aspectRatio, float nearPlane,
                                                      float farPlane) noexcept;
      std::shared_ptr<Camera> createOrthographicCamera(float left,
                                                       float right,
                                                       float top,
                                                       float bottom,
                                                       float nearPlane,
                                                       float farPlane) noexcept;

      /// @brief Metodi statici per creazione delle luci
      std::shared_ptr<OmnidirectionalLight> createOmnidirectionalLight();
      std::shared_ptr<DirectionalLight> createDirectionalLight();
      std::shared_ptr<Spotlight> createSpotlight();


      /// @brief  Metodo statico per creazione di una mesh
      std::shared_ptr<Mesh> createMesh(const std::vector<glm::vec3>& vertexes,
                                              const std::string& materialName,
                                              const std::shared_ptr<Material>& material);


      /// @brief Getters e setters
      void setWidth(const int width) noexcept { width_ = width; };
      void setHeight(const int height) noexcept { height_ = height; };
      static void setFrames(const float frames) noexcept { frames_ = frames; };
      void setActiveCamera(const std::shared_ptr<Camera>& camera) noexcept { activeCamera_ = camera; };

      [[nodiscard]] int getWidth() const { return width_; };
      [[nodiscard]] int getHeight() const { return height_; };
      [[nodiscard]] static float getFrames() { return frames_; };
      [[nodiscard]] std::shared_ptr<Camera> getActiveCamera() const noexcept { return activeCamera_; };


      /// @brief Metodi messi a disposizione per sovrascrivere il comportamento di stati "speciali"
      void overrideUpArrowBehaviour(const SpecialKeyAction& action) noexcept { up_arrow_key_ = action; }
      void overrideDownArrowBehaviour(const SpecialKeyAction& action) noexcept { down_arrow_key_ = action; }
      void overrideLeftArrowBehaviour(const SpecialKeyAction& action) noexcept { left_arrow_key_ = action; }
      void overrideRightArrowBehaviour(const SpecialKeyAction& action) noexcept { right_arrow_key_ = action; }
      void overrideF1KeyBehaviour(const SpecialKeyAction& action) noexcept { F1_key = action; }
      void overrideF2KeyBehaviour(const SpecialKeyAction& action) noexcept { F2_key = action; }
      void overrideF3KeyBehaviour(const SpecialKeyAction& action) noexcept { F3_key = action; }
      void overrideF4KeyBehaviour(const SpecialKeyAction& action) noexcept { F4_key = action; }


      /// @brief Registrazione callback di tasti "normali"
      static void useCustomKeyboardCallback(unsigned char key, int mouseX, int mouseY) noexcept;

      void overrideKeyboardCallback(const KeyboardCallback& keyboardCallback) noexcept
      {
         customKeyboardCallbackVar_ = keyboardCallback;
      };


      /// @brief Innesca la catena di rendering
      void render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList,
                  const std::list<std::string>& excludedList) const;

      /// @brief Mostra il conteggio degli FPS a schermo
      void showFps() const;

      /// @brief Stampa informazioni testuali a schermo
      static void infoPrinter(const std::string& info);
      static void statusPrinter(const std::string& info);


      /// @brief Cerca un nodo nel grafo della scena tramite il nome
      static std::shared_ptr<Node> findByName(const std::shared_ptr<Node>& node, const std::string& name);


      /// @brief Abilita o disabilita la modalità wireframe
      void setWireframe(bool enable);

      /// @brief Abilita o disabilita l'illuminazione
      void setLighting(bool enable);


   private:
      /// @brief ID della finestra
      int windowId_;
      /// @brief Larghezza della finestra
      int width_ = 1200;
      /// @brief Altezza della finestra
      int height_ = 1000;
      /// @brief Contatore dei frame
      static float frames_;
      /// @brief Camera attiva
      std::shared_ptr<Camera> activeCamera_;


      /// @brief Indice vettore callback timer
      static int timerCallbackVectorIndex_;
      /// @brief Seconda callback timer
      static std::function<void()> secondTimerCallback_;

      /// @brief Variabile per callback tastiera custom
      static KeyboardCallback customKeyboardCallbackVar_;

      /// @brief Azione tasto freccia su
      static SpecialKeyAction up_arrow_key_;
      /// @brief Azione tasto freccia giù
      static SpecialKeyAction down_arrow_key_;
      /// @brief Azione tasto freccia sinistra
      static SpecialKeyAction left_arrow_key_;
      /// @brief Azione tasto freccia destra
      static SpecialKeyAction right_arrow_key_;
      /// @brief Azione tasto F1
      static SpecialKeyAction F1_key;
      /// @brief Azione tasto F2
      static SpecialKeyAction F2_key;
      /// @brief Azione tasto F3
      static SpecialKeyAction F3_key;
      /// @brief Azione tasto F4
      static SpecialKeyAction F4_key;

      /// @brief Materiale per le ombre
      std::shared_ptr<Material> shadowMaterial_;

      struct Reserved;
      /// @brief Puntatore pimpl idiom
      std::unique_ptr<Reserved> reserved_;

      /// @brief Costruttore privato
      Base();
   };
};
