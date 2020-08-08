// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once
#include <SDL_events.h>

#include <array>
#include <cstdint>

#include "utils/Constants.h"
#include "utils/Vector2.h"

struct SDL_Point;
struct SDL_Rect;

class GameObject;

/**
 * \brief The Input manager that handles keyboard and mouse input. It's the
 * interface between direct SDL2 calls.
 *
 * \note This class is a Singleton, and must have the clear() method called
 * before starting to handling events, and the update() method on every received
 * event.
 *
 * This class also collects all input events and saves them on internal
 * structures.
 */
class Input final {
  /**
   * \brief Internal SDL keyboard state.
   */
  static const uint8_t* keyboard_;

  /**
   * \brief Internal SDL mouse state.
   */
  static uint32_t mouse_;

  /**
   * \brief The current mouse position.
   */
  static Vector2<int32_t> mousePosition_;

  /**
   * \brief The keys that are currently down.
   */
  static std::array<bool, static_cast<size_t>(KeyboardKey::kKeyLimit)> keyDown_;

  /**
   * \brief The keys that are currently up.
   */
  static std::array<bool, static_cast<size_t>(KeyboardKey::kKeyLimit)> keyUp_;

  /**
   * \brief The mouse buttons that are currently down.
   */
  static std::array<bool, static_cast<size_t>(MouseKey::kKeyLimit)> mouseDown_;

  /**
   * \brief The mouse buttons that are currently up.
   */
  static std::array<bool, static_cast<size_t>(MouseKey::kKeyLimit)> mouseUp_;

  /**
   * \brief The GameObject that was raycasted in the current mouse position.
   * This is used only to optimize repetitive calls to
   * Input::screenMouseToRay(), and is assigned to nullptr when the cursor
   * moves.
   */
  static GameObject* casted_;

 public:
  Input() = delete;
  ~Input() = delete;
  Input(const Input&) = delete;              // Copy Constructor
  Input(const Input&&) = delete;             // Move Constructor
  Input& operator=(const Input&) = delete;   // Assignment Operator
  Input& operator=(const Input&&) = delete;  // Move Operator

  /**
   * \brief Cleans up the states, must be called before the event loop.
   */
  static void clear() noexcept;

  /**
   * \brief Update this instance from the event loop.
   * \param event The event to handle.
   */
  static void update(const SDL_Event& event) noexcept;

  /**
   * \brief Gets the current mouse's position.
   * \return The current mouse's position.
   */
  [[nodiscard]] inline static const Vector2<int32_t>& mousePosition() noexcept {
    return mousePosition_;
  }

  /**
   * \brief Gets the current mouse's position as a SDL object.
   * @return The current mouse's position.
   */
  [[nodiscard]] static SDL_Point rawMousePosition() noexcept;

  /**
   * \brief Checks if a key if currently pressed.
   * \param key The keyboard key to check if it is currently pressed.
   * \return Whether or not the specified key is currently pressed.
   */
  [[nodiscard]] inline static bool keyPressed(KeyboardKey key) noexcept {
    if (!keyboard_) return false;
    return keyboard_[static_cast<size_t>(key)];
  }

  /**
   * \brief Checks if a key was pressed in this tick cycle.
   * \param key The keyboard key to check if it got pressed in this tick cycle.
   * \return Whether or not the specified keyboard key was pressed in this tick
   * cycle.
   */
  [[nodiscard]] inline static bool keyDown(KeyboardKey key) noexcept {
    return keyDown_[static_cast<size_t>(key)];
  }

  /**
   * \brief Checks if a key was released in this tick cycle.
   * \param key The keyboard key to check if it got released in this tick cycle.
   * \return Whether or not the specified keyboard key was released in this tick
   * cycle.
   */
  [[nodiscard]] inline static bool keyUp(KeyboardKey key) noexcept {
    return keyUp_[static_cast<size_t>(key)];
  }

  /**
   * \brief Checks if a mouse button was pressed in this tick cycle.
   * \param button The mouse button to check if it got pressed in this tick
   * cycle.
   * \return Whether or not the specified mouse key was pressed in this
   * tick cycle.
   */
  [[nodiscard]] inline static bool mouseButtonDown(MouseKey button) noexcept {
    return mouseDown_[static_cast<size_t>(button)];
  }

  /**
   * \brief Checks if a mouse button was released in this tick cycle.
   * \param button The mouse button to check if it got released in this tick
   * cycle.
   * \return Whether or not the specified mouse key was released in this
   * tick cycle.
   */
  [[nodiscard]] inline static bool mouseButtonUp(MouseKey button) noexcept {
    return mouseUp_[static_cast<size_t>(button)];
  }

  /**
   * \brief Checks if the mouse is inside a rectangle.
   * \param rectangle The area to check against.
   * \return Whether or not the mouse is inside the specified rectangle.
   */
  [[nodiscard]] static bool isMouseInside(const SDL_Rect* rectangle) noexcept;

  /**
   * \brief Checks if either the right or left shift keys are being pressed.
   * \return Whether or not any shift key is being pressed.
   */
  [[nodiscard]] inline static bool shift() noexcept {
    return keyPressed(KeyboardKey::LEFT_SHIFT) ||
           keyPressed(KeyboardKey::RIGHT_SHIFT);
  }

  /**
   * \brief Checks if either the right or left control keys are being pressed.
   * \return Whether or not any control key is being pressed.
   */
  [[nodiscard]] inline static bool ctrl() noexcept {
    return keyPressed(KeyboardKey::LEFT_CTRL) ||
           keyPressed(KeyboardKey::RIGHT_CTRL);
  }

  /**
   * \brief Checks if either the right or left alt keys are being pressed.
   * \return Whether or not any alt key is being pressed.
   */
  [[nodiscard]] inline static bool alt() noexcept {
    return keyPressed(KeyboardKey::LEFT_ALT) ||
           keyPressed(KeyboardKey::RIGHT_ALT);
  }

  [[nodiscard]] static GameObject* screenMouseToRay() noexcept;
};