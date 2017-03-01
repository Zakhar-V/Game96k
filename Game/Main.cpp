#include "../Engine/Engine.cpp"


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

/*
+ Система сущностей/компонентов
+ Отложенное добавление/удаление компонентов и узлов
+ 

+ Система поведения

+ Система активации/деактивации объектов
+ Игровой цикл
*/

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

#define gGame Game::Get()

uint __stdcall TestThread(void* _param)
{
	while (true)
	{
		Sleep(1);
	}
}

uint g_frameCount = 0;

class TestBehavior : public Behavior
{
public:
	RTTI("TestBehavior");

protected:
	void _Start(void) override
	{
		LOG("TestBehavior Start (%d)", g_frameCount);
	}
	void _Update(void) override
	{
	}
	void _OnDestroy(void) override
	{
		LOG("TestBehavior OnDestroy (%d)", g_frameCount);
	}

};

class FirstPersonCamera : public Behavior
{
public:
	RTTI("FirstPersonCamera");

protected:
	void _Start(void) override
	{
		LOG("FirstPersonCamera Start (%d)", g_frameCount);
	}
	void _Update(void) override
	{
	}
	void _OnDestroy(void) override
	{
		LOG("FirstPersonCamera OnDestroy (%d)", g_frameCount);
	}

};

class Game : public Application
{
public:
	static Game* Get(void) { return static_cast<Game*>(s_instance); }

	//HashMap<StringHash, String> test;
	ScenePtr m_scene;
	EntityPtr m_cameraNode;
	Game(void)
	{
		LOG_NODE("Init game");

		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)TestThread, 0, 0, nullptr);

		//
		{
			//test["abc"] = "def";
		}

		// test scene
		{
			gReflection->GetOrCreateTypeInfo<FirstPersonCamera>()->SetFactory([]() { return ObjectPtr(new FirstPersonCamera); });
			gReflection->GetOrCreateTypeInfo<TestBehavior>()->SetFactory([]() { return ObjectPtr(new TestBehavior); });



			m_scene = new Scene;
			gSceneManager->AddScene(m_scene);


			m_cameraNode = new Entity;
			Camera* _camera = m_cameraNode->AddComponent<Camera>();
			FirstPersonCamera* _cameraController = m_cameraNode->AddComponent<FirstPersonCamera>();
			m_scene->AddEntity(m_cameraNode);


			EntityPtr _test = m_scene->AddEntity();
			_test->AddComponent<TestBehavior>();

			m_cameraNode->SetParent(_test);
			//m_scene->AddEntity(_test);

			//m_scene->AddEntity(_test);


		}

		// quad
		{
			Vertex _fsq[4];
			_fsq[0].position.Set(-1, 1, 0); // lt
			_fsq[1].position.Set(-1, -1, 0); // lb
			_fsq[2].position.Set(1, -1, 0); // rb
			_fsq[3].position.Set(1, 1, 0); // rt

			Vector4(1, 0, 0, 1).Pack(_fsq[0].color); // lt
			Vector4(0, 1, 0, 1).Pack(_fsq[1].color); // lb
			Vector4(0, 0, 1, 1).Pack(_fsq[2].color); // rb
			Vector4(1, 1, 1, 1).Pack(_fsq[3].color); // rt


			_fsq[0].SetTexCoord( 0, 1 ); // lt
			_fsq[1].SetTexCoord( 0, 0 ); // lb
			_fsq[2].SetTexCoord( 1, 0 ); // rb
			_fsq[3].SetTexCoord( 1, 1 ); // rt 

			const uint16 _idx[6] = { 3, 0, 1, 3, 1, 2 };

			m_fsQuadVB = new Buffer;
			m_fsQuadVB->Realloc(sizeof(_fsq), _fsq);

			m_fsQuadIB = new Buffer;
			m_fsQuadIB->Realloc(sizeof(_idx), _idx);

			Vertex _q[4];
			_q[0].position.Set(-0.1f, 0.1f, 0); // lt
			_q[1].position.Set(-0.1f, -0.1f, 0); // lb
			_q[2].position.Set(0.1f, -0.1f, 0); // rb
			_q[3].position.Set(0.1f, 0.1f, 0); // rt

			Vector4(1, 0, 0, 1).Pack(_q[0].color); // lt
			Vector4(0, 1, 0, 1).Pack(_q[1].color); // lb
			Vector4(0, 0, 1, 1).Pack(_q[2].color); // rb
			Vector4(1, 1, 1, 1).Pack(_q[3].color); // rt

			_q[0].texcoord[0] = FloatToHalf(0), _q[0].texcoord[1] = FloatToHalf(1); // lt
			_q[1].texcoord[0] = FloatToHalf(0), _q[1].texcoord[1] = FloatToHalf(0); // lb
			_q[2].texcoord[0] = FloatToHalf(1), _q[2].texcoord[1] = FloatToHalf(0); // rb
			_q[3].texcoord[0] = FloatToHalf(1), _q[3].texcoord[1] = FloatToHalf(1); // rt

			m_quadVB = new Buffer;
			m_quadVB->Realloc(sizeof(_q), _q);
		}

		// texture
		{
			Vector3 _img[2*2] =
			{
				{ 1, 0, 0 }, { 0, 1, 0 },
				{ 0, 0, 1 }, { 1, 1, 1 },
			};
			m_texture = new Texture;
			m_texture->Init(TextureType::Quad, PixelFormat::RGBA8, -1);
			m_texture->Realloc(2, 2);
			m_texture->Write(PixelFormat::RGB32F, _img);
			m_texture->GenerateMipmap();
		}

		//rtt
		{
			m_colorBuffer.Init(PixelFormat::RGBA8);
			m_depthBuffer.Init(PixelFormat::D24S8);
		}

	}
	~Game(void)
	{
	}

protected:


	virtual EventResult _PreEvent(int _type, void* _data = nullptr)
	{
		switch (_type)
		{
		case ET_BeginFrame:
		{
			++g_frameCount;
		} break;

		case ET_EndFrame:
		{
			m_quit = !gDevice->IsOpened();

		} break;

		case ET_PreRender:
		{
			//Sleep(1000 / 60); // TODO: frame limit

			/*if (!m_cameraNode)
			{
				m_cameraNode = new Entity;
				Camera* _camera = m_cameraNode->AddComponent<Camera>();
				FirstPersonCamera* _cameraController = m_cameraNode->AddComponent<FirstPersonCamera>();

				m_scene->AddEntity(m_cameraNode);
			}*/
				
			//m_scene->RemoveEntity(m_cameraNode);
			

			gGraphics->SetDepthState(DepthState::Enabled);
			gGraphics->ClearFrameBuffers(FBT_All, 0, 1, Vector4(0.75f, 0.75f, 0.8f, 1));

			// rtt
			{
				m_colorBuffer.Realloc(gDevice->Width(), gDevice->Height());
				m_depthBuffer.Realloc(gDevice->Width(), gDevice->Height());
			}
			
			// Camera
			{
				gGraphics->Camera->CameraPos = Vector3::Zero;
				gGraphics->Camera->ViewMat = Matrix44::Identity;
				gGraphics->Camera->ProjMat = Matrix44::Identity;
				gGraphics->Camera->ViewProjMat = Matrix44::Identity;
				gGraphics->Camera->InvViewProjMat = Matrix44::Identity;
				gGraphics->Camera->NormMat = Matrix44::Identity;
				gGraphics->Camera->CameraPos = Vector4::Identity;
				gGraphics->Camera->Depth = Vector4(0.1f, 100.f, 0, 1); // near, far, C=constant, FC = 1.0/log(far*C + 1)
				gGraphics->Camera->ScreenSize = {800, 600};
				gGraphics->Camera->InvScreenSize = 1 / gGraphics->Camera->ScreenSize;
				gGraphics->Invalidate(ShaderParam::Camera);
			}

			// Raster
			{
				gGraphics->RasterizerParams->DepthBias = 0;
				gGraphics->RasterizerParams->SilhouetteOffset = 0;
				gGraphics->Invalidate(ShaderParam::RasterizerParams);
			}

			// world mat
			{
				gGraphics->InstanceMatrices[0] = Matrix44::Identity;
				gGraphics->Invalidate(ShaderParam::InstanceMatrices, 1);
			}

			// params
			{
				gGraphics->InstanceParams[0].r[0] = Vector4::One;
				gGraphics->InstanceParams[0].r[1] = Vector4::One;
				gGraphics->Invalidate(ShaderParam::InstanceParams, 1);
			}


			gGraphics->SetDepthState(DepthState::Disabled);
			gGraphics->SetVertexBuffer(m_fsQuadVB);
			gGraphics->SetIndexBuffer(m_fsQuadIB);
			gGraphics->SetShader(Shader::VS_FSQuad);
			gGraphics->SetShader(Shader::GS_None);
			gGraphics->SetShader(Shader::PS_Texture);
			gGraphics->SetTexture(0, m_texture);
			gGraphics->SetSampler(0, Sampler::NearestClamp);
			gGraphics->DrawIndexed(PrimitiveType::Triangles, 0, 0, 6);


			//gGraphics->ResetTextures();
			//gGraphics->SetShader(Shader::PS_NoTexture);
			gGraphics->SetShader(Shader::VS_StaticModel);
			gGraphics->SetDepthState(DepthState::Enabled);

			uint _numInstances = 200;
			for (uint i = 0; i < 300; ++i)
			{
				// world mat
				{
					gGraphics->InstanceMatrices[0] = Matrix44::Identity;
					gGraphics->Invalidate(ShaderParam::InstanceMatrices, _numInstances);
				}

				// params
				{
					gGraphics->InstanceParams[0].r[0] = { .5f, .5f, .5f, 1 };
					gGraphics->InstanceParams[0].r[1] = Vector4::Identity;
					gGraphics->Invalidate(ShaderParam::InstanceParams, _numInstances);
				}

				gGraphics->SetVertexBuffer(m_quadVB);
				
				gGraphics->DrawIndexed(PrimitiveType::Triangles, 0, 0, 6, _numInstances);
			}
#if defined(_STATISTICS) && 0
			LOG("Buffers: %d, %s (IO: %s/%s, SP: %s), Draw calls: %d (%d instances, %s, %s)", 
				gGraphics->stats.bufferCount,
				*String::PrintSize(gGraphics->stats.bufferMemory, 1024, "", "b"),
				*String::PrintSize(gGraphics->stats.frame.bufferWrite, 1024, "", "b"),
				*String::PrintSize(gGraphics->stats.frame.bufferRead, 1024, "", "b"),
				*String::PrintSize(gGraphics->stats.frame.spUploaded, 1024, "", "b"),
				gGraphics->stats.frame.drawCalls, 
				gGraphics->stats.frame.drawInstances, 
				*String::PrintSize(gGraphics->stats.frame.drawVertices, 1000, "", " verts"),
				*String::PrintSize(gGraphics->stats.frame.drawPoints + gGraphics->stats.frame.drawLines + gGraphics->stats.frame.drawTriangles, 1000, "", " primitives"));
#endif
		} break;

		}
		return ER_Pass; 
	};
	virtual EventResult _PostEvent(int _type, void* _data = nullptr)
	{ 
		return ER_Pass;
	};


	//temp
	BufferPtr m_fsQuadVB;
	BufferPtr m_fsQuadIB;

	BufferPtr m_quadVB;
	BufferPtr m_quadIB;


	TexturePtr m_texture;

	RenderBuffer m_colorBuffer;
	RenderBuffer m_depthBuffer;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

template <class T> struct DbvtArrayContainer : DbvtContainer
{
	void AddObject(void* _object, const AlignedBox& _bbox, bool _withTest) override
	{ 
		objects.Push(reinterpret_cast<T*>(_object));
	}

	Array<T*> objects;
};


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

/*
-Add Scene/Entity/Component/SceneSystem/SceneManager
-Add Viewport/View/Renderer
-Add Object Factory
-Add HashMap
*/

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

// todo: remove sdbm checksum

#define PRINT_SIZEOF(T) LOG("sizeof("#T") = %d", sizeof(T))

void main(void)
{
	//printf("");
	/*String _str("abc");
	File _file;
	_str = _file.Read<String>();
	Test(_str);*/

	{
		/*Frustum _bv;
		DbvtArrayContainer<RenderComponent*> _dst;
		DbvtFrustumCallback _cb;
		_cb.container = &_dst; 

		Dbvt _dbvt;
		_dbvt.Enum(&_cb); 

		Matrix44 _m;
		_m.CreateTransform(0, 1, 1);
		_m.Inverse();
		if (_m.GetScale().Length() > 1)*/
		{

		LOG(APP_NAME " build v0.2.0 [WIP] " __DATE__ " " __TIME__);
		PRINT_SIZEOF(Shader::Desc);
			Game _game;
			_game.Run();

		}

	}

#ifdef _DEBUG
	//system("pause");
#endif

	ExitProcess(0);
}
