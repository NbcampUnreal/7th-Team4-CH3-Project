#include "Characters/Enemy/EnemySpawner.h"
#include "Characters/Enemy/F4EnemyBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "System/F4GameState.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 루트 컴포넌트 설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	// 시각화용 구체 생성
	VisualizeRadius = CreateDefaultSubobject<USphereComponent>(TEXT("VisualizeRadius"));
	VisualizeRadius->SetupAttachment(RootComponent);
    
	// 게임 중에는 안 보이게 설정, 충돌도 끔
	VisualizeRadius->SetCanEverAffectNavigation(false);
	VisualizeRadius->SetCollisionProfileName(TEXT("NoCollision"));
	VisualizeRadius->bHiddenInGame = true;

	// 초기 반지름 설정
	VisualizeRadius->SetSphereRadius(SpawnRadius);
	
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (DistanceCheckTarget == nullptr)
	{
		DistanceCheckTarget = UGameplayStatics::GetPlayerPawn(this, 0);
	}
	
	// 레벨 진입시 1회 즉시 스폰
	TrySpawnBatch();
	// 일정시간마다 스폰함수 호출
	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle, 
		this,
		&AEnemySpawner::TrySpawnBatch, 
		SpawnInterval, 
		true
		);
}

bool AEnemySpawner::IsTargetTooClose() const
{
	if (!DistanceCheckTarget) return false;
	
	float Distance = FVector::Distance(GetActorLocation(), DistanceCheckTarget->GetActorLocation());
	UE_LOG(LogTemp, Log, TEXT("Distance: %f"), Distance);
	return Distance < SafeDistance;
}

void AEnemySpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	FName PropertyName 
			= (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// SpawnRadius 변수가 수정되었을 때만 실행
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AEnemySpawner, SpawnRadius))
	{
		if (VisualizeRadius)
		{
			VisualizeRadius->SetSphereRadius(SpawnRadius);
		}
	}
}

int32 AEnemySpawner::GetCurrentMaxCount() const
{
	int32 FinalMaxCount = BaseMaxEnemyCount;

	if (UWorld* World = GetWorld())
	{
		if (AF4GameState* GameState = World->GetGameState<AF4GameState>())
		{
			int32 IncreaseAmount = (GameState->DifficultyPhase - 1) * IncreaseEnemyCount;
			
			// 음수 상승치 방지
			if (IncreaseAmount < 0)
			{
				IncreaseAmount = 0;
			}

			FinalMaxCount += IncreaseAmount;
			
			// 상한 적용
			if (FinalMaxCount > SpawnLimit)
			{
				FinalMaxCount = SpawnLimit;
			}
		}
	}

	return FinalMaxCount;
}

void AEnemySpawner::CleanUpList()
{
	SpawnedEnemies.RemoveAll(
		[](AActor* Enemy) 
		{
			return !IsValid(Enemy);
		}
	);
}

void AEnemySpawner::TrySpawnBatch()
{
	// 플레이어가 SafeDistance 거리 안에 있으면 적 스폰 제한
	if (IsTargetTooClose()) 
	{
		UE_LOG(LogTemp, Log, TEXT("Spawn skipped: Target too close."));
		return;
	}

	// 리스트 정리 및 부족한 수 계산
	CleanUpList();
    
	int32 MaxCount = GetCurrentMaxCount();
	int32 AmountToSpawn = MaxCount - SpawnedEnemies.Num();

	if (AmountToSpawn <= 0 || EnemyClasses.Num() == 0) 
	{
		return;
	}

	// 일괄 스폰
	UWorld* World = GetWorld();
	if (World == nullptr) 
	{
		return;
	}

	for (int32 i = 0; i < AmountToSpawn; ++i)
	{
		// 평면상의 랜덤 위치 계산
		FVector2D RandomCircle = FMath::RandPointInCircle(SpawnRadius);
        
		FVector TraceStart = GetActorLocation() + FVector(RandomCircle.X, RandomCircle.Y, TraceHeight);
		FVector TraceEnd = TraceStart - FVector(0.f, 0.f, TraceHeight + TraceDepth);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 스포너 자신은 레이저에 맞지 않도록 설정
		
		if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			// 실제 레이저가 닿은 지면 위치
			FVector SpawnPos = HitResult.Location;
			// 캐릭터의 발이 땅에 박히지 않게 살짝 띄워줌
			SpawnPos.Z += 50.f;

			int32 RandIdx = FMath::RandRange(0, EnemyClasses.Num() - 1);
            
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AF4EnemyBase* NewEnemy = World->SpawnActor<AF4EnemyBase>(
				EnemyClasses[RandIdx], 
				SpawnPos, 
				FRotator::ZeroRotator, 
				Params
				);
            
			if (NewEnemy)
			{
				SpawnedEnemies.Add(NewEnemy);
			}
		}
	}
}
