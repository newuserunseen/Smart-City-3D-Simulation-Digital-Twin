#include "DayNightManager.h"
#include "Components/DirectionalLightComponent.h"

ADayNightManager::ADayNightManager()
{
	PrimaryActorTick.bCanEverTick = true;
	TargetPitch = DaySunPitch;
}

void ADayNightManager::BeginPlay()
{
	Super::BeginPlay();
	TargetPitch = DaySunPitch;
}

void ADayNightManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsTransitioning || !SunLightActor) return;

	FRotator CurrentRot = SunLightActor->GetActorRotation();
	float NewPitch = FMath::FInterpTo(CurrentRot.Pitch, TargetPitch, DeltaTime, TransitionSpeed);
	SunLightActor->SetActorRotation(FRotator(NewPitch, CurrentRot.Yaw, CurrentRot.Roll));

	if (FMath::IsNearlyEqual(NewPitch, TargetPitch, 0.5f))
	{
		bIsTransitioning = false;
		SunLightActor->SetActorRotation(FRotator(TargetPitch, CurrentRot.Yaw, CurrentRot.Roll));
	}

	UDirectionalLightComponent* LightComp = SunLightActor->FindComponentByClass<UDirectionalLightComponent>();
	if (LightComp)
	{
		float TargetIntensity = bIsNightTime ? NightLightIntensity : DayLightIntensity;
		float NewIntensity = FMath::FInterpTo(LightComp->Intensity, TargetIntensity, DeltaTime, TransitionSpeed);
		LightComp->SetIntensity(NewIntensity);
	}
}

void ADayNightManager::ToggleDayNight()
{
	SetNight(!bIsNightTime);
}

void ADayNightManager::SetNight(bool bNight)
{
	bIsNightTime = bNight;
	TargetPitch = bNight ? NightSunPitch : DaySunPitch;
	bIsTransitioning = true;
	OnDayNightChanged.Broadcast(bIsNightTime);
}
