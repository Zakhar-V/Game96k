#include "Common.hpp"

#if 0

#include "../Engine/Engine.hpp"



////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

#define gGame Game::Get()

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

#define PRINT_SIZEOF(...) LOG("sizeof(" _QUOTE(##__VA_ARGS__) ") = %d", sizeof(##__VA_ARGS__))

//----------------------------------------------------------------------------//
// FirstPersonCamera
//----------------------------------------------------------------------------//

class FirstPersonCamera : public Behavior
{
public:
	RTTI("FirstPersonCamera");

	void SetTargetCamera(Entity* _target)
	{
		m_targetCamera = _target;
		if (m_targetCamera)
			m_cameraRotation = m_targetCamera->GetLocalRotation();

		//m_targetCamera->SetInheritRotation(false);
	}

	void SetTargetCharacter(Entity* _target)
	{
		m_targetCharacter = _target;
		if (m_targetCharacter)
			m_characterRotation = m_targetCharacter->GetLocalRotation();
	}
	
	float sensitivity = 0.25f; // Чувствительность
	bool smooth = true; // Сглаживать движения
	float smoothTime = 15; // Время сглаживания движения

	bool limitedPitch = true; // Ограничивать вращение по вертикали
	float minPitch = -90; // Минимальный угол по вертикали
	float maxPitch = 90; // Максимальный угол по вертикали

protected:

	Entity* m_targetCamera = nullptr; // todo: use ref
	Entity* m_targetCharacter = nullptr; // todo: use ref
	Quaternion m_cameraRotation = Quaternion::Identity;
	Quaternion m_characterRotation = Quaternion::Identity;

	void _Update(void) override
	{
		if (m_targetCamera && m_targetCharacter)
		{
			float _yaw = gDevice->CameraDelta().x * -sensitivity;
			float _pitch = gDevice->CameraDelta().y * -sensitivity;

			m_characterRotation *= Quaternion().FromAxisAngle(Vector3::UnitY, Radians(_yaw));
			m_cameraRotation *= Quaternion().FromAxisAngle(Vector3::UnitX, Radians(_pitch));

			m_characterRotation.Normalize();
			m_cameraRotation.Normalize();

			if (limitedPitch)
			{
				const float _mp = Radians(.5f);
				m_cameraRotation.x = Tan(Clamp(ATan2(m_cameraRotation.x, m_cameraRotation.w), minPitch * _mp, maxPitch * _mp));
				m_cameraRotation.y /= m_cameraRotation.w;
				m_cameraRotation.z /= m_cameraRotation.w;
				m_cameraRotation.w = 1;
			}

			if (smooth)
			{
				m_targetCharacter->SetLocalRotation(m_targetCharacter->GetLocalRotation().Slerp(m_characterRotation, Clamp01(smoothTime * gTimer->FrameTime())));
				m_targetCamera->SetLocalRotation(m_targetCamera->GetLocalRotation().Slerp(m_cameraRotation, Clamp01(smoothTime * gTimer->FrameTime())));
			}
			else
			{
				m_targetCharacter->SetLocalRotation(m_characterRotation);
				m_targetCamera->SetLocalRotation(m_cameraRotation);
			}
			//LOG("%f %f %f", m_entity->GetWorldPosition().x, m_entity->GetWorldPosition().y, m_entity->GetWorldPosition().z);
		}

		/*Vector3 _move = Vector3::Zero;
		if (GetAsyncKeyState('W') & 0x8000)
		{
			_move.z -= 1;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			_move.z += 1;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			_move.x -= 1;
		}
		if (GetAsyncKeyState('D') & 0x8000)
		{
			_move.x += 1;
		}

		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			_move *= 50;
		}

		_move *= gTimer->FrameTime();
		//LOG("%04x %f %f %f", GetAsyncKeyState('W'), _move.x, _move.y, _move.z);

		m_entity->SetWorldPosition(m_entity->GetWorldPosition() + _move);*/
	}
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class PlayerMovement : public Behavior
{
public:
	RTTI("PlayerMovement");

	float forwardSpeed = 1;
	float backwardSpeed = .5;
	float strafeSpeed = 2.5f;

	float crouchCoeff = .5f; // Коэффициент скорости в присяде
	float runCoeff = 100; // Коэффициент скорости при беге

	bool smooth = true; // Сглаживать движения
	float smoothTime = 100; // Время сглаживания движения

protected:

  	//CharacterController character;
	float m_jumpTime = 0;


	void _Update(void) override
	{
		/*if (character.isGrounded)
		{
			jumpTime = 0;
			// TODO: jump
		}
		else
			jumpTime += Time.deltaTime;

		float forward = InputManager.GetAxis("MoveY");
		float strafe = InputManager.GetAxis("MoveX");

		forward *= forward > 0 ? forwardSpeed : backwardSpeed;
		strafe *= strafeSpeed;

		Vector3 move = transform.TransformDirection(new Vector3(strafe, 0, forward));
		character.Move(move * Time.deltaTime + Physics.gravity * jumpTime * Time.deltaTime);*/


		float _forward = 0;
		float _strafe = 0;

		if (GetAsyncKeyState('W') & 0x8000)
			_forward += 1;
		if (GetAsyncKeyState('S') & 0x8000)
			_forward -= 1;
		if (GetAsyncKeyState('A') & 0x8000)
			_strafe -= 1;
		if (GetAsyncKeyState('D') & 0x8000)
			_strafe += 1;

		_forward *= _forward > 0 ? forwardSpeed : backwardSpeed;
		_strafe *= strafeSpeed;

		Vector3 _move(_strafe, 0, _forward);
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			_move *= runCoeff;
		//_move = m_entity->GetWorldTransform().TransformVector(_move);
		_move = _move * m_entity->GetChild()->GetWorldRotation().Copy().Inverse(); // fly camera

		m_entity->SetWorldPosition(m_entity->GetWorldPosition() + _move * gTimer->FrameTime());
	}
};

//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//

class TestCamera : public Behavior
{
public:
	RTTI("TestCamera");

	float sensitivity = .25f; // Чувствительность
	bool smooth = false; // Сглаживать движения
	float smoothTime = 5; // Время сглаживания движения

	bool limitedPitch = false; // Ограничивать вращение по вертикали
	float minPitch = -90; // Минимальный угол по вертикали
	float maxPitch = 90; // Максимальный угол по вертикали



	float forwardSpeed = 3;
	float backwardSpeed = 2;
	float strafeSpeed = 2.5f;

	float crouchCoeff = 1.5f; // Коэффициент скорости в присяде
	float runCoeff = 1000; // Коэффициент скорости при беге


protected:
	float _cYaw = 0;
	float _cPitch = 0;

	float m_pitch, m_yaw;
	void _Start(void) override
	{
		Quaternion _rot = m_entity->GetLocalRotation();
		m_pitch = _rot.Pitch();
		m_yaw = _rot.Yaw();
	}

	void _Update(void) override
	{
		//Sleep(16);
		float _yaw = gDevice->CameraDelta().x * sensitivity;
		float _pitch = gDevice->CameraDelta().y * sensitivity;

		if (_yaw || _pitch)
		{
			Quaternion _rot = m_entity->GetLocalRotation();


			float _newYaw = _rot.Yaw() + (_yaw);
			float _newPitch = _rot.Pitch() + Radians(_pitch);

			//_rot.FromAxisAngle(Vector3::UnitX )
			
			Quaternion _rx, _ry;
			_rot.Normalize();

			//m_pitch.FromAxisAngle(Vector3::UnitX, m_pitch.Pitch() + _pitch);
			//m_yaw.FromAxisAngle(Vector3::UnitX, m_yaw.Yaw() + _yaw);

			m_pitch += Radians(_pitch);
			m_yaw += Radians(_yaw);

			_rx.FromAxisAngle(Vector3::UnitX, m_pitch).Normalize();
			_ry.FromAxisAngle(Vector3::UnitY, m_yaw).Normalize();
			m_entity->SetLocalRotation((_rx * _ry).Normalize());

			

			//m_entity->OnView();
			//LOG("(%f %f) (%f %f) %f", m_pitch, m_entity->GetLocalRotation().Pitch(), m_yaw, m_entity->GetLocalRotation().Yaw(), m_entity->GetLocalRotation().Roll());
		}

		{
			float _forward = 0;
			float _strafe = 0;

			if (GetAsyncKeyState('W') & 0x8000)
				_forward += 1;
			if (GetAsyncKeyState('S') & 0x8000)
				_forward -= 1;
			if (GetAsyncKeyState('A') & 0x8000)
				_strafe += 1;
			if (GetAsyncKeyState('D') & 0x8000)
				_strafe -= 1;

			_forward *= _forward > 0 ? forwardSpeed : backwardSpeed;
			_strafe *= strafeSpeed;

			Vector3 _move(_strafe, 0, _forward);
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
				_move *= runCoeff;
			_move = _move * m_entity->GetWorldRotation().Copy().Inverse();

			m_entity->SetWorldPosition(m_entity->GetWorldPosition() + _move * gTimer->FrameTime());

			//LOG("%f %f %f", m_entity->GetWorldPosition().x, m_entity->GetWorldPosition().y, m_entity->GetWorldPosition().z);
		}

		//LOG("%f", gTimer->FrameTime());

		// quat test	
		{
			//Quaternion _q1 = 
		
		}
		//Vector3 _euler = _rot.ToEuler();
		//_euler.x -= _pitch;
		//_euler.y += _yaw;
		//_rot.FromEuler(_euler);
		//_rot.Normalize();

		//float _cYaw = _rot.Yaw();
		//float _cPitch = _rot.Pitch();
		//_rot = Quaternion().FromAxisAngle(Vector3::UnitY, _cYaw += _yaw) * Quaternion().FromAxisAngle(Vector3::UnitY, _cPitch -= _pitch);
		//_rot.Normalize();
		//_rot.Normalize();
		//_rot.FromEuler(Vector3(_pitch, _yaw, 0));

		/*Quaternion _charRotation = Quaternion().FromAxisAngle(Vector3::UnitY, _rot.Yaw());
		Quaternion _camRotation = Quaternion().FromAxisAngle(Vector3::UnitX, _rot.Pitch());

		_charRotation *= Quaternion().FromAxisAngle(Vector3::UnitY, _yaw);
		_camRotation *= Quaternion().FromAxisAngle(Vector3::UnitX, -_pitch);  */

		/*if (limitedPitch)
		{
			const float _mp = Radians(.5f);
			m_cameraRotation.x = Tan(Clamp(ATan2(m_cameraRotation.x, m_cameraRotation.w), minPitch * _mp, maxPitch * _mp));
			m_cameraRotation.y /= m_cameraRotation.w;
			m_cameraRotation.z /= m_cameraRotation.w;
			m_cameraRotation.w = 1;
		} */

		/*if (smooth)
		{
			m_targetCamera->SetLocalRotation(m_targetCamera->GetLocalRotation().Slerp(m_cameraRotation, smoothTime * gTimer->FrameTime()));
			m_targetCharacter->SetLocalRotation(m_targetCharacter->GetLocalRotation().Slerp(m_characterRotation, smoothTime * gTimer->FrameTime()));
		}
		else 
		{
			m_targetCamera->SetLocalRotation(m_cameraRotation);
			m_targetCharacter->SetLocalRotation(m_characterRotation);
		}*/

		//LOG("%f %f %f (%f %f %f)", m_entity->GetWorldPosition().x, m_entity->GetWorldPosition().y, m_entity->GetWorldPosition().z, m_entity->GetWorldRotation().Pitch(), m_entity->GetWorldRotation().Yaw(), m_entity->GetWorldRotation().Roll());
	}

};



//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
Text* gDebugText = nullptr;
class TestLevel
{
public:

	void CreatePlayer(void)
	{
		EntityPtr _player = new Entity;
		_player->Instantiate();

		Camera* _cameraNode = _player->AddChild<Camera>();
		//_cameraNode->SetNear(.25f);
		_cameraNode->SetFar(25000);
		//_cameraNode->SetFov(-1);
		//_cameraNode->AddComponent<FirstPersonCamera>();

		_player->AddComponent<PlayerMovement>();
		FirstPersonCamera* _fpsCam = _player->AddComponent<FirstPersonCamera>();
		_fpsCam->SetTargetCamera(_cameraNode);
		_fpsCam->SetTargetCharacter(_player);

		/*CameraPtr _camera = new Camera;
		_camera->SetWorldPosition(Vector3(0, 0, -10));
		_camera->AddComponent<TestCamera>();
		_camera->Instantiate();*/

		_player->SetWorldPosition(Vector3(0,  5, -10));
	}

	MeshPtr CreateQuad(float _size)
	{
		MeshPtr _mesh = new Mesh;

		VertexArray& _v = _mesh->Vertices();
		_v.Resize(4);
		_v[0].position.Set(-_size, _size, 0); // lt
		_v[1].position.Set(-_size, -_size, 0); // lb
		_v[2].position.Set(_size, -_size, 0); // rb
		_v[3].position.Set(_size, _size, 0); // rt
		_v[0].color = Vector4(1, 0, 0, 1); // lt
		_v[1].color = Vector4(0, 1, 0, 1); // lb
		_v[2].color = Vector4(0, 0, 1, .0f); // rb
		_v[3].color = Vector4(1, 1, 1, .0f); // rt
		_v[0].SetTexCoord(0, 1); // lt
		_v[1].SetTexCoord(0, 0); // lb
		_v[2].SetTexCoord(1, 0); // rb
		_v[3].SetTexCoord(1, 1); // rt

		_mesh->Indices() = { 3, 0, 1, 3, 1, 2 };
		_mesh->Upload();

		_mesh->ComputeBounds();
		_mesh->SetType(PrimitiveType::Triangles);

		return _mesh;
	}

	MeshPtr CreateAxes(float size)
	{
		MeshPtr _mesh = new Mesh;

		VertexArray& _v = _mesh->Vertices();
		_v.Resize(6);
		_v[0].position.Set(0, 0, 0); // c
		_v[1].position.Set(size, 0, 0); // x
		_v[2].position.Set(0, 0, 0); // c
		_v[3].position.Set(0, size, 0); // y
		_v[4].position.Set(0, 0, 0); // c
		_v[5].position.Set(0, 0, size); // z


		_v[0].color = Vector4(1, 0, 0, 1); // c
		_v[1].color = Vector4(1, 0, 0, 1); // x
		_v[2].color = Vector4(0, 1, 0, 1); // c
		_v[3].color = Vector4(0, 1, 0, 1); // y
		_v[4].color = Vector4(0, 0, 1, 1); // c
		_v[5].color = Vector4(0, 0, 1, 1); // z

		_mesh->Indices() = { 0, 1, 2, 3, 4, 5 };
		_mesh->Upload();

		_mesh->ComputeBounds();
		_mesh->SetType(PrimitiveType::Lines);

		return _mesh;
	}

	void CreateScene(void)
	{
		CreatePlayer();
		{
		}



		{
			float _g[5];
			Image::CreateBlurCoeff(_g, 5, 0.9f);
		}

		/*{
			SharedPtr<UIQuad> _e = new UIQuad;
			_e->SetRect({ 10, 10, 310, 610 });
			_e->m_color.Set(0, 0, 0.25f, .3f);
			_e->Instantiate();
			
			gDebugText = _e->AddChild<UIText>();
			gDebugText->SetRect({ 15, 15, 305, 605 });
			gDebugText->SetTexture(_font, 0);
			gDebugText->SetText("abc\ndef");
			gDebugText->SetColor({ 1, 1, 1, 1 });
		}*/
			
		MeshPtr _axes = CreateAxes(1);
		{

			ImagePtr _img = new Image;
			//_img->CreateFont("Pollock3CTT", 24, .5f, false);
			_img->CreateFont("podchencoCTT", 24, .4f, false);
			TexturePtr _font = _img->_Export();

			if (1)
			{
				File _file;
				_file.SetName("test.bmp");
				_img->SaveBMP(_file);
				_file.Save();

				memset(_img->Data(), 0, _img->Width() * _img->Height() * 4 * 4);
				gGraphics->SetTexture(0, _font);
				_font->GenerateMipmap();
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, _img->Data());
				
				//_font->AddRef()
				_file.Clear();
				_img->SaveBMP(_file);
				_file.Save("tex.bmp");
			}

			/*SharedPtr<Sprite> _sprite = new Sprite;
			_sprite->SetAnchor({ 0, 0, 1, 1 });
			_sprite->SetPivot({1, 1});
			_sprite->Instantiate();
			_sprite->SetColor({ 1 });
			_sprite->SetTexture(_font, -1);	*/
			   
			/*Sprite* _sprite2 = _sprite->AddChild<Sprite>();
			_sprite2->SetAnchor({ 1, 1, 1, 1 });
			_sprite2->SetPivot({ 0,0});
			_sprite2->SetColor({1, 0, 0, 1});
			_sprite2->SetSize({ 100, 200 });
			_sprite2->SetLocalPosition({ -600, -100, 0 });

			_sprite->SetLocalPosition({ 0, 0, 1 });
			//_sprite->SetLocalRotation(Quaternion().FromAxisAngle(Vector3::UnitZ, Radians(45)));
			  */


			SharedPtr<Text> _text = new Text;
			_text->SetAnchor({ 0, 0, 1, 1 });
			_text->SetPivot({ 0, 0 });
			_text->SetFont(_font, 0);
			_text->SetText("text");
			_text->SetColor({1, 0, 1, .5f});
			_text->Instantiate();
			//_text->SetLocalScale(10);
			gDebugText = _text;


			/*Model* _model = _sprite->AddChild<Model>();
			_model->m_mesh = _axes;
			_model->Instantiate();
			_model->SetLocalPosition({ 0, 0, 0.5f });
			_model->SetLocalScale(50);
				 
			_model = _text->AddChild<Model>();
			_model->m_mesh = _axes;
			_model->Instantiate();
			_model->SetLocalPosition({ 0, 0, 0.5f });
			_model->SetLocalScale(50);	*/
		}

		{
			SharedPtr<Model> _model = new Model;
			_model->m_mesh = _axes;
			_model->Instantiate();
			_model->SetLocalScale(100);
		}

		ImagePtr _hmap = new Image;
		{
			_hmap->CreatePerlinNoise(513, 513, 5, 0, 5);
			_hmap->Blur(15, 15, 10, false);
			_hmap->Blur(15, 15, 10, false);
		}

		if(0)
		{


			/*_inf->Realloc(256, 256);
			for (uint y = 0; y < _inf->Height(); ++y)
			{
				for (uint x = 0; x < _inf->Width(); ++x)
				{
					_inf->SetPixelRaw(x, y, Vector4(RandomRange(0, 1)));
				}
			} */

			//ImagePtr _nmap = _hmap->CreateTerrainNormalmap(10, 300);
			//_nmap->Blur(10, 10, 10, false);
			SharedPtr<Sprite> _sprite = new Sprite;
			_sprite->SetRenderMode(Renderable2D::WorldSpace);
			_sprite->Instantiate();
			_sprite->SetColor({ 1 });
			_sprite->SetTexture(_hmap->_Export());
			_sprite->SetSize(1);
			_sprite->SetLocalScale(1);
			_sprite->SetLocalRotation(Quaternion().FromAxisAngle(Vector3::UnitX, Radians(90)));
		}

		MeshPtr _quad = CreateQuad(1);

		if (0)
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			float _width = 128;

			/*for (uint i = 0; i < 8; ++i)
			{
				MeshPtr _terrainPatch = new Mesh;
				_terrainPatch->CreateTerrainSector(_width, (1 << i) + 1, 1.f / 7, 10);

				SharedPtr<Model> _model = new Model;
				_model->m_mesh = _terrainPatch;
				_model->Instantiate();
				_model->SetLocalPosition({ _width * i, 0, 0 });
			}*/

			SharedPtr<Terrain> _terrain = new Terrain;
			_terrain->Create(_hmap, 10000, 15, 700);
			_terrain->Instantiate();
			_terrain->SetLodBias(3);
			_terrain->SetLodDistance(5000);

			/*SharedPtr<Terrain> _terrain2 = new Terrain;
			_terrain2->Create(nullptr, 30000, 1, 100);
			_terrain2->Instantiate(); */

			//_model->SetLocalScale(100);
		}
#if 0
		float _area = 5000;
		//MeshPtr _axes = CreateAxes(1);
		for(uint i = 0; i < 10000; ++i)
		{
			/*SharedPtr<Model> _model = new Model;
			_model->m_mesh = _quad;
			_model->Instantiate();
			Model * _model2 = _model->AddChild<Model>();
			_model2->m_mesh = _axes; */

			SharedPtr<Model> _model = new Model;
			_model->m_mesh = gMeshes->meshes[0];
			_model->Instantiate();
			Model * _model2 = _model->AddChild<Model>();
			_model2->m_mesh = gMeshes->meshes[1];
			Model * _model3 = _model->AddChild<Model>();
			_model3->m_mesh = gMeshes->meshes[2];

			_model->SetLocalRotation(Quaternion().FromAxisAngle(Vector3::UnitX, Radians(90)));


			_model->SetWorldPosition(Vector3(RandomRange(-_area, _area), RandomRange(0, 0), RandomRange(-_area, _area)));
			_model->SetWorldScale(20);
		}
#endif

		if(0)
		{
			SharedPtr<Model> _model = new Model;
			_model->m_mesh = _quad;
			_model->Instantiate();
			Model * _model2 = _model->AddChild<Model>();
			_model2->m_mesh = _axes;

			_model->SetWorldPosition(Vector3::Zero);
			_model->SetLocalRotation(Quaternion().FromAxisAngle(Vector3::UnitX, Radians(90)));
			_model->SetLocalScale(5000);
		}



		if (1) // test model
		{
			SharedPtr<Model> _model = new Model;
			_model->m_mesh = gMeshes->meshes[0];
			_model->Instantiate();
			Model * _model2 = _model->AddChild<Model>();
			_model2->m_mesh = gMeshes->meshes[1];
			Model * _model3 = _model->AddChild<Model>();
			_model3->m_mesh = gMeshes->meshes[2];

			_model->SetLocalRotation(Quaternion().FromAxisAngle(Vector3::UnitX, Radians(90)));
			_model->SetWorldScale(20);
			//_model->SetWorldPosition({0, 205, 0});
		}






		/*
		{

			SharedPtr<Model> _model = new Model;
			_model->SetWorldPosition(Vector3(0, 0, 0));
			_model->m_mesh = _quad;

			SharedPtr<Model> _axesModel = new Model;
			_axesModel->m_mesh = _axes;
			_axesModel->SetParent(_model);

			_model->Instantiate();
		}*/

		/*{

			SharedPtr<Model> _axesModel = new Model;
			_axesModel->m_mesh = CreateAxes(1);
			_axesModel->Instantiate();
		}
		*/
		/*{
			SharedPtr<Model> _model = new Model;
			_model->SetWorldPosition(Vector3(0, 0, 0));
			_model->m_mesh = _tmesh;
			_model->Instantiate();
		}*/
	}
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

class Game : public Application
{
public:
	static Game* Get(void) { return static_cast<Game*>(s_instance); }

	Game(void)
	{
		LOG_NODE("Init game");

		gReflection->GetOrCreateTypeInfo<FirstPersonCamera>()->SetFactory([]() { return ObjectPtr(new FirstPersonCamera); });
		gReflection->GetOrCreateTypeInfo<PlayerMovement>()->SetFactory([]() { return ObjectPtr(new PlayerMovement); });
		gReflection->GetOrCreateTypeInfo<TestCamera>()->SetFactory([]() { return ObjectPtr(new TestCamera); });
		m_level.CreateScene();
		gDevice->SetCursorMode(CursorMode::Camera);
		gTimer->SetFpsLimit(0);
	}
	~Game(void)
	{
	}

protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		switch (_type)
		{
		case SystemEvent::BeginFrame:
		{

		} break;

		case SystemEvent::Update:
		{
		} break;


		case SystemEvent::EndFrame:
		{


#if defined(_STATISTICS) && 1
			String _dbgStr;
			_dbgStr += "FPS: " + String().FromFloat(gTimer->Fps(), 1) + ", " + String().FromFloat(gTimer->FrameTime() * 1000, 2) + " ms\n";
			_dbgStr += "Draw calls: " + String().FromInt(gGraphics->stats.frame.drawCalls) + "\n";
			_dbgStr += "Instances: " + String().FromInt(gGraphics->stats.frame.drawInstances) + "\n";
			_dbgStr += "Vertices: " + String::PrintSize(gGraphics->stats.frame.drawVertices, 1000, "", "") + "\n";
			_dbgStr += "Primitives: " + String::PrintSize(gGraphics->stats.frame.drawPoints + gGraphics->stats.frame.drawLines + gGraphics->stats.frame.drawTriangles, 1000, "", "") + "\n";
			_dbgStr += "Buffers IO: " + String::PrintSize(gGraphics->stats.frame.bufferRead, 1024, "", "b") + "/" + String::PrintSize(gGraphics->stats.frame.bufferWrite, 1024, "", "b") + "\n";

			//gDebugText->SetColor(.8f);
			gDebugText->SetText(_dbgStr);
			/*LOG("%.2f (IO: %s/%s, SP: %s), Draw calls: %d (%d instances, %s, %s)",
				gTimer->FrameTime(),
				*String::PrintSize(gGraphics->stats.frame.bufferWrite, 1024, "", "b"),
				*String::PrintSize(gGraphics->stats.frame.bufferRead, 1024, "", "b"),
				*String::PrintSize(gGraphics->stats.frame.spUploaded, 1024, "", "b"),
				gGraphics->stats.frame.drawCalls,
				gGraphics->stats.frame.drawInstances,
				*String::PrintSize(gGraphics->stats.frame.drawVertices, 1000, "", " verts"),
				*String::PrintSize(gGraphics->stats.frame.drawPoints + gGraphics->stats.frame.drawLines + gGraphics->stats.frame.drawTriangles, 1000, "", " primitives"));*/
#endif

			m_quit = !gDevice->IsOpened();

		} break;

		case SystemEvent::Render:
		{
			//gRenderer->Set2DMode(Rect::Zero);

			/*Vertex _sprite;
			_sprite.position.x = 60;
			_sprite.position.y = 60;
			_sprite.position.z = 0;
			_sprite.size[0] = 100;
			_sprite.size[1] = 100;
			Vector4(1, 0, 0, 1).Pack(_sprite.color);

			gRenderer->DrawSprites(&_sprite, 1, nullptr);
			 */
			/*
			Vertex _v[2];
			_v[0].position = Vector3(100, 100, 0);
			Vector4(1, 0, 0, 1).Pack(_v[0].color);

			_v[1].position = Vector3(200, 100, 0);
			Vector4(0, 0, 0, 1).Pack(_v[1].color);

			gRenderer->DrawLines(_v, 2, nullptr);
			 */ 


			//gRenderer->FlushBatch();

		} break;

		}
		return ER_Pass; 
	};
	virtual EventResult _PostEvent(int _type, void* _data = nullptr)
	{ 
		return ER_Pass;
	};

	TestLevel m_level;
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

//gReflection->GetOrCreateTypeInfo<FirstPersonCamera>()->SetFactory([]() { return ObjectPtr(new FirstPersonCamera); });

/*
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
*/


////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

class CameraController : public Behavior
{
public:
	RTTI("CameraController");

	void SetTargetCamera(Entity* _camera)
	{
		m_targetCamera = _camera;
	}

protected:

	void _Start(void) override
	{
		if (m_targetCamera)
			m_cameraRotation = m_targetCamera->GetLocalRotation();
		m_characterRotation = m_entity->GetLocalRotation();
	}

	void _Update(void) override
	{
		if (m_targetCamera) // !m_pause
		{
			float _yaw = gDevice->CameraDelta().x * -m_sensitivity;
			float _pitch = gDevice->CameraDelta().y * -m_sensitivity;

			m_characterRotation *= Quaternion().FromAxisAngle(Vector3::UnitY, Radians(_yaw));
			m_cameraRotation *= Quaternion().FromAxisAngle(Vector3::UnitX, Radians(_pitch));

			m_characterRotation.Normalize();
			m_cameraRotation.Normalize();

			if (m_limitedPitch)
			{
				const float _mp = Radians(.5f);
				m_cameraRotation.x = Tan(Clamp(ATan2(m_cameraRotation.x, m_cameraRotation.w), m_minPitch * _mp, m_maxPitch * _mp));
				m_cameraRotation.y /= m_cameraRotation.w;
				m_cameraRotation.z /= m_cameraRotation.w;
				m_cameraRotation.w = 1;
			}

			if (m_smooth)
			{
				m_entity->SetLocalRotation(m_entity->GetLocalRotation().Slerp(m_characterRotation, Clamp01(m_smoothTime * gTimer->FrameTime())));
				m_targetCamera->SetLocalRotation(m_targetCamera->GetLocalRotation().Slerp(m_cameraRotation, Clamp01(m_smoothTime * gTimer->FrameTime())));
			}
			else
			{
				m_entity->SetLocalRotation(m_characterRotation);
				m_targetCamera->SetLocalRotation(m_cameraRotation);
			}
		}
	}


	Entity* m_targetCamera = nullptr; // todo: use ref
	Quaternion m_cameraRotation = Quaternion::Identity;
	Quaternion m_characterRotation = Quaternion::Identity;

	float m_sensitivity = 0.25f;
	bool m_smooth = true;
	float m_smoothTime = 15;

	bool m_limitedPitch = true;
	float m_minPitch = -90;
	float m_maxPitch = 90;
};

class PlayerMovement : public Behavior
{
public:

protected:
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


constexpr const PackedColor _palette[] = 
{
#if 0
	// http://color.romanuke.com/tsvetovaya-palitra-3175/
	0x010300ff,
	0x052427ff,
	0x14435bff,
	0xe0ce90ff,
	0xffe056ff,
#elif 0
	// http://color.romanuke.com/tsvetovaya-palitra-2827/
	0x010006ff,
	0x010027ff,
	0x000077ff,
	0x6a2ecdff,
	0x9f64d8ff,
#elif 0
	// http://color.romanuke.com/tsvetovaya-palitra-2567/
	0x040203ff,
	0x010a3dff,
	0x467b8dff,
	0x43bd8eff,
	0xe1fea4ff,
#elif 1
	0x000000ff,
	0x000221ff,
	0xFF5110ff,
	0xE8EAECff,
	0xF5F5F2ff,
#else
	// http://color.romanuke.com/tsvetovaya-palitra-3165/
	0xffaa2fff,
	0xe3e4e5ff,
	0xada2a6ff,
	0x584948ff,
	0x0f0603ff,
#endif
};


constexpr const ImageScript _images[] = 
{
	// font
	ImageScript::Font, // op
	//"Pollock3CTT", // name
	"Times new roman", // name
	64, // height
	0.6f, // width
	false, // italic

	/*ImageScript::Blur, // op
	3, // width
	3, // height
	1.5f, // r
	false, // wrap*/

	ImageScript::Export2D, // op
	String::ConstHash("Font"), // name
	PixelFormat::RGBA8,	// format

						
	// terrain
	ImageScript::PerlinNoise, // op
	512, // width
	512, // height
	5.f, // scale
	0, // seed
	4, // num_octaves

	ImageScript::Blur, // op
	5, // width
	5, // height
	1.f, // r
	false, // wrap

	ImageScript::ExportImage, // op
	String::ConstHash("TerrainHeightmap"), // name
	

	ImageScript::Export2D, // op
	String::ConstHash("Background"), // name
	PixelFormat::RGBA8,	// format

	//ImageScript::ExportImage, // op
	//String::ConstHash("TerrainHeightmap"), // name
					 

	ImageScript::End,
};

////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////

struct AppEvent
{
	enum Enum
	{
		ChangleState = String::ConstHash("AppEvent::ChangeState"), // arg = AppState::Enum*
	};

};

struct AppState
{
	enum Enum
	{
		StartScreen,
		Menu,
		Loading,
		Game,
	};


	static void ChangeState(Enum _newState)
	{
		if (_newState != s_currentState)
		{
			System::SendEvent(AppEvent::ChangleState, _newState);
			s_currentState = _newState;
		}
	}

	static Enum CurrentState(void) { return s_currentState; }

protected:
	static Enum s_currentState;
};

AppState::Enum AppState::s_currentState = AppState::StartScreen;

//----------------------------------------------------------------------------//
// Resources
//----------------------------------------------------------------------------//

class Resources : public System, public Singleton<Resources>
{
public:
	Resources(void)
	{
		LOG_NODE("Create Resources");

		gImageManager->ExecuteScript(_images);
	}
	~Resources(void)
	{

	}
protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		return ER_Pass;
	}

	void _PreloadResources(void)
	{
		//
	}
};

//----------------------------------------------------------------------------//
// StartScreen
//----------------------------------------------------------------------------//

class StartScreen : public System, public Singleton<StartScreen>
{
public:
	StartScreen(void)
	{
		LOG("StartScreen");

		m_frame = new Renderable2D;
		m_frame->SetRenderMode(Renderable2D::ScreenSpace);
		m_frame->SetPivot(0);

		m_background = m_frame->AddChild<Sprite>();
		m_background->SetAnchor({ 0, .1f, 1, .9f });
		m_background->SetSize({ 40, 40 });
		m_background->SetPivot({ 0, 0 });
		m_background->SetColor(_palette[1]);
		//m_background->SetTexture(gImageManager->GetTexture(String::ConstHash("Background")), 0);
		//_sprite->SetWorldPosition({ -50, -50, 0 });
		//_sprite->Instantiate();

		m_text = m_background->AddChild<Text>();
		m_text->SetFont(gImageManager->GetTexture(String::ConstHash("Font")), 0);
		m_text->SetText("GAME 96K");
		//m_text->SetFontSize(30);
		m_text->SetAnchor({ 0, 0, 1, 1 });
		m_text->SetPivot({ 0, 0 });
		m_text->SetColor(0);
		m_text->SetAlignment(Text::Center, Text::Center);
		m_text->SetWrapMode(Text::Wrap, Text::Wrap);

		//m_text->SetLocalRotation(Quaternion().FromAxisAngle(Vector3::UnitZ, Radians(5)));
		//m_text->SetLocalPosition({100, 100, 0});

		//_sprite = m_frame->AddChild<Sprite>();
		//_sprite->SetAnchor({ 0, 0, 1, 1 });
		//_sprite->SetPivot({ 0, 0 });
		//_sprite->SetPosition({ 50, 50 });

		m_frame->Instantiate();

		gRenderer->SetClearColor(_palette[0]);

		_NextSlide();

	}
	~StartScreen(void)
	{

	}

	bool IsActive(void) { return AppState::CurrentState() == AppState::StartScreen; }

protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		switch (_type)
		{
		case SystemEvent::Update:
		{
			if (IsActive())
			{
				if (GetAsyncKeyState(VK_ESCAPE) & 0x1) // temp
				{
					_Finalize();
					break;
				}

				//m_background->Color().a = (Sin((m_time / m_timePerSlide) * PI));
				m_text->Color().a = (1 - Cos((m_time / m_timePerSlide) * 2 * PI)) * .5f;
				m_background->Color().a = m_text->Color().a;

				m_time += gTimer->FrameTimeUnscaled();
				if (m_time >= m_timePerSlide)
					_NextSlide();
			}

		} break;
		}

		return ER_Pass;
	}

	void _NextSlide(void)
	{
		m_slide++;
		m_time = 0;

		if (m_slide == 1)
		{
			LOG("StartScreen: Slide 1");
			m_text->SetText("Специально для Gamedev.ru");
			m_text->SetColor(_palette[3]);
			m_text->Color().a = 0;
			m_text->SetFontSize(40);
		}
		else if (m_slide == 2)
		{
			LOG("StartScreen: Slide 2");
			m_text->SetText("На конкурс игр с искусственным интеллектом 2017");
			m_text->SetColor(_palette[3]);
			m_text->Color().a = 0;
			m_text->SetFontSize(40);

		}
		else if (m_slide == 3)
		{
			LOG("StartScreen: Slide 3");
			m_text->SetText("GAME 96K");
			m_text->SetColor(_palette[1]);
			m_text->Color().a = 0;
			m_text->SetFontSize(120);
			m_timePerSlide *= 2;

			m_background->SetColor(_palette[3]);
			m_background->Color().a = 0;
			//gRenderer->SetClearColor(_palette[4]);
		}
		else
		{
			_Finalize();
		}
	}

	void _Finalize(void)
	{
		m_frame->Destroy();
		m_frame = nullptr;
		m_text = nullptr;
		AppState::ChangeState(AppState::Menu);
	}


	float m_timePerSlide = 7;
	float m_time = 0;
	uint m_slide = 0;

	Renderable2D* m_frame = nullptr;
	Sprite* m_background = nullptr;
	Text* m_text = nullptr;
};

//----------------------------------------------------------------------------//
// Loading
//----------------------------------------------------------------------------//

class Loading : public System, public Singleton<Loading>
{
public:

	bool IsActive(void) { return AppState::CurrentState() == AppState::Loading; }

protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		return ER_Pass;
	}

	uint m_level = 0;
};

//----------------------------------------------------------------------------//
// Menu
//----------------------------------------------------------------------------//

class Menu : public System, public Singleton<Menu>
{
public:

	bool IsActive(void) { return AppState::CurrentState() == AppState::Menu; }

protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		if (IsActive())
		{
			switch (_type)
			{
			case SystemEvent::Update:
			{
				// ...

			} break;

			case AppEvent::ChangleState:
			{
				_Deactivate();

			} break;

			}
		}
		else if (_type == AppEvent::ChangleState)
		{
			AppState::Enum _state = *reinterpret_cast<AppState::Enum*>(_data);

			if (_state == AppState::Menu)
				_Activate();
		}

		return ER_Pass;
	}

	void _Activate(void)
	{
		LOG("Activate menu");

		if (!m_text)
		{
			m_text = new Text;
			m_text->SetFont(gImageManager->GetTexture(String::ConstHash("Font")), 0);
			m_text->SetText("Main menu");
			//m_text->SetFontSize(30);
			m_text->SetAnchor({ 0, 0, 1, 1 });
			m_text->SetPivot({ 0, 0 });
			m_text->SetColor(_palette[1]);
			m_text->Instantiate();

			gRenderer->SetClearColor(_palette[3]);
		}

		m_text->Enable(true);
	}

	void _Deactivate(void)
	{
		LOG("Deactivate menu");

		m_text->Enable(false);
	}

	SharedPtr<Text> m_text = nullptr;
};

//----------------------------------------------------------------------------//
// Level
//----------------------------------------------------------------------------//

class Level : public NonCopyable
{
public:

	virtual void ShowLoadingStatus(void) { }
	virtual void Load(void) { }
};

//----------------------------------------------------------------------------//
// Game
//----------------------------------------------------------------------------//

class Game : public System, public Singleton<Game>
{
public:

	enum class State
	{
		Loading,
		WaitAnyKey,
		Pause,
		Play,
	};

	bool IsActive(void) { return AppState::CurrentState() == AppState::Game; }

protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		if (IsActive())
		{
			switch (_type)
			{
			case SystemEvent::Update:
			{
				// ...

			} break;

			case AppEvent::ChangleState:
			{
				//_Deactivate();

			} break;

			}
		}
		else if (_type == AppEvent::ChangleState)
		{
			AppState::Enum _state = *reinterpret_cast<AppState::Enum*>(_data);

			//if (_state == AppState::Game)
				//_Activate();
		}
		return ER_Pass;
	}

	//uint m_level;

};

//----------------------------------------------------------------------------//
// Defs
//----------------------------------------------------------------------------//

#define gApp App::Get()

//----------------------------------------------------------------------------//
// App
//----------------------------------------------------------------------------//

class App : public Application
{
public:
	static App* Get(void) { return static_cast<App*>(s_instance); }

	App(void)
	{
		LOG("Start App");
	}
	~App(void)
	{

	}

	void Exit(void) { m_quit = true; }

protected:

	EventResult _OnEvent(int _type, void* _data = nullptr) override
	{
		switch (_type)
		{
		case SystemEvent::Update:
		{
			if (!gDevice->IsOpened()) // temp
				m_quit = true;

		} break;

		};
		return ER_Pass;
	}

	Resources m_preload;
	StartScreen m_startScreen;
	Menu m_menu;
	Loading m_loading;
	Game m_game;
};



////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////


void main(void)
{
	{
		LOG(APP_NAME " build v0.2.0 [WIP] " __DATE__ " " __TIME__);
		//PRINT_SIZEOF(RenderOp);
		//PRINT_SIZEOF(PageAllocator<RenderOp>);

		//Game _game;
		//_game.Run();

		App _app;
		_app.Run();
	}

#ifdef _DEBUG
	system("timeout 5");
#endif

	ExitProcess(0);
}
