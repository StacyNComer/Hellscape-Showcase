// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
/*
 An enum used to expose an item's type without the need for type casting. 
 This is also used to label what item slots are meant to hold. Use Typeless if you want a slot to be able to hold anything.
 The order of this enum's members is not arbitrary; The item types are declared in the order they'll be be sorted by when the player's inventory is sorted by item type.
*/
enum class EItemType : uint8
{
	//Typeless is used for inventory storage slots (which can hold anything). This could also be used for key items.
	Typeless,
	Focus,
	Spell,
	Charm
};

UCLASS()
class HELLSCAPE_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Display")
		FText itemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Display")
		FText itemTypeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Display")
		/*
		* The UI image of the item.
		*/
		UTexture2D* itemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Display")
		/*
		* A UI image showing the type of item (such as a weapon's damage type) for the item's inventory slot
		*/
		UTexture2D* typeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Display")
		UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Display")
		/*
		* Favorited ites always appear before unfavorited items in the player's inventory.
		* Players should also have the option to sell all non-favorited items in the future.
		*/
		bool favorited;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		/*
		* If true, turns the item into its "Dropped state", enabling collision and physics.
		*/
		virtual void SetDropState(bool isDropped);

};
