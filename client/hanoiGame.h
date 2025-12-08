#pragma once
#include <memory>
#include <vector>
#include "engine.h"
#include "spotlight.h"


class HanoiGame {
public:
   HanoiGame(const std::shared_ptr<Eng::Node>& sceneRoot);
   void processInput(int poleIndex);
   [[nodiscard]] std::string getStatusMessage() const { return statusMessage_; }

private:
   std::shared_ptr<Eng::Node> root;
   std::shared_ptr<Eng::Node> selectedDisk;
   std::shared_ptr<Eng::Node> sourcePole;
   std::vector<std::shared_ptr<Eng::Spotlight>> poleLights;

   // CACHE: Salviamo i pali qui per non cercarli sempre
   std::vector<std::shared_ptr<Eng::Node>> poles;

   void initLights();
   void updateLightsColors(int selectedIndex);

   // Helper interni 
   std::shared_ptr<Eng::Node> getTopDisk(std::shared_ptr<Eng::Node> pole);
   bool isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove);
   float getMeshHeight(const std::shared_ptr<Eng::Node>& node);
   bool checkVictory(const std::shared_ptr<Eng::Node>& poleToMonitor);

   const int numberOfDisks_ = 2;
   std::string poleToMonitorName_ = "palo3";
   std::string statusMessage_ = R"(--- ISTRUZIONI DI GIOCO ---
1 - Seleziona da 1 a 3 il palo da cui vuoi prendere il disco
2 - Seleziona da 1 a 3 il palo a cui vuoi aggiungere il disco
(Per annullare la mossa richiama il tasto del palo selezionato)
)";
};