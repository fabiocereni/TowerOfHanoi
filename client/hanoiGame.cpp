#include "hanoiGame.h"

#include "mesh.h"
#include <limits>
#include <iostream>
#include <algorithm>

// hanoiGame.cpp

void HanoiGame::undoMove() {
   // 1. Se ho un disco in mano, 'x' lo rimette giù (annulla selezione)
   if (selectedDisk != nullptr) {
      processInput(sourcePoleIndex);
      return;
   }

   // 2. Se non c'è una mossa valida da annullare
   if (!hasLastMove_) {
      std::cout << ">> Nessuna mossa da annullare!" << std::endl;
      return;
   }

   // 3. Esegui l'Undo
   int currentIdx = lastMove_.toPoleIndex;
   int targetIdx = lastMove_.fromPoleIndex;

   auto currentPole = poles[currentIdx];
   auto targetPole = poles[targetIdx];
   auto disk = getTopDisk(currentPole);

   if (disk) {
      std::cout << ">> UNDO: Torna indietro da " << currentIdx << " a " << targetIdx << std::endl;

      currentPole->removeChildren(disk->getName());
      targetPole->addChildren(disk);

      // Ricalcola altezza (Y) sul vecchio palo
      float h = 0.0f;
      for (auto& child : targetPole->getChildren()) {
         if (child != disk && std::dynamic_pointer_cast<Eng::Mesh>(child))
            h += getMeshHeight(child);
      }

      glm::mat4 m = disk->getMatrix();
      m[3][1] = h; // Imposta altezza corretta
      disk->setMatrix(m);
   }

   // 4. DIMENTICA LA MOSSA (Così non puoi rifarlo due volte)
   hasLastMove_ = false;

   updateLightsColors(-1); // Spegni le luci
}

void HanoiGame::initLights() {
   poleLights.resize(4, nullptr);

   for (int i = 1; i <= 3; i++)
   {
      auto pole = poles[i];
      if (!pole) continue;

      auto light = Eng::Base::getInstance().createSpotlight();

      light->setCutoff(10.0f);
      light->setExponent(100.0f);

      glm::mat4 m = glm::mat4(1.0f);
      m = glm::translate(m, glm::vec3(0.0f, 1000.0f, -700.0f));
      m = glm::rotate(m, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      light->setMatrix(m);

      light->setDiffuse(glm::vec3(0.0f));
      light->setSpecular(glm::vec3(0.0f));
      light->setAmbient(glm::vec3(0.0f));

      pole->addChildren(light);
      poleLights[i] = light;
   }
}

void HanoiGame::updateLightsColors(int selectedIndex) {
   glm::vec3 red = glm::vec3(100.0f, 0.0f, 0.0f);
   glm::vec3 blue = glm::vec3(0.0f, 50.0f, 100.0f);
   glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);

   for (int i = 1; i <= 3; i++) {
      auto light = poleLights[i];
      if (!light) continue;

      if (selectedIndex == -1) {
         // Tutto spento
         light->setDiffuse(black);
         light->setSpecular(black);
         light->setAmbient(black);
      }
      else if (i == selectedIndex) {
         // Rosso
         light->setDiffuse(red);
         light->setSpecular(red);
         light->setAmbient(glm::vec3(0.9f, 0.0f, 0.0f));
      }
      else {
         // Blu
         light->setDiffuse(blue);
         light->setSpecular(blue);
         light->setAmbient(glm::vec3(0.0f, 0.45f, 0.9f));
      }
   }
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



   // // modifica per test vittoria
   // std::vector<std::shared_ptr<Eng::Node>> tmp;
   // tmp.push_back(poles[1]->getChildren().at(poles[1]->getChildren().size() - 1));
   // tmp.push_back(poles[1]->getChildren().at(poles[1]->getChildren().size() - 2));
   //
   // std::vector<std::shared_ptr<Eng::Node>> tmpChildren;
   // poles[1]->setChildren(tmpChildren);
   // poles[1]->addChildren(tmp.at(0));
   // poles[1]->addChildren(tmp.at(1));


   // Verifica
   if (poles[1]) std::cout << ">> Palo 1 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 1 non trovato (neanche ricorsivamente)." << std::endl;

   if (poles[2]) std::cout << ">> Palo 2 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 2 non trovato." << std::endl;

   if (poles[3]) std::cout << ">> Palo 3 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 3 non trovato." << std::endl;

       initLights();
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
            updateLightsColors(poleIndex);
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
            // spengo la luce
            updateLightsColors(-1);
            return;
         }

         // Se clicco su un altro palo -> PROVA A SPOSTARE
         if (isValidMove(clickedPole, selectedDisk)) {
            lastMove_.fromPoleIndex = sourcePoleIndex;
            lastMove_.toPoleIndex = poleIndex;
            hasLastMove_ = true;

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
            updateLightsColors(-1);
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