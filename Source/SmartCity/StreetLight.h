#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StreetLight.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;

UCLASS()
class SMARTCITY_API AStreetLight : public AActor
{
	GENERATED_BODY()

public:
	AStreetLight();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PoleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StreetLight")
	float LightIntensity = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StreetLight")
	FLinearColor LightColor = FLinearColor(1.0f, 0.9f, 0.7f);

private:
	UFUNCTION()
	void OnDayNightChanged(bool bIsNight);
};
