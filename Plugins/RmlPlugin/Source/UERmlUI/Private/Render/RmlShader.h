﻿#pragma once
#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "ShaderParameterStruct.h"

class FRmlShaderVs : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FRmlShaderVs, Global)
public:
	FRmlShaderVs()
	{ }

	FRmlShaderVs(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
	{
		InTransform.Bind(Initializer.ParameterMap, TEXT("InTransform"));
	}

	void SetParameters(FRHICommandList& RHICmdList, const FMatrix& TransformValue)
	{
		SetShaderValue(RHICmdList, RHICmdList.GetBoundVertexShader(), InTransform, TransformValue);
	}

	static bool ShouldCompilePermutation(
        const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(
        const FGlobalShaderPermutationParameters& Parameters,
        FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters,OutEnvironment);
	}
private:
	LAYOUT_FIELD(FShaderParameter, InTransform)
};

class FRmlShaderPs : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FRmlShaderPs, Global)
public:
	FRmlShaderPs()
	{ }

	FRmlShaderPs(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
	{
		InTexture.Bind(Initializer.ParameterMap, TEXT("InTexture"));
		InTextureSampler.Bind(Initializer.ParameterMap, TEXT("InTextureSampler"));
	}
	
	static bool ShouldCompilePermutation(
        const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(
        const FGlobalShaderPermutationParameters& Parameters,
        FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters,OutEnvironment);
	}

	template<typename TShaderRHIParamRef>
    void SetParameters(FRHICommandListImmediate& RHICmdList, const TShaderRHIParamRef ShaderRHI, FRHITexture2D* SourceTexture)
	{
		SetTextureParameter(RHICmdList, ShaderRHI, InTexture, SourceTexture);
		RHICmdList.SetShaderSampler(ShaderRHI, InTextureSampler.GetBaseIndex(), TStaticSamplerState<SF_Bilinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI());
	}
private:
	LAYOUT_FIELD(FShaderResourceParameter, InTexture)
	LAYOUT_FIELD(FShaderResourceParameter, InTextureSampler)
};

class FRmlShaderPsNoTex : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FRmlShaderPsNoTex, Global)
public:
	FRmlShaderPsNoTex()
	{ }

	FRmlShaderPsNoTex(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
	{
	}
	
	static bool ShouldCompilePermutation(
        const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
	static void ModifyCompilationEnvironment(
        const FGlobalShaderPermutationParameters& Parameters,
        FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters,OutEnvironment);
	}
};