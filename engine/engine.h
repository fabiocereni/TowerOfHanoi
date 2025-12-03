#pragma once

#include <memory>




// #include "camera.h"
#include <functional>

#include "list.h"
#include "mesh.h"
#include "node.h"
#include "perspective_camera.h"
#include "orthographic_camera.h"



///////////////
// NAMESPACE //
///////////////

namespace Eng {



   //////////////
   // #INCLUDE //
   //////////////

   // You can subinclude here other headers of your engine...



   ///////////////////////
   // MAIN ENGINE CLASS //
   ///////////////////////

   /**
    * @brief Base engine main class. This class is a singleton.
    */
   class Base final {
      //////////
   public: //
      //////////


      // alias per definire funzioni di callback
      using KeyboardCallback = std::function<void(unsigned char key, int mouseX, int mouseY)>;
      using SpecialKeyAction = std::function<void()>;


      // Const/dest:
      ENG_API Base(Base const &) = delete;
      ENG_API ~Base();

      // Operators:
      ENG_API void operator=(Base const &) = delete;

      // Singleton:
      ENG_API static Base &getInstance();

      // Init/free:
      ENG_API bool init(int argc, char **argv, const std::string& title = "Engine Window");
      ENG_API bool free() const;

      ENG_API void setWindowId(const int windowId) noexcept {windowId_ = windowId;};
      ENG_API [[nodiscard]] int getWindowId() const noexcept {return windowId_;};


      ENG_API [[nodiscard]] bool update() const noexcept;
      ENG_API void clear() noexcept;
      ENG_API void begin3D(const std::shared_ptr<Camera>& camera) noexcept;
      ENG_API void end3D() noexcept;
      ENG_API void swap() noexcept;
      ENG_API [[nodiscard]] std::shared_ptr<Node> load(const std::string &path) const noexcept;
      ENG_API [[nodiscard]] std::shared_ptr<Texture> loadTextureFromFile(const std::string& path) const noexcept;


      ENG_API static void displayCallback();
      ENG_API static void timerCallback(int value);
      ENG_API static void reshapeCallback(int width, int height);
      ENG_API static void specialCallback(int key, int mouseX, int mouseY);

      ENG_API void changeWindowSize(int width, int height);
      ENG_API void changeWindowTitle(const std::string &title);
      ENG_API void changeBackgroundColor(float r, float g, float b);


      ENG_API std::shared_ptr<Camera> createPerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
      ENG_API std::shared_ptr<Camera> createOrthographicCamera(float size, float nearPlane, float farPlane) noexcept;


      ENG_API static std::shared_ptr<Mesh> createMesh(const std::vector<glm::vec3>& vertexes,
                                                      const std::string& materialName,
                                                      const std::shared_ptr<Material>& material);


      ENG_API void setWidth(const int width) noexcept {width_ = width;};
      ENG_API void setHeight(const int height) noexcept {height_ = height;};
      ENG_API static void setFrames(const float frames) noexcept {frames_ = frames;};
      ENG_API void setActiveCamera(const std::shared_ptr<Camera>& camera) noexcept {activeCamera_ = camera;};


      ENG_API void overrideKeyboardCallback(const KeyboardCallback &keyboardCallback) noexcept {customKeyboardCallbackVar_ = keyboardCallback;};
      ENG_API void overrideUpArrowBehaviour(SpecialKeyAction action)   noexcept { up_arrow_key_ = action; }
      ENG_API void overrideDownArrowBehaviour(SpecialKeyAction action) noexcept { down_arrow_key_ = action; }
      ENG_API void overrideLeftArrowBehaviour(SpecialKeyAction action) noexcept { left_arrow_key_ = action; }
      ENG_API void overrideRightArrowBehaviour(SpecialKeyAction action) noexcept { right_arrow_key_ = action; }






      ENG_API [[nodiscard]] int getWidth() const {return width_;};
      ENG_API [[nodiscard]] int getHeight() const {return height_;};
      ENG_API [[nodiscard]] static float getFrames() {return frames_;};
      ENG_API [[nodiscard]] std::shared_ptr<Camera> getActiveCamera() const noexcept {return activeCamera_;};



      ENG_API static void useCustomKeyboardCallback(unsigned char key, int mouseX, int mouseY) noexcept;



      ENG_API void render(const std::shared_ptr<Camera>& camera, const std::shared_ptr<List>& renderList) noexcept;
      ENG_API void showFps();

      ///////////
   private: //
      ///////////
      int windowId_;
      int width_ = 800;
      int height_ = 600;
      static float frames_;
      std::shared_ptr<Camera> activeCamera_;


      static KeyboardCallback customKeyboardCallbackVar_;
      static SpecialKeyAction up_arrow_key_;
      static SpecialKeyAction down_arrow_key_;
      static SpecialKeyAction left_arrow_key_;
      static SpecialKeyAction right_arrow_key_;

      // Reserved:
      struct Reserved;
      std::unique_ptr<Reserved> reserved_;


      // Const/dest:
      Base();
   };

}; // end of namespace Eng::

