// Copyright (c) 2022 Mika Pi

#include "QofL.h"
#include <Components/InstancedStaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <Engine/StaticMeshActor.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Materials/MaterialInterface.h>
#include <QofL/instanced_static_mesh_actor.h>
#include <QofL/loc.h>
#include <QofL/log.h>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#define LOCTEXT_NAMESPACE "FQofLModule"

auto FQofLModule::GetMenuExtender(const TSharedRef<FUICommandList> CommandList,
                                  const TArray<AActor *> InActors) -> TSharedRef<FExtender>
{
  TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender);
  MenuExtender->AddMenuExtension(
    "ActorConvert",
    EExtensionHook::After,
    nullptr,
    FMenuExtensionDelegate::CreateRaw(this, &FQofLModule::MakeMenu, InActors));
  return MenuExtender;
}

auto FQofLModule::StartupModule() -> void
{
  LOG_INFO("Startup QofL module");
  if (!IsRunningCommandlet())
  {
    FLevelEditorModule &LevelEditorModule =
      FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    auto &menuExtenders = LevelEditorModule.GetAllLevelViewportContextMenuExtenders();
    menuExtenderIdx =
      menuExtenders.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(
        this, &FQofLModule::GetMenuExtender));
  }
  IModuleInterface::StartupModule();
}

auto FQofLModule::ShutdownModule() -> void
{
  IModuleInterface::ShutdownModule();
  LOG_INFO("Shutdown Ordy module");
  FLevelEditorModule &LevelEditorModule =
    FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
  LevelEditorModule.GetAllLevelViewportContextMenuExtenders().RemoveAt(menuExtenderIdx);
}

auto FQofLModule::MakeMenu(FMenuBuilder &MenuBuilder, const TArray<AActor *> InActors) -> void
{
  MenuBuilder.AddMenuEntry(
    LOC("Consolidate"),
    LOC("Consolidate"),
    FSlateIcon(),
    FExecuteAction::CreateRaw(this, &FQofLModule::ConvertToInstancedStaticMesh, InActors));
  MenuBuilder.AddMenuEntry(
    LOC("Breakdown"),
    LOC("Breakdown"),
    FSlateIcon(),
    FExecuteAction::CreateRaw(this, &FQofLModule::BreakdownInstancedStaticMesh, InActors));
}

auto FQofLModule::ConvertToInstancedStaticMesh(const TArray<AActor *> InActors) -> void
{
  if (InActors.Num() == 0)
  {
    LOG_INFO("No actors selected");
    return;
  }
  auto world = GEditor->GetEditorWorldContext().World();
  if (!world)
    return;
  // spawn an actor
  auto newActor = world->SpawnActor<AActor>();
  if (!newActor)
    return;
  // add to the actor a USceneComponent
  auto root = NewObject<USceneComponent>(newActor);
  root->RegisterComponent();
  // mark as static
  root->SetMobility(EComponentMobility::Static);
  newActor->SetRootComponent(root);
  newActor->SetActorLocation(InActors[0]->GetActorLocation());
  newActor->SetActorRotation(InActors[0]->GetActorRotation());

  std::map<std::pair<UStaticMesh *, std::vector<UMaterialInterface *>>, UInstancedStaticMeshComponent *>
    comps;
  std::unordered_map<std::string, int> names;

  // add all actors to the actor
  for (auto actor : InActors)
  {
    auto displayName = [=]() {
      auto tmp = std::string{TCHAR_TO_UTF8(*UKismetSystemLibrary::GetDisplayName(actor))};
      // return part before _
      return tmp.substr(0, tmp.find('_'));
    }();
    // iterate through all components
    for (auto srcComp : actor->GetComponents())
    {
      if (auto srcInstancedStaticMeshComp = Cast<UInstancedStaticMeshComponent>(srcComp))
      {
        auto destComp = [&]() {
          auto key = std::make_pair(srcInstancedStaticMeshComp->GetStaticMesh(),
                                    std::vector<UMaterialInterface *>{});
          for (auto i = 0; i < srcInstancedStaticMeshComp->GetNumMaterials(); ++i)
            key.second.push_back(srcInstancedStaticMeshComp->GetMaterial(i));

          auto it = comps.find(key);
          if (it == comps.end())
          {
            // create instanced static mesh component with the name based on static mesh name
            auto comp = NewObject<UInstancedStaticMeshComponent>(newActor);
            newActor->AddInstanceComponent(comp);
            comp->RegisterComponent();
            comp->SetStaticMesh(srcInstancedStaticMeshComp->GetStaticMesh());
            for (auto i = 0; i < srcInstancedStaticMeshComp->GetNumMaterials(); ++i)
              comp->SetMaterial(i, srcInstancedStaticMeshComp->GetMaterial(i));
            comp->SetMobility(EComponentMobility::Static);
            comp->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
            comp->NumCustomDataFloats = srcInstancedStaticMeshComp->NumCustomDataFloats;
            auto compName = [=]() {
              auto mesh = srcInstancedStaticMeshComp->GetStaticMesh();
              auto name = mesh->GetName();
              if (comp->Rename(*name, nullptr, REN_Test))
                return name;
              for (auto i = 1;; ++i)
              {
                auto newName = name + "_" + FString::FromInt(i);
                if (comp->Rename(*newName, nullptr, REN_Test))
                  return newName;
              }
            }();
            comp->Rename(*compName);

            comps.insert(std::make_pair(key, comp));
            return comp;
          }
          return it->second;
        }();
        // copy instances
        for (auto i = 0; i < srcInstancedStaticMeshComp->GetInstanceCount(); ++i)
        {
          names[displayName]++;
          if (destComp->NumCustomDataFloats < srcInstancedStaticMeshComp->NumCustomDataFloats)
            destComp->NumCustomDataFloats = srcInstancedStaticMeshComp->NumCustomDataFloats;
          FTransform instanceTransform;
          srcInstancedStaticMeshComp->GetInstanceTransform(i, instanceTransform);
          const auto inst = destComp->AddInstance(instanceTransform *
                                                  srcInstancedStaticMeshComp->GetComponentTransform());
          for (auto j = 0; j < srcInstancedStaticMeshComp->NumCustomDataFloats; ++j)
          {
            const auto value =
              srcInstancedStaticMeshComp
                ->PerInstanceSMCustomData[j + i * srcInstancedStaticMeshComp->NumCustomDataFloats];
            destComp->SetCustomDataValue(inst, j, value);
          }
        }

        actor->Destroy();
        continue;
      }

      if (auto srcStaticMeshComp = Cast<UStaticMeshComponent>(srcComp))
      {
        names[displayName]++;
        auto destComp = [&]() {
          auto key =
            std::make_pair(srcStaticMeshComp->GetStaticMesh(), std::vector<UMaterialInterface *>{});
          for (auto i = 0; i < srcStaticMeshComp->GetNumMaterials(); ++i)
            key.second.push_back(srcStaticMeshComp->GetMaterial(i));

          auto it = comps.find(key);
          if (it == comps.end())
          {
            auto comp = NewObject<UInstancedStaticMeshComponent>(newActor);
            newActor->AddInstanceComponent(comp);
            comp->RegisterComponent();
            comp->SetStaticMesh(srcStaticMeshComp->GetStaticMesh());
            for (auto i = 0; i < srcStaticMeshComp->GetNumMaterials(); ++i)
              comp->SetMaterial(i, srcStaticMeshComp->GetMaterial(i));
            comp->SetMobility(EComponentMobility::Static);
            comp->AttachToComponent(root, FAttachmentTransformRules::KeepWorldTransform);
            comps.insert(std::make_pair(key, comp));
            return comp;
          }
          return it->second;
        }();
        destComp->AddInstance(srcStaticMeshComp->GetComponentTransform());
        actor->Destroy();
        continue;
      }
    }
  }
  // find the name with highest number of meshes
  auto max = 0;
  auto maxName = std::string{};
  for (auto const &[name, count] : names)
  {
    if (count > max)
    {
      max = count;
      maxName = name;
    }
  }
  newActor->SetActorLabel(maxName.c_str());
}

auto FQofLModule::BreakdownInstancedStaticMesh(const TArray<AActor *> InActors) -> void
{
  if (InActors.Num() == 0)
  {
    LOG_INFO("No actors selected");
    return;
  }
  auto world = GEditor->GetEditorWorldContext().World();
  if (!world)
    return;
  for (auto srcActor : InActors)
  {
    // iterate through actor components
    for (auto srcComp : srcActor->GetComponents())
    {
      auto srcInstancedStaticMeshComp = Cast<UInstancedStaticMeshComponent>(srcComp);
      if (!srcInstancedStaticMeshComp)
        continue;
      const auto cnt = srcInstancedStaticMeshComp->GetInstanceCount();
      for (auto i = 0; i < cnt; i++)
      {
        FTransform instanceTransform;
        srcInstancedStaticMeshComp->GetInstanceTransform(i, instanceTransform);
        auto destActor = world->SpawnActor<AInstancedStaticMeshActor>(
          AInstancedStaticMeshActor::StaticClass(),
          instanceTransform * srcInstancedStaticMeshComp->GetComponentTransform());
        if (!destActor)
        {
          LOG_INFO("Failed to spawn actor");
          continue;
        }
        // mark as static
        destActor->SetMobility(EComponentMobility::Static);
        // set display name
        destActor->SetActorLabel(UKismetSystemLibrary::GetDisplayName(srcActor) + TEXT("_") +
                                 srcInstancedStaticMeshComp->GetStaticMesh()->GetName() + TEXT("_") +
                                 FString::FromInt(i));
        auto destInstancedStaticMeshComp = destActor->GetInstancedStaticMeshComponent();
        destInstancedStaticMeshComp->SetStaticMesh(srcInstancedStaticMeshComp->GetStaticMesh());
        for (auto m = 0; m < srcInstancedStaticMeshComp->GetNumMaterials(); ++m)
          destInstancedStaticMeshComp->SetMaterial(m, srcInstancedStaticMeshComp->GetMaterial(m));
        destInstancedStaticMeshComp->NumCustomDataFloats =
          srcInstancedStaticMeshComp->NumCustomDataFloats;
        const auto inst =
          destInstancedStaticMeshComp->AddInstance(/* indentity transform */ FTransform::Identity);
        // copy custom data values
        for (auto j = 0; j < srcInstancedStaticMeshComp->NumCustomDataFloats; ++j)
        {
          const auto value =
            srcInstancedStaticMeshComp
              ->PerInstanceSMCustomData[j + i * srcInstancedStaticMeshComp->NumCustomDataFloats];
          destInstancedStaticMeshComp->SetCustomDataValue(inst, j, value);
        }
      }
    }
    srcActor->Destroy();
  }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FQofLModule, QofL)
