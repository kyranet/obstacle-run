// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

#include <vector>

#include "utils/Vector2D.h"
#include "SDL.h"

class Component;

class GameObject final {
  bool active_ = false;
  bool destroyed_ = false;
  bool transparent_ = false;
  std::string name_{};
  std::vector<GameObject*> children_{};
  std::vector<Component*> components_{};

  Vector2D<int> position_{0, 0};
  Vector2D<int> size_{0, 0};

 public:
  GameObject() noexcept;
  ~GameObject() noexcept;

  std::string getName() const noexcept;
  void setName(std::string name) noexcept;

  bool getActive() const noexcept;
  void setActive(bool active) noexcept;

  bool getTransparent() const noexcept;
  void setTransparent(bool transparent) noexcept;

  bool getDestroy() const noexcept;
  void setDestroy(bool destroy) noexcept;

  SDL_Rect getRectangle() const noexcept;

  GameObject* clickScan(SDL_Point point) const noexcept;
  std::vector<GameObject*> getChildren() const noexcept;
  std::vector<Component*> getComponents() const noexcept;

  void load(const Json::Value& value);
  void onAwake() noexcept;
  void onUpdate() noexcept;
  void onRender() noexcept;
  void onDestroy() noexcept;
};
