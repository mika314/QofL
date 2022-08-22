// Copyright (c) 2022 Mika Pi

#pragma once
#include <Components/SceneComponent.h>
#include <GameFramework/Actor.h>
#include <utility>

template <typename... Args>
constexpr auto setLoc(AActor *actor, Args &&...args)
{
  actor->SetActorLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setLoc(AActor &actor, Args &&...args)
{
  actor.SetActorLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getLoc(AActor *actor, Args &&...args)
{
  return actor->GetActorLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getLoc(AActor &actor, Args &&...args)
{
  return actor.GetActorLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setLoc(USceneComponent *comp, Args &&...args)
{
  comp->SetRelativeLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setLoc(USceneComponent &comp, Args &&...args)
{
  comp.SetRelativeLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getLoc(USceneComponent *comp, Args &&...args)
{
  return comp->GetRelativeLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getLoc(USceneComponent &comp, Args &&...args)
{
  return comp.GetRelativeLocation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getRot(AActor *actor, Args &&...args)
{
  return actor->GetActorRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getRot(AActor &actor, Args &&...args)
{
  return actor.GetActorRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getRot(USceneComponent *comp, Args &&...args)
{
  return comp->GetRelativeRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto getRot(USceneComponent &comp, Args &&...args)
{
  return comp.GetRelativeRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setRot(AActor *actor, Args &&...args)
{
  actor->SetActorRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setRot(AActor &actor, Args &&...args)
{
  actor.SetActorRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setRot(USceneComponent *comp, Args &&...args)
{
  comp->SetRelativeRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto setRot(USceneComponent &comp, Args &&...args)
{
  return comp.SetRelativeRotation(std::forward<Args>(args)...);
}

template <typename... Args>
constexpr auto vec(Args &&...args)
{
  return FVector{std::forward<Args>(args)...};
}

template <typename... Args>
constexpr auto rot(Args &&...args)
{
  return FRotator{std::forward<Args>(args)...};
}

template <typename... Args>
auto vec2(Args &&...args)
{
  return FVector2D{std::forward<Args>(args)...};
}
