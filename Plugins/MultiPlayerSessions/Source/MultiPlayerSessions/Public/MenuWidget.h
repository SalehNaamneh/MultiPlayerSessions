// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumPublicConnections = 8 ,FString MatchType = "FreeForAll" );
    void MenuTearDown();
protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
private:
	// Host Button to bind to the host button in unreal engine
	UPROPERTY(meta=(BindWidget))
	class UButton * HostButton;
	
	// Join Button to bind to the host button in unreal engine
	UPROPERTY(meta=(BindWidget))
	class UButton * JoinButton;


	// CallBack Func to bind for the Buttons
	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	// Our SubSystem to handle the subsystem throw the menu Widget
	class UMultiPlayerSessionSubSystem * PlayerSessionSubSystem;
	int32 NumberPublicConnections{8};
	FString MatchType{TEXT("FreeForAll")};
};
