#pragma once

#include <memory>




// #include "camera.h"
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
      ENG_API static void clear() noexcept;
      ENG_API static void begin3D(const std::shared_ptr<Camera>& camera) noexcept;
      ENG_API static void end3D() noexcept;
      ENG_API static void swap() noexcept;
      ENG_API [[nodiscard]] std::shared_ptr<Node> load(const std::string &path) const noexcept;


      // quella statica serve perchè glutDisplayFunc(...) accetta una funzione C statica
      ENG_API static void displayCallback();
      ENG_API void internalDisplay();

      ENG_API void changeWindowSize(int width, int height);
      ENG_API void changeWindowTitle(const std::string &title);
      ENG_API void changeBackgroundColor(float r, float g, float b);


      ENG_API static std::shared_ptr<Camera> createPerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
      ENG_API static std::shared_ptr<Camera> createOrthographicCamera(float size, float nearPlane, float farPlane) noexcept;


      ENG_API static std::shared_ptr<Mesh> createMesh(const std::vector<glm::vec3>& vertexes, const std::shared_ptr<Material>& material);


      ENG_API void setWidth(const int width) noexcept {width_ = width;};
      ENG_API void setHeight(const int height) noexcept {height_ = height;};

      ENG_API [[nodiscard]] int getWidth() const {return width_;};
      ENG_API [[nodiscard]] int getHeight() const {return height_;};


      ENG_API void render(std::shared_ptr<Camera> camera, std::shared_ptr<List> renderList) noexcept;


      ///////////
   private: //
      ///////////
      int windowId_;
      int width_ = 800;
      int height_ = 600;

      // Reserved:
      struct Reserved;
      std::unique_ptr<Reserved> reserved_;


      // Const/dest:
      Base();
   };

}; // end of namespace Eng::

