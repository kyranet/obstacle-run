// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <json/json.h>

#include <vector>

#include "SDL.h"
#include "utils/Vector2D.h"

class Component;

class GameObject final {
  bool active_ = false;
  bool destroyed_ = false;
  bool transparent_ = false;
  std::string name_{};
  GameObject* parent_ = nullptr;
  std::vector<GameObject*> children_{};
  std::vector<Component*> components_{};

  Vector2D<int> position_{0, 0};
  Vector2D<int> size_{0, 0};

 public:
  GameObject() noexcept;
  explicit GameObject(GameObject* parent) noexcept;
  ~GameObject() noexcept;

  [[nodiscard]] std::string getName() const noexcept;
  void setName(std::string name) noexcept;

  [[nodiscard]] GameObject* getParent() const noexcept;

  [[nodiscard]] bool getActive() const noexcept;
  void setActive(bool active) noexcept;

  [[nodiscard]] bool getTransparent() const noexcept;
  void setTransparent(bool transparent) noexcept;

  [[nodiscard]] bool getDestroy() const noexcept;
  void setDestroy(bool destroy) noexcept;

  [[nodiscard]] SDL_Rect getRectangle() const noexcept;

  [[nodiscard]] GameObject* clickScan(SDL_Point point) const noexcept;
  [[nodiscard]] std::vector<GameObject*> getChildren() const noexcept;
  [[nodiscard]] std::vector<Component*> getComponents() const noexcept;

  void load(const Json::Value& value);
  void onAwake() noexcept;
  void onUpdate() const noexcept;
  void onRender() noexcept;
  void onDestroy() noexcept;
};
