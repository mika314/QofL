# QofL
The purpose of the life of this plugin is to reduce the amount of typing while writing C++ scripts for Unreal Engine projects.

## Instalation
- Drop the source directory inside `your_unreal_project/Plugins/QofL/`
- Add a `QofL` dependency in `your_unreal_project/Source/your_unreal_project/your_unreal_project.Build.cs` file, e.g.:
```C#
PrivateDependencyModuleNames.AddRange(new string[] { "HTTP", "Json", "JsonUtilities", "AIModule", "Niagara", "QofL" });
```

## Logging

```C++
#include <QofL/log.h>
/*...*/
LOG(getNpcName(), "Invalid response");
```

## Check, Log and Return

```C++
#include <QofL/check_ret.h>
/*...*/
auto npc = gpt3.getNpc();
CHECK_RET(npc);
```

If function returns non-void, you can put return value after check

```C++
#include <QofL/check_ret.h>
/*...*/
auto npc = gpt3.getNpc();
CHECK_RET(npc, false);
```

## Objects Finder

```C++
#include <QofL/obj_finder.h>
/* ... */
GetStaticMeshComponent()->SetStaticMesh(OBJ_FINDER(StaticMesh, "BathroomAndShowersPack/Mesh_01", "SM_Shower_Bathtub"));
```

## Class Finder

```C++
#include <QofL/class_finder.h>
/* ... */
  mesh->SetAnimInstanceClass(CLASS_FINDER(AnimInstance, "Animation", "BP_DefaultMaleCharacterAnim"));
```

## Get List of Overlaping Objects

```C++
#include <QofL/get_overlapping.h>
/* ... */
auto overlappingRooms = getOverlapping<ARoom>(*pawn);
```
