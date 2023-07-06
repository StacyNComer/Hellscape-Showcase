// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterBase.h"
#include "Hellscape.h"
#include <algorithm>
#include "AttackDamageType.h"
#include "PlayerHUDWidget.h"
#include "WeaponWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
	//Widget Creation
	static ConstructorHelpers::FClassFinder<UPlayerHUDWidget> PlayerHUDClassFinder(TEXT("/Game/Blueprints/UI/PlayerHUD_WBP"));

	if (PlayerHUDClassFinder.Class != nullptr)
	{
		PlayerHUDClass = PlayerHUDClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UInventoryWidget> InventoryClassFinder(TEXT("/Game/Blueprints/UI/PlayerInventory_WBP"));

	if (InventoryClassFinder.Class != nullptr)
	{
		InventoryClass = InventoryClassFinder.Class;
	}

 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Inventory
	fociEquipped.Init(nullptr, 2);
	spellsEquipped.Init(nullptr, 2);

	//Components
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->SetRelativeLocation(FVector(-40,0,64));
	Camera->bUsePawnControlRotation = true;

	WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Attach Point"));
	WeaponAttachPoint->SetupAttachment(Camera);
	WeaponAttachPoint->SetRelativeLocation(FVector(0, 0, -24));

	//Movement Component
	JumpMaxCount = 2;
	UCharacterMovementComponent* moveComp = GetCharacterMovement();
	moveComp->MaxAcceleration = 9999;
	moveComp->MaxWalkSpeed = 1000;
	moveComp->JumpZVelocity = 525;
	moveComp->AirControl = .6;

	//Stats
	currentHealth = maxHealth;
	currentMana = maxMana;
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	HudUI = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDClass);
	HudUI->AddToViewport();
	HudUI->owningPlayer = this;

	InventoryUI = CreateWidget<UInventoryWidget>(GetWorld(), InventoryClass);
	InventoryUI->AddToViewport();
	InventoryUI->owningPlayer = this;

	//Hide character menu UI
	InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (focusAxisSwapDelayTracker > 0)
		focusAxisSwapDelayTracker -= DeltaTime;

	//DrawDebugLine(GetWorld(), Camera->GetComponentLocation(), Camera->GetComponentLocation() + Camera->GetForwardVector() * 600, FColor::Red, false, 0.1f);

	/*
	FHitResult pickupHit;
	if (GetWorld()->LineTraceSingleByObjectType
	(
		pickupHit,
		Camera->GetComponentLocation(),
		Camera->GetComponentLocation() + Camera->GetForwardVector() * interactRange,
		FCollisionObjectQueryParams(ECC_PICKUP)
	))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("THOT DETECTED"));
	}
	*/
}

void APlayerCharacterBase::FireSpell(ASpellBase* spell)
{
	if (spell)
	{
		spell->AttemptCast();
	}
}

#pragma region Input
// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	#pragma region Movement
	PlayerInputComponent->BindAxis
	(
		"MoveForward",
		this,
		&APlayerCharacterBase::ForwardMovement
	);

	PlayerInputComponent->BindAxis
	(
		"MoveRight",
		this,
		&APlayerCharacterBase::LateralMovement
	);

	PlayerInputComponent->BindAxis
	(
		"Turn",
		this,
		&APlayerCharacterBase::AddControllerYawInput
	);

	PlayerInputComponent->BindAxis
	(
		"LookUp",
		this,
		&APlayerCharacterBase::AddControllerPitchInput
	);

	PlayerInputComponent->BindAction
	(
		"Jump",
		IE_Pressed,
		this,
		&APlayerCharacterBase::Jump
	);

	PlayerInputComponent->BindAction
	(
		"Jump",
		IE_Released,
		this,
		&APlayerCharacterBase::StopJumping
	);
	#pragma endregion
	
	#pragma region Combat
		//Combat
	PlayerInputComponent->BindAction
	(
		"Fire",
		IE_Pressed,
		this,
		&APlayerCharacterBase::FireWieldedFocus
	);

	PlayerInputComponent->BindAction
	(
		"Fire",
		IE_Released,
		this,
		&APlayerCharacterBase::StopFiringWieldedFocus
	);

	PlayerInputComponent->BindAction
	(
		"NextFocus",
		IE_Pressed,
		this,
		&APlayerCharacterBase::SwapNextFocus
	);

	PlayerInputComponent->BindAction
	(
		"PrevFocus",
		IE_Pressed,
		this,
		&APlayerCharacterBase::SwapPrevFocus
	);

	PlayerInputComponent->BindAxis
	(
		"SwapFocusAxis",
		this,
		&APlayerCharacterBase::AxisSwapFocus
	);

	PlayerInputComponent->BindAction
	(
		"Spell1",
		IE_Pressed,
		this,
		&APlayerCharacterBase::FireSpell1
	);

	PlayerInputComponent->BindAction
	(
		"Spell2",
		IE_Pressed,
		this,
		&APlayerCharacterBase::FireSpell2
	);
	#pragma endregion

	#pragma region Misc
	PlayerInputComponent->BindAction
	(
		"Interact",
		IE_Pressed,
		this,
		&APlayerCharacterBase::Interact
	);
	#pragma endregion

	#pragma region Gamepad Axis
	PlayerInputComponent->BindAxis
	(
		"TurnRate",
		this,
		&APlayerCharacterBase::GamepadTurnCharacter
	);

	PlayerInputComponent->BindAxis
	(
		"LookUpRate",
		this,
		&APlayerCharacterBase::GamepadLookUp
	);
	#pragma endregion
}

void APlayerCharacterBase::ForwardMovement(float axisValue)
{
	AddMovementInput(GetActorForwardVector(), axisValue);
}

void APlayerCharacterBase::LateralMovement(float axisValue)
{
	AddMovementInput(GetActorRightVector(), axisValue);
}

void APlayerCharacterBase::TurnCharacter(float axisValue)
{
	AddControllerYawInput(axisValue);
}

void APlayerCharacterBase::LookUp(float axisValue)
{
	AddControllerPitchInput(axisValue);
}

void APlayerCharacterBase::GamepadTurnCharacter(float axisValue)
{
	AddControllerYawInput(axisValue * turnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void APlayerCharacterBase::GamepadLookUp(float axisValue)
{
	AddControllerPitchInput(axisValue * turnRate * UGameplayStatics::GetWorldDeltaSeconds(this));
}

void APlayerCharacterBase::FireWieldedFocus()
{
	AFocusBase* toFire = GetFocusWielded();
	if (toFire)
	{
		toFire->StartCasting();
	}
}

void APlayerCharacterBase::StopFiringWieldedFocus()
{
	AFocusBase* toFire = GetFocusWielded();
	if (toFire)
	{
		toFire->StopCasting();
	}
}

void APlayerCharacterBase::SwapNextFocus()
{
	AFocusBase* toWield;
	int i = focusSlotWielded;
	
	do
	{
		i++;
		if (i == fociEquipped.Num())
			i = 0;

		toWield = fociEquipped[i];

		if (toWield && toWield != GetFocusWielded())
		{
			WieldFocusSlot(i);
			break;
		}

	} while (toWield != GetFocusWielded());
}

void APlayerCharacterBase::SwapPrevFocus()
{
	AFocusBase* toWield;
	int i = focusSlotWielded;

	do
	{
		i--;
		if (i < 0)
			i = fociEquipped.Num()-1;

		toWield = fociEquipped[i];

		if (toWield && toWield != GetFocusWielded())
		{
			WieldFocusSlot(i);
			break;
		}

	} while (toWield != GetFocusWielded());
}

void APlayerCharacterBase::AxisSwapFocus(float axisValue)
{
	if (focusAxisSwapDelayTracker > 0)
	{
	}
	else if(axisValue != 0)
	{
		if (axisValue > 0)
			SwapNextFocus();
		else if (axisValue < 0)
			SwapPrevFocus();

		focusAxisSwapDelayTracker = focusAxisSwapDelay;
	}
	
}

void APlayerCharacterBase::Interact()
{
	FHitResult pickupHit;
	if (GetWorld()->LineTraceSingleByObjectType
	(
		pickupHit,
		Camera->GetComponentLocation(),
		Camera->GetComponentLocation() + Camera->GetForwardVector() * interactRange,
		FCollisionObjectQueryParams(ECC_PICKUP)
	))
	{
		AItemBase* item = Cast<AItemBase>(pickupHit.Actor);
		PickupItem(item);
	}
}

void APlayerCharacterBase::ToggleCharacterMenu()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (inCharacterMenu)
	{
			//Hide the Character View if it is being displayed

		InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
			//Show the Character View if the player isn't in it.

		GetCharacterMovement()->StopMovementImmediately();

		UpdateInventoryWidget();
		InventoryUI->SetVisibility(ESlateVisibility::Visible);
	}
	
	inCharacterMenu = !inCharacterMenu;
}

void APlayerCharacterBase::FireWeapon(AWeaponBase* weapon)
{
	weapon->StartCasting();
}

void APlayerCharacterBase::FireSpell1()
{
	FireSpell(spellsEquipped[0]);
}

void APlayerCharacterBase::FireSpell2()
{
	FireSpell(spellsEquipped[1]);
}
#pragma endregion

float APlayerCharacterBase::GetStatusBuildup(EDamageType damageType)
{
	return statusEffectBuildup[damageType];
}

float APlayerCharacterBase::SetStatusBuildup(EDamageType damageType, float value)
{
	statusEffectBuildup[damageType] = GetStatusBuildup(damageType) + value;
	return statusEffectBuildup[damageType];
}

void APlayerCharacterBase::ApplyStatusEffect(AEnemyBase* target, EStatusEffect statusEffect, AWeaponBase* weaponUsed)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("FROZEN!!!"));

	switch (statusEffect)
	{
		case EStatusEffect::Stagger:
			break;
		case EStatusEffect::Burn:
			break;
		case EStatusEffect::Irradiate:
			break;
		case EStatusEffect::Freeze:
			target->isStunned++;
			GetWorldTimerManager().SetTimer(target->statusTimer[statusEffect], target, &AEnemyBase::ReduceStun, 6, false);
			break;
		case EStatusEffect::Shock:
			break;
		case EStatusEffect::Seal:
			break;
		default:
			break;
	}
}

#pragma region Inventory

AItemBase* APlayerCharacterBase::GetInventoryItem(int32 index)
{
	return inventory[index];
}

bool APlayerCharacterBase::AddInventoryItem(AItemBase* item)
{
	if (itemsInInventory == inventorySize)
	{
		return false;
	}
	else
	{
		inventory[itemsInInventory] = item;
		itemsInInventory++;
		return true;
	}
}

void APlayerCharacterBase::EquipSpell(int32 index, ASpellBase* spell)
{
	spellsEquipped[index] = spell;
	HudUI->SetEquippedSpell(index, spell);
}

AItemBase* APlayerCharacterBase::RemoveInventoryItem(int32 index)
{
	auto removed = inventory[index];
	inventory[index] = nullptr;

	for (int i = index + 1; i < GetInventorySize() && inventory[i] != nullptr; i++)
	{
		inventory[i - 1] = inventory[i];
	}

	itemsInInventory--;
	inventory[itemsInInventory] = nullptr;
	return removed;
}

void APlayerCharacterBase::WieldFocusSlot(int32 slot)
{
	AFocusBase* focus = fociEquipped[slot]; //The game should never try and equip a focus slot with nothing in it!

	if (GetFocusWielded() != nullptr)
	{
		GetFocusWielded()->SetWielded(false);
	}

	focusSlotWielded = slot;
	focus->SetWielded(true);
	focus->wieldingPlayer = this;
	focus->AttachToComponent(WeaponAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

AFocusBase* APlayerCharacterBase::GetFocusWielded()
{
	return fociEquipped[focusSlotWielded];
}

void APlayerCharacterBase::PickupItem(AItemBase* item)
{
	if (AddInventoryItem(item))
	{
		item->SetDropState(false);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Inventory Full"));

		//Make items a wittle hop when the player's inventory is full.
		item->Mesh->AddImpulse(FVector::UpVector*200, NAME_None, true);
	}
}

void APlayerCharacterBase::DropItem(int32 index)
{

}

int APlayerCharacterBase::GetInventorySize()
{
	return inventorySize;
}

void APlayerCharacterBase::SortInventory()
{
	switch (inventorySortType)
	{
	case EItemSortType::Name:
		std::sort(inventory, inventory + GetInventorySize(), itemNameComparator);
		break;
	case EItemSortType::ItemType:
		std::sort(inventory, inventory + GetInventorySize(), itemTypeComparator);
		break;
	default:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("UNDEFINED SORTING TYPE! COMBUSTION IMINENT!"));
		break;
	}
}

void APlayerCharacterBase::SetInventorySortType(EItemSortType sortType)
{
	inventorySortType = sortType;

	SortInventory();
}

void APlayerCharacterBase::UpdateInventoryWidget()
{
	SortInventory();
	InventoryUI->UpdateDisplay();
}

	//Sorting Methods

bool APlayerCharacterBase::itemNameComparator(AItemBase* item1, AItemBase* item2)
{
	if (item1 == nullptr || item2 == nullptr)
		return item2 == nullptr;
	else if (item1->favorited == item2->favorited)
		return item1->itemName.ToString() < item2->itemName.ToString();
	else
		return item1->favorited;
}

bool APlayerCharacterBase::itemTypeComparator(AItemBase* item1, AItemBase* item2)
{
	if (item1 == nullptr || item2 == nullptr)
	{
		return item2 == nullptr;
	}
	else if (item1->favorited == item2->favorited)
	{
		auto type1 = item1->itemType;
		auto type2 = item2->itemType;
		if (type1 == type2)
			return item1->itemName.ToString() < item2->itemName.ToString();
		else
			return type1 < type2;
	}
	else
	{
		return item1->favorited;
	}
}

#pragma endregion

