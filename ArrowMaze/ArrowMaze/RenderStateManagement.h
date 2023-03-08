// In the name of God
// Ehsan Marufi Azar, 2012 (c)
// v1.2 Jan 2013

#pragma once
#ifndef RENDERSTATEMANAGEMENT_H
#define RENDERSTATEMANAGEMENT_H

#include <Windows.h>
#include <d3dx9.h>
#include <map>
#include "DeviceAccessor.h"
#include "safe.h"
#include "hr_Exception.h"
#include "Assertion.h"


namespace Direct3D
{

#define NUM_OF(ary_) (sizeof(ary_)/sizeof(ary_[0]))
#define STAGES_COUNT (8)
#define ASSERT_STAGE(uiStage) ASSERT((uiStage) < STAGES_COUNT, "The 'uiStage' Should be in valid range. typically: [0-8)!")

	template<typename Enum>
	struct s_enum_value
	{
		Enum m_State;
		DWORD m_dwValue;
	};

	typedef s_enum_value<D3DRENDERSTATETYPE> Render_State;
	typedef s_enum_value<D3DTEXTURESTAGESTATETYPE> Texture_Stage_State;
	typedef s_enum_value<D3DSAMPLERSTATETYPE> Sampler_State;

	typedef std::map<D3DRENDERSTATETYPE, DWORD> ListOfRenderStates;
	typedef std::map<D3DTEXTURESTAGESTATETYPE, DWORD> ListOfTextureStageState;
	typedef std::map<D3DSAMPLERSTATETYPE, DWORD> ListOfSamplerStageState;


	class CRenderStateManagement
		: public CDeviceAccessor
	{
	public:
		CRenderStateManagement( void );
		CRenderStateManagement( const CRenderStateManagement& copy );
		~CRenderStateManagement( void );

		inline void setState( const D3DRENDERSTATETYPE& state, const DWORD& dwValue ) { m_mapRenderStates[state] = dwValue; }
		inline void setState( const UINT& uiStage, const D3DTEXTURESTAGESTATETYPE& state, const DWORD& dwValue )
		{
			if ( uiStage < STAGES_COUNT )
				(m_arrOfmapTextureStageState[uiStage])[state] = dwValue;
		}

		inline void setState( const UINT& uiStage, const D3DSAMPLERSTATETYPE& state, const DWORD& dwValue)
		{
			if ( uiStage < STAGES_COUNT )
				(m_arrOfmapSamplerStageState[uiStage])[state] = dwValue;
		}

		inline void setState( const Render_State& stateAndValue ) { setState( stateAndValue.m_State, stateAndValue.m_dwValue ); }
		inline void setState( const UINT& uiStage, const Texture_Stage_State& stateAndValue ) { setState( uiStage, stateAndValue.m_State, stateAndValue.m_dwValue ); }
		inline void setState( const UINT& uiStage, const Sampler_State& stateAndValue ) { setState( uiStage, stateAndValue.m_State, stateAndValue.m_dwValue ); }

		void setStates( const Render_State* arrStates, const UINT& uiStatesCount );
		void setStates( const UINT& uiStage, const Texture_Stage_State* arrStates, const UINT& uiStatesCount );
		void setStates( const UINT& uiStage, const Sampler_State* arrStates, const UINT& uiStatesCount );


		//If the argument key value is not found, then the 'map::at()' function throws an object of class out_of_range Class.
		inline const DWORD& getState( const D3DRENDERSTATETYPE& state ) const
		{
			return m_mapRenderStates.at( state );
		}

		inline const DWORD& getState( const UINT& uiStage, const D3DTEXTURESTAGESTATETYPE& state ) const
		{
			ASSERT_STAGE( uiStage );
			return ( m_arrOfmapTextureStageState[uiStage] ).at( state );
		}

		inline const DWORD& getState( const UINT& uiStage, const D3DSAMPLERSTATETYPE& state ) const
		{
			ASSERT_STAGE( uiStage );
			return ( m_arrOfmapSamplerStageState[uiStage] ).at( state );
		}

		inline const ListOfRenderStates& getRenderStates() const
		{
			return m_mapRenderStates;
		}

		inline const ListOfTextureStageState& getTextureStageStates( const UINT& uiStage ) const
		{
			ASSERT_STAGE( uiStage );
			return m_arrOfmapTextureStageState[ uiStage ];
		}

		inline const ListOfSamplerStageState& getSamplerStageStates( const UINT& uiStage ) const
		{
			ASSERT_STAGE( uiStage );
			return m_arrOfmapSamplerStageState[ uiStage ];
		}

		void getStates( Render_State *states, UINT* puiCount = 0 ) const;
		void getStates( const UINT& uiStage, Texture_Stage_State* arrStates, UINT* puiCount = 0 ) const;
		void getStates( const UINT& uiStage, Sampler_State* arrStates, UINT* puiCount = 0 ) const;

		void createStateBlock();
		void startUsingStateBlock();
		void endUsingStateBlock();


	private:
		LPDIRECT3DSTATEBLOCK9 m_pStateBlock;
		LPDIRECT3DSTATEBLOCK9 m_pTempDeviceStateBlock;
		ListOfRenderStates m_mapRenderStates;
		ListOfTextureStageState m_arrOfmapTextureStageState[STAGES_COUNT];
		ListOfSamplerStageState m_arrOfmapSamplerStageState[STAGES_COUNT];

	}; // end of class 'CRenderStateManagement'
} // end of namespace 'Direct3D'
#endif // RENDERSTATEMANAGEMENT_H