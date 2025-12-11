#pragma once
#include <memory>

#include "engine.h"

class Setup {

public:
   std::shared_ptr<Eng::Light> initLampadario();
   std::shared_ptr<Eng::Camera> returnFrontTableCamera(Eng::Base& eng);
   std::shared_ptr<Eng::Camera> returnTopTableCamera(Eng::Base& eng);
   void updateDynamicCamera(const std::shared_ptr<Eng::Camera>& cam, const std::shared_ptr<Eng::Node>& sceneRoot);
   std::shared_ptr<Eng::Node> createDynamicLight(const std::shared_ptr<Eng::Node>& sceneRoot);




private:
   void computeDynamicCameraLimits(const std::shared_ptr<Eng::Node>& sceneRoot);

   const std::shared_ptr<Eng::Node> engine;
   const Eng::Node root;

};