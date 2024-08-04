
#include "MenuWidget.h"
#include "MultiPlayerSessions/Public/MultiPlayerSessionSubSystem.h"
#include "Components/Button.h"


void UMenuWidget::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld *World = GetWorld();
	if (World)
	{
		APlayerController * PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
		}
	}
	// Access the our subsystem
	UGameInstance * GameInstance = GetGameInstance();
	if (GameInstance)
	{
		PlayerSessionSubSystem = GameInstance->GetSubsystem<UMultiPlayerSessionSubSystem>();
	}
}

bool UMenuWidget::Initialize()
{
	// here bind the button with the call back function 
	if (!Super::Initialize())
	{
		return false;
	}
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this,&ThisClass::HostButtonClicked);	
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this,&ThisClass::JoinButtonClicked);	
	}
	return true;
	
}

void UMenuWidget::HostButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Blue,FString("Host Button"));
		// create session 
		if (PlayerSessionSubSystem)
		{
			PlayerSessionSubSystem->CreateSession(4,FString("FreeForAll"));
		}
	}
}

void UMenuWidget::JoinButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,FString("Join Button"));

}
