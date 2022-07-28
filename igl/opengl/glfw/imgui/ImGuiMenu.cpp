// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2018 Jérémie Dumas <jeremie.dumas@ens-lyon.org>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
////////////////////////////////////////////////////////////////////////////////
//#include "ImGuiMenu.h"
//#include "ImGuiHelpers.h"
#include <igl/project.h>
#include "ImGuiHelpers.h"

#include "ImGuiMenu.h"
#include "../imgui.h"
#include "igl/opengl/glfw/imgui/imgui_impl_glfw.h"
#include "igl/opengl/glfw/imgui/imgui_impl_opengl3.h"

//#include <imgui_fonts_droid_sans.h>
//#include <GLFW/glfw3.h>
#include <iostream>
////////////////////////////////////////////////////////////////////////////////

namespace igl
{
namespace opengl
{
namespace glfw
{
namespace imgui
{
IGL_INLINE void ImGuiMenu::init(Display* disp)
{
  // Setup ImGui binding
  if (disp->window)
  {
    IMGUI_CHECKVERSION();
    if (!context_)
    {
      // Single global context by default, but can be overridden by the user
      static ImGuiContext * __global_context = ImGui::CreateContext();
      context_ = __global_context;
    }
    const char* glsl_version = "#version 150";

    ImGui_ImplGlfw_InitForOpenGL(disp->window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::GetIO().IniFilename = nullptr;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    reload_font();
  }
}

IGL_INLINE void ImGuiMenu::reload_font(int font_size)
{
  hidpi_scaling_ = hidpi_scaling();
  pixel_ratio_ = pixel_ratio();
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->Clear();
 // io.Fonts->AddFontFromMemoryCompressedTTF(droid_sans_compressed_data,
 //   droid_sans_compressed_size, font_size * hidpi_scaling_);
  io.FontGlobalScale = 1.0 / pixel_ratio_;
}

IGL_INLINE void ImGuiMenu::shutdown()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  // User is responsible for destroying context if a custom context is given
  // ImGui::DestroyContext(*context_);
}

IGL_INLINE bool ImGuiMenu::pre_draw()
{
  glfwPollEvents();

  // Check whether window dpi has changed
  float scaling = hidpi_scaling();
  if (std::abs(scaling - hidpi_scaling_) > 1e-5)
  {
    reload_font();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  return false;
}

IGL_INLINE bool ImGuiMenu::post_draw()
{
  //draw_menu(viewer,core);
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return false;
}

IGL_INLINE void ImGuiMenu::post_resize(int width, int height)
{
  if (context_)
  {
    ImGui::GetIO().DisplaySize.x = float(width);
    ImGui::GetIO().DisplaySize.y = float(height);
  }
}

// Mouse IO
IGL_INLINE bool ImGuiMenu::mouse_down(GLFWwindow* window, int button, int modifier)
{
  ImGui_ImplGlfw_MouseButtonCallback(window, button, GLFW_PRESS, modifier);
  return ImGui::GetIO().WantCaptureMouse;
}

IGL_INLINE bool ImGuiMenu::mouse_up(GLFWwindow* window,int button, int modifier)
{
  //return ImGui::GetIO().WantCaptureMouse;
  // !! Should not steal mouse up
  return false;
}

IGL_INLINE bool ImGuiMenu::mouse_move(GLFWwindow* window,int mouse_x, int mouse_y)
{
  return ImGui::GetIO().WantCaptureMouse;
}

IGL_INLINE bool ImGuiMenu::mouse_scroll(GLFWwindow* window,float delta_y)
{
  ImGui_ImplGlfw_ScrollCallback(window, 0.f, delta_y);
  return ImGui::GetIO().WantCaptureMouse;
}

// Keyboard IO
IGL_INLINE bool ImGuiMenu::key_pressed(GLFWwindow* window,unsigned int key, int modifiers)
{
  ImGui_ImplGlfw_CharCallback(nullptr, key);
  return ImGui::GetIO().WantCaptureKeyboard;
}

IGL_INLINE bool ImGuiMenu::key_down(GLFWwindow* window, int key, int modifiers)
{
  ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_PRESS, modifiers);
  return ImGui::GetIO().WantCaptureKeyboard;
}

IGL_INLINE bool ImGuiMenu::key_up(GLFWwindow* window,int key, int modifiers)
{
  ImGui_ImplGlfw_KeyCallback(window, key, 0, GLFW_RELEASE, modifiers);
  return ImGui::GetIO().WantCaptureKeyboard;
}



IGL_INLINE void ImGuiMenu::draw_viewer_menu(igl::opengl::glfw::Viewer *viewer, std::vector<igl::opengl::Camera*> &camera,Eigen::Vector4i& viewWindow,std::vector<DrawInfo *> drawInfos)
{
    bool* p_open = NULL;
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = true;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    ImGui::Begin(
        "Viewer", p_open,
        window_flags
    );

    ImGui::SetWindowPos(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2((float)0, (float)0), ImGuiCond_Always);
    ImGui::End();
    no_move = true;
    no_resize = true;
    ImGui::Begin(
        "Viewer", p_open,
        window_flags
    );

  // Mesh
  if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
  {
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("Load##Mesh", ImVec2(-1, 0)))
    {
      int savedIndx = viewer->selected_data_index;
      viewer->open_dialog_load_mesh();
      if (viewer->data_list.size() > viewer->parents.size())
      {
        viewer->data()->type = 9;
        viewer->data()->mode = 4;
        viewer->data()->shaderID = 2;
        viewer->data()->viewports = 1;
        viewer->data()->show_lines = 0;
        viewer->data()->hide = false;
        viewer->data()->show_overlay = 0;
        viewer->SetShapeMaterial(viewer->data_list.size() - 1, 2);
        viewer->parents.emplace_back(-1);
          viewer->selected_data_index = savedIndx;
      }
    }
  }
    ImGui::Checkbox("Orthographic view", &(camera[viewer->currCamera]->_ortho));
    if (camera[viewer->currCamera]->_ortho) {
        camera[viewer->currCamera]->SetProjection(0,camera[viewer->currCamera]->_relationWH);
      }
    else {
        camera[viewer->currCamera]->SetProjection(camera[viewer->currCamera]->_fov > 0 ? camera[viewer->currCamera]->_fov : 45,camera[viewer->currCamera]->_relationWH);
      }
    ImGui::Checkbox("Fog", &(viewer->fog));
    ImGui::ColorEdit3("Fog Colour", viewer->fog_colour);
    if(ImGui::InputFloat("Scale", &(viewer->scaleFactor))){
      if(viewer->scaleFactor > 0)
        viewer->ScalePickedShape();
    }

  if (ImGui::Button("Change Background", ImVec2(-1, 0)))
  {
    viewer->ChangeBackground();
  }
  ImGui::PushItemWidth(100 * menu_scaling());

  if (ImGui::Button("Set Material", ImVec2(-1, 0)))
  {
    int tidx = viewer->open_dialog_load_mat();
    if (tidx != -1){
      unsigned int arr[1] = {tidx};
      int matid  = viewer->AddMaterial(arr, arr, 1);

      //selected == -1 if there was no single picking after multi picking i.e. the multipicking is the real picking
      if (viewer->selectedShapes.size() > 0 && viewer->selected == -1){ 
        for (int shape : viewer->selectedShapes){
          if (shape != 0 && viewer->cameraShapesMap.find(shape) == viewer->cameraShapesMap.end()){
            viewer->SetShapeMaterial(shape, matid);
          }
      }
    
      }
      else{
        if (viewer->selected != 0 && viewer->cameraShapesMap.find(viewer->selected) == viewer->cameraShapesMap.end()){
          viewer->SetShapeMaterial(viewer->selected, matid);
        }
      }
    }
  }

  if (ImGui::Button("Blend on/off", ImVec2(-1, 0))){
    if (viewer->selectedShapes.size() > 0 && viewer->selected == -1){ 
      for (int shape : viewer->selectedShapes){
        if (shape != 0){
          if(viewer->data_list[shape]->shaderID == 2)
            viewer->SetShapeShader(shape, 3);
          else 
            viewer->SetShapeShader(shape, 2);
        }
      }
    }
    else if(viewer->selected !=0 ){
      if(viewer->data_list[viewer->selected]->shaderID == 2)
        viewer->SetShapeShader(viewer->selected, 3);
      else 
        viewer->SetShapeShader(viewer->selected, 2);
    }
  }




  auto make_checkbox = [&](const char *label, unsigned int &option)
  {
    return ImGui::Checkbox(label,
      [&]() { return drawInfos[1]->is_set(option); },
      [&](bool value) { return drawInfos[1]->set(option, value); }
    );
  };
      // ImGui::ColorEdit4("Background", drawInfos[1]->Clear_RGBA.data(),
      // ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);

  float w = ImGui::GetContentRegionAvailWidth();
  float p = ImGui::GetStyle().FramePadding.x;
  if (ImGui::Button("Reset Shape", ImVec2((w-p)/2.0, 0)))
  {
    if (viewer->selected > 0 && viewer->data_list[viewer->selected]->Is2Render(0))
      {
        viewer->data_list[viewer->selected]->ZeroTrans();
        auto it = viewer->cameraShapesMap.find(viewer->selected);
        if(it != viewer->cameraShapesMap.end()){
            camera[it->second]->ZeroTrans();
            viewer->cameraLocs[it->second-1] = {0,0,0};
        }
      }
  }
  ImGui::SameLine(0, p);
  if(ImGui::Button("Reset Rotation", ImVec2((w-p)/2.0, 0))){
    if (viewer->selectedShapes.size() > 0 && viewer->selected == -1){ 
        for (int shape : viewer->selectedShapes){
          if (shape > 0){
            viewer->data_list[shape]->ZeroRot();
          }
      }
    
      }
      else{
        if (viewer->selected > 0){
          viewer->data_list[viewer->selected]->ZeroRot();
        }
      }
  }
  if (ImGui::CollapsingHeader("Cameras", ImGuiTreeNodeFlags_DefaultOpen))
  {
    if (ImGui::Button("Reset Camera", ImVec2(-1, 0)))
    {
      camera[viewer->currCamera]->ZeroTrans();
      camera[viewer->currCamera]->MyTranslate({0,0,10}, true);
      viewer->cameraLocs[viewer->currCamera] = {0,0,10};
      if (viewer->currCamera !=0){
        viewer->data(viewer->cameraShapes[viewer->currCamera-2])->ZeroTrans();
        viewer->data(viewer->cameraShapes[viewer->currCamera-2])->MyTranslate({0,0,10}, true);
      }

    }
    if (ImGui::Button("Add Camera", ImVec2(-1,0)))
    {
      int id = viewer->AddShape(viewer->Cube, -1, viewer->TRIANGLES);
      viewer->cameraShapes.push_back(id);
      viewer->cameraShapesMap[id] = camera.size();
      viewer->SetShapeShader(id, 2);
      viewer->SetShapeMaterial(id, 6);
      camera.push_back(new igl::opengl::Camera(45, 1200.0/1600.0, 1.0, 120));
      viewer->cameraLocs.push_back({0,0,0});
    }

    if (ImGui::Button("Set Animation Camera", ImVec2(-1,0)))
    {
        viewer->animCamera = viewer->currCamera;
    }

    for(int i = 0; i < camera.size() - 1; i++){
      char label[64] = "change to camera ";
      char integer_string[32];

      sprintf(integer_string, "%d", i);
      strcat(label, integer_string);
      if (ImGui::Button(label, ImVec2(-1, 0))){
        std::vector<int> drawInfs = {0,1, 4,6,7,8};
        if(viewer->currCamera != 0){
          viewer->data_list[viewer->cameraShapes[viewer->currCamera-2]]->UnHide();
        }
        if(i != 0){
          viewer->data_list[viewer->cameraShapes[i-1]]->Hide();
          std::cout<< viewer->cameraShapes[i-1] << std::endl;
        }
        for (int draw : drawInfs){
          if(i != 0)
            drawInfos[draw]->SetCamera(i + 1);
          else
            drawInfos[draw]->SetCamera(0);
        }
        if(i != 0) viewer->currCamera = i + 1;
        else viewer->currCamera = 0;
      }
    }
  }

  // Draw options
//   if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen))
//   {
//     if (ImGui::Checkbox("Face-based", &(viewer->data()->face_based)))
//     {
//       viewer->data()->dirty = MeshGL::DIRTY_ALL;
//     }
// //
// //    make_checkbox("Show texture", viewer->data().show_texture);
// //    if (ImGui::Checkbox("Invert normals", &(viewer->data().invert_normals)))
// //    {
// //      viewer->data().dirty |= igl::opengl::MeshGL::DIRTY_NORMAL;
// //    }
//     make_checkbox("Show overlay", viewer->data()->show_overlay);
//     make_checkbox("Show overlay depth", viewer->data()->show_overlay_depth);

//     ImGui::ColorEdit4("Line color", viewer->data()->line_color.data(),
//         ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
  
  if(viewer->finished){
    std::vector<int> drawInfs = {0,1, 4,6,7,8};
    for (int i = 0; i < viewer->cameraShapes.size(); i++){
        if (i+2 != viewer->currCamera){
          viewer->data_list[viewer->cameraShapes[i]]->UnHide();
        }
      }
    for (int draw : drawInfs){
      drawInfos[draw]->SetCamera(viewer->currCamera);
    }
    viewer->finished = false;
  }
  
  if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen))
  {
    ImGui::SliderFloat("Time", &(viewer->time), 1.0f, 30.0f);
    // ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("Play", ImVec2((w-p)/2.f, 0)))
    {
      std::cout << "Playing" << std::endl;
      viewer->Activate();
      if(camera.size() > 2){
        for (int shape : viewer->cameraShapes){
          viewer->data_list[shape]->Hide();
        }
        std::vector<int> drawInfs = {0,1, 4,6,7,8};
        for (int draw : drawInfs){
          drawInfos[draw]->SetCamera(viewer->animCamera);
        }
      }
      
    }
    ImGui::SameLine(0, p);
    if (ImGui::Button("Pause", ImVec2((w-p)/2.f, 0)))
    {
      std::cout << "Pause" << std::endl;
      viewer->Deactivate();
      for (int i = 0; i < viewer->cameraShapes.size(); i++){
          if (i+2 != viewer->currCamera){
            viewer->data_list[viewer->cameraShapes[i]]->UnHide();
          }
      }
      std::vector<int> drawInfs = {0,1, 4,6,7,8};
      for (int draw : drawInfs){
        drawInfos[draw]->SetCamera(viewer->currCamera);
      }
    }
  }
    // ImGui::PopItemWidth();
//   }

  // Overlays
  // if (ImGui::CollapsingHeader("Overlays", ImGuiTreeNodeFlags_DefaultOpen))
  // {
  //   make_checkbox("Wireframe", viewer->data()->show_lines);
  //   make_checkbox("Fill", viewer->data()->show_faces);

  // }

 if (ImGui::CollapsingHeader("Layers", ImGuiTreeNodeFlags_DefaultOpen)){
    float w = ImGui::GetContentRegionAvailWidth();
    float p = ImGui::GetStyle().FramePadding.x;
    if (ImGui::Button("Add new Layer", ImVec2(-1,0))){
      viewer->layers.push_back(std::vector<int>());
      viewer->showLayers.push_back(1);
    }
    int  i = 0;
    for(std::vector<int> curr : viewer->layers){
      char label[64] = "Add to Layer ";
      char integer_string[32];

      sprintf(integer_string, "%d", i);
      strcat(label, integer_string);

      if (ImGui::Button(label, ImVec2(3*(w-p)/5.f, 0)))
      {
        if (viewer->selectedShapes.size() > 0 && viewer->selected == -1){ 
          for (int shape : viewer->selectedShapes){
            if (shape != 0){
              viewer->layers[i].push_back(shape);
              if(!viewer->showLayers[i])
              viewer->data_list[shape]->Hide();
            }
          }
        }
        else if(viewer->selected != 0){
          viewer->layers[i].push_back(viewer->selected);
          if(!viewer->showLayers[i])
            viewer->data_list[viewer->selected]->Hide();
        }
      }
      ImGui::SameLine(0, p);
      char listLabel[64] = "layer ";
      strcat(listLabel, integer_string);      
      bool tmp = viewer->showLayers[i];
      if (ImGui::Checkbox(listLabel, &tmp))
      {
        viewer->showLayers[i] = tmp;
        if(!viewer->showLayers[i]){
          for (int shape : viewer->layers[i]){
            viewer->data_list[shape]->Hide();
          }
        }
        else{
          for (int shape : viewer->layers[i]){
            viewer->data_list[shape]->UnHide();
          }
        }
      }
      i++;
    }
  }
 

  ImGui::End();
}



IGL_INLINE float ImGuiMenu::pixel_ratio()
{
  // Computes pixel ratio for hidpi devices
  int buf_size[2];
  int win_size[2];
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetFramebufferSize(window, &buf_size[0], &buf_size[1]);
  glfwGetWindowSize(window, &win_size[0], &win_size[1]);
  return (float) buf_size[0] / (float) win_size[0];
}

IGL_INLINE float ImGuiMenu::hidpi_scaling()
{
  // Computes scaling factor for hidpi devices
  float xscale, yscale;
  GLFWwindow* window = glfwGetCurrentContext();
  glfwGetWindowContentScale(window, &xscale, &yscale);
  return 0.5 * (xscale + yscale);
}

} // end namespace
} // end namespace
} // end namespace
} // end namespace
