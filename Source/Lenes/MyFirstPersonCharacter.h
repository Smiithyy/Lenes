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
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // BeginPlay override
    virtual void BeginPlay() override;

private:
    // First-person camera
    UPROPERTY(VisibleAnywhere)
    class UCameraComponent* FirstPersonCameraComponent;

    // Movement variables
    float SprintMultiplier; // Multiplier for sprinting speed

    // Head bob variables
    UPROPERTY(EditAnywhere, Category = "Head Bob")
    float BobFrequency; // Frequency of the head bob oscillation

    UPROPERTY(EditAnywhere, Category = "Head Bob")
    float BobAmplitude; // Amplitude of the head bob oscillation

    UPROPERTY(EditAnywhere, Category = "Head Bob")
    float BobRunMultiplier; // Multiplier for head bob when running

    FVector DefaultCameraPosition; // The default relative position of the camera
    float BobTimer; // Timer to track the head bob oscillation

    // Input functions
    void MoveForward(float Value);
    void MoveRight(float Value);
    void StartJump();
    void StopJump();
    void StartSprint();
    void StopSprint();
    void StartCrouch();
    void StopCrouch();

    // Head bob function
    void ApplyHeadBob(float DeltaTime);
};
