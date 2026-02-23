#pragma once

#include "CoreMinimal.h"
#include "F4ItemFragment.h"
#include "Engine/DataAsset.h"
#include "F4ItemDefinition.generated.h"

UCLASS()
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
