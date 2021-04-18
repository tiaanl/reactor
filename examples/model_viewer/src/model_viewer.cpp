#include <canvas/App.h>
#include <floats/Transform.h>
#include <hive/PhysicalResourceLocator.h>
#include <legion/Controllers/FirstPersonCameraController.h>
#include <legion/Rendering/Rendering.h>
#include <legion/Resources/Model.h>
#include <legion/Resources/ResourceManager.h>

inline fl::Vec2 f(const fl::Pos& p) {
  return {fl::Vec2{static_cast<F32>(p.x), static_cast<F32>(p.y)}};
}

si::Scene create_box_scene() {
  si::Scene result;

  si::Material material;
  material.diffuse.color = si::RGBA{255, 0, 0, 0};
  result.materials().pushBack(material);

  si::Mesh box;
  box.materialIndex = 0;
  box.positions.pushBack({0.0f, 0.0f, 0.0f});
  box.positions.pushBack({1.0f, 0.0f, 0.0f});
  box.positions.pushBack({0.0f, 1.0f, 0.0f});
  box.texCoords.pushBack({0.0f, 0.0f});
  box.texCoords.pushBack({1.0f, 0.0f});
  box.texCoords.pushBack({0.0f, 1.0f});
  result.meshes().pushBack(box);

  result.root_node().meshIndices.pushBack(0);
  result.root_node().transform = fl::Mat4::identity;

  return result;
}

class ModelViewerDelegate : public ca::WindowDelegate {
public:
  ModelViewerDelegate() : ca::WindowDelegate{"legion | Model Viewer"} {}

  bool onWindowCreated(ca::Window* window) override {
    if (!WindowDelegate::onWindowCreated(window)) {
      return false;
    }

    resource_manager_.setRenderer(window->getRenderer());

    fixtures_locator_.setRootPath(nu::FilePath{"/home/tilo/code/game/silhouette/tests/fixtures"});
    resource_manager_.addResourceLocatorBack(&fixtures_locator_);
    asteroids_locator_.setRootPath(nu::FilePath{"/home/tilo/code/game/AsteroidDefender/assets"});
    resource_manager_.addResourceLocatorBack(&asteroids_locator_);

    model_ = resource_manager_.get<le::Model>("box.dae");
    if (!model_) {
      LOG(Error) << "Could not load model.";
      return false;
    }

    return true;
  }

  bool onMousePressed(const ca::MouseEvent& evt) override {
    if (WindowDelegate::onMousePressed(evt)) {
      return true;
    }

    camera_controller_.onMousePressed(evt.button, f(evt.pos));

    return true;
  }

  void onMouseMoved(const ca::MouseEvent& evt) override {
    WindowDelegate::onMouseMoved(evt);

    camera_controller_.onMouseMoved(f(evt.pos));
  }

  void onMouseReleased(const ca::MouseEvent& evt) override {
    WindowDelegate::onMouseReleased(evt);

    camera_controller_.onMouseReleased(evt.button, f(evt.pos));
  }

  void tick(F32 delta) override {
    WindowDelegate::tick(delta);

    camera_controller_.tick(delta);
  }

  void onRender(ca::Renderer* renderer) override {
#if 0
    main_camera_.moveTo({0.0f, 0.0f, 10.0f});

    fl::Mat4 projection = fl::Mat4::identity;
    fl::Mat4 view = fl::Mat4::identity;

    main_camera_.updateProjectionMatrix(&projection);
    main_camera_.updateViewMatrix(&view);

    auto mvp = projection * view;

    LOG(Info) << "-------------";
    LOG(Info) << mvp.col[0].x << " " << mvp.col[1].x << " " << mvp.col[2].x << " " << mvp.col[3].x;
    LOG(Info) << mvp.col[0].y << " " << mvp.col[1].y << " " << mvp.col[2].y << " " << mvp.col[3].y;
    LOG(Info) << mvp.col[0].z << " " << mvp.col[1].z << " " << mvp.col[2].z << " " << mvp.col[3].z;
    LOG(Info) << mvp.col[0].w << " " << mvp.col[1].w << " " << mvp.col[2].w << " " << mvp.col[3].w;
#endif  // 0

    fl::Mat4 mvp = fl::Mat4::identity;

    // le::renderModel(renderer, *model_, mvp);
  }

private:
  hi::PhysicalFileResourceLocator fixtures_locator_;
  hi::PhysicalFileResourceLocator asteroids_locator_;
  le::ResourceManager resource_manager_;

  le::Camera main_camera_;
  le::FirstPersonCameraController camera_controller_{&main_camera_};

  le::Model* model_ = nullptr;
};

CANVAS_APP(ModelViewerDelegate)
