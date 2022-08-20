// Copyright (c) 2022 Mika Pi

#include <QofL/instanced_static_mesh_actor.h>

#include <Components/InstancedStaticMeshComponent.h>

AInstancedStaticMeshActor::AInstancedStaticMeshActor()
  : MeshComponent(
      CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMeshComponent")))
{
  PrimaryActorTick.bCanEverTick = true;
  RootComponent = MeshComponent;
}

auto AInstancedStaticMeshActor::GetInstancedStaticMeshComponent() const
  -> const UInstancedStaticMeshComponent *
{
  return MeshComponent;
}

auto AInstancedStaticMeshActor::GetInstancedStaticMeshComponent() -> UInstancedStaticMeshComponent *
{
  return MeshComponent;
}

auto AInstancedStaticMeshActor::SetMobility(EComponentMobility::Type NewMobility) -> void
{
  MeshComponent->SetMobility(NewMobility);
}
