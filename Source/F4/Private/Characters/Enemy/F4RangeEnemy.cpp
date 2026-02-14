#include "Characters/Enemy/F4RangeEnemy.h"
#include "Components/SceneComponent.h"

AF4RangeEnemy::AF4RangeEnemy()
{
	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	MuzzleComponent->SetupAttachment(GetMesh()); // 나중에 에디터에서 소켓에 붙이면 됨
}
