// Copyright (c) 2022 Mika Pi

#pragma once

#include <CoreMinimal.h>
#include <Framework/MultiBox/MultiBoxBuilder.h>
#include <LevelEditor.h>
#include <Modules/ModuleManager.h>

class FQofLModule final : public IModuleInterface
{
public:
  /** IModuleInterface implementation */
  void StartupModule() final;
  void ShutdownModule() final;

private:
  size_t menuExtenderIdx;

  auto ConvertToInstancedStaticMesh(const TArray<AActor *> InActors) -> void;
  auto BreakdownInstancedStaticMesh(const TArray<AActor *> InActors) -> void;
  auto GetMenuExtender(const TSharedRef<FUICommandList> CommandList, const TArray<AActor *> InActors)
    -> TSharedRef<FExtender>;
  auto MakeMenu(FMenuBuilder &MenuBuilder, const TArray<AActor *> InActors) -> void;
};
