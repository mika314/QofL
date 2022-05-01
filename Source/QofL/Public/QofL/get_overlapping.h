// Copyright (c) 2022 Mika Pi

#pragma once

#include <Containers/Array.h>
#include <GameFramework/Actor.h>
#include <functional>
#include <vector>

template <typename T, typename U>
auto getOverlapping(const U &u) -> std::vector<std::reference_wrapper<T>>
{
  TArray<AActor *> oa;
  u.GetOverlappingActors(oa, T::StaticClass());
  auto ret = std::vector<std::reference_wrapper<T>>();
  for (auto a : oa)
    ret.push_back(*Cast<T>(a));
  return ret;
}
