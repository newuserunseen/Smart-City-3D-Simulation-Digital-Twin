#include "StreetLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "DayNightManager.h"
#include "Kismet/GameplayStatics.h"

AStreetLight::AStreetLight()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PoleMesh"));
	PoleMesh->SetupAttachment(Root);

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Root);
	Light->SetRelativeLocation(FVector(0.0f, 100.0f, 800.0f));
	Light->SetIntensity(LightIntensity);
	Light->SetLightColor(LightColor);
	Light->SetAttenuationRadius(1000.0f);
	Light->SetVisibility(false);
}

void AStreetLight::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ADayNightManager* Manager = Cast<ADayNightManager>(FoundActors[0]);
		if (Manager)
		{
			Manager->OnDayNightChanged.AddDynamic(this, &AStreetLight::OnDayNightChanged);
			Light->SetVisibility(Manager->IsNight());
		}
	}
}

void AStreetLight::OnDayNightChanged(bool bIsNight)
{
	Light->SetVisibility(bIsNight);
}
