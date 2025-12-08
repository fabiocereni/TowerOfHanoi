#pragma once

#include <memory>


#include <functional>

#include "directionallight.h"
#include "list.h"
#include "mesh.h"
#include "node.h"
#include "omnidirectionallight.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"
#include "spotlight.h"



namespace Eng {


   /**
    * @class Base
    * @brief Base engine main class. This class is a singleton.
    */
   class Base final {
   public:


      /// @brief Alias per definire le funzioni di callback
      using KeyboardCallback = std::function<void(unsigned char key, int mouseX, int mouseY)>;
      using SpecialKeyAction = std::function<void()>;


      /// @brief Costruttori, distruttori, operatore di copia
      ENG_API Base(Base const &) = delete;
      ENG_API ~Base();
      ENG_API void operator=(Base const &) = delete;

      /// @brief Singleton
      ENG_API static Base &getInstance();

      /// @brief Init e free del motore
      ENG_API bool init(int argc, char **argv, const std::string& title = "Engine Window");
      ENG_API [[nodiscard]] bool free() const;

      /// @brief getter e setter del WindowID
      ENG_API void setWindowId(const int windowId) noexcept {windowId_ = windowId;};
      ENG_API [[nodiscard]] int getWindowId() const noexcept {return windowId_;};

      /// @brief Richiama il main loop
      ENG_API [[nodiscard]] bool update() const noexcept;

      /// @brief Pulisce i buffer per preparare il nuovo frame
      ENG_API void clear() noexcept;

      /// @brief Inizializza il contesto di rendering 3D
      ENG_API void begin3D(const std::shared_ptr<Camera>& camera) noexcept;

      /// @brief De-inizializza il contesto di rendering 3D
      ENG_API void end3D() noexcept;

      /// @brief Eseguo lo swap dei buffer (front e back)
      ENG_API void swap() noexcept;


      /// @brief Permette il caricamento di una scena
      ENG_API [[nodiscard]] std::shared_ptr<Node> load(const std::string &path) const noexcept;

      /// @brief Permette il caricamento di una texture
      ENG_API [[nodiscard]] std::shared_ptr<Texture> loadTextureFromFile(const std::string& path) const noexcept;

      /// @brief Permette il binding delle texture a tutti i nodi della scena
      ENG_API void bindTexturesToMaterials(const std::shared_ptr<Node>& root, const std::vector<std::shared_ptr<Texture>>& textures) const noexcept;



      /// @brief Callbacks
      ENG_API static void displayCallback();
      ENG_API static void timerCallback(int value);
      ENG_API static void reshapeCallback(int width, int height);
      ENG_API static void specialCallback(int key, int mouseX, int mouseY);


      /// @brief Gestisce il resize della finestra
      ENG_API void changeWindowSize(int width, int height);

      /// @brief Si occupa di cambiare il nome della finestra
      ENG_API void changeWindowTitle(const std::string &title);


      /// @brief Metodi statici per creazione delle telecamere
      ENG_API std::shared_ptr<Camera> createPerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
      ENG_API std::shared_ptr<Camera> createOrthographicCamera(float left,
                                                               float right,
                                                               float top,
                                                               float bottom,
                                                               float nearPlane,
                                                               float farPlane) noexcept;

      /// @brief Metodi statici per creazione delle luci
      ENG_API std::shared_ptr<OmnidirectionalLight> createOmnidirectionalLight();
      ENG_API std::shared_ptr<DirectionalLight> createDirectionalLight();
      ENG_API std::shared_ptr<Spotlight> createSpotlight();



      /// @brief  Metodo statico per creazione di una mesh
      ENG_API static std::shared_ptr<Mesh> createMesh(const std::vector<glm::vec3>& vertexes,
                                                      const std::string& materialName,
                                                      const std::shared_ptr<Material>& material);



      /// @brief getters e setters
      ENG_API void setWidth(const int width) noexcept {width_ = width;};
      ENG_API void setHeight(const int height) noexcept {height_ = height;};
      ENG_API static void setFrames(const float frames) noexcept {frames_ = frames;};
      ENG_API void setActiveCamera(const std::shared_ptr<Camera>& camera) noexcept {activeCamera_ = camera;};
      ENG_API void setInfo(const std::string& info) noexcept { info_ = info; };
      ENG_API [[nodiscard]] int getWidth() const {return width_;};
      ENG_API [[nodiscard]] int getHeight() const {return height_;};
      ENG_API [[nodiscard]] static float getFrames() {return frames_;};
      ENG_API [[nodiscard]] std::shared_ptr<Camera> getActiveCamera() const noexcept {return activeCamera_;};
      ENG_API void setWireframe(bool enable);
      ENG_API void setLighting(bool enable);
      ENG_API void setCulling(bool enable);



      /// @brief Metodi messi a disposizione per sovrascrivere il comportamento di stati "speciali"
      ENG_API void overrideKeyboardCallback(const KeyboardCallback &keyboardCallback) noexcept {customKeyboardCallbackVar_ = keyboardCallback;};
      ENG_API void overrideUpArrowBehaviour(const SpecialKeyAction& action)   noexcept { up_arrow_key_ = action; }
      ENG_API void overrideDownArrowBehaviour(const SpecialKeyAction& action) noexcept { down_arrow_key_ = action; }
      ENG_API void overrideLeftArrowBehaviour(const SpecialKeyAction& action) noexcept { left_arrow_key_ = action; }
      ENG_API void overrideRightArrowBehaviour(const SpecialKeyAction& action) noexcept { right_arrow_key_ = action; }
      ENG_API void overrideF1KeyBehaviour(const SpecialKeyAction& action) noexcept { F1_key = action; }
      ENG_API void overrideF2KeyBehaviour(const SpecialKeyAction& action) noexcept { F2_key = action; }
      ENG_API void overrideF3KeyBehaviour(const SpecialKeyAction& action) noexcept { F3_key = action; }
      ENG_API void overrideF4KeyBehaviour(const SpecialKeyAction& action) noexcept { F4_key = action; }


      /// @brief Registrazione callback di tasti "normali"
      ENG_API static void useCustomKeyboardCallback(unsigned char key, int mouseX, int mouseY) noexcept;



      ENG_API void render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList) noexcept;
      ENG_API void renderPlanarShadows(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList);
      ENG_API void showFps();
      ENG_API static void infoPrinter(const std::string& info);
      ENG_API void infoClear() { return info_.clear(); };
      ENG_API static void clearInfoPrinter();


      std::shared_ptr<Node> findByName(const std::shared_ptr<Node>& node, const std::string& name) const;




      ///////////
   private: //
      ///////////
      int windowId_;
      int width_ = 1800;
      int height_ = 1000;
      static float frames_;
      std::string info_;
      std::shared_ptr<Camera> activeCamera_;


      static KeyboardCallback customKeyboardCallbackVar_;
      static SpecialKeyAction up_arrow_key_;
      static SpecialKeyAction down_arrow_key_;
      static SpecialKeyAction left_arrow_key_;
      static SpecialKeyAction right_arrow_key_;
      static SpecialKeyAction F1_key;
      static SpecialKeyAction F2_key;
      static SpecialKeyAction F3_key;
      static SpecialKeyAction F4_key;

      // Reserved:
      struct Reserved;
      std::unique_ptr<Reserved> reserved_;


      // Const/dest:
      Base();
   };

}; // end of namespace Eng::

