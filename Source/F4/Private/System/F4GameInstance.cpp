#include "System/F4GameInstance.h"
#include "Characters/Player/F4PlayerCharacter.h"
#include "Components/AudioComponent.h"
#include "Inventory/F4InventoryComponent.h"
#include "Inventory/F4EquipmentComponent.h"
#include "Inventory/F4QuickSlotComponent.h"
#include "Inventory/F4ItemInstance.h"

#include "Kismet/GameplayStatics.h"

void UF4GameInstance::Init()
{
	Super::Init();
	
	BGMPlayer = NewObject<UAudioComponent>(this);
	if (BGMPlayer)
	{
		BGMPlayer->RegisterComponent(); 
		BGMPlayer->bAutoDestroy = false; 
		BGMPlayer->bIsUISound = true;   
	}
}

void UF4GameInstance::WipeData()
{
	SavedInventory = FInventorySaveData();
	bIsDeathTransition = false;
}

void UF4GameInstance::SaveData(AF4PlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter)
	{
		return;
	}

	UF4InventoryComponent* InventoryComp = PlayerCharacter->FindComponentByClass<UF4InventoryComponent>();
	UF4EquipmentComponent* EquipmentComp = PlayerCharacter->FindComponentByClass<UF4EquipmentComponent>();
	UF4QuickSlotComponent* QuickSlotComp = PlayerCharacter->FindComponentByClass<UF4QuickSlotComponent>();

	if (!InventoryComp || !EquipmentComp || !QuickSlotComp)
	{
		return;
	}

	WipeData();

	const TMap<UF4ItemInstance*, int32> ItemIndexMap = SaveInventoryItems(InventoryComp);
	SaveEquipmentSlots(EquipmentComp, ItemIndexMap);
	SaveQuickSlots(QuickSlotComp, ItemIndexMap);

	SavedInventory.bHasSavedData = true;
}

void UF4GameInstance::RestoreData(AF4PlayerCharacter* PlayerCharacter)
{
	if (!PlayerCharacter || !SavedInventory.bHasSavedData)
	{
		return;
	}

	UF4InventoryComponent* InventoryComp = PlayerCharacter->FindComponentByClass<UF4InventoryComponent>();
	UF4EquipmentComponent* EquipmentComp = PlayerCharacter->FindComponentByClass<UF4EquipmentComponent>();
	UF4QuickSlotComponent* QuickSlotComp = PlayerCharacter->FindComponentByClass<UF4QuickSlotComponent>();

	if (!InventoryComp || !EquipmentComp || !QuickSlotComp)
	{
		return;
	}

	const TArray<UF4ItemInstance*> RestoredItems = RestoreInventoryItems(InventoryComp, PlayerCharacter);
	RestoreEquipmentSlots(EquipmentComp, RestoredItems);
	RestoreQuickSlots(QuickSlotComp, RestoredItems);
}

TMap<UF4ItemInstance*, int32> UF4GameInstance::SaveInventoryItems(UF4InventoryComponent* InventoryComp)
{
	TMap<UF4ItemInstance*, int32> ItemIndexMap;
	const TArray<TObjectPtr<UF4ItemInstance>>& Items = InventoryComp->GetInventoryItems();

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		UF4ItemInstance* Item = Items[i];
		if (!Item)
		{
			continue;
		}

		FItemSaveData ItemData;
		ItemData.ItemDefinition = Item->ItemDefinition;
		ItemData.Quantity = Item->Quantity;
		ItemData.DynamicStats = Item->DynamicStats;

		SavedInventory.InventoryItems.Add(ItemData);
		ItemIndexMap.Add(Item, i);
	}

	return ItemIndexMap;
}

void UF4GameInstance::SaveEquipmentSlots(UF4EquipmentComponent* EquipmentComp, const TMap<UF4ItemInstance*, int32>& ItemIndexMap)
{
	auto FindIndex = [&ItemIndexMap](UF4ItemInstance* Item) -> int32
	{
		const int32* Found = Item ? ItemIndexMap.Find(Item) : nullptr;
		return Found ? *Found : -1;
	};

	SavedInventory.PrimaryWeaponIndex   = FindIndex(EquipmentComp->GetWeaponInSlot(EWeaponSlot::Primary));
	SavedInventory.SecondaryWeaponIndex = FindIndex(EquipmentComp->GetWeaponInSlot(EWeaponSlot::Secondary));
	SavedInventory.ActiveWeaponSlot     = static_cast<uint8>(EquipmentComp->GetActiveSlot());
}

void UF4GameInstance::SaveQuickSlots(UF4QuickSlotComponent* QuickSlotComp, const TMap<UF4ItemInstance*, int32>& ItemIndexMap)
{
	SavedInventory.QuickSlotIndices.SetNum(UF4QuickSlotComponent::TotalSlotCount);

	for (int32 i = 0; i < UF4QuickSlotComponent::TotalSlotCount; ++i)
	{
		UF4ItemInstance* SlotItem = QuickSlotComp->GetItemAtIndex(i);
		const int32* Found = SlotItem ? ItemIndexMap.Find(SlotItem) : nullptr;
		SavedInventory.QuickSlotIndices[i] = Found ? *Found : -1;
	}
}

TArray<UF4ItemInstance*> UF4GameInstance::RestoreInventoryItems(UF4InventoryComponent* InventoryComp, AActor* Outer)
{
	TArray<UF4ItemInstance*> RestoredItems;
	RestoredItems.Reserve(SavedInventory.InventoryItems.Num());

	for (const FItemSaveData& ItemData : SavedInventory.InventoryItems)
	{
		if (!ItemData.ItemDefinition)
		{
			RestoredItems.Add(nullptr);
			continue;
		}

		UF4ItemInstance* NewItem = NewObject<UF4ItemInstance>(Outer);
		NewItem->ItemDefinition = ItemData.ItemDefinition;
		NewItem->Quantity = ItemData.Quantity;
		NewItem->DynamicStats = ItemData.DynamicStats;

		InventoryComp->LoadItem(NewItem);
		RestoredItems.Add(NewItem);
	}

	return RestoredItems;
}

void UF4GameInstance::RestoreEquipmentSlots(UF4EquipmentComponent* EquipmentComp, const TArray<UF4ItemInstance*>& RestoredItems)
{
	auto TryEquip = [&](int32 Index, EWeaponSlot Slot)
	{
		if (Index >= 0 && RestoredItems.IsValidIndex(Index) && RestoredItems[Index])
		{
			EquipmentComp->EquipItemToSlot(RestoredItems[Index], Slot);
		}
	};

	TryEquip(SavedInventory.PrimaryWeaponIndex, EWeaponSlot::Primary);
	TryEquip(SavedInventory.SecondaryWeaponIndex, EWeaponSlot::Secondary);
}

void UF4GameInstance::RestoreQuickSlots(UF4QuickSlotComponent* QuickSlotComp, const TArray<UF4ItemInstance*>& RestoredItems)
{
	for (int32 i = 0; i < UF4QuickSlotComponent::TotalSlotCount; ++i)
	{
		if (!SavedInventory.QuickSlotIndices.IsValidIndex(i))
		{
			continue;
		}

		int32 ItemIndex = SavedInventory.QuickSlotIndices[i];
		if (ItemIndex >= 0 && RestoredItems.IsValidIndex(ItemIndex) && RestoredItems[ItemIndex])
		{
			QuickSlotComp->RegisterItem(i, RestoredItems[ItemIndex]);
		}
	}
}

void UF4GameInstance::IncrementPermanentDifficulty()
{
	if (bAllowDifficultyScaling)
	{
		PermanentDifficulty++;
		UE_LOG(LogTemp, Log, TEXT("Permanent Difficulty Increased! New Level: %d"), PermanentDifficulty);
	}
}

void UF4GameInstance::PlayBGM(USoundBase* NewBGM, bool bFade)
{
	if (!NewBGM || !BGMPlayer) return;
	
	BGMPlayer->bIsUISound = true; 
	BGMPlayer->bAllowSpatialization = false;
	BGMPlayer->SetVolumeMultiplier(1.5f);    // 테스트를 위해 볼륨을 크게 키움
	
	if (BGMPlayer->IsPlaying() && BGMPlayer->GetSound() == NewBGM) return;

	
	if (bFade)
	{
		BGMPlayer->FadeOut(0.5f, 0.0f);
		BGMPlayer->Activate(true);
		BGMPlayer->SetSound(NewBGM);
		BGMPlayer->FadeIn(1.0f, 1.0f);
	}
	else
	{
		BGMPlayer->Stop();
		BGMPlayer->SetSound(NewBGM);
		BGMPlayer->Activate(true);
		BGMPlayer->Play();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("BGM Debug | Playing: %s | Volume: %f | Sound: %s | IsUI: %s"), 
	BGMPlayer->IsPlaying() ? TEXT("Yes") : TEXT("No"),
	BGMPlayer->VolumeMultiplier,
	BGMPlayer->GetSound() ? *BGMPlayer->GetSound()->GetName() : TEXT("None"),
	BGMPlayer->bIsUISound ? TEXT("Yes") : TEXT("No"));
}

void UF4GameInstance::StopBGM(float FadeTime)
{
	if (!BGMPlayer) return;
	
	BGMPlayer->FadeOut(FadeTime, 0.0f);
	BGMPlayer->Stop(); 
}

void UF4GameInstance::MoveToLevel(FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
