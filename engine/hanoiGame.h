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
      // --- Membri dati (Stato del gioco) ---
      std::shared_ptr<Eng::Node> root;          // Riferimento alla scena
      std::shared_ptr<Eng::Node> selectedDisk;  // Il disco che hai "in mano" (nullptr se nessuno)
      std::shared_ptr<Eng::Node> sourcePole;    // Da quale palo hai preso il disco

      // --- Metodi Helper (Logica interna) ---

      // Trova il nodo del palo dato il numero (es. "Palo1")
      std::shared_ptr<Eng::Node> getPole(int number);

      // Trova il disco più in alto (il più piccolo) di un palo
      std::shared_ptr<Eng::Node> getTopDisk(std::shared_ptr<Eng::Node> pole);

      // Controlla se le regole di Hanoi permettono la mossa
      bool isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove);

      // Esegue lo spostamento effettivo nel grafo della scena
      void moveDisk(std::shared_ptr<Eng::Node> destPole);
   };