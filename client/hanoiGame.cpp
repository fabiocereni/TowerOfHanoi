#include "hanoiGame.h"

#include "mesh.h"
#include <limits>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>


void HanoiGame::undoMove() {
   if (selectedDisk != nullptr) {
      processInput(sourcePoleIndex);
      return;
   }

   if (undoStack_.empty()) {
      return;
   }

   MoveRecord lastMove = undoStack_.top();
   undoStack_.pop();

   int currentIdx = lastMove.toPoleIndex;
   int targetIdx = lastMove.fromPoleIndex;

   auto currentPole = poles[currentIdx];
   auto targetPole = poles[targetIdx];
   auto disk = getTopDisk(currentPole);

   if (disk) {
      currentPole->removeChild(disk->getName());
      targetPole->addChildren(disk);
      float h = 0.0f;
      for (auto& child : targetPole->getChildren()) {
         if (child != disk && std::dynamic_pointer_cast<Eng::Mesh>(child))
            h += getMeshHeight(child);
      }
      glm::mat4 m = disk->getMatrix();
      m[3][1] = h;
      disk->setMatrix(m);
      redoStack_.push(lastMove);
   }
}

void HanoiGame::redoMove() {
   if (redoStack_.empty()) {
      return;
   }

   MoveRecord move = redoStack_.top();
   redoStack_.pop();

   int fromIdx = move.fromPoleIndex;
   int toIdx = move.toPoleIndex;

   auto sourcePole = poles[fromIdx];
   auto destPole = poles[toIdx];
   auto disk = getTopDisk(sourcePole);

   if (disk) {
      sourcePole->removeChild(disk->getName());
      destPole->addChildren(disk);
      float h = 0.0f;
      for (auto& child : destPole->getChildren()) {
         if (child != disk && std::dynamic_pointer_cast<Eng::Mesh>(child))
            h += getMeshHeight(child);
      }
      glm::mat4 m = disk->getMatrix();
      m[3][1] = h;
      disk->setMatrix(m);
      undoStack_.push(move);
   }
}

void HanoiGame::resetGame() {
   std::cout << ">> RESET GAME: Riavvio partita..." << std::endl;

   selectedDisk = nullptr;
   sourcePole = nullptr;
   sourcePoleIndex = -1;

   while (!undoStack_.empty()) undoStack_.pop();
   while (!redoStack_.empty()) redoStack_.pop();
   std::vector<std::shared_ptr<Eng::Node>> allDisks;

   for (int i = 1; i <= 3; i++) {
      if (!poles[i]) continue;
      auto children = poles[i]->getChildren();
      for (auto& child : children) {
         if (std::dynamic_pointer_cast<Eng::Mesh>(child)) {
            allDisks.push_back(child);
            poles[i]->removeChild(child->getName());
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

   this->statusMessage_ = "Partita riavviata";
}

void debugPrintGraph(const std::shared_ptr<Eng::Node>& node, const int depth) {
   if (!node) return;

   std::string indent;
   for (int i = 0; i < depth; i++) indent += "  |-- ";

   std::cout << indent << node->getName() << std::endl;

   for (auto& child : node->getChildren()) {
      debugPrintGraph(child, depth + 1);
   }
}


bool HanoiGame::checkVictory(const std::shared_ptr<Eng::Node>& poleToMonitor) {
   if (poleToMonitor == nullptr) return false;

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


HanoiGame::HanoiGame(const std::shared_ptr<Eng::Node>& sceneRoot)
   : root(sceneRoot), selectedDisk(nullptr), sourcePole(nullptr)
{
   if (!root) {
      std::cerr << "!!! FATAL ERROR: sceneRoot e' NULL !!!" << std::endl;
      return;
   }

   std::cout << "\n=== STRUTTURA SCENA OVO ===" << std::endl;
   debugPrintGraph(root, 0);
   std::cout << "===========================\n" << std::endl;

   poles.resize(4, nullptr);

   // Usiamo la nostra ricerca ricorsiva
   poles[1] = Eng::Base::findByName(root, "palo1");
   poles[2] = Eng::Base::findByName(root, "palo2");
   poles[3] = Eng::Base::findByName(root, "palo3");


   if (poles[1]) std::cout << ">> Palo 1 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 1 non trovato (neanche ricorsivamente)." << std::endl;

   if (poles[2]) std::cout << ">> Palo 2 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 2 non trovato." << std::endl;

   if (poles[3]) std::cout << ">> Palo 3 TROVATO!" << std::endl;
   else std::cerr << ">> ERRORE: Palo 3 non trovato." << std::endl;

}

std::shared_ptr<Eng::Node> HanoiGame::getTopDisk(std::shared_ptr<Eng::Node> pole) {
   if (!pole) return nullptr;

   auto disks = pole->getChildren();

   disks.erase(std::remove(disks.begin(), disks.end(), nullptr), disks.end());

   if (disks.empty()) return nullptr;

   std::sort(disks.begin(), disks.end(), [](const auto& a, const auto& b) {
      return a->getName() > b->getName();
      });

   return disks.front();
}

bool HanoiGame::isValidMove(std::shared_ptr<Eng::Node> destPole, std::shared_ptr<Eng::Node> diskToMove) {
   auto topDiskDest = getTopDisk(destPole);

   if (!topDiskDest) return true;

   return diskToMove->getName() > topDiskDest->getName();
}


void HanoiGame::processInput(const int poleIndex) {
    this->statusMessage_ = "";
    if (poleIndex < 1 || poleIndex > 3) return;

    auto clickedPole = poles[poleIndex];
    if (!clickedPole) return;

  
    const float LIFT_HEIGHT = 500.0f;

   if (!checkVictory(Eng::Base::findByName(root, poleToMonitorName_)))
   {
      if (selectedDisk == nullptr) {
         auto disk = getTopDisk(clickedPole);
         if (disk) {
            selectedDisk = disk;
            sourcePole = clickedPole;

            sourcePoleIndex = poleIndex;

            glm::mat4 matrix = selectedDisk->getMatrix();
            matrix[3][1] += LIFT_HEIGHT;
            selectedDisk->setMatrix(matrix);

            std::cout << ">> PRESO: " << disk->getName() << " (Alzato)" << std::endl;
         }
         else {
            std::cout << ">> Palo vuoto." << std::endl;
         }
      }
      else {
         if (clickedPole == sourcePole) {
            std::cout << ">> Annullato." << std::endl;

            glm::mat4 matrix = selectedDisk->getMatrix();
            matrix[3][1] -= LIFT_HEIGHT;
            selectedDisk->setMatrix(matrix);

            selectedDisk = nullptr;
            sourcePole = nullptr;
            return;
         }

         if (isValidMove(clickedPole, selectedDisk)) {
            MoveRecord newMove;
            newMove.fromPoleIndex = sourcePoleIndex;
            newMove.toPoleIndex = poleIndex;

            undoStack_.push(newMove);
            while (!redoStack_.empty()) {
               redoStack_.pop();
            }

            std::cout << ">> SPOSTATO su Palo " << poleIndex << std::endl;

            sourcePole->removeChild(selectedDisk->getName());
            clickedPole->addChildren(selectedDisk);
            checkVictory(Eng::Base::findByName(root, poleToMonitorName_));

            float currentStackHeight = 0.0f;
            float baseOffset = 0.0f;

            auto& children = clickedPole->getChildren();
            for (size_t i = 0; i < children.size() - 1; i++) {
               currentStackHeight += getMeshHeight(children[i]);
            }

            float newY = baseOffset + currentStackHeight;

            glm::mat4 currentMatrix = selectedDisk->getMatrix();

            currentMatrix[3] = glm::vec4(0.0f, newY, 0.0f, 1.0f);
            selectedDisk->setMatrix(currentMatrix);

            selectedDisk = nullptr;
            sourcePole = nullptr;
            sourcePoleIndex = -1;
            checkVictory(Eng::Base::findByName(root, poleToMonitorName_));
         }
         else {
            std::cout << ">> MOSSA NON VALIDA" << std::endl;

         }
      }
   }
}

float HanoiGame::getMeshHeight(const std::shared_ptr<Eng::Node>& node) {
    auto mesh = std::dynamic_pointer_cast<Eng::Mesh>(node);
    if (!mesh) return 0.0f;

    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& v : mesh->getVertexes()) {
        if (v.y < minY) minY = v.y;
        if (v.y > maxY) maxY = v.y;
    }

    float height = maxY - minY;

    float scaleY = glm::length(glm::vec3(node->getMatrix()[1]));

    return height * scaleY;
}