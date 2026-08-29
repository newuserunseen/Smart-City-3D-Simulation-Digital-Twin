#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmergencyManager.generated.h"

UENUM(BlueprintType)
enum class EEmergencyState : uint8
{
	None,
	Active,
	Resolved
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmergencyStateChanged, EEmergencyState, NewState, FString, Message);

UCLASS()
class SMARTCITY_API AEmergencyManager : public AActor
{
	GENERATED_BODY()

public:
	AEmergencyManager();

	UFUNCTION(BlueprintCallable, Category = "Emergency")
	void TriggerEmergency();

	UFUNCTION(BlueprintPure, Category = "Emergency")
	EEmergencyState GetEmergencyState() const { return CurrentState; }

	UPROPERTY(BlueprintAssignable, Category = "Emergency")
	FOnEmergencyStateChanged OnEmergencyStateChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
	FVector AccidentLocation = FVector(2000.0f, 1000.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
	FString AccidentLocationName = TEXT("Main Road");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
	float ResolutionTime = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emergency")
	TSubclassOf<AActor> MarkerActorClass;

private:
	EEmergencyState CurrentState = EEmergencyState::None;
	FTimerHandle ResolutionTimerHandle;

	UPROPERTY()
	AActor* SpawnedMarker = nullptr;

	UFUNCTION()
	void ResolveEmergency();

	UFUNCTION()
	void ClearEmergency();
};
