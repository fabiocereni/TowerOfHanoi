#pragma once

   // Includiamo le librerie standard necessarie
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

   // Includiamo l'engine perché dobbiamo conoscere Eng::Node
#include "engine.h" 

class HanoiGame {
public:
      // Costruttore: richiede la radice della scena per trovare i pali
   HanoiGame(std::shared_ptr<Eng::Node> sceneRoot);

      // Il metodo principale da chiamare nello switch della tastiera
   void processInput(int poleIndex);

private:

   // CACHE: Salviamo i pali qui per non cercarli sempre
   std::vector<std::shared_ptr<Eng::Node>> poles;

   // Helper interni
   std::shared_ptr<Eng::Node> getTopDisk(std::shared_ptr<Eng::Node> pole);

      // Controlla se le regole di Hanoi permettono la mossa
   bool isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove);

      // Esegue lo spostamento effettivo nel grafo della scena
      void moveDisk(std::shared_ptr<Eng::Node> destPole);
};