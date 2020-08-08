// Copyright (c) 2020 Antonio Román. All rights reserved.

#include "managers/Input.h"

#include "managers/SceneManager.h"
#include "objects/GameObject.h"
#include "scenes/Scene.h"

Input::Input() : mousePosition_(0, 0) {}
Input::~Input() = default;

Input* Input::instance_ = nullptr;

Input* Input::instance() {
  if (instance_ == nullptr) {
    instance_ = new Input();
  }
  return instance_;
}

void Input::clear() noexcept {
  for (uint16_t i = 0, max = static_cast<uint16_t>(KeyboardKey::kKeyLimit);
       i < max; i++) {
    keyDown_[i] = false;
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
      keyDown_[event.key.keysym.scancode] = true;
      break;
    }
    case SDL_KEYUP: {
      keyboard_ = SDL_GetKeyboardState(nullptr);
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

Vector2D<int32_t> Input::getMousePosition() noexcept {
  return instance()->mousePosition_;
}

SDL_Point Input::getRawMousePosition() noexcept {
  const auto& position = getMousePosition();
  return {position.x(), position.y()};
}

bool Input::isKeyPressed(KeyboardKey key) noexcept {
  const auto input = instance();
  if (!input->keyboard_) return false;
  return input->keyboard_[static_cast<size_t>(key)];
}

bool Input::isKeyDown(KeyboardKey key) noexcept {
  return instance()->keyDown_[static_cast<size_t>(key)];
}
bool Input::isKeyUp(KeyboardKey key) noexcept {
  return instance()->keyUp_[static_cast<size_t>(key)];
}

bool Input::isMouseButtonDown(MouseKey button) noexcept {
  return instance()->mouseDown_[static_cast<size_t>(button)];
}
bool Input::isMouseButtonUp(MouseKey button) noexcept {
  return instance()->mouseUp_[static_cast<size_t>(button)];
}

bool Input::getShift() noexcept {
  const auto input = instance();
  return input->isKeyPressed(KeyboardKey::LEFT_SHIFT) ||
         input->isKeyPressed(KeyboardKey::RIGHT_SHIFT);
}

bool Input::getCtrl() noexcept {
  const auto input = instance();
  return input->isKeyPressed(KeyboardKey::LEFT_CTRL) ||
         input->isKeyPressed(KeyboardKey::RIGHT_CTRL);
}

bool Input::getAlt() noexcept {
  const auto input = instance();
  return input->isKeyPressed(KeyboardKey::LEFT_ALT) ||
         input->isKeyPressed(KeyboardKey::RIGHT_ALT);
}

GameObject* Input::screenMouseToRay() noexcept {
  const auto input = instance();
  if (input->casted_) return input->casted_;
  const auto position = input->mousePosition_;
  const auto point = SDL_Point{position.x(), position.y()};
  const auto scene = SceneManager::getActiveScene();
  const auto& gameObjects = scene->gameObjects();
  for (auto it = gameObjects.rbegin(); it != gameObjects.rend(); ++it) {
    const auto gameObject = *it;
    const auto scanned = gameObject->clickScan(point);
    if (scanned) {
      input->casted_ = scanned;
      return scanned;
    }
  }
  return nullptr;
}

bool Input::isMouseInside(const SDL_Rect* rectangle) noexcept {
  const auto input = instance();
  const auto position = input->mousePosition_;
  const auto point = SDL_Point{position.x(), position.y()};
  return SDL_PointInRect(&point, rectangle);
}

void Input::destroy() {
  if (instance_ != nullptr) {
    delete instance_;
    instance_ = nullptr;
  }
}
