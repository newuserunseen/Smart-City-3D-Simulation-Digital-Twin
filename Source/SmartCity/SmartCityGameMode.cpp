#include "SmartCityGameMode.h"
#include "SmartCityPlayerCharacter.h"

ASmartCityGameMode::ASmartCityGameMode()
{
	DefaultPawnClass = ASmartCityPlayerCharacter::StaticClass();
}
