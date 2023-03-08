#include "Lights.h"

using namespace Direct3D;

extern D3DXCOLOR Direct3D::WHITE = D3DXCOLOR( 0xFFFFFFFF );


BaseLight::BaseLight( const D3DLIGHTTYPE& Type, const D3DXCOLOR& AmbientColor, const D3DXCOLOR& DiffuseColor, const D3DXCOLOR& SpecularColor )
{
	m_Type = Type;
	m_AmbientColor = AmbientColor;
	m_DiffuseColor = DiffuseColor;
	m_SpecularColor = SpecularColor;
}


BaseLight::BaseLight( const D3DXCOLOR& LightColor, const D3DLIGHTTYPE& Type )
{
	m_Type = Type;

	m_AmbientColor = LightColor * 0.4f;
	m_DiffuseColor = LightColor;
	m_SpecularColor = LightColor * 0.6f;
}


/*
 * DirectionalLight
 */
DirectionalLight::DirectionalLight( const D3DXCOLOR& LigthColor, const D3DXVECTOR3& vecDirection )
	:BaseLight( LigthColor, D3DLIGHT_DIRECTIONAL )
{
	m_vecDirection = vecDirection;
}


DirectionalLight::DirectionalLight ( const D3DXVECTOR3& vecDirection, const D3DXCOLOR& AmbientColor, const D3DXCOLOR& DiffuseColor, const D3DXCOLOR& SpecularColor )
	:BaseLight( D3DLIGHT_DIRECTIONAL, AmbientColor, DiffuseColor, SpecularColor )
{
	m_vecDirection = vecDirection;
}


DirectionalLight::DirectionalLight( const D3DLIGHT9& Light )
	:BaseLight( D3DLIGHT_DIRECTIONAL, Light.Ambient, Light.Diffuse, Light.Specular )
{
	m_vecDirection = Light.Direction;
}

DirectionalLight::operator D3DLIGHT9() const 
{
	D3DXVECTOR3 vecPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DLIGHT9 out =
	{
		m_Type,
		m_DiffuseColor,
		m_SpecularColor,
		m_AmbientColor,
		vecPos,
		m_vecDirection,
		0.0f,	//Range
		0.0f,	//Falloff
		0.0f,	//Attenuation0
		0.0f,	//Attenuation1
		0.0f,	//Attenuation2
		0.0f,	//Theta
		0.0f	//Phi
	};
	return out;
}


/*
 * SpotLight
 */
SpotLight::SpotLight( const D3DXVECTOR3& vecPosition, const D3DXVECTOR3& vecDirection, const D3DXCOLOR& Color )
	:BaseLight( Color, D3DLIGHT_SPOT ), m_Params()
{
	m_vecPosition  = vecPosition;
	m_vecDirection = vecDirection;
}


SpotLight::SpotLight( const D3DXVECTOR3& vecPosition, const D3DXVECTOR3& vecDirection, const D3DXCOLOR& Color, const SpotLightParams& Params )
	:BaseLight(Color, D3DLIGHT_SPOT), m_Params(Params)
{
	m_vecPosition  = vecPosition;
	m_vecDirection = vecDirection;
}


SpotLight::SpotLight( const D3DXVECTOR3& vecPosition, const D3DVECTOR& vecDirection, const D3DCOLORVALUE& Color,
	const float& fRange, const float& fFalloff, const float& fAttenuation0,
	const float& fAttenuation1, const float& fAttenuation2, const float& fTheta, const float& fPhi )
	: BaseLight( Color, D3DLIGHT_SPOT ),
	m_Params( fRange, fFalloff, fAttenuation0, fAttenuation1, fAttenuation2,
	fTheta, fPhi )
{
	m_vecPosition  = vecPosition;
	m_vecDirection = vecDirection;
}


SpotLight::SpotLight( const D3DLIGHT9& Light )
	: BaseLight( Light.Diffuse, D3DLIGHT_SPOT ),
	m_Params( Light.Range, Light.Falloff, Light.Attenuation0, Light.Attenuation1, Light.Attenuation2,
	Light.Theta, Light.Phi )
{
	m_AmbientColor = Light.Ambient;
	m_DiffuseColor = Light.Diffuse;
	m_SpecularColor = Light.Specular;
	m_vecPosition  = Light.Position;
	m_vecDirection = Light.Direction;
}


SpotLight::operator D3DLIGHT9() const 
{
	D3DLIGHT9 out =
	{
		m_Type,
		m_DiffuseColor,
		m_SpecularColor,
		m_AmbientColor,
		m_vecPosition,
		m_vecDirection,
		m_Params.m_fRange,
		m_Params.m_fFalloff,
		m_Params.m_fAttenuation0,
		m_Params.m_fAttenuation1,
		m_Params.m_fAttenuation2,
		m_Params.m_fTheta,
		m_Params.m_fPhi
	};
	return out;
}


/*
 * Point Light
 */
PointLight::PointLight( const D3DXVECTOR3& vecPosition, const D3DXCOLOR& Color, const PointLightParams& Params )
	: BaseLight( Color, D3DLIGHT_POINT ), m_Params( Params )
{
	m_vecPosition = vecPosition;
}


PointLight::PointLight( const D3DXVECTOR3& vecPosition, const D3DXCOLOR& Color, const float& Range, const float& Attenuation0,
	const float& Attenuation1, const float& Attenuation2 )
	: BaseLight( Color, D3DLIGHT_POINT ),
	m_Params( Range, Attenuation0, Attenuation1, Attenuation2 )
{
	m_vecPosition = vecPosition;
}


PointLight::PointLight( const D3DLIGHT9& Light )
	: BaseLight( Light.Diffuse, D3DLIGHT_POINT )
{
	m_AmbientColor = Light.Ambient;
	m_DiffuseColor = Light.Diffuse;
	m_SpecularColor = Light.Specular;
	m_vecPosition = Light.Position;
}


PointLight::operator D3DLIGHT9() const 
{
	D3DLIGHT9 out =
	{
		m_Type,
		m_DiffuseColor,
		m_SpecularColor,
		m_AmbientColor,
		m_vecPosition,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),	//Direction
		m_Params.m_fRange,
		0.0f,							//Falloff
		m_Params.m_fAttenuation0,
		m_Params.m_fAttenuation1,
		m_Params.m_fAttenuation2,
		0.0f,							//Theta
		0.0f							//Phi
	};
	return out;
}
