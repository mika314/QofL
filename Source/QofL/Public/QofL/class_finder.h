// Copyright (c) 2022 Mika Pi

#pragma once
#include <CoreMinimal.h>
#include <UObject/ConstructorHelpers.h>
#include <unordered_map>

template <typename T>
auto classFinder(const TCHAR *assetPath)
{
  static std::unordered_map<const TCHAR *, TSubclassOf<T>> classes;
  auto it = classes.find(assetPath);
  if (it == std::end(classes))
    it = classes.emplace(assetPath, ConstructorHelpers::FClassFinder<T>{assetPath}.Class).first;
  return it->second;
}

#define CLASS_FINDER(C, D, O) classFinder<U##C>(TEXT("/Game/" D "/" O))
