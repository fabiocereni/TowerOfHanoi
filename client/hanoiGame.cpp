#include "hanoiGame.h"


#include "mesh.h"
#include <limits>
#include <iostream>
#include <algorithm>


// --- HELPER DI DEBUG (Per vedere la struttura della scena) ---
void debugPrintGraph(std::shared_ptr<Eng::Node> node, int depth) {
   if (!node) return;

   // Crea l'indentazione visiva
   std::string indent = "";
   for (int i = 0; i < depth; i++) indent += "  |-- ";

   std::cout << indent << node->getName() << std::endl;

   for (auto& child : node->getChildrens()) {
      debugPrintGraph(child, depth + 1);
   }
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
   for (auto& child : current->getChildrens()) {
      auto result = findRecursive(child, nameToFind);
      if (result != nullptr) {
         return result; // Trovato! Risaliamo la catena
      }
   }

   // 3. Non trovato qui sotto
   return nullptr;
}

// --- COSTRUTTORE ---
HanoiGame::HanoiGame(std::shared_ptr<Eng::Node> sceneRoot)
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
}

std::shared_ptr<Eng::Node> HanoiGame::getTopDisk(std::shared_ptr<Eng::Node> pole) {
   if (!pole) return nullptr;

   auto disks = pole->getChildrens();

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
   // Poich� "disco7" (piccolo) > "disco1" (grande) alfabeticamente:
   // Il disco che muovo deve avere un nome "MAGGIORE" di quello che sta sotto.
   // Esempio valido: Muovo "disco7" su "disco1" -> "disco7" > "disco1" (VERO)
   // Esempio errato: Muovo "disco1" su "disco7" -> "disco1" > "disco7" (FALSO)

   return diskToMove->getName() > topDiskDest->getName(); // <--- CAMBIA QUI DA < A >
}

// In Gruppo_03/engine/hanoiGame.cpp

void HanoiGame::processInput(int poleIndex) {
    if (poleIndex < 1 || poleIndex > 3) return;

    auto clickedPole = poles[poleIndex];
    if (!clickedPole) return;

    //DA CONTROLLARE
    // Parametro: quanto alzare il disco quando è selezionato
    const float LIFT_HEIGHT = 500.0f; // Regola questo valore in base alla scala della tua scena

    // STATO 1: PRENDI (PICK)
    if (selectedDisk == nullptr) {
        auto disk = getTopDisk(clickedPole);
        if (disk) {
            selectedDisk = disk;
            sourcePole = clickedPole;

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
            std::cout << ">> SPOSTATO su Palo " << poleIndex << std::endl;

            sourcePole->removeChildren(selectedDisk->getName());
            clickedPole->addChildren(selectedDisk);

            // --- CALCOLO NUOVA POSIZIONE (Codice precedente) ---
            // Nota: Qui non dobbiamo "abbassare" il disco sottraendo LIFT_HEIGHT,
            // perché stiamo ricalcolando la Y da zero basandoci sui dischi sotto.

            float currentStackHeight = 0.0f;
            float baseOffset = 0.0f;

            auto& children = clickedPole->getChildrens();
            for (size_t i = 0; i < children.size() - 1; i++) {
                currentStackHeight += getMeshHeight(children[i]);
            }

            float newY = baseOffset + currentStackHeight;

            glm::mat4 currentMatrix = selectedDisk->getMatrix();
            currentMatrix[3] = glm::vec4(0.0f, newY, 0.0f, 1.0f); // Sovrascrive la Y "alzata" con quella finale
            selectedDisk->setMatrix(currentMatrix);
            // ---------------------------------------------------

            selectedDisk = nullptr;
            sourcePole = nullptr;

            // Debug print...
        }
        else {
            std::cout << ">> MOSSA NON VALIDA" << std::endl;
            // Opzionale: Se la mossa non è valida, il disco rimane alzato e selezionato
            // così l'utente può provare un altro palo.
        }
    }
}
//DA CONTROLLARE SE DEVE STARE QUI
// Calcola l'altezza fisica (Y max - Y min) di un nodo Mesh
float HanoiGame::getMeshHeight(std::shared_ptr<Eng::Node> node) {
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