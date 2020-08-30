/*----------------------------------------------------------------------------*/
/* Copyright (c) 2019-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <atomic>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace wpi::gui {

struct SavedSettings {
  bool loadedWidthHeight = false;
  int width;
  int height;
  int maximized = 0;
  int xPos = -1;
  int yPos = -1;
  int userScale = 2;
  int style = 0;
};

struct Font {
  static constexpr int kScaledLevels = 9;
  ImFont* scaled[kScaledLevels];
};

struct Context : public SavedSettings {
  std::atomic_bool exit{false};

  std::string title;
  int defaultWidth;
  int defaultHeight;

  GLFWwindow* window = nullptr;

  std::vector<std::function<void()>> initializers;
  std::vector<std::function<void(float scale)>> windowScalers;
  std::vector<std::pair<
      const char*,
      std::function<ImFont*(ImGuiIO& io, float size, const ImFontConfig* cfg)>>>
      makeFonts;

  ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  std::vector<std::function<void()>> earlyExecutors;
  std::vector<std::function<void()>> lateExecutors;

  int fontScale = 2;  // updated by main loop
  std::vector<Font> fonts;
};

extern Context* gContext;

void PlatformCreateContext();
void PlatformDestroyContext();
void PlatformGlfwInitHints();
void PlatformGlfwWindowHints();
bool PlatformInitRenderer();
void PlatformRenderFrame();
void PlatformShutdown();

void CommonRenderFrame();

}  // namespace wpi::gui
