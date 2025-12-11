#include "hanoiGame.h"

#include "mesh.h"
#include <limits>
#include <iostream>
#include <algorithm>

#include "omnidirectionallight.h"

// hanoiGame.cpp

// --- HELPER DI RICERCA RICORSIVA SICURA ---
// Cerca un nodo per nome in tutto l'albero, non solo nel primo livello
std::shared_ptr<Eng::Node> findRecursive(std::shared_ptr<Eng::Node> current, const std::string& nameToFind) {
   if (!current) return nullptr;

   // 1. Controlla se � questo
   if (current->getName() == nameToFind) {
      return current;
   }

   // 2. Cerca nei figli
   for (auto& child : current->getChildren()) {
      auto result = findRecursive(child, nameToFind);
      if (result != nullptr) {
         return result; // Trovato! Risaliamo la catena
      }
   }

   // 3. Non trovato qui sotto
   return nullptr;
}



void HanoiGame::undoMove() {
   if (selectedDisk != nullptr) {
      processInput(sourcePoleIndex);
      return;
   }

   // Se non c'è mossa o l'ho già annullata
   if (!hasLastMove_ || isUndoPerformed_) {
      std::cout << ">> Non puoi annullare oltre!" << std::endl;
      return;
   }

   // Esegui UNDO: da TO a FROM
   int currentIdx = lastMove_.toPoleIndex;
   int targetIdx = lastMove_.fromPoleIndex;

   auto currentPole = poles[currentIdx];
   auto targetPole = poles[targetIdx];
   auto disk = getTopDisk(currentPole);

   if (disk) {
      std::cout << ">> UNDO: Torna indietro..." << std::endl;
      currentPole->removeChildren(disk->getName());
      targetPole->addChildren(disk);

      float h = 0.0f;
      for (auto& child : targetPole->getChildren()) {
         if (child != disk && std::dynamic_pointer_cast<Eng::Mesh>(child))
            h += getMeshHeight(child);
      }
      glm::mat4 m = disk->getMatrix();
      m[3][1] = h;
      disk->setMatrix(m);
   }

   isUndoPerformed_ = true;
}

void HanoiGame::redoMove() {
   // Posso fare Redo solo se ho appena fatto un Undo
   if (!hasLastMove_ || !isUndoPerformed_) {
      std::cout << ">> Nulla da ripristinare (Redo)!" << std::endl;
      return;
   }

   int fromIdx = lastMove_.fromPoleIndex;
   int toIdx = lastMove_.toPoleIndex;

   auto sourcePole = poles[fromIdx];
   auto destPole = poles[toIdx];
   auto disk = getTopDisk(sourcePole);

   if (disk) {
      std::cout << ">> REDO: Ripristino mossa..." << std::endl;
      sourcePole->removeChildren(disk->getName());
      destPole->addChildren(disk);

      float h = 0.0f;
      for (auto& child : destPole->getChildren()) {
         if (child != disk && std::dynamic_pointer_cast<Eng::Mesh>(child))
            h += getMeshHeight(child);
      }
      glm::mat4 m = disk->getMatrix();
      m[3][1] = h;
      disk->setMatrix(m);
   }

   isUndoPerformed_ = false;
}

void HanoiGame::resetGame() {
   std::cout << ">> RESET GAME: Riavvio partita..." << std::endl;

   selectedDisk = nullptr;
   sourcePole = nullptr;
   sourcePoleIndex = -1;
   hasLastMove_ = false;
   isUndoPerformed_ = false;

   std::vector<std::shared_ptr<Eng::Node>> allDisks;

   for (int i = 1; i <= 3; i++) {
      if (!poles[i]) continue;

      auto children = poles[i]->getChildren();
      for (auto& child : children) {
         if (std::dynamic_pointer_cast<Eng::Mesh>(child)) {
            allDisks.push_back(child);
            poles[i]->removeChildren(child->getName());
         }
      }
   }

   std::sort(allDisks.begin(), allDisks.end(), [](const auto& a, const auto& b) {
      return a->getName() < b->getName();
      });

   float currentHeight = 0.0f;
   auto pole1 = poles[1];

   for (auto& disk : allDisks) {
      pole1->addChildren(disk);

      glm::mat4 m = disk->getMatrix();
      m[3] = glm::vec4(0.0f, currentHeight, 0.0f, 1.0f);
      disk->setMatrix(m);

      currentHeight += getMeshHeight(disk);
   }

   this->statusMessage_ = "Partita Resettata";
}

// --- HELPER DI DEBUG (Per vedere la struttura della scena) ---
void debugPrintGraph(const std::shared_ptr<Eng::Node>& node, const int depth) {
   if (!node) return;

   // Crea l'indentazione visiva
   std::string indent;
   for (int i = 0; i < depth; i++) indent += "  |-- ";

   std::cout << indent << node->getName() << std::endl;

   for (auto& child : node->getChildren()) {
      debugPrintGraph(child, depth + 1);
   }
}


bool HanoiGame::checkVictory(const std::shared_ptr<Eng::Node>& poleToMonitor) {
   if (poleToMonitor == nullptr) return false;

   // conto i dischi in questo modo altrimenti mi conterebbe anche le luci
   int diskCount = 0;
   for (const std::shared_ptr<Eng::Node>& child : poleToMonitor->getChildren())
   {
      if (std::dynamic_pointer_cast<Eng::Mesh>(child)) {
         diskCount++;
      }
   }

   if (diskCount == numberOfDisks_)
   {
      this->statusMessage_ = "HAI VINTO!!!";
      return true;
   }
   return false;
}


// --- COSTRUTTORE ---
HanoiGame::HanoiGame(const std::shared_ptr<Eng::Node>& sceneRoot)
   : root(sceneRoot), selectedDisk(nullptr), sourcePole(nullptr)
{
   if (!root) {
      std::cerr << "!!! FATAL ERROR: sceneRoot e' NULL !!!" << std::endl;
      return;
   }

   std::cout << "\n=== STRUTTURA SCENA OVO ===" << std::endl;
   debugPrintGraph(root, 0); // <--- GUARDA QUI NELLA CONSOLE!
   std::cout << "===========================\n" << std::endl;

   // Inizializziamo il vettore
   poles.resize(4, nullptr);

   // Usiamo la nostra ricerca ricorsiva
   poles[1] = findRecursive(root, "palo1");
   poles[2] = findRecursive(root, "palo2");
   poles[3] = findRecursive(root, "palo3");


   // Verifica
   if (poles[1]) std::cout << ">> Palo 1 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 1 non trovato (neanche ricorsivamente)." << std::endl;

   if (poles[2]) std::cout << ">> Palo 2 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 2 non trovato." << std::endl;

   if (poles[3]) std::cout << ">> Palo 3 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 3 non trovato." << std::endl;

        // initLampadario();
}

std::shared_ptr<Eng::Node> HanoiGame::getTopDisk(std::shared_ptr<Eng::Node> pole) {
   if (!pole) return nullptr;

   auto disks = pole->getChildren();

   // Rimuovi ptr nulli
   disks.erase(std::remove(disks.begin(), disks.end(), nullptr), disks.end());

   if (disks.empty()) return nullptr;

   // Ordina (Modifica qui se l'ordine alfabetico non � corretto per la grandezza)
   std::sort(disks.begin(), disks.end(), [](const auto& a, const auto& b) {
      // Logica: il nome "maggiore" (es. Disk3) sta sopra a "Disk1"?
      // Oppure Z position?
      return a->getName() > b->getName();
      });

   return disks.front();
}

bool HanoiGame::isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove) {
   auto topDiskDest = getTopDisk(destPole);

   // Se il palo � vuoto, � sempre valido
   if (!topDiskDest) return true;

   // LOGICA CORRETTA:
   // Poiche' "disco7" (piccolo) > "disco1" (grande) alfabeticamente:
   // Il disco che muovo deve avere un nome "MAGGIORE" di quello che sta sotto.
   // Esempio valido: Muovo "disco7" su "disco1" -> "disco7" > "disco1" (VERO)
   // Esempio errato: Muovo "disco1" su "disco7" -> "disco1" > "disco7" (FALSO)

   return diskToMove->getName() > topDiskDest->getName(); // <--- CAMBIA QUI DA < A >
}

// In Gruppo_03/engine/hanoiGame.cpp

void HanoiGame::processInput(const int poleIndex) {
    if (poleIndex < 1 || poleIndex > 3) return;

    auto clickedPole = poles[poleIndex];
    if (!clickedPole) return;

    //DA CONTROLLARE
    // Parametro: quanto alzare il disco quando è selezionato
    const float LIFT_HEIGHT = 500.0f; // Regola questo valore in base alla scala della tua scena

   if (!checkVictory(findRecursive(root, poleToMonitorName_)))
   {
      // STATO 1: PRENDI (PICK)
      if (selectedDisk == nullptr) {
         auto disk = getTopDisk(clickedPole);
         if (disk) {
            selectedDisk = disk;
            sourcePole = clickedPole;

            sourcePoleIndex = poleIndex; // salvo da dove ho preso il disco per l'undo


            // --- NUOVO: ALZA IL DISCO ---
            glm::mat4 matrix = selectedDisk->getMatrix();
            matrix[3][1] += LIFT_HEIGHT; // Aggiunge altezza alla Y (colonna 3, riga 1)
            selectedDisk->setMatrix(matrix);
            // ----------------------------

            std::cout << ">> PRESO: " << disk->getName() << " (Alzato)" << std::endl;
         }
         else {
            std::cout << ">> Palo vuoto." << std::endl;
         }
      }
      // STATO 2: POSA (DROP)
      else {
         // Se clicco sullo stesso palo -> ANNULLA
         if (clickedPole == sourcePole) {
            std::cout << ">> Annullato." << std::endl;

            // --- NUOVO: ABBASSA IL DISCO (Torna alla posizione originale) ---
            glm::mat4 matrix = selectedDisk->getMatrix();
            matrix[3][1] -= LIFT_HEIGHT; // Rimuove l'altezza aggiunta prima
            selectedDisk->setMatrix(matrix);
            // ----------------------------------------------------------------

            selectedDisk = nullptr;
            sourcePole = nullptr;
            return;
         }

         // Se clicco su un altro palo -> PROVA A SPOSTARE
         if (isValidMove(clickedPole, selectedDisk)) {
            lastMove_.fromPoleIndex = sourcePoleIndex;
            lastMove_.toPoleIndex = poleIndex;
            hasLastMove_ = true;
            //Se faccio una nuova mossa, non posso più fare Redo della vecchia << <
            isUndoPerformed_ = false;

            std::cout << ">> SPOSTATO su Palo " << poleIndex << std::endl;

            sourcePole->removeChildren(selectedDisk->getName());
            clickedPole->addChildren(selectedDisk);
            checkVictory(findRecursive(root, poleToMonitorName_));

            // --- CALCOLO NUOVA POSIZIONE (Codice precedente) ---
            // Nota: Qui non dobbiamo "abbassare" il disco sottraendo LIFT_HEIGHT,
            // perché stiamo ricalcolando la Y da zero basandoci sui dischi sotto.

            float currentStackHeight = 0.0f;
            float baseOffset = 0.0f;

            auto& children = clickedPole->getChildren();
            for (size_t i = 0; i < children.size() - 1; i++) {
               currentStackHeight += getMeshHeight(children[i]);
            }

            float newY = baseOffset + currentStackHeight;

            glm::mat4 currentMatrix = selectedDisk->getMatrix();

            // TODO: da controllare, mi sa che bisogna applicare una traslazione e non modificare la matrice
            currentMatrix[3] = glm::vec4(0.0f, newY, 0.0f, 1.0f); // Sovrascrive la Y "alzata" con quella finale
            selectedDisk->setMatrix(currentMatrix);
            // ---------------------------------------------------

            selectedDisk = nullptr;
            sourcePole = nullptr;
            sourcePoleIndex = -1;
            checkVictory(findRecursive(root, poleToMonitorName_));
            // Debug print...
         }
         else {
            std::cout << ">> MOSSA NON VALIDA" << std::endl;
            // Opzionale: Se la mossa non è valida, il disco rimane alzato e selezionato
            // così l'utente può provare un altro palo.
         }
      }
   }
}


//DA CONTROLLARE SE DEVE STARE QUI
// Calcola l'altezza fisica (Y max - Y min) di un nodo Mesh
float HanoiGame::getMeshHeight(const std::shared_ptr<Eng::Node>& node) {
    auto mesh = std::dynamic_pointer_cast<Eng::Mesh>(node);
    if (!mesh) return 0.0f; // Se non è una mesh (es. luce), altezza 0

    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    // Scansiona tutti i vertici per trovare l'estensione su Y
    for (const auto& v : mesh->getVertexes()) {
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
    }

    // Calcoliamo lo spessore base
    float height = maxY - minY;

    // Moltiplichiamo per la scala Y della matrice locale (se l'oggetto è scalato)
    // La lunghezza della seconda colonna della matrice (index 1) è la scala Y
    float scaleY = glm::length(glm::vec3(node->getMatrix()[1]));

    return height * scaleY;
}