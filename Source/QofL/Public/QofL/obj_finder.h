// Copyright (c) 2022 Mika Pi

#pragma once
#include <CoreMinimal.h>
#include <UObject/ConstructorHelpers.h>

template <typename T>
T *objFinder(const TCHAR *assetPath)
{
  return ConstructorHelpers::FObjectFinder<T>{assetPath}.Object;
}

#define OBJ_FINDER(C, D, O) objFinder<U##C>(TEXT(#C "'/Game/" D "/" O "." O "'"))
