#pragma once
#include <vector>

#include "../utils/Vector2D.h"
#include "SDL.h"

class GameObject {
  bool active_ = false;
  bool destroyed_ = false;
  bool transparent_ = false;
  std::vector<GameObject*> children_{};

  Vector2D<int> position_{0, 0};
  Vector2D<int> size_{0, 0};

 public:
  bool getActive() const noexcept;
  void setActive(bool active) noexcept;

  bool getTransparent() const noexcept;
  void setTransparent(bool transparent) noexcept;

  bool getDestroy() const noexcept;
  void setDestroy(bool destroy) noexcept;

  SDL_Rect getRectangle() const noexcept;

  GameObject* clickScan(SDL_Point point) const noexcept;
  std::vector<GameObject*> getChildren() const noexcept;

  virtual void onAwake() noexcept;
  virtual void onUpdate() noexcept;
  virtual void onEvent(const SDL_Event& event) noexcept;
  virtual void onRender() noexcept;
  virtual void onDestroy() noexcept;
};
