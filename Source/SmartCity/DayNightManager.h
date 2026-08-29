#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DayNightManager.generated.h"

class UDirectionalLightComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayNightChanged, bool, bIsNight);

UCLASS()
class SMARTCITY_API ADayNightManager : public AActor
{
	GENERATED_BODY()

public:
	ADayNightManager();

	UFUNCTION(BlueprintCallable, Category = "DayNight")
	void ToggleDayNight();

	UFUNCTION(BlueprintCallable, Category = "DayNight")
	void SetNight(bool bNight);

	UFUNCTION(BlueprintPure, Category = "DayNight")
	bool IsNight() const { return bIsNightTime; }

	UPROPERTY(BlueprintAssignable, Category = "DayNight")
	FOnDayNightChanged OnDayNightChanged;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	AActor* SunLightActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float DaySunPitch = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float NightSunPitch = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float TransitionSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float DayLightIntensity = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	float NightLightIntensity = 0.1f;

private:
	bool bIsNightTime = false;
	bool bIsTransitioning = false;
	float TargetPitch;
};
