﻿#include "SRmlWidget.h"
#include "RmlHelper.h"
#include "UERmlSubsystem.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SConstraintCanvas.h"

void SRmlWidget::Construct(const FArguments& InArgs)
{
	BoundContext = Rml::CreateContext(TCHAR_TO_UTF8(*InArgs._Name), Rml::Vector2i(0));
}

SRmlWidget::~SRmlWidget()
{
	Rml::RemoveContext(BoundContext->GetName());
}

bool SRmlWidget::AddToViewport(UWorld* InWorld, int32 ZOrder)
{
	// get game view port 
	UGameViewportClient* ViewportClient = InWorld->GetGameViewport();
	if (!ViewportClient) return false;

	// add to view port 
	ViewportClient->AddViewportWidgetContent(this->AsShared(), ZOrder + 10);

	return true;
}

void SRmlWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{	
	// update size 
	FVector2D CurSizeUE = AllottedGeometry.GetAbsoluteSize();
	Rml::Vector2i CurSize((int)CurSizeUE.X, (int)CurSizeUE.Y);
	Rml::Vector2i LastSize = BoundContext->GetDimensions();
	if (CurSize != LastSize)
	{
		BoundContext->SetDimensions(CurSize);
	}

	// call update 
	BoundContext->Update();
}

int32 SRmlWidget::OnPaint(
	const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	auto& RenderInterface = UUERmlSubsystem::Get()->GetRmlRenderInterface();

	auto Size = AllottedGeometry.GetAbsoluteSize();
	RenderInterface.CurrentElementList = &OutDrawElements;
	RenderInterface.CurrentLayer = LayerId;
	RenderInterface.CurrentRenderMatrix = AllottedGeometry.GetAccumulatedRenderTransform().To3DMatrix();
	RenderInterface.CurrentRenderMatrix *= FMatrix(
			FPlane(2.0f / Size.X,0.0f,			0.0f,		0.0f),
			FPlane(0.0f,			-2.0f / Size.Y,	0.0f,		0.0f),
			FPlane(0.0f,			0.0f,			1.0f,		0.0f),
			FPlane(-1.0f,		1.0f,			0,			1.0f));
	BoundContext->Render();
	
	return LayerId + 1;
}

FVector2D SRmlWidget::ComputeDesiredSize(float) const
{
	return FVector2D::ZeroVector;
}

FReply SRmlWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	auto ModifierState = InKeyEvent.GetModifierKeys();
	return BoundContext->ProcessKeyDown(
		FRmlHelper::ConvertKey(InKeyEvent.GetKey()),
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	auto ModifierState = InKeyEvent.GetModifierKeys();
	return BoundContext->ProcessKeyUp(
        FRmlHelper::ConvertKey(InKeyEvent.GetKey()),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnKeyChar(const FGeometry& MyGeometry, const FCharacterEvent& InCharacterEvent)
{
	return BoundContext->ProcessTextInput(InCharacterEvent.GetCharacter()) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	auto MousePos = MouseEvent.GetScreenSpacePosition();
	return BoundContext->ProcessMouseMove(
		MousePos.X,
		MousePos.Y,
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseButtonDown(
		FRmlHelper::GetMouseKey(MouseEvent.GetEffectingButton()),
		FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseButtonUp(
        FRmlHelper::GetMouseKey(MouseEvent.GetEffectingButton()),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}

FReply SRmlWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	auto ModifierState = MouseEvent.GetModifierKeys();
	return BoundContext->ProcessMouseWheel(
        MouseEvent.GetWheelDelta(),
        FRmlHelper::GetKeyModifierState(ModifierState)) ? FReply::Handled() : FReply::Unhandled();
}
