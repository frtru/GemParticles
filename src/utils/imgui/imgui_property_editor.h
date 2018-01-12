/*************************************************************************
* Copyright (c) 2018 François Trudel
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*************************************************************************/
#pragma once
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <vector>
#include <tuple>

#include "utils/imgui/imgui.h"
#include "utils/imgui/imgui_log.h"
#include "utils/singleton.hh"

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#if (__GNUC__ >= 6)
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

enum EditableProperty {
  TOGGLE_BOOL,
  INPUT_INT,
  DRAG_INT,
  INPUT_FLOAT,
  DRAG_FLOAT,
  COLOR,
  VEC3,
  STRING,
  NEW_OBJECT
};

class ImGuiPropertyEditor : public Singleton<ImGuiPropertyEditor>
{
public:
  using Property = std::tuple<std::string, EditableProperty, void*>;
  void AddObject(const std::string& name, void *uid) {
    _PropertiesVector.push_back(std::make_tuple(name, NEW_OBJECT, uid));
  }

  void AddProperty(const std::string& prefix, EditableProperty type, void* data) {
    _PropertiesVector.push_back(std::make_tuple(prefix, type, data));
  }

  void Draw(const char* title, bool* p_open = NULL) {
    if (!ImGui::Begin(title, p_open))
    {
      ImGui::End();
      return;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    ImGui::Columns(2);
    for (int i = 0; i < _PropertiesVector.size(); ++i) {
      const Property &prop = _PropertiesVector[i];
      CreateComponentFromType(prop, i); 
    }
    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
  }
private:

  void CreateComponentFromType(const Property& prop, int id) {
    EditableProperty type = std::get<EditableProperty>(prop);
    std::string name      = std::get<std::string>(prop);
    void* data            = std::get<void*>(prop);
    switch (type) {
      case NEW_OBJECT:
        CreateParent(name, data);
        break;
      case TOGGLE_BOOL:
        break;
      case INPUT_INT:
        CreateIntInputComponent(name, data, id);
        break;
      case DRAG_INT:
        CreateIntDragComponent(name, data, id);
        break;
      case INPUT_FLOAT:
        CreateFloatInputComponent(name, data, id);
        break;
      case DRAG_FLOAT:
        CreateFloatDragComponent(name, data, id);
        break;
      case COLOR:
        CreateColorPickerComponent(name, data, id);
        break;
      case VEC3:
        CreateVec3DragComponent(name, data, id);
        break;
      case STRING:
        break;
      default:
        break;
    }
  }

  void CreateIntInputComponent(const std::string &name, void *data, int id) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::InputInt("##value", static_cast<int*>(data), 1U);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();
  }
  void CreateFloatInputComponent(const std::string &name, void *data, int id) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::InputFloat("##value", static_cast<float*>(data), 1.0f);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();
  }
  void CreateIntDragComponent(const std::string &name, void *data, int id) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::DragInt("##value", static_cast<int*>(data), 1);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();
  }
  void CreateFloatDragComponent(const std::string &name, void *data, int id) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::DragFloat("##value", static_cast<float*>(data), 0.01f);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();
  }
  void CreateColorPickerComponent(const std::string &name, void *data, int id) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::ColorEdit4("MyColor##2", static_cast<float*>(data), ImGuiColorEditFlags_Uint8); // TODO: This probably won't work!
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();
  }
  void CreateParent(const std::string & name, void *data) {
    ImGui::PushID(data);
    ImGui::Separator();
    ImGui::AlignTextToFramePadding();
    ImGui::Text(name.c_str());
    ImGui::NextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("");
    ImGui::NextColumn();
    ImGui::PopID();
  }
  void CreateVec3DragComponent(const std::string &name, void *data, int id) {
    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::DragFloat3("##value", static_cast<float*>(data));
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();
  }
  std::vector<Property> _PropertiesVector;
};