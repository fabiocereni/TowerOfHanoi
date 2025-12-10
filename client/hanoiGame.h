#pragma once
#include <memory>
#include <vector>
#include "engine.h"
#include "spotlight.h"

#include <stack>

struct MoveRecord {
   int fromPoleIndex;
   int toPoleIndex;
};

class HanoiGame {
public:
   HanoiGame(const std::shared_ptr<Eng::Node>& sceneRoot);

   void processInput(int poleIndex);
   [[nodiscard]] std::string getStatusMessage() const { return statusMessage_; }

   void undoMove();
   void redoMove();
   void resetGame();

   void initLampadario();

private:
   std::shared_ptr<Eng::Node> root;
   std::shared_ptr<Eng::Node> selectedDisk;
   std::shared_ptr<Eng::Node> sourcePole;

   MoveRecord lastMove_;
   bool hasLastMove_ = false; // ci dice se esiste una mossa da annullare
   int sourcePoleIndex = -1;
   bool isUndoPerformed_ = false; // True se ho appena fatto un Undo e posso fare Redo


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

   std::shared_ptr<Eng::OmnidirectionalLight> lampadarioLight;


   const int numberOfDisks_ = 7;
   std::string poleToMonitorName_ = "palo3";
   std::string statusMessage_ = R"(--- ISTRUZIONI DI GIOCO ---
1 - Seleziona da 1 a 3 il palo da cui vuoi prendere il disco
2 - Seleziona da 1 a 3 il palo a cui vuoi aggiungere il disco
3 - Seleziona da 1 a 3 il palo a cui vuoi aggiungere il disco
(Per annullare la mossa richiama il tasto del palo selezionato)
(x) -> undo
(y) -> redo
(r) -> reset game
(fn + f1) -> cam principale
(fn + f2) -> cam secondaria
(fn + f3) -> cam mobile
(wasd) -> per muoversi
(u) -> alza visuale
(g) -> abbassa visuale
(freccia su) -> ti alzi
(freccia giu) -> ti abbassi 
)";
};