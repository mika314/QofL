// Copyright (c) 2022 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "instanced_static_mesh_actor.generated.h"

UCLASS()
class QOFL_API AInstancedStaticMeshActor : public AActor
{
  GENERATED_BODY()

public:
  AInstancedStaticMeshActor();

  auto GetInstancedStaticMeshComponent() -> UInstancedStaticMeshComponent *;
  auto GetInstancedStaticMeshComponent() const -> const UInstancedStaticMeshComponent *;
  auto SetMobility(EComponentMobility::Type NewMobility) -> void;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
  class UInstancedStaticMeshComponent *MeshComponent;
};
