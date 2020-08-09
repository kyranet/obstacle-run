// Copyright (c) 2020 Antonio Román. All rights reserved.

#pragma once

#include "objects/Component.h"
#include "utils/Vector2.h"

class Image;
class Transform;

enum class ImageFit {
  /**
   * \brief The replaced content is not resized.
   */
  None,
  /**
   * \brief The replaced content is sized to fill the element’s content box. The
   * entire object will completely fill the box. If the object's aspect ratio
   * does not match the aspect ratio of its box, then the object will be
   * stretched to fit.
   */
  Fill,
  /**
   * \brief The replaced content is scaled to maintain its aspect ratio while
   * fitting within the element’s content box. The entire object is made to fill
   * the box, while preserving its aspect ratio, so the object will be
   * "letterboxed" if its aspect ratio does not match the aspect ratio of the
   * box.
   */
  Contain,
  /**
   * \brief The replaced content is sized to maintain its aspect ratio while
   * filling the element’s entire content box. If the object's aspect ratio does
   * not match the aspect ratio of its box, then the object will be clipped to
   * fit.
   */
  Cover
};

struct SDL_Texture;

class ImageRenderer final : public Component {
  std::string path_;
  ImageFit fit_;
  Image* image_ = nullptr;
  SDL_Texture* texture_ = nullptr;

  void updateImageFit() noexcept;

 public:
  explicit ImageRenderer(std::string path, ImageFit fit) noexcept;
  ~ImageRenderer() noexcept;

  void onAwake() noexcept override;
  void onRender() noexcept override;

  [[nodiscard]] inline const std::string& path() const noexcept {
    return path_;
  }
  inline std::string& path() noexcept { return path_; }

  [[nodiscard]] inline const ImageFit& fit() const noexcept { return fit_; }
  inline ImageFit& fit() noexcept { return fit_; }

  [[nodiscard]] inline const Image* image() const noexcept { return image_; }
};
