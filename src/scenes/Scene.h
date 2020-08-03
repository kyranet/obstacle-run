#pragma once
#include <vector>

#include "../objects/GameObject.h"

class Scene {
 private:
  std::vector<GameObject*> newGameObjects_{};
  std::vector<GameObject*> gameObjects_{};
  bool stop_ = false;

 protected:
  Scene() noexcept;
  ~Scene() noexcept;
  void end() noexcept;
  void stop() noexcept;
  void onStart();
  void onCreate();
  void onEvents() noexcept;
  void onUpdate();
  void onRender();
  void onEnd();

 public:
  std::vector<GameObject*> getGameObjects() const noexcept;
};
