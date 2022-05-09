// Copyright (c) 2021 Mika Pi

#pragma once
#include <CoreMinimal.h>
#include <type_traits>

template <typename T>
auto getProp(UObject *obj, const FName &propName) -> T *
{
  if constexpr (std::is_base_of<UObject, T>::value)
  {
    auto property = CastField<FObjectProperty>(obj->GetClass()->FindPropertyByName(propName));
    if (!property)
      return nullptr;
    return Cast<T>(property->GetPropertyValue_InContainer(obj, 0));
  }
}