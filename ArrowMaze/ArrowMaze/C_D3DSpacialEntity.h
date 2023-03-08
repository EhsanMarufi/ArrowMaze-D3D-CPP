//Class for maintaing the properties for a typical 3D (spacial) object.
//Ehsan Marufi Azar, 2012 (c)
//All rigths reserved.
//
//NOTE: THIS CLASS USES THE 'D3DFMT_INDEX16' FORMAT FOR THE INDEX BUFFER (WORD SIZED QUANTITIES),
//FOR THE NEEDS OTHER THAN 16 BITS (32 BITS) YOU MUST CHANGE THE RELATED ENTERIES. MAYBE ANOTHER TEMPLATE ENTRY!

#pragma once

#ifndef C_D3DSPACIALENTITY
#define C_D3DSPACIALENTITY

#include <Windows.h>
#include "hr_exception.h"
#include "DXUT\Core\DXUT.h"
#include "DXUT\Optional\SDKmisc.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "Lights.h"

namespace SpacialEntity {

	template <typename T> class test {public: test(int *){}~test(void){}};

#define TO_RADIANS (D3DX_PI/180.0f)
#define TO_DEGREE (180.0f/D3DX_PI)

// number of elements in a fixed-size array
#define NUM_OF(ary_) (sizeof(ary_)/sizeof(ary_[0]))
#define _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT ((short)1<<0)
#define _3DOBJECTFLAG_AUTOCOMPUTEVERTCOUNT ((short)1<<1)
#define _3DOBJECTFLAG_USEDEFINEDVERTEXFVF (1<<2)
#define _3DOBJECTFLAG_USELIGHTING (1<<3)
#define _3DOBJECTFLAG_USETEXTURES (1<<4)

	inline DWORD float_dword(float value) {return *reinterpret_cast<DWORD *>(&value);}
	inline float dword_float(DWORD value) {return *reinterpret_cast<float *>(&value);}

	extern D3DXCOLOR WHITE;
	extern D3DXCOLOR YELLOW;
	extern D3DMATERIAL9 WHITE_MTRL;

	typedef struct Vertex {
		Vertex(float x =.0f, float y =.0f, float z =.0f,
			float u =0.0f, float v = 0.0f) /*,D3DCOLOR color = WHITE*/
			//float nx = 0.0f, float ny = 0.0f, float nz = -1.0f 
		{
			_x = x;_y = y; _z=z;
			//_color = color;
			//_nx = nx; _ny = ny; _nz = nz;
			_u = u; _v = v;
		}
		float _x, _y, _z;
		//float _nx, _ny, _nz;
		float _u, _v;
		//D3DCOLOR _color;
		static const DWORD FVF;
	} SimpleVertex;

	struct lightCache {D3DLIGHT9 m_Light; bool m_bIsActive;};
	struct textureCache {
		IDirect3DTexture9 *m_pTexture;
	};
	// It is good practice to match the usage parameter in CreateVertexBuffer with the behavior flags in IDirect3D9::CreateDevice.
	typedef struct _IndexBufferCreationParameters {
		//CAUTION: Buffers created with D3DPOOL_DEFAULT that do not specify D3DUSAGE_WRITEONLY might suffer a severe performance penalty.
		_IndexBufferCreationParameters() {}
		_IndexBufferCreationParameters(UINT LengthInBytes, DWORD Usage = 0, D3DFORMAT Format = D3DFMT_INDEX16, D3DPOOL Pool = D3DPOOL_MANAGED, bool AutoInitCreateIndexBuffer = false)
		{
			if (Pool == D3DPOOL_DEFAULT && !(Usage & D3DUSAGE_WRITEONLY))
				THRM(D3DERR_INVALIDCALL, "Creating Index buffer resource in 'DEFAULT' memory pool without the 'WRITEONLY' usage might cause a sever performance penalty!");
			m_LengthInBytes=LengthInBytes; m_Usage=Usage; m_Format=Format; m_Pool=Pool;
			m_bAutoInitCreateIndexBuffer = ((m_bAutoInitCreateIndexBuffer = AutoInitCreateIndexBuffer) == true && m_LengthInBytes!=0) ? true: false;
		}
		UINT m_LengthInBytes;
		DWORD m_Usage;
		D3DFORMAT m_Format;
		D3DPOOL m_Pool;
		bool m_bAutoInitCreateIndexBuffer;
	} IndexBufferCreationParameters;
	
	typedef struct _VertexBufferCreationParameters {
		_VertexBufferCreationParameters(){}
		_VertexBufferCreationParameters(UINT LengthInBytes, DWORD Usage, DWORD FVF, D3DPOOL Pool = D3DPOOL_MANAGED, bool AutoInitCreateVertexBuffer = false)
		{
			if (Pool == D3DPOOL_SCRATCH)
				THRM(D3DERR_INVALIDCALL, "Don't set the Vertex Buffer resource pool to 'SCRATCH'.");
			m_LengthInBytes=LengthInBytes; m_Usage = Usage; m_FVF=FVF; m_Pool=Pool;
			m_bAutoInitCreateVertexBuffer = ((m_bAutoInitCreateVertexBuffer = AutoInitCreateVertexBuffer) == true && m_LengthInBytes!=0 && m_FVF!=0) ? true: false;
		}
		UINT m_LengthInBytes;
		DWORD m_Usage;
		DWORD m_FVF;
		D3DPOOL m_Pool;
		bool m_bAutoInitCreateVertexBuffer;
	} VertexBufferCreationParameters;

	typedef struct _DrawingParams 
	{
		_DrawingParams(){}
		_DrawingParams(UINT _uiPrimitiveCount, UINT _uiNumVertices,
			D3DPRIMITIVETYPE _PrimitiveType = D3DPT_TRIANGLELIST, UINT _uiStartIndex = 0,
			INT _iBaseVertexIndex = 0, UINT _uiMinimumVertexIndex = 0) 
		{
				AutomaticallyCount = _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT | _3DOBJECTFLAG_AUTOCOMPUTEVERTCOUNT;
				PrimitiveType = _PrimitiveType;
				uiPrimitiveCount = _uiPrimitiveCount;
				uiStartIndex = _uiStartIndex;
				uiNumVertices = _uiNumVertices;
				uiMinimumVertexIndex = _uiMinimumVertexIndex;
				iBaseVertexIndex = _iBaseVertexIndex;
		}
		short AutomaticallyCount; 
		D3DPRIMITIVETYPE PrimitiveType;
		UINT uiPrimitiveCount;
		UINT uiStartIndex;
		UINT uiNumVertices;
		UINT uiMinimumVertexIndex;
		INT iBaseVertexIndex;
	} DrawingParams;

	typedef struct _LockParams 
	{
		_LockParams(UINT _uiOffsetToLock =0, UINT _uiSizeToLock =0, UINT _dwFlags =0) 
		{
			uiOffsetToLock=_uiOffsetToLock;
			uiSizeToLock=_uiSizeToLock;
			dwFlags=_dwFlags;
		}
		UINT uiOffsetToLock;
		UINT uiSizeToLock;
		DWORD dwFlags;
	} LockParams;

	typedef struct _StreamSourceParams 
	{
		_StreamSourceParams(UINT _uiStreamNumber =0, UINT _uiOffsetInBytes =0, UINT _uiStride=0) 
		{ 
			uiStreamNumber = _uiStreamNumber;
			uiOffsetInBytes = _uiOffsetInBytes; 
			uiStride = _uiStride;
			AutomaticallyComputeStride = 1;
		}
		short AutomaticallyComputeStride;
		UINT uiStreamNumber;
		UINT uiOffsetInBytes;
		UINT uiStride;
	} StreamSourceParams;
	
	typedef enum tagAngleMode {RADIANS, DEGREE} AngleMode;

	typedef struct _InitializaionParameters 
	{
		VertexBufferCreationParameters *pVBcreationParams;
		IndexBufferCreationParameters *pIBcreationParams;
		DWORD *pDeviceBehaviours;
		AngleMode *angleMode;
		DrawingParams *pDrawingParams;
		StreamSourceParams *pStreamSourceParams;
		LockParams *pLockParams;
		DWORD *pFVF;
		DWORD *pFlags;
	} InitializaionParameters;

	struct _DEFAULTS 
	{
		_DEFAULTS(): lockParams() {
			AngleMode = RADIANS;
			uiStreamNumber = 0;
			uiOffsetInBytes = 0; //D3DDEVCAPS2_STREAMOFFSET in D3DCAPS9::DevCaps2 has to be set, to be anything but 0;
			dwFlags = _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT | _3DOBJECTFLAG_USEDEFINEDVERTEXFVF;
			//UINT m_uiStride = 0;//It is equal to sizeof(T);

			PrimitiveType = D3DPT_TRIANGLELIST;

			iBaseVertexIndex =0;
			uiNumVertices = 0; //here 0 means all;
			uiStartIndex = 0;
			uiMinimumVertexIndex = 0;//TODO: CHANGE THESE WHILE CREATING VB AND IB;
			uiPrimitiveCount = 0; 
			FVF = D3DFVF_XYZ;
			Position = D3DXVECTOR3(.0f, .0f, .0f);
			Rotation = D3DXVECTOR3(.0f, .0f, .0f);
			Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		}
		AngleMode AngleMode;
		UINT uiStreamNumber, uiOffsetInBytes;
		//UINT m_uiStride = 0;//It is equal to sizeof(T);

		D3DPRIMITIVETYPE PrimitiveType;

		LockParams lockParams;
		UINT iBaseVertexIndex, uiNumVertices, uiStartIndex, uiPrimitiveCount, uiMinimumVertexIndex; //TODO: CHANGE THESE WHILE CREATING VB AND IB;
		DWORD FVF, dwFlags;
		D3DXVECTOR3 Position, Rotation, Scale;
	};
	
	template <typename T>
	class C_D3DSpacialEntity
	{
	public:
		C_D3DSpacialEntity(IDirect3DDevice9 *device =0, InitializaionParameters *ip = 0);
		~C_D3DSpacialEntity(void);

		void InitEssentials(IDirect3DDevice9 *device, InitializaionParameters *ip = 0);
		void Init(InitializaionParameters*);
		const InitializaionParameters GetAllCurrentSettings();

		HRESULT OnDeviceReset(/*input*/IDirect3DDevice9 *pDevice);
		HRESULT OnDeviceLost(); 
		HRESULT OnDeviceCreate(/*input*/IDirect3DDevice9 *pDevice);
		HRESULT OnDeviceDestroy();

		void SetAngleMode(AngleMode);
		inline const AngleMode &GetAngleMode() const {return m_AngleMode;}

		HRESULT CreateIndexBuffer(UINT LengthInBytes, DWORD Usage = 0, D3DFORMAT Format = D3DFMT_INDEX16, D3DPOOL Pool = D3DPOOL_MANAGED);
		HRESULT CreateVertexBuffer(UINT LengthInBytes, DWORD Usage, DWORD FVF, D3DPOOL Pool = D3DPOOL_MANAGED);
		HRESULT FeedVertexBuffer(CGrowableArray<T> *Vertices, UINT VertCount =0);
		HRESULT FeedVertexBufferStream(T *pStream, UINT count);
		HRESULT FeedIndexBuffer(CGrowableArray<WORD> *Indices, UINT IndicesCount =0); //UPGRADE: DETERMINE WETHER USING INDEX32 OR INDEX16 TO DETERMINE DATATYPE USED TO FEED (WORD, DWORD)
		HRESULT FeedIndexBufferStream(WORD *pStream, UINT count);
		
		HRESULT Draw();

		inline DWORD GetFlags() const {return m_dwFlags;}
		inline void SetFlags(DWORD dwFlags);

		inline D3DXVECTOR3 &GetPosition() {return m_Position;} //for fast getting and setting
		inline D3DXVECTOR3 &GetRotation() {return (m_AngleMode == RADIANS ? m_Rotation : m_Rotation *= TO_RADIANS);}
		inline D3DXVECTOR3 &GetScale() {return m_Scale;} //for fast getting and setting

		void SetMaterial(D3DMATERIAL9 *pMaterial);
		const D3DMATERIAL9 *GetMaterial() const {return m_Material;}

		void SetLight(D3DLIGHT9 *pLight, UINT index = 0, bool enable = true, bool force = false);
		D3DLIGHT9 GetLight(D3DLIGHT9 *pLight, UINT index, bool *enable=0);
		inline void SetLightEnable(UINT index, bool enable, bool force = false);
		inline void GetLightEnable(UINT index, bool *enable);
		inline UINT GetActiveLightsCount() const;

		HRESULT CreateTextureFromFile(LPCTSTR pSrcFile);
		HRESULT SetTexture(UINT Stage);
	private:
		DWORD m_dwFlags;
		enum {MAX_TEXTURE_STAGES = 8, MAX_TEXTURES = 8};
		bool m_bEssentialInnited, m_bInnited;
		UINT m_uiLatestCreatedTextureIndex;
		//you can choose wether or not the your decimal system works with Radiance or Degree.
		IDirect3DDevice9 *m_pDevice;
		AngleMode m_AngleMode;

		//IB and VB relatedd
		IDirect3DVertexBuffer9 *m_pVB;
		IDirect3DIndexBuffer9 *m_pIB;
		VertexBufferCreationParameters m_VBcreationParams;
		IndexBufferCreationParameters m_IBcreationParams;
		bool m_bUsesIndexBuffer;
		LockParams m_LockParams;
		
		//Drawing parameters
		D3DPRIMITIVETYPE m_PrimitiveType;
		UINT m_uiMinimumVertexIndex, m_uiNumVertices, m_uiStartIndex, m_uiPrimitiveCount;
		INT m_iBaseVertexIndex;
		
		//Stream source parameters
		UINT m_uiStreamNumber;
		UINT m_uiOffsetInBytes;
		UINT m_uiStride; //TODO

		CGrowableArray<T> m_Vertices;
		CGrowableArray<WORD> m_Indices;
		UINT m_uiVertCount;
		DWORD m_FVF;

		//Lights and materials
		D3DMATERIAL9 m_Material;
		CGrowableArray<lightCache> m_Lights;

		//Textures
		textureCache m_arrTextureCache[MAX_TEXTURES];

		//Expected parameters!
		D3DXVECTOR3 m_Position;
		D3DXVECTOR3 m_Rotation;
		D3DXVECTOR3 m_Scale;

		//Utility functions:
		bool updatePrimitiveCount();
		void ApplyLights();
	};
	//====================================================================================================
	//Unfortunatly it has to be implemented over here Coz of inability of the compiler to find the class
	//and instanciate the template class outside the scope of file!
	template <typename T>
	void C_D3DSpacialEntity<T>::InitEssentials(IDirect3DDevice9 *pDevice, InitializaionParameters *ip)
	{
		UINT count =0, i=0;

		if (!pDevice) THRTM(E_FAIL,"The Device for constructing the 3D Entity dose NOT have to be null!");
		m_pDevice = pDevice;

		//default device states
		GetStates((Render_State*)0, &count);
		m_pRenderState = new Render_State[count];
		if (!m_pRenderState) THRTM(E_OUTOFMEMORY, "E_OUTOFMEMORY.");
		GetStates(m_pRenderState);

		GetStates(0, (Texture_Stage_State*)0, &i);
		m_pTextureStageState = new Texture_Stage_State[i][MAX_TEXTURE_STAGES];
		if (!m_pTextureStageState) THRTM(E_OUTOFMEMORY, "E_OUTOFMEMORY.");
		for (count=0; count<MAX_TEXTURE_STAGES; count++)
			GetStates(count, m_pTextureStageState[count], 0);

		GetStates(0, (Sampler_State*)0, &i);
		m_pSamplerState = new Sampler_State[i][MAX_TEXTURE_STAGES];
		if (!m_pSamplerState) THRTM(E_OUTOFMEMORY, "E_OUTOFMEMORY.");
		for (count=0; count<MAX_TEXTURE_STAGES; count++)
			GetStates(count, m_pSamplerState[count]);

		ApplyStates();
		m_bEssentialInnited = true;
		Init(ip);
	}
	template <typename T>
	void C_D3DSpacialEntity<T>::Init(InitializaionParameters *ip) 
	{
		extern _DEFAULTS Defaults;
		if(ip) {
			if (ip->pVBcreationParams) {
				m_VBcreationParams = *ip->pVBcreationParams;
				if (m_VBcreationParams.m_bAutoInitCreateVertexBuffer)
					CreateVertexBuffer(m_VBcreationParams.m_LengthInBytes, m_VBcreationParams.m_Usage, m_VBcreationParams.m_FVF, m_VBcreationParams.m_Pool);
			}
			if (ip->pIBcreationParams) {
				m_IBcreationParams = *ip->pIBcreationParams;
				if (m_IBcreationParams.m_bAutoInitCreateIndexBuffer)
					CreateIndexBuffer(m_IBcreationParams.m_LengthInBytes, m_IBcreationParams.m_Usage, m_IBcreationParams.m_Format, m_IBcreationParams.m_Pool);
			}
			m_AngleMode = ip->angleMode ? *ip->angleMode : Defaults.AngleMode;

			if(ip->pRenderStates) SetStates(ip->pRenderStates);
			if(ip->pTextureStageStates) SetStates(0, ip->pTextureStageStates); //TODO: Consider Stages
			if(ip->pSamplerStates) SetStates(0, ip->pSamplerStates); //TODO: Consider Stages

			if(ip->pDrawingParams) {
				m_PrimitiveType = ip->pDrawingParams->PrimitiveType;
				m_uiMinimumVertexIndex = ip->pDrawingParams->uiMinimumVertexIndex;
				m_uiStartIndex = ip->pDrawingParams->uiStartIndex;
				m_iBaseVertexIndex = ip->pDrawingParams->iBaseVertexIndex;

				if(!(_3DOBJECTFLAG_AUTOCOMPUTEVERTCOUNT & ip->pDrawingParams->AutomaticallyCount))
					m_uiNumVertices = ip->pDrawingParams->uiNumVertices;
				else SetFlags(GetFlags() | _3DOBJECTFLAG_AUTOCOMPUTEVERTCOUNT);
				if(!(_3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT & ip->pDrawingParams->AutomaticallyCount))
					m_uiPrimitiveCount = ip->pDrawingParams->uiPrimitiveCount;
				else SetFlags(GetFlags() | _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT);
			} else {
				m_PrimitiveType = Defaults.PrimitiveType;
				m_uiMinimumVertexIndex = Defaults.uiMinimumVertexIndex;
				m_uiStartIndex = Defaults.uiStartIndex;
				m_iBaseVertexIndex = Defaults.iBaseVertexIndex;

				m_uiNumVertices = Defaults.uiNumVertices;
				m_uiPrimitiveCount = Defaults.uiPrimitiveCount;
			}

			if(ip->pStreamSourceParams) {
				m_uiStreamNumber = ip->pStreamSourceParams->uiStreamNumber;
				m_uiOffsetInBytes = (ip->pStreamSourceParams->uiOffsetInBytes>0 && ::DXUTGetD3D9DeviceCaps()->DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)?ip->pStreamSourceParams->uiOffsetInBytes:0;
				m_uiStride = (!ip->pStreamSourceParams->AutomaticallyComputeStride) ?
					ip->pStreamSourceParams->uiStride : sizeof(T);
			} else {
				m_uiStreamNumber = Defaults.uiStreamNumber;
				m_uiOffsetInBytes = (Defaults.uiOffsetInBytes>0 && ::DXUTGetD3D9DeviceCaps()->DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)?Defaults.uiOffsetInBytes:0;
				m_uiStride =  sizeof(T);
			}
			m_FVF = (ip->pFVF) ? *ip->pFVF : Defaults.FVF;
			m_LockParams = (ip->pLockParams) ? *ip->pLockParams : Defaults.lockParams;

			SetFlags((ip->pFlags) ? *ip->pFlags: Defaults.dwFlags);//CAUTION: THIS HAS TO BE THE LAST ONE, ALWAYS!
		} else {//No ip defined
			m_AngleMode =  Defaults.AngleMode;

			m_PrimitiveType = Defaults.PrimitiveType;
			m_uiMinimumVertexIndex = Defaults.uiMinimumVertexIndex;
			m_uiStartIndex = Defaults.uiStartIndex;
			m_iBaseVertexIndex = Defaults.iBaseVertexIndex;

			m_uiNumVertices = Defaults.uiNumVertices;
			m_uiPrimitiveCount = Defaults.uiPrimitiveCount;
			m_uiStreamNumber = Defaults.uiStreamNumber;
			m_uiOffsetInBytes = (Defaults.uiOffsetInBytes>0 && ::DXUTGetD3D9DeviceCaps()->DevCaps2 & D3DDEVCAPS2_STREAMOFFSET)?Defaults.uiOffsetInBytes:0;
			m_uiStride = sizeof(T);

			m_FVF = Defaults.FVF;

			m_LockParams = Defaults.lockParams;

			SetFlags(Defaults.dwFlags);//CAUTION: THIS HAS TO BE THE LAST ONE, ALWAYS!
		} //endif(ip)
		m_bInnited = true;
	}
	
	template <typename T>
	C_D3DSpacialEntity<T>::C_D3DSpacialEntity(IDirect3DDevice9 *pDevice, InitializaionParameters *ip)
	{
		//Device Independent initializations go here:
		m_pIB =0; m_pVB =0;
		m_pDevice = 0;
		m_bUsesIndexBuffer = false;

		m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Scale    = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		m_uiLatestCreatedTextureIndex = -1;
		m_bEssentialInnited = m_bInnited = false;
		//Device Dependent initializations go here:
		if (pDevice)
			InitEssentials(pDevice, ip);
	}

	template <typename T>
	C_D3DSpacialEntity<T>::~C_D3DSpacialEntity(void)
	{
		SAFE_DELETE_ARRAY(m_pSamplerState);
		SAFE_DELETE_ARRAY(m_pRenderState);
		SAFE_DELETE_ARRAY(m_pTextureStageState);

		//SAFE_RELEASE(m_pDevice); //NEVER, EVER THINK OF THAT, THE DEVICE IS NOT UNDER THE CONTROL OF HERE! NO BUSSINESS TO HERE!
		SAFE_RELEASE(m_pIB);
		SAFE_RELEASE(m_pVB);
		for (UINT i=0; i<MAX_TEXTURES; i++)
			SAFE_RELEASE(m_arrTextureCache[i].m_pTexture);
	}
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::CreateTextureFromFile(LPCTSTR pSrcFile)
	{
		if (!pSrcFile || (Stage > MAX_TEXTURES && Stage != -1)) return THR(D3DERR_INVALIDCALL);
		IDirect3DTexture9 *pTex;
		HRESULT hr = THRTM(D3DXCreateTextureFromFile(m_pDevice, pSrcFile, &pTex),
			"D3DXCreateTextureFromFile() - FAILED.");
		if (Stage == -1) 
			Stage = (m_uiLatestCreatedTextureIndex == -1) ? m_uiLatestCreatedTextureIndex =0: m_uiLatestCreatedTextureIndex++;
		if (pOverrideSamplerStates)
			if (uiSamplerStatesCount == 0) 
				THRTM(D3DERR_INVALIDCALL, 
				"If you want to override the Sampler states provide some states and \nOF COURSE THE COUNT of those states!");
			else SetStates(Stage, pOverrideSamplerStates, uiSamplerStatesCount);

		textureCache txc = {pTex, Stage, m_pSamplerState[Stage]};
		m_arrTextureCache[Stage] = txc;
		return S_OK;
	}
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::SetTexture(UINT Stage) 
	{
		if (Stage> 7) return THR(D3DERR_INVALIDCALL);
		return THR(m_pDevice->SetTexture(Stage, m_arrTextureCache[Stage].m_pTexture));
	}

	template <typename T>
	void C_D3DSpacialEntity<T>::SetMaterial(D3DMATERIAL9 *pMaterial)
	{
		m_Material = *pMaterial;
		THR(m_pDevice->SetMaterial(pMaterial));
	}

	template <typename T>
	void C_D3DSpacialEntity<T>::SetLight(D3DLIGHT9 *pLight, UINT index, bool enable, bool force)
	{
		lightCache lc = {*pLight, enable};
		m_Lights.Add(lc);
		if (force) {
			THR(m_pDevice->SetLight(index, pLight));
			THR(m_pDevice->LightEnable(index, enable));
			//if (!(m_FVF & D3DFVF_NORMAL)) //TODO: tell the user to use the lighting object should have D3DFVF_NORMAL in the its FVF.
			Render_State EnableLighting[] = {D3DRS_LIGHTING, TRUE};
			SetStates(EnableLighting, NUM_OF(EnableLighting));
		}
	}
	template <typename T>
	void C_D3DSpacialEntity<T>::ApplyLights() 
	{
		UINT size = m_Lights.GetSize();
		lightCache lc;
		for (UINT i= 0; i<size; i++){
			lc = m_Lights[i];
			THR(m_pDevice->SetLight(i, &lc.m_Light));
			THR(m_pDevice->LightEnable(i, lc.m_bIsActive));
		}
		//if (!(m_FVF & D3DFVF_NORMAL)) //TODO: tell the user to use the lighting object should have D3DFVF_NORMAL in the its FVF.
		Render_State EnableLighting[] = {D3DRS_LIGHTING, TRUE};
		SetStates(EnableLighting, NUM_OF(EnableLighting));
	}
	template <typename T>
	inline void C_D3DSpacialEntity<T>::SetLightEnable(UINT index, bool enable, bool force) {
		UINT size = m_Lights.GetSize();
		if (index > size && force) {
			//In this case the device creates a default directional one, so let the device do that, then store the created one!
			THR(m_pDevice->LightEnable(index, enable));
			THR(m_pDevice->GetLight(index, &light));
			lightCache lc = {light, enable};
			m_Lights.Add(lc);
		} else {
		m_Lights[i].m_bIsActive = enable;
		if(force) THR(m_pDevice->LightEnable(index, enable));
		}
	}
	template <typename T>
	inline void C_D3DSpacialEntity<T>::GetLightEnable(UINT index, bool *enable) {
		if (!enable) return;
		UINT size = m_Lights.GetSize();
		if (size < index) return;
		*enable = m_Lights[index].m_bIsActive;
	}
	template <typename T>
	inline UINT C_D3DSpacialEntity<T>::GetActiveLightsCount() const {
		UINT size = m_Lights.GetSize(), count =0;
		for(UINT i =0; i<size; i++)
			count +=m_Lights[i].m_bIsActive;
		return count;
	}
	template <typename T>
	D3DLIGHT9 C_D3DSpacialEntity<T>::GetLight(D3DLIGHT9 *pLight, UINT index, bool *enable)
	{
		UINT size = m_Lights.GetSize();
		if (size < index) return;
		pLight = &m_Lights[index].m_Light;
		if (enable) *enable = m_Lights[index].m_bIsActive;
	}

	template <typename T>		
	const InitializaionParameters C_D3DSpacialEntity<T>::GetAllCurrentSettings()
	{
		InitializaionParameters out;
		out.pVBcreationParams = &m_VBcreationParams; 
		out.pIBcreationParams = &m_IBcreationParams;
		//DWORD DeviceBehaviours;
		out.pAngleMode = &m_AngleMode;

		out.pRenderStates = m_pRenderState;
		out.pTextureStageStates = m_pTextureStageState[0]; //TODO: CONSIDER OTHER STAGES, AND EVEN THE COUNT
		out.pSamplerStates = m_pSamplerState[0]; //TODO: CONSIDER OTHER STAGES, AND EVEN THE COUNT

		DrawingParams drawingParams;
		drawingParams.PrimitiveType = m_PrimitiveType;
		drawingParams.uiMinimumVertexIndex = m_uiMinimumVertexIndex;
		drawingParams.uiStartIndex = m_uiStartIndex;
		drawingParams.iBaseVertexIndex = m_iBaseVertexIndex;
		drawingParams.uiNumVertices = m_uiNumVertices;
		drawingParams.uiPrimitiveCount = m_uiPrimitiveCount;
		drawingParams.AutomaticallyCount = (m_dwFlags & _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT) |
			(m_dwFlags & _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT);
		out.pDrawingParams = &drawingParams;

		StreamSourceParams streamSourceParams;
		streamSourceParams.uiStreamNumber = m_uiStreamNumber;
		streamSourceParams.uiOffsetInBytes = m_uiOffsetInBytes; 
		streamSourceParams.uiStride = m_uiStride;
		out.pStreamSourceParams = &streamSourceParams;

		out.pLockParams = &m_LockParams;
		out.pFVF = &m_FVF;
		out.Flags = &m_dwFlags;
		return out;
	}

	// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
	// or that are tied to the back buffer size 
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::OnDeviceReset(IDirect3DDevice9 *pDevice)
	{
		if (m_IBcreationParams.m_Pool == D3DPOOL_DEFAULT && m_pIB == 0) 
		{
				THRTM(CreateIndexBuffer(m_IBcreationParams.m_LengthInBytes, m_IBcreationParams.m_Usage, m_IBcreationParams.m_Format, m_IBcreationParams.m_Pool),
					"CreateIndexBuffer() - FAILED");
				if (m_Indices.GetSize())
					FeedIndexBuffer(&m_Indices);
		}
		if (m_VBcreationParams.m_Pool == D3DPOOL_DEFAULT && m_pVB == 0) 
		{
			THRTM(CreateVertexBuffer(m_VBcreationParams.m_LengthInBytes, m_VBcreationParams.m_Usage, m_VBcreationParams.m_FVF, m_VBcreationParams.m_Pool),
				"CreateVertexBuffer() - FAILED");
			if (m_Vertices.GetSize())
				FeedVertexBuffer(&m_Vertices);
		}
		return S_OK;
	}
	// Release D3D9 resources created in the OnDeviceReset() 
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::OnDeviceLost()
	{
		/*Device Reset will fail unless the application releases all resources that are allocated in 
		D3DPOOL_DEFAULT, including those created by the IDirect3DDevice9::CreateRenderTarget and 
		IDirect3DDevice9::CreateDepthStencilSurface methods.*/
		if (m_pIB) {
			D3DINDEXBUFFER_DESC IB_Desc;
			THRTM(m_pIB->GetDesc(&IB_Desc), "IndexBuffer->GetDesc() - D3DERR_INVALIDCALL.");
			if (IB_Desc.Pool == D3DPOOL_DEFAULT)
				SAFE_RELEASE(m_pIB);
		}

		if (m_pVB) {
			D3DVERTEXBUFFER_DESC VB_Desc;
			THRTM(m_pVB->GetDesc(&VB_Desc), "VertexBuffer->GetDesc() - D3DERR_INVALIDCALL.");
			if (VB_Desc.Pool == D3DPOOL_DEFAULT)
				SAFE_RELEASE(m_pVB);
		}
		return S_OK;
	}
	
	// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
	// and aren't tied to the back buffer size
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::OnDeviceCreate(IDirect3DDevice9 *pDevice)
	{
		if (!m_bEssentialInnited) 
			InitEssentials(pDevice);
		return S_OK;
	}
	// Release D3D9 resources created in the OnDeviceCreate()
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::OnDeviceDestroy()
	{
		if (m_pIB) {
			D3DINDEXBUFFER_DESC IB_Desc;
			THRTM(m_pIB->GetDesc(&IB_Desc), "IndexBuffer->GetDesc() - D3DERR_INVALIDCALL.");
			if (IB_Desc.Pool == D3DPOOL_MANAGED)
				SAFE_RELEASE(m_pIB);
		}

		if (m_pVB) {
			D3DVERTEXBUFFER_DESC VB_Desc;
			THRTM(m_pVB->GetDesc(&VB_Desc), "VertexBuffer->GetDesc() - D3DERR_INVALIDCALL.");
			if (VB_Desc.Pool == D3DPOOL_MANAGED)
				SAFE_RELEASE(m_pVB);
		}
		return S_OK;
	}

	template <typename T>
	void C_D3DSpacialEntity<T>::SetFlags(DWORD flags)
	{
	//extern const DWORD T::FVF;
		m_dwFlags = flags;
		if (m_dwFlags & _3DOBJECTFLAG_USEDEFINEDVERTEXFVF)
		{
			m_FVF = (T::FVF);
		}
	}

	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::CreateIndexBuffer(UINT LengthInBytes, DWORD Usage = 0, D3DFORMAT Format = D3DFMT_INDEX16, D3DPOOL Pool = D3DPOOL_MANAGED)
	{
		HRESULT hr;
		if (Pool == D3DPOOL_DEFAULT && !(Usage & D3DUSAGE_WRITEONLY)) THRTM(D3DERR_INVALIDCALL, "Creating Index buffer resource in 'DEFAULT' memory pool without the 'WRITEONLY' usage might cause a sever performance penalty!");
		m_IBcreationParams = IndexBufferCreationParameters(LengthInBytes, Usage, Format, Pool);
		hr = THRTM(m_pDevice->CreateIndexBuffer(LengthInBytes, Usage, Format, Pool, &m_pIB, 0), "CreateIndexBuffer() - FAILED.");
		m_bUsesIndexBuffer = true;
		return hr;
	}

	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::CreateVertexBuffer(UINT LengthInBytes, DWORD Usage, DWORD FVF, D3DPOOL Pool = D3DPOOL_MANAGED)
	{
		HRESULT hr;
		if (Pool == D3DPOOL_SCRATCH) THRTM(D3DERR_INVALIDCALL ,"Don't set the Vertex Buffer resource pool to 'SCRATCH'.");
		DXUTDeviceSettings DevSettings = ::DXUTGetDeviceSettings();
		DWORD DeviceBehaviourFlags = (DevSettings.ver == DXUT_D3D9_DEVICE) ? DevSettings.d3d9.BehaviorFlags : DevSettings.d3d10.CreateFlags;
		if (D3DCREATE_HARDWARE_VERTEXPROCESSING & DeviceBehaviourFlags || 
			D3DCREATE_SOFTWARE_VERTEXPROCESSING & DeviceBehaviourFlags ||
			D3DCREATE_MIXED_VERTEXPROCESSING & DeviceBehaviourFlags) Usage = 0;
		//However, for the second or third condition it could've been the D3DUSAGE_SOFTWAREPROCESSING, but the results would've been identical, in eighter cases.

		m_VBcreationParams = VertexBufferCreationParameters(LengthInBytes, Usage, FVF, Pool);
		hr = THRTM(m_pDevice->CreateVertexBuffer(LengthInBytes, Usage, FVF, Pool, &m_pVB, 0), "CreateVertexBuffer() - FAILED.");
		return hr;
	}
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::FeedVertexBuffer(CGrowableArray<T> *pVertices, UINT VertCount) 
	{
		int size = pVertices->GetSize();
		if (!size) THRTM(D3DERR_INVALIDCALL, "Invalid 'Vertices' argument, (empty array).");
		if (!m_pVB) THRTM(E_FAIL, "Please create the 'VertexBuffer' before attemping to feed it!");
		if (VertCount == 0) VertCount = size;
		if (VertCount * sizeof(T) > m_VBcreationParams.m_LengthInBytes)
			THRTM(D3DERR_INVALIDCALL, "The size of whole Vertices is bigger than the size of 'Vetex Buffer'.");
		T* temp;
		THRTM(m_pVB->Lock(m_LockParams.uiOffsetToLock, m_LockParams.uiSizeToLock, (void**)&temp, m_LockParams.dwFlags),
			"VertexBuffer->Lock() - D3DERR_INVALIDCALL.");

		m_Vertices.RemoveAll();
		m_Vertices = *pVertices;
		updatePrimitiveCount();
		for (UINT i=0; i<VertCount; i++){
			SimpleVertex v = m_Vertices[i];
			temp[i] = m_Vertices[i];}
		m_uiNumVertices = (m_dwFlags & _3DOBJECTFLAG_AUTOCOMPUTEVERTCOUNT) ? size :0;//0 indicates all verts to be drawn.
		THRTM(m_pVB->Unlock(), "VertexBuffer->Unlock() - D3DERR_INVALIDCALL.");
		return S_OK;
	}
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::FeedIndexBuffer(CGrowableArray<WORD> *pIndices, UINT IndicesCount =0)
	{
		int size = pIndices->GetSize();
		if (!size) THRTM(D3DERR_INVALIDCALL, "Invalid 'Indices' argument, (empty array).");
		if (!m_pIB) THRTM(E_FAIL, "Please create the 'IndexBuffer' before attemping to feed it!");
		if (IndicesCount == 0) IndicesCount = size;
		if (IndicesCount * sizeof(WORD) > m_IBcreationParams.m_LengthInBytes)
			THRTM(D3DERR_INVALIDCALL, "The size of whole Indices is bigger than the size of 'Index Buffer'.");
		WORD* temp;
		THRTM(m_pIB->Lock(m_LockParams.uiOffsetToLock, m_LockParams.uiSizeToLock, (void**)&temp, m_LockParams.dwFlags),
			"IndexBuffer->Lock() - D3DERR_INVALIDCALL.");

		m_Indices.RemoveAll();
		m_Indices = *pIndices;
		m_bUsesIndexBuffer = true;
		updatePrimitiveCount();
		for (UINT i=0; i<IndicesCount; i++)
			temp[i] = m_Indices[i];
		THRTM(m_pIB->Unlock(), "IndexBuffer->Unlock() - D3DERR_INVALIDCALL.");

		return S_OK;
	}
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::FeedVertexBufferStream(T *pStream, UINT count)
	{
		if (!pStream || count == 0) return E_FAIL;
		CGrowableArray<T> temp;
		for (UINT i=0; i<count; i++)
			temp.Add(pStream[i]);
		return FeedVertexBuffer(&temp);
	}
	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::FeedIndexBufferStream(WORD *pStream, UINT count)
	{
		if (!pStream || count == 0) return E_FAIL;
		CGrowableArray<WORD> temp;
		for (UINT i=0; i<count; i++)
			temp.Add(pStream[i]);
		return FeedIndexBuffer(&temp);
	}

	template <typename T>
	bool C_D3DSpacialEntity<T>::updatePrimitiveCount() {
		bool ret = false;
		if (m_dwFlags & _3DOBJECTFLAG_AUTOCOMPUTEPRIMITIVESCOUNT) {
			if (!m_bUsesIndexBuffer)
			{
				switch (m_PrimitiveType) {
					case D3DPT_TRIANGLELIST:
						m_uiPrimitiveCount = m_Vertices.GetSize() / 3; 
						ret = true;
						break;
					case D3DPT_POINTLIST:
						m_uiPrimitiveCount = m_Vertices.GetSize();
						ret = true;
						break;
					case D3DPT_LINELIST:
						m_uiPrimitiveCount = m_Vertices.GetSize() / 2;
						ret = true;
						break;
				}//end of switch
			}
			else {//m_bUsesIndexBuffer
				switch (m_PrimitiveType) {
					case D3DPT_TRIANGLELIST:
						m_uiPrimitiveCount = m_Indices.GetSize() / 3; 
						ret = true;
						break;
					//TODO: CONSIDER OTHER POSSIBILITIES FOR COMPUTING THE (m_uiPrimitiveCount)
					default:
						ret=!(m_uiPrimitiveCount = 0);
				}//end of switch
			}
		}
		return ret;
	}
	template <typename T>
	void C_D3DSpacialEntity<T>::SetAngleMode(AngleMode angleMode)
	{
		if (angleMode == m_AngleMode) return;
		m_AngleMode = angleMode;
		float coefficient = (m_AngleMode == RADIANS ? TO_RADIANS : TO_DEGREE);
		//convert from Degree to Radians or vice-versa;
		m_Rotation.x *= coefficient;
		m_Rotation.y *= coefficient;
		m_Rotation.z *= coefficient;
	}


	template <typename T>
	HRESULT C_D3DSpacialEntity<T>::Draw() 
	{
		D3DXMATRIX combined, pos, Rx, Ry, Rz, Scale;
		::D3DXMatrixTranslation(&pos, m_Position.x, m_Position.y, m_Position.z);

		::D3DXMatrixRotationX (&Rx, m_Rotation.x); //No wrong retrun could be out of these methods, no need to exception handling.
		::D3DXMatrixRotationY (&Ry, m_Rotation.y);
		::D3DXMatrixRotationZ (&Rz, m_Rotation.z);

		::D3DXMatrixScaling(&Scale, m_Scale.x, m_Scale.y, m_Scale.z);

		combined = pos * Rx * Ry * Rz * Scale;
		THRTM(m_pDevice->SetTransform(D3DTS_WORLD, &combined), "SetTransform() - D3DERR_INVALIDCALL.");
		
		THRTM(m_pDevice->SetStreamSource(m_uiStreamNumber, m_pVB, m_uiOffsetInBytes, m_uiStride), "SetStreamSource() - D3DERR_INVALIDCALL");
		if (m_bUsesIndexBuffer)
			THRTM(m_pDevice->SetIndices(m_pIB), "SetIndices() - D3DERR_INVALIDCALL.");
		THRTM(m_pDevice->SetFVF(m_FVF), "SetFVF() - D3DERR_INVALIDCALL.");

		if (m_dwFlags & _3DOBJECTFLAG_USELIGHTING) 
			ApplyLights();
		if (m_dwFlags & _3DOBJECTFLAG_USETEXTURES) 
			SetTexture(0);
	
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		if (m_bUsesIndexBuffer)
			THRTM(m_pDevice->DrawIndexedPrimitive(m_PrimitiveType, m_iBaseVertexIndex, m_uiMinimumVertexIndex,
			m_uiNumVertices, m_uiStartIndex, m_uiPrimitiveCount), "DrawIndexedPrimitive() - D3DERR_INVALIDCALL.");
		else
			THRTM(m_pDevice->DrawPrimitive(m_PrimitiveType, m_uiStartIndex, m_uiPrimitiveCount), "DrawPrimitive() - D3DERR_INVALIDCALL.");
		return S_OK;
	}

}//end of namespace
#endif