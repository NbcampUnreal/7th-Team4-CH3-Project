#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "F4ItemFragment.generated.h"

// 에디터 디테일 패널에서 직접(Inline) 생성하게 해주는 지시자
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class F4_API UF4ItemFragment : public UObject
{
	GENERATED_BODY()
};
