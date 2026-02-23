#pragma once

#include "CoreMinimal.h"
#include "Inventory/F4ItemFragment.h"
#include "F4ItemFragment_UI.generated.h"

UCLASS()
class F4_API UF4ItemFragment_UI : public UF4ItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText ItemDescription;
};
