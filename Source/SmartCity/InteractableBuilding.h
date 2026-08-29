#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBuilding.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;
class ADayNightManager;

USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuildingName = TEXT("Building");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuildingType = TEXT("Commercial");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Capacity = TEXT("N/A");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Status = TEXT("Operational");
};

UCLASS()
class SMARTCITY_API AInteractableBuilding : public AActor
{
	GENERATED_BODY()

public:
	AInteractableBuilding();

	UFUNCTION(BlueprintPure, Category = "Building")
	const FBuildingData& GetBuildingData() const { return BuildingInfo; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	FBuildingData BuildingInfo;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* InteriorLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
	float InteriorLightIntensity = 5000.0f;

private:
	UFUNCTION()
	void OnDayNightChanged(bool bIsNight);
};
