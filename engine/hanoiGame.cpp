#include "hanoigame.h"

// --- COSTRUTTORE ---
HanoiGame::HanoiGame(std::shared_ptr<Eng::Node> sceneRoot)
   : root(sceneRoot), selectedDisk(nullptr), sourcePole(nullptr)
{
}

// --- METODI PRIVATI (HELPER) ---

std::shared_ptr<Eng::Node> HanoiGame::getPole(int number) {
   // Cerca "Palo1", "Palo2", ecc.
   return root->findByName(root, "Palo" + std::to_string(number));
}

std::shared_ptr<Eng::Node> HanoiGame::getTopDisk(std::shared_ptr<Eng::Node> pole) {
   auto disks = pole->getChildrens();
   if (disks.empty()) return nullptr;

   // Ordiniamo per trovare quello in cima (assumendo logica alfabetica o di inserimento)
   // Qui usiamo la logica: il nome "maggiore" sta in cima? O viceversa?
   // Adatta questo sort in base a come si chiamano i tuoi dischi nel .ovo
   std::sort(disks.begin(), disks.end(), [](const auto& a, const auto& b) {
      return a->getName() > b->getName();
      });

   return disks.front();
}

bool HanoiGame::isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove) {
   auto topDiskDest = getTopDisk(destPole);

   // Se il palo è vuoto, è sempre valido
   if (!topDiskDest) return true;

   // Se c'è un disco, il nostro deve essere più piccolo
   return diskToMove->getName() < topDiskDest->getName();
}

// --- METODO PUBBLICO ---

void HanoiGame::processInput(int poleIndex) {
   auto clickedPole = getPole(poleIndex);

   if (!clickedPole) {
      std::cout << "ERRORE: Palo " << poleIndex << " non trovato!" << std::endl;
      return;
   }

   // STATO 1: PRENDI (PICK)
   if (selectedDisk == nullptr) {
      auto disk = getTopDisk(clickedPole);
      if (disk) {
         selectedDisk = disk;
         sourcePole = clickedPole;
         std::cout << ">> Preso disco: " << disk->getName() << " dal Palo " << poleIndex << std::endl;
      }
      else {
         std::cout << ">> Il Palo " << poleIndex << " e' vuoto." << std::endl;
      }
   }
   // STATO 2: POSA (DROP)
   else {
      // Se clicco lo stesso palo, annullo
      if (clickedPole == sourcePole) {
         std::cout << ">> Selezione annullata." << std::endl;
         selectedDisk = nullptr;
         sourcePole = nullptr;
         return;
      }

      if (isValidMove(clickedPole, selectedDisk)) {
         std::cout << ">> Sposto " << selectedDisk->getName() << " su Palo " << poleIndex << std::endl;

         // Logica del grafo: stacco dal vecchio padre, attacco al nuovo
         sourcePole->removeChildren(selectedDisk->getName());
         clickedPole->addChildren(selectedDisk);

         // Reset
         selectedDisk = nullptr;
         sourcePole = nullptr;
      }
      else {
         std::cout << ">> MOSSA NON VALIDA! Disco troppo grande." << std::endl;
      }
   }
}