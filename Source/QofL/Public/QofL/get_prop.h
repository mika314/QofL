// Copyright (c) 2021 Mika Pi

#pragma once
#include "check_ret.h"
#include "log.h"
#include <CoreMinimal.h>
#include <type_traits>

template <typename T>
auto getProp(UObject *obj, const FName &propName)
{
  auto genericProperty = obj->GetClass()->FindPropertyByName(propName);
  if constexpr (std::is_base_of_v<UObject, T>)
  {
    CHECK_RET(genericProperty, static_cast<T *>(nullptr));
    auto property = CastField<FObjectProperty>(genericProperty);
    CHECK_RET(property, static_cast<T *>(nullptr));
    return Cast<T>(property->GetPropertyValue_InContainer(obj, 0));
  }
  else if constexpr (std::is_same_v<float, T>)
  {
    CHECK_RET(genericProperty, std::optional<float>{});
    auto property = CastField<FFloatProperty>(genericProperty);
    CHECK_RET(property, std::optional<float>{});
    return std::optional<float>{property->GetPropertyValue_InContainer(obj, 0)};
  }
  else if constexpr (std::is_same_v<double, T>)
  {
    CHECK_RET(genericProperty, std::optional<double>{});
    auto property = CastField<FDoubleProperty>(genericProperty);
    CHECK_RET(property, std::optional<double>{});
    return std::optional<double>{property->GetPropertyValue_InContainer(obj, 0)};
  }
  else if constexpr (std::is_same_v<bool, T>)
  {
    CHECK_RET(genericProperty, std::optional<bool>{});
    auto property = CastField<FBoolProperty>(genericProperty);
    CHECK_RET(property, std::optional<bool>{});
    return std::optional<bool>{property->GetPropertyValue_InContainer(obj, 0)};
  }
}

template <typename T>
auto setProp(UObject *obj, const FName &propName, T value)
{
  auto genericProperty = obj->GetClass()->FindPropertyByName(propName);
  if (!genericProperty)
  {
    LOG("Property not found:", propName.ToString());
    return;
  }
  auto property = [&]() {
    if constexpr (std::is_same_v<float, T>)
      return CastField<FFloatProperty>(genericProperty);
    else if constexpr (std::is_same_v<double, T>)
      return CastField<FDoubleProperty>(genericProperty);
    else if constexpr (std::is_same_v<bool, T>)
      return CastField<FBoolProperty>(genericProperty);
  }();
  if (!property)
  {
    LOG("Wrong type:", propName.ToString());
    return;
  }
  property->SetPropertyValue_InContainer(obj, value, 0);
}
