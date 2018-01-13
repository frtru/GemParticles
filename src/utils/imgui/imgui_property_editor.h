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
#include <functional>
#include <limits>

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

enum class PropertyType {
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
struct IProperty {
  IProperty(const std::string &name,
    void* data, std::function<void()> callback)
    : _name(name), _data(data), _callback(callback) {
  }
  
  virtual void AllocateDataCopy() = 0;
  virtual void Draw(int id) = 0;
  
  std::string           _name;
  void*                 _data;
  void*                 _previous_data;
  std::function<void()> _callback;
};
template <PropertyType>
struct Property : public IProperty{};
template <>
struct Property<PropertyType::NEW_OBJECT> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  virtual void AllocateDataCopy() override {
    _previous_data = nullptr;
  }
  virtual void Draw(int id) override {
    ImGui::PushID(_data);
    ImGui::Separator();
    ImGui::AlignTextToFramePadding();
    ImGui::Text(_name.c_str());
    ImGui::NextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::Text("");
    ImGui::NextColumn();
    ImGui::PopID();
  }
};
template <>
struct Property<PropertyType::INPUT_INT> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  ~Property() { free(_previous_data); }

  virtual void AllocateDataCopy() override {
    _previous_data = malloc(sizeof(int));
    memcpy(_previous_data, _data, sizeof(int));
  }
  virtual void Draw(int id) override {
    int *wData = static_cast<int*>(_data),
      *wPreviousData = static_cast<int*>(_previous_data);

    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(_name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::InputInt("##value", wData, 1U);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();

    if (_callback != nullptr && *wData != *wPreviousData) {
      _callback();
      *wPreviousData = *wData;
    }
  }
};
template <>
struct Property<PropertyType::DRAG_INT> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  ~Property() { free(_previous_data); }

  virtual void AllocateDataCopy() override {
    _previous_data = malloc(sizeof(int));
    memcpy(_previous_data, _data, sizeof(int));
  }
  virtual void Draw(int id) override {
    int *wData = static_cast<int*>(_data),
      *wPreviousData = static_cast<int*>(_previous_data);

    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(_name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::DragInt("##value", wData, 1);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();

    if (_callback != nullptr && *wData != *wPreviousData) {
      _callback();
      *wPreviousData = *wData;
    }
  }
};
template <>
struct Property<PropertyType::INPUT_FLOAT> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  ~Property() { free(_previous_data); }

  virtual void AllocateDataCopy() override {
    _previous_data = malloc(sizeof(float));
    memcpy(_previous_data, _data, sizeof(float));
  }
  virtual void Draw(int id) override {
    float *wData = static_cast<float*>(_data),
      *wPreviousData = static_cast<float*>(_previous_data);

    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(_name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::InputFloat("##value", wData, 1.0f);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();

    if (_callback != nullptr && fabs(*wData - *wPreviousData) >= 0.1f) {
      _callback();
      *wPreviousData = *wData;
    }
  }
};
template <>
struct Property<PropertyType::DRAG_FLOAT> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  ~Property() { free(_previous_data); }

  virtual void AllocateDataCopy() override {
    _previous_data = malloc(sizeof(float));
    memcpy(_previous_data, _data, sizeof(float));
  }
  virtual void Draw(int id) override {
    float *wData = static_cast<float*>(_data),
      *wPreviousData = static_cast<float*>(_previous_data);

    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(_name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::DragFloat("##value", wData, 0.01f);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();

    if (_callback != nullptr && fabs(*wData - *wPreviousData) >= 0.1f) {
      _callback();
      *wPreviousData = *wData;
    }
  }
};
template <>
struct Property<PropertyType::COLOR> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  ~Property() { free(_previous_data); }

  virtual void AllocateDataCopy() override {
    _previous_data = malloc(4 * sizeof(float));
    memcpy(_previous_data, _data, 4 * sizeof(float));
  }
  virtual void Draw(int id) override {
    float *wData = static_cast<float*>(_data),
      *wPreviousData = static_cast<float*>(_previous_data);

    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(_name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::ColorEdit4("MyColor##2", wData, ImGuiColorEditFlags_Uint8);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();

    if (_callback != nullptr  &&
      std::fabs(*wData - *wPreviousData) >= 0.1f &&
      std::fabs(*(wData + 1) - *(wPreviousData + 1)) >= 0.1f &&
      std::fabs(*(wData + 2) - *(wPreviousData + 2)) >= 0.1f &&
      std::fabs(*(wData + 3) - *(wPreviousData + 3)) >= 0.1f) {
      _callback();
      *wPreviousData = *wData;
      *(wPreviousData + 1) = *(wData + 1);
      *(wPreviousData + 2) = *(wData + 2);
      *(wPreviousData + 3) = *(wData + 3);
    }
  }
};
template <>
struct Property<PropertyType::VEC3> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}

  ~Property() { free(_previous_data); }

  virtual void AllocateDataCopy() override {
    _previous_data = malloc(3 * sizeof(float));
    memcpy(_previous_data, _data, 3 * sizeof(float));
  }
  virtual void Draw(int id) override {
    float *wData = static_cast<float*>(_data),
      *wPreviousData = static_cast<float*>(_previous_data);

    ImGui::PushID(id);
    ImGui::AlignTextToFramePadding();
    // -------------------
    ImGui::Bullet();
    ImGui::Selectable(_name.c_str());
    ImGui::NextColumn();
    // -------------------
    ImGui::PushItemWidth(-1);
    ImGui::DragFloat3("##value", wData);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    // -------------------
    ImGui::PopID();

    if (_callback != nullptr  &&
      std::fabs(*wData - *wPreviousData) >= 0.1f &&
      std::fabs(*(wData + 1) - *(wPreviousData + 1)) >= 0.1f &&
      std::fabs(*(wData + 2) - *(wPreviousData + 2)) >= 0.1f) {
      _callback();
      *wPreviousData = *wData;
      *(wPreviousData + 1) = *(wData + 1);
      *(wPreviousData + 2) = *(wData + 2);
    }
  }
};
template <>
// TODO: Complete following property types when they will actually be used
struct Property<PropertyType::STRING> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}
};
template <>
struct Property<PropertyType::TOGGLE_BOOL> : IProperty {
  Property(const std::string &name,
    void* data, std::function<void()> callback)
    : IProperty(name, data, callback) {}
};

class ImGuiPropertyEditor : public Singleton<ImGuiPropertyEditor>
{
public:
  void AddObject(const std::string& name, void *uid) {
    AddProperty<PropertyType::NEW_OBJECT>(name, uid, nullptr);
  }

  template<PropertyType type>
  void AddProperty(const std::string& name, void* data, std::function<void()> callback = nullptr) {
    auto ptr = std::make_shared<Property<type> >(name, data, callback);
    ptr->AllocateDataCopy();
    _PropertiesVector.emplace_back(ptr);
  }

  void Draw(const char* title, bool openFlag = true, bool* p_open = NULL) {
    if (!openFlag)
      return;
    if (!ImGui::Begin(title, p_open))
    {
      ImGui::End();
      return;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    ImGui::Columns(2);
    for (int i = 0; i < _PropertiesVector.size(); ++i) {
      std::shared_ptr<IProperty> prop = _PropertiesVector[i];
      prop->Draw(i);
    }
    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
  }
private:
  std::vector<std::shared_ptr<IProperty> > _PropertiesVector;
};