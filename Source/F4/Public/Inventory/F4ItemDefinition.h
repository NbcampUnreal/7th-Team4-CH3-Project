#pragma once

#include "CoreMinimal.h"
#include "F4ItemFragment.h"
#include "Engine/DataAsset.h"
#include "F4ItemDefinition.generated.h"

// DataAsset은 클래스가 아니라 인스턴스를 생성하므로 스폰하려면 필요한 지시자
UCLASS(Blueprintable, BlueprintType)
class F4_API UF4ItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Item")
	TArray<TObjectPtr<UF4ItemFragment>> Fragments;

public:
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		for (const UF4ItemFragment* Fragment : Fragments)
		{
			if (const ResultClass* Result = Cast<ResultClass>(Fragment))
			{
				return Result;
			}
		}
		return nullptr;
	}
};
