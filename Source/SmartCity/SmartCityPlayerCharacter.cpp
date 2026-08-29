#include "SmartCityPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SmartCityHUDWidget.h"
#include "InteractableBuilding.h"
#include "DayNightManager.h"
#include "EmergencyManager.h"
#include "Kismet/GameplayStatics.h"

ASmartCityPlayerCharacter::ASmartCityPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->JumpZVelocity = 420.0f;
	GetCharacterMovement()->AirControl = 0.2f;
}

void ASmartCityPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<USmartCityHUDWidget>(PC, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}

	TArray<AActor*> EmergencyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEmergencyManager::StaticClass(), EmergencyActors);
	if (EmergencyActors.Num() > 0)
	{
		AEmergencyManager* EmMgr = Cast<AEmergencyManager>(EmergencyActors[0]);
		if (EmMgr)
		{
			EmMgr->OnEmergencyStateChanged.AddDynamic(this, &ASmartCityPlayerCharacter::OnEmergencyStateChanged);
		}
	}

	TArray<AActor*> DayNightActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightManager::StaticClass(), DayNightActors);
	if (DayNightActors.Num() > 0)
	{
		ADayNightManager* DNMgr = Cast<ADayNightManager>(DayNightActors[0]);
		if (DNMgr)
		{
			DNMgr->OnDayNightChanged.AddDynamic(this, &ASmartCityPlayerCharacter::OnDayNightChanged);
		}
	}
}

void ASmartCityPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

void ASmartCityPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput) return;

	if (MoveAction)
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASmartCityPlayerCharacter::Move);

	if (LookAction)
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASmartCityPlayerCharacter::Look);

	if (JumpAction)
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

	if (SprintAction)
	{
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASmartCityPlayerCharacter::StartSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASmartCityPlayerCharacter::StopSprint);
	}

	if (InteractAction)
		EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ASmartCityPlayerCharacter::Interact);

	if (ToggleDayNightAction)
		EnhancedInput->BindAction(ToggleDayNightAction, ETriggerEvent::Started, this, &ASmartCityPlayerCharacter::ToggleDayNight);

	if (TriggerEmergencyAction)
		EnhancedInput->BindAction(TriggerEmergencyAction, ETriggerEvent::Started, this, &ASmartCityPlayerCharacter::TriggerEmergency);
}

void ASmartCityPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASmartCityPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASmartCityPlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	bIsSprinting = true;
}

void ASmartCityPlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
}

void ASmartCityPlayerCharacter::Interact(const FInputActionValue& Value)
{
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractionRange;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		AInteractableBuilding* Building = Cast<AInteractableBuilding>(HitResult.GetActor());
		if (Building && HUDWidgetInstance)
		{
			HUDWidgetInstance->ShowBuildingInfo(Building->GetBuildingData());

			GetWorldTimerManager().ClearTimer(BuildingInfoTimerHandle);
			GetWorldTimerManager().SetTimer(
				BuildingInfoTimerHandle, this,
				&ASmartCityPlayerCharacter::HideBuildingInfo,
				BuildingInfoDisplayTime, false);
		}
	}
}

void ASmartCityPlayerCharacter::ToggleDayNight(const FInputActionValue& Value)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADayNightManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		ADayNightManager* Manager = Cast<ADayNightManager>(FoundActors[0]);
		if (Manager)
		{
			Manager->ToggleDayNight();
		}
	}
}

void ASmartCityPlayerCharacter::TriggerEmergency(const FInputActionValue& Value)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEmergencyManager::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		AEmergencyManager* Manager = Cast<AEmergencyManager>(FoundActors[0]);
		if (Manager)
		{
			Manager->TriggerEmergency();
		}
	}
}

void ASmartCityPlayerCharacter::OnEmergencyStateChanged(EEmergencyState NewState, FString Message)
{
	if (!HUDWidgetInstance) return;

	switch (NewState)
	{
	case EEmergencyState::Active:
		HUDWidgetInstance->ShowEmergencyAlert(Message);
		break;
	case EEmergencyState::Resolved:
		HUDWidgetInstance->ShowEmergencyResolved(Message);
		break;
	case EEmergencyState::None:
		HUDWidgetInstance->HideEmergencyAlert();
		break;
	}
}

void ASmartCityPlayerCharacter::OnDayNightChanged(bool bIsNight)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->UpdateDayNightStatus(bIsNight);
	}
}

void ASmartCityPlayerCharacter::HideBuildingInfo()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->HideBuildingInfo();
	}
}
