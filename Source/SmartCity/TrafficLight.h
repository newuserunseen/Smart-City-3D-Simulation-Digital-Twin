#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrafficLight.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;

UENUM(BlueprintType)
enum class ETrafficLightState : uint8
{
	Red,
	Yellow,
	Green
};

UCLASS()
class SMARTCITY_API ATrafficLight : public AActor
{
	GENERATED_BODY()

public:
	ATrafficLight();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PoleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* RedLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* YellowLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPointLightComponent* GreenLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float RedDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float YellowDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float GreenDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traffic")
	float InitialDelay = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Traffic")
	ETrafficLightState CurrentState = ETrafficLightState::Red;

private:
	UFUNCTION()
	void CycleLight();

	void SetLightState(ETrafficLightState NewState);

	FTimerHandle CycleTimerHandle;
};
