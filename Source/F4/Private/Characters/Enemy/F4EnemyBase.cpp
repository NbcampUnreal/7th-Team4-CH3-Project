#include "Characters/Enemy/F4EnemyBase.h"
#include "AIController.h"
#include "AbilitySystem/Attributes/F4AttributeSetEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/WidgetComponent.h"
#include "UI/UW_EnemyWidget.h"

AF4EnemyBase::AF4EnemyBase()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	EnemyAttributeSet = CreateDefaultSubobject<UF4AttributeSetEnemy>(TEXT("EnemyAttributeSet"));
	
	AttributeSet = EnemyAttributeSet;
	
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AF4EnemyBase::InitializeHealthBar()
{
	if (ASC && EnemyAttributeSet)
	{
		if (HealthBarWidget->GetWidget() == nullptr)
		{
			GetWorldTimerManager().SetTimerForNextTick(this, &AF4EnemyBase::InitializeHealthBar);
			return;
		}

		ASC->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetHealthAttribute())
		   .AddUObject(this, &AF4EnemyBase::OnHealthChanged);
		
		float CurrentHealth = EnemyAttributeSet->GetHealth();
		float MaxHealth = EnemyAttributeSet->GetMaxHealth();
		UpdateHealthBar(CurrentHealth, MaxHealth);
		UpdateHealthBarVisibility();
	}
}

void AF4EnemyBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!EnemyAttributeSet) return;
	
	const float CurrentHealth = Data.NewValue; 
	const float MaxHealth = EnemyAttributeSet->GetMaxHealth();

	UpdateHealthBar(CurrentHealth, MaxHealth);
	
	UpdateHealthBarVisibility();
}

void AF4EnemyBase::UpdateHealthBar(float Current, float Max)
{
	if (!HealthBarWidget)
	{
		return;
	}
	
	UUserWidget* RawWidget = HealthBarWidget->GetWidget();
	if (!RawWidget)
	{
		return;
	}
	
	UUW_EnemyWidget* EnemyWidget = Cast<UUW_EnemyWidget>(RawWidget);
	if (!EnemyWidget)
	{
		return;
	}
	
	EnemyWidget->UpdateStatBar(Current, Max);
}

void AF4EnemyBase::UpdateHealthBarVisibility()
{
	if (UUserWidget* RawWidget = HealthBarWidget->GetWidget())
	{
		if (UUW_EnemyWidget* EnemyWidget = Cast<UUW_EnemyWidget>(RawWidget))
		{
			// 현재 체력 상태에 따라 노출 여부 판단
			float CurrentHealth = EnemyAttributeSet->GetHealth();
			float MaxHealth = EnemyAttributeSet->GetMaxHealth();
			
			bool bShouldBeVisible = (CurrentHealth < MaxHealth) && (CurrentHealth > 0.f);
			
			EnemyWidget->UpdateWidgetVisibility(bShouldBeVisible);
		}
	}
}

void AF4EnemyBase::InitializeAttributes()
{
	Super::InitializeAttributes();
	
	if (ASC && DefaultEnemyStatus)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DefaultEnemyStatus, 1.f, EffectContext);
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AF4EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultWeapon();
}

void AF4EnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 무기 액터가 존재한다면 함께 파괴
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
	
	Super::EndPlay(EndPlayReason);
}

void AF4EnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitializeAttributes();
	
	if (ASC && DifficultySideEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
			DifficultySideEffectClass, 
			1.0f, 
			EffectContext
			);
        
		if (SpecHandle.IsValid())
		{
			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
		
		// 체력바 초기화
		InitializeHealthBar();
	}
	
	AAIController* AIC = Cast<AAIController>(NewController);
	if (AIC && BehaviorTree)
	{
		AIC->RunBehaviorTree(BehaviorTree);
		
		UBlackboardComponent* BlackboardComp = AIC->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsVector("HomeLocation", GetActorLocation());
		}
	}
	
	if (ASC)
	{
		FGameplayTag TypeTag = FGameplayTag::RequestGameplayTag("Character.Enemy");
		ASC->AddLooseGameplayTag(TypeTag);
	}
}

void AF4EnemyBase::SpawnDefaultWeapon()
{
	// 무기 클래스가 설정되어 있지 않으면 실행하지 않음
	if (!WeaponClass || !GetMesh()) return;

	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
        
		// 무기 액터 스폰
		EquippedWeapon = World->SpawnActor<AActor>(
			WeaponClass, 
			FVector::ZeroVector, 
			FRotator::ZeroRotator, 
			SpawnParams
			);

		if (EquippedWeapon)
		{
			// 소켓에 자석처럼 붙이기
			// SnapToTarget: 무기의 Transform을 소켓 위치에 강제로 맞춤
			EquippedWeapon->AttachToComponent(
				GetMesh(), 
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				WeaponSocketName
				);
		}
	}
}
