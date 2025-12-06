#pragma once
#include <memory>
#include <vector>
#include "engine.h"


class HanoiGame {
public:
   HanoiGame(const std::shared_ptr<Eng::Node>& sceneRoot);
   void processInput(int poleIndex);

private:
   std::shared_ptr<Eng::Node> root;
   std::shared_ptr<Eng::Node> selectedDisk;
   std::shared_ptr<Eng::Node> sourcePole;

   // CACHE: Salviamo i pali qui per non cercarli sempre
   std::vector<std::shared_ptr<Eng::Node>> poles;

   // Helper interni
   std::shared_ptr<Eng::Node> getTopDisk(std::shared_ptr<Eng::Node> pole);
   bool isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove);
   float getMeshHeight(const std::shared_ptr<Eng::Node>& node);
   bool checkVictory(const std::shared_ptr<Eng::Node>& poleToMonitor) const;

   const int numberOfDisks_ = 2;
   std::string poleToMonitorName_ = "palo1";
};