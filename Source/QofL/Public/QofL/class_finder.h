// Copyright (c) 2022 Mika Pi

#pragma once
#include <CoreMinimal.h>
#include <UObject/ConstructorHelpers.h>

template <typename T>
auto classFinder(const TCHAR *assetPath)
{
  return ConstructorHelpers::FClassFinder<T>{assetPath}.Class;
}

#define CLASS_FINDER(C, D, O) classFinder<C>(TEXT("/Game/" D "/" O))
