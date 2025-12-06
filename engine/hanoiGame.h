#pragma once
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "engine.h" 

class HanoiGame {
public:
   HanoiGame(std::shared_ptr<Eng::Node> sceneRoot);
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
};