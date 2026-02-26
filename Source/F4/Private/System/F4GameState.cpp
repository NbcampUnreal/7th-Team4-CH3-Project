#include "System/F4GameState.h"

void AF4GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SurvivalTimer += DeltaTime; 
	
	// TODO:  시간 마다 난이도 증가 로직
	
}
