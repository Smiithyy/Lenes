#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyFirstPersonCharacter.generated.h"

UCLASS()
class LENES_API AMyFirstPersonCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Constructor
    AMyFirstPersonCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Camera component
    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* FirstPersonCameraComponent;

    // Sprint speed multiplier
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float SprintMultiplier;

    // Base walk speed
    float BaseWalkSpeed;

    // Functions for movement
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);
    void StartJump();
    void StopJump();
    void StartCrouch();
    void StopCrouch();
    void StartSprint();
    void StopSprint();

    // Sensitivity for camera
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float BaseTurnRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    float BaseLookUpRate;
};