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
  std::string path_{};
  ImageFit fit_{};
  std::shared_ptr<Image> image_{};
  SDL_Texture* texture_ = nullptr;

  void updateImageFit() noexcept;

  [[nodiscard]] static ImageFit getImageFitFromName(
      const std::string& value) noexcept;
  [[nodiscard]] static std::string getNameFromImageFit(ImageFit value) noexcept;

 public:
  explicit ImageRenderer(std::weak_ptr<GameObject> gameObject) noexcept;
  ~ImageRenderer() noexcept override;

  void onAwake() noexcept override;
  void onRender() noexcept override;

  [[nodiscard]] inline const std::string& path() const noexcept {
    return path_;
  }
  inline std::string& path() noexcept { return path_; }

  [[nodiscard]] inline const ImageFit& fit() const noexcept { return fit_; }
  inline ImageFit& fit() noexcept { return fit_; }

  [[nodiscard]] inline const std::shared_ptr<Image> image() const noexcept {
    return image_;
  }

  [[nodiscard]] Json::Value toJson() const noexcept override;
  void patch(const Json::Value& json) noexcept override;
};
