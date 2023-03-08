// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.1 Jan 2013

#pragma once
#ifndef LIGHTS_H
#define LIGHTS_H

#include <Windows.h>
#include <d3dx9.h>


namespace Direct3D
{

	extern D3DXCOLOR WHITE;

	struct BaseLight
	{
		BaseLight( const D3DLIGHTTYPE& Type, const D3DXCOLOR& AmbientColor = WHITE, const D3DXCOLOR& DiffuseColor = WHITE, const D3DXCOLOR& SpecularColor = WHITE );
		BaseLight( const D3DXCOLOR& LightColor, const D3DLIGHTTYPE& Type );

		
		virtual operator D3DLIGHT9() const = 0; //Pure Virtual function makes this class an abstract base class 
		
		D3DLIGHTTYPE m_Type;
		D3DXCOLOR m_AmbientColor;
		D3DXCOLOR m_DiffuseColor;
		D3DXCOLOR m_SpecularColor;
	}; // end of struct 'BaseLight'


	struct DirectionalLight : public BaseLight
	{
		DirectionalLight( const D3DXCOLOR& LigthColor, const D3DXVECTOR3& vecDirection );
		DirectionalLight( const D3DXVECTOR3& vecDirection, const D3DXCOLOR& AmbientColor = WHITE,
			const D3DXCOLOR& DiffuseColor = WHITE, const D3DXCOLOR& SpecularColor = WHITE );
		DirectionalLight( const D3DLIGHT9& Light );

		virtual operator D3DLIGHT9() const;

		D3DXVECTOR3 m_vecDirection;
	}; // end of struct 'DirectionalLight'


	struct PointLightParams
	{
		PointLightParams( void )
		{
			m_fRange        = 1000.0f;
			m_fAttenuation0 = 1.0f;
			m_fAttenuation1 = 0.0f;
			m_fAttenuation2 = 0.0f;
		}

		PointLightParams( const float& Range, const float& Attenuation0 = 1.0f,
			const float& Attenuation1 = 0.0f, const float& Attenuation2 = 0.0f )
		{
			m_fRange        = Range;
			m_fAttenuation0 = Attenuation0;
			m_fAttenuation1 = Attenuation1;
			m_fAttenuation2 = Attenuation2;
		}

		float m_fRange, m_fAttenuation0, m_fAttenuation1, m_fAttenuation2;
	}; // end of struct 'PointLightParams'


	struct SpotLightParams : public PointLightParams
	{
		SpotLightParams() : PointLightParams()
		{
			m_fFalloff	= 1.0f;
			m_fTheta	= 0.4f;
			m_fPhi		= 0.9f;
		}

		SpotLightParams( const float& Falloff, const float& Theta = 0.4f, const float& Phi = 0.9f,
			const float& Range = 1000.0f, const float& Attenuation0 = 1.0f,
			const float& Attenuation1 = 0.0f, const float& Attenuation2 = 0.0f )
			: PointLightParams( Range, Attenuation0, Attenuation1, Attenuation2 )
		{
			m_fFalloff = Falloff;
			m_fTheta   = Theta;
			m_fPhi     = Phi;
		}

		float m_fFalloff, m_fTheta, m_fPhi;
	}; // end of struct 'SpotLightParams'


	struct SpotLight : public BaseLight
	{
		SpotLight( const D3DXVECTOR3& vecPosition, const D3DXVECTOR3& vecDirection, const D3DXCOLOR& Color );
		SpotLight( const D3DXVECTOR3& vecPosition, const D3DXVECTOR3& vecDirection, const D3DXCOLOR& Color, const SpotLightParams& Params );
		SpotLight( const D3DXVECTOR3& vecPosition, const D3DVECTOR& vecDirection, const D3DCOLORVALUE& Color,
			const float& fRange = 1000.0f, const float& fFalloff = 1.0f, const float& fAttenuation0 = 1.0f,
			const float& fAttenuation1 = 0.0f, const float& fAttenuation2 = 0.0f, const float& fTheta = 0.4f, const float& fPhi = 0.9f );
		SpotLight( const D3DLIGHT9& Light );

		virtual operator D3DLIGHT9() const;

		D3DXVECTOR3 m_vecPosition, m_vecDirection;
		SpotLightParams m_Params;
	}; // end of struct 'SpotLight'


	struct PointLight : public BaseLight
	{
		PointLight( const D3DXVECTOR3& vecPosition, const D3DXCOLOR& Color, const PointLightParams& Params );
		PointLight( const D3DXVECTOR3& vecPosition, const D3DXCOLOR& Color, const float& Range = 1000.0f, const float& Attenuation0 = 1.0f,
			const float& Attenuation1 = 0.0f, const float& Attenuation2 = 0.0f );
		PointLight( const D3DLIGHT9& Light );

		virtual operator D3DLIGHT9() const;

		D3DVECTOR m_vecPosition;
		PointLightParams m_Params;
	}; // end of struct 'PointLight'

} // end of namespace 'Direct3D'
#endif // LIGHTS_H