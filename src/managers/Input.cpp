// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "managers/Input.h"

#include "managers/SceneManager.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

const uint8_t* Input::keyboard_{nullptr};
uint32_t Input::mouse_{0};
Vector2<int32_t> Input::mousePosition_{0, 0};
std::array<bool, static_cast<size_t>(KeyboardKey::kKeyLimit)> Input::keyDown_{};
std::array<bool, static_cast<size_t>(KeyboardKey::kKeyLimit)> Input::keyUp_{};
std::array<bool, static_cast<size_t>(MouseKey::kKeyLimit)> Input::mouseDown_{};
std::array<bool, static_cast<size_t>(MouseKey::kKeyLimit)> Input::mouseUp_{};
GameObject* Input::casted_{nullptr};

void Input::clear() noexcept {
  for (uint16_t i = 0, max = static_cast<uint16_t>(KeyboardKey::kKeyLimit);
       i < max; i++) {
    keyUp_[i] = false;
  }

  for (uint16_t i = 0, max = static_cast<uint16_t>(MouseKey::kKeyLimit);
       i < max; i++) {
    mouseDown_[i] = false;
    mouseUp_[i] = false;
  }

  // Though in most cases this is useless, objects may create, destroy, or move,
  // when a tick happens.
  casted_ = nullptr;
}

void Input::update(const SDL_Event& event) noexcept {
  switch (event.type) {
    case SDL_KEYDOWN: {
      keyboard_ = SDL_GetKeyboardState(nullptr);
      keyUp_[event.key.keysym.scancode] = false;
      keyDown_[event.key.keysym.scancode] = true;
      break;
    }
    case SDL_KEYUP: {
      keyboard_ = SDL_GetKeyboardState(nullptr);
      keyDown_[event.key.keysym.scancode] = false;
      keyUp_[event.key.keysym.scancode] = true;
      break;
    }
    case SDL_MOUSEMOTION: {
      mousePosition_.set({event.motion.x, event.motion.y});
      casted_ = nullptr;
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      mouse_ = SDL_GetMouseState(nullptr, nullptr);
      switch (event.button.button) {
        case SDL_BUTTON_LEFT:
          mouseDown_[static_cast<size_t>(MouseKey::LEFT)] = true;
          break;
        case SDL_BUTTON_MIDDLE:
          mouseDown_[static_cast<size_t>(MouseKey::MIDDLE)] = true;
          break;
        case SDL_BUTTON_RIGHT:
          mouseDown_[static_cast<size_t>(MouseKey::RIGHT)] = true;
          break;
        default:
          break;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      mouse_ = SDL_GetMouseState(nullptr, nullptr);
      switch (event.button.button) {
        case SDL_BUTTON_LEFT:
          mouseUp_[static_cast<size_t>(MouseKey::LEFT)] = true;
          break;
        case SDL_BUTTON_MIDDLE:
          mouseUp_[static_cast<size_t>(MouseKey::MIDDLE)] = true;
          break;
        case SDL_BUTTON_RIGHT:
          mouseUp_[static_cast<size_t>(MouseKey::RIGHT)] = true;
          break;
        default:
          break;
      }
      break;
    }
    default:
      break;
  }
}

SDL_Point Input::rawMousePosition() noexcept {
  const auto& position = mousePosition();
  return {position.x(), position.y()};
}

GameObject* Input::screenMouseToRay() noexcept {
  if (casted_) return casted_;
  const auto& scene = SceneManager::getActiveScene();
  const auto& gameObjects = scene->gameObjects();
  const auto& position = mousePosition_;
  const auto point = SDL_Point{position.x(), position.y()};
  for (auto it = gameObjects.rbegin(); it != gameObjects.rend(); ++it) {
    const auto& gameObject = *it;
    const auto& scanned = gameObject->clickScan(point);
    if (scanned) {
      casted_ = scanned;
      return scanned;
    }
  }
  return nullptr;
}

bool Input::isMouseInside(const SDL_Rect* rectangle) noexcept {
  const auto& position = mousePosition_;
  const auto point = SDL_Point{position.x(), position.y()};
  return SDL_PointInRect(&point, rectangle);
}
