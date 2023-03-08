#include "RenderStateManagement.h"

using namespace Direct3D;


CRenderStateManagement::CRenderStateManagement()
{
	m_pStateBlock = 0;
	m_pTempDeviceStateBlock = 0;
}


CRenderStateManagement::CRenderStateManagement( const CRenderStateManagement& copy )
{
	m_mapRenderStates = copy.getRenderStates();

	for ( UINT uiStage = 0; uiStage < STAGES_COUNT; uiStage++ )
	{
		m_arrOfmapSamplerStageState[ uiStage ] = copy.getSamplerStageStates( uiStage );
		m_arrOfmapTextureStageState[ uiStage ] = copy.getTextureStageStates( uiStage );
	}

	m_pTempDeviceStateBlock = 0;
	m_pStateBlock = 0;
	createStateBlock();
}


CRenderStateManagement::~CRenderStateManagement()
{
	SAFE_RELEASE( m_pStateBlock );
	ASSERT( !m_pTempDeviceStateBlock, "There is an started state block which has not got ended!" );
}


void CRenderStateManagement::setStates( const Render_State* arrStates, const UINT& uiStatesCount )
{
	ASSERT( uiStatesCount, "States' count cann't be zero!" );
	D3DRENDERSTATETYPE rs;
	DWORD dwValue;

	for ( UINT i = 0; i < uiStatesCount; i++ )
	{
		rs = arrStates[i].m_State;
		dwValue = arrStates[i].m_dwValue;
		setState( rs, dwValue );
	}
}


void CRenderStateManagement::setStates( const UINT& uiStage, const Texture_Stage_State* arrStates, const UINT& uiStatesCount )
{
	ASSERT(uiStatesCount, "States' count cann't be zero!");
	D3DTEXTURESTAGESTATETYPE tss;
	DWORD dwValue;

	for ( UINT i = 0; i < uiStatesCount; i++ )
	{
		tss = arrStates[i].m_State;
		dwValue = arrStates[i].m_dwValue;
		setState( uiStage, tss, dwValue );
	}
}


void CRenderStateManagement::setStates( const UINT& uiStage, const Sampler_State* arrStates, const UINT& uiStatesCount )
{
	ASSERT(uiStatesCount, "States' count cann't be zero!");
	D3DSAMPLERSTATETYPE ss;
	DWORD dwValue;

	for ( UINT i = 0; i < uiStatesCount; i++ )
	{
		ss = arrStates[i].m_State;
		dwValue = arrStates[i].m_dwValue;
		setState( uiStage, ss, dwValue );	
	}
}


void CRenderStateManagement::getStates( Render_State *arrStates, UINT* puiCount ) const
{
	// If user suplies 'null' with arrStates, then return the size of 'RenderStates' array through 'puiCount'
	if ( !arrStates )
	{
		ASSERT_POINTER( puiCount );
		*puiCount = m_mapRenderStates.size();
		return;
	}

	Render_State rs;
	UINT i = 0;

	ListOfRenderStates::const_iterator cIter = m_mapRenderStates.cbegin(),
		cend = m_mapRenderStates.cend();
	for ( ; cIter != cend; cIter++ )
	{
		rs.m_State = cIter->first;
		rs.m_dwValue = cIter->second;
		arrStates[ i++ ] = rs;
	}
}


void CRenderStateManagement::getStates( const UINT& uiStage, Texture_Stage_State* arrStates, UINT* puiCount ) const
{
	ASSERT_STAGE(uiStage);

	// If user suplies 'null' with arrStates, then return the size of 'RenderStates' array through 'puiCount'
	const ListOfTextureStageState& lstCurrentTSS = m_arrOfmapTextureStageState[ uiStage ];
	if ( !arrStates )
	{
		ASSERT_POINTER( puiCount );
		*puiCount = lstCurrentTSS.size();
		return;
	}

	Texture_Stage_State tss;
	UINT i = 0;

	ListOfTextureStageState::const_iterator cend = lstCurrentTSS.cend();
	for ( ListOfTextureStageState::const_iterator cIter = lstCurrentTSS.cbegin();
		cIter != cend;
		cIter++ )
	{
		tss.m_State = cIter->first;
		tss.m_dwValue = cIter->second;
		arrStates[ i++ ] = tss;
	}	
}


void CRenderStateManagement::getStates( const UINT& uiStage, Sampler_State* arrStates, UINT* puiCount ) const
{
	ASSERT_STAGE(uiStage);

	// If user suplies 'null' with arrStates, then return the size of 'RenderStates' array through 'puiCount'
	const ListOfSamplerStageState& lstCurrentSS = m_arrOfmapSamplerStageState[ uiStage ];
	if ( !arrStates )
	{
		ASSERT_POINTER( puiCount );
		*puiCount = lstCurrentSS.size();
		return;
	}


	Sampler_State ss;
	UINT i = 0;

	ListOfSamplerStageState::const_iterator cend = lstCurrentSS.cend();
	for ( ListOfSamplerStageState::const_iterator cIter = lstCurrentSS.cbegin();
		cIter != lstCurrentSS.cend();
		cIter++ )
	{
		ss.m_State = cIter->first;
		ss.m_dwValue = cIter->second;
		arrStates[ i++ ] = ss;
	}	
}


void CRenderStateManagement::createStateBlock()
{
	ASSERT_D3DDEVICE( s_pD3DDevice );

	if ( m_pStateBlock )
		SAFE_RELEASE( m_pStateBlock );

	THR( s_pD3DDevice->BeginStateBlock() );

	// Setting captured "Render States" on the device
	for ( ListOfRenderStates::const_iterator cIter = m_mapRenderStates.cbegin(); cIter != m_mapRenderStates.cend(); cIter++ )
		THR( s_pD3DDevice->SetRenderState( cIter->first, cIter->second ) );

	// Setting captured "Texture States" in the appropriate stage on the device
	for ( UINT uiStage = 0; uiStage < STAGES_COUNT; uiStage++ )
	{
		const ListOfTextureStageState& lstCurrentTS = m_arrOfmapTextureStageState[ uiStage ];
		const ListOfTextureStageState::const_iterator& cend = lstCurrentTS.cend();

		for ( ListOfTextureStageState::const_iterator cIter = lstCurrentTS.cbegin();
			cIter != cend;
			cIter++ )
			THR( s_pD3DDevice->SetTextureStageState( uiStage, cIter->first, cIter->second ) );
	}

	// Setting captured "Sampler States" in the appropriate stage on the device
	for ( UINT uiStage = 0; uiStage < STAGES_COUNT; uiStage++ )
	{
		const ListOfSamplerStageState& lstCurrentSS = m_arrOfmapSamplerStageState[ uiStage ];
		const ListOfSamplerStageState::const_iterator& cend = lstCurrentSS.cend();
		
		for ( ListOfSamplerStageState::const_iterator cIter = lstCurrentSS.cbegin();
			cIter != cend;
			cIter++ )
			THR( s_pD3DDevice->SetSamplerState( uiStage, cIter->first, cIter->second ) );
	}

	// Create the stateBlock, and hold a pointer to the stored location in memory
	THR( s_pD3DDevice->EndStateBlock( &m_pStateBlock ) );
}


void CRenderStateManagement::startUsingStateBlock()
{
	if( m_pStateBlock )
	{
		//ASSERT_D3DDEVICE(s_pD3DDevice);
		//ASSERT_POINTER(m_pStateBlock);
		ASSERT( !m_pTempDeviceStateBlock, "You can NOT start a new state block, whitout ending the previous pending one!" );

		
		THR( s_pD3DDevice->CreateStateBlock( D3DSBT_ALL, &m_pTempDeviceStateBlock) );
		THR( m_pStateBlock->Apply() );
	}
}


void CRenderStateManagement::endUsingStateBlock()
{
	if( m_pStateBlock )
	{
		//ASSERT_D3DDEVICE(s_pD3DDevice);
		//ASSERT_POINTER(m_pStateBlock);
		ASSERT( m_pTempDeviceStateBlock, "You can NOT end an state block, While you haven't started it yet!" );

		THR( m_pTempDeviceStateBlock->Apply() );
		SAFE_RELEASE( m_pTempDeviceStateBlock );
	}
}