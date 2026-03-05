#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class F4_API IInteractable
{
	GENERATED_BODY()

public:
	
	virtual void DoInteract(AActor* Interactor) = 0; 
	
	virtual FText GetInteractionText() const = 0;
};
