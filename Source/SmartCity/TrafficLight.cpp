#include "TrafficLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"

ATrafficLight::ATrafficLight()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoleMesh"));
	PoleMesh->SetupAttachment(Root);

	RedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("RedLight"));
	RedLight->SetupAttachment(Root);
	RedLight->SetRelativeLocation(FVector(0.0f, 30.0f, 650.0f));
	RedLight->SetLightColor(FLinearColor::Red);
	RedLight->SetIntensity(5000.0f);
	RedLight->SetAttenuationRadius(200.0f);

	YellowLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("YellowLight"));
	YellowLight->SetupAttachment(Root);
	YellowLight->SetRelativeLocation(FVector(0.0f, 30.0f, 600.0f));
	YellowLight->SetLightColor(FLinearColor(1.0f, 0.8f, 0.0f));
	YellowLight->SetIntensity(5000.0f);
	YellowLight->SetAttenuationRadius(200.0f);

	GreenLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("GreenLight"));
	GreenLight->SetupAttachment(Root);
	GreenLight->SetRelativeLocation(FVector(0.0f, 30.0f, 550.0f));
	GreenLight->SetLightColor(FLinearColor::Green);
	GreenLight->SetIntensity(5000.0f);
	GreenLight->SetAttenuationRadius(200.0f);
}

void ATrafficLight::BeginPlay()
{
	Super::BeginPlay();
	SetLightState(ETrafficLightState::Red);

	float Delay = InitialDelay > 0.0f ? InitialDelay : RedDuration;
	GetWorldTimerManager().SetTimer(CycleTimerHandle, this, &ATrafficLight::CycleLight, Delay, false);
}

void ATrafficLight::CycleLight()
{
	float NextDuration = 0.0f;

	switch (CurrentState)
	{
	case ETrafficLightState::Red:
		SetLightState(ETrafficLightState::Green);
		NextDuration = GreenDuration;
		break;
	case ETrafficLightState::Green:
		SetLightState(ETrafficLightState::Yellow);
		NextDuration = YellowDuration;
		break;
	case ETrafficLightState::Yellow:
		SetLightState(ETrafficLightState::Red);
		NextDuration = RedDuration;
		break;
	}

	GetWorldTimerManager().SetTimer(CycleTimerHandle, this, &ATrafficLight::CycleLight, NextDuration, false);
}

void ATrafficLight::SetLightState(ETrafficLightState NewState)
{
	CurrentState = NewState;

	RedLight->SetVisibility(NewState == ETrafficLightState::Red);
	YellowLight->SetVisibility(NewState == ETrafficLightState::Yellow);
	GreenLight->SetVisibility(NewState == ETrafficLightState::Green);
}
