

// 5.1 - Añadir a Graphics 
//           "Material.h", "MaterialManager.cpp", "MaterialManager.h",
//           "RenderableObject.cpp", "RenderableObject.h", "RenderManager.cpp" y "RenderManager.h"
// 5.1.2  - Substituir "DebugRender.h", "DebugRender.cpp", "Frustum.cpp"
// 5.1.3  - Añadir const dónde haga falta para que compile todo
// 5.2 - Modificar CApplication para que use el RenderManager
// 5.2.1  - Añadir el include y la definición de "CRenderManager m_RenderManager" y "CMaterialManager m_MaterialManager"
// 5.2.2  - Substituir "SPositionOrientation m_CubeTransform;" por "CRenderableObject m_Cube"
//           - Tendréis que cambiar la definición del TweakBar para que compile. Ahora se le pasará m_Cube.GetPtrTransform()
// 5.2.3  - Crear CApplication::Init() y llamarlo en el main
// 5.2.4  - Actualizar el update para que actualize las cámaras
// 5.2.5  - Actualizar el CApplication::Render para que llame al rendermanager
// 5.2.6  - Copiar el "materials.xml" dentro de Data

// TODO 5.1.3: corregir const
void CContextManager::Draw(const CRenderableVertexs* _VerticesToRender, ERasterizerState _RS = RS_SOLID, EDepthStencilState _DSS = DSS_DEPTH_ON, EBlendState _BS = BLEND_SOLID);
void CRenderableVertexs::Draw(ID3D11DeviceContext *Device, unsigned int IndexCount = -1, unsigned int StartIndexLocation = 0, unsigned int BaseVertexLocation = 0) const
void CRenderableVertexs::SetBuffers(ID3D11DeviceContext *Device) const

// TODO 5.2.3: añadir el init de CApplication
void CApplication::Init()
{
	m_MaterialManager.AddMaterials("Data\\materials.xml");

	m_Cube.AddSubmesh(m_DebugRender->GetSimpleCube(), "solid material", m_DebugRender->GetSimpleCubeBSRadi(), m_DebugRender->GetSimpleCubeBBMin(), m_DebugRender->GetSimpleCubeBBMax());
}

// TODO 5.2.4: 
{
	CCamera camera;
	m_FPSCamera.SetCamera(&camera);
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);
	camera.SetMatrixs();
	m_RenderManager.SetCurrentCamera(camera);

	m_SphericalCamera.SetCamera(&camera);
	camera.SetFOV(1.047f);
	camera.SetAspectRatio(m_ContextManager->GetAspectRatio());
	camera.SetZNear(0.1f);
	camera.SetZFar(100.f);
	camera.SetMatrixs();
	m_RenderManager.SetDebugCamera(camera);

	m_RenderManager.SetUseDebugCamera(m_CurrentCamera == 0);
}

// TODO 5.2.5: 
void CApplication::Render()
{
	m_ContextManager->BeginRender(m_BackgroundColor);

	// añadir todos los objetos que se quiere pintar
	m_RenderManager.AddRenderableObjectToRenderList(&m_Cube);

	m_RenderManager.Render(m_ContextManager, &m_MaterialManager);


	Mat44f world;

	world.SetIdentity();
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetAxis());

	world.SetIdentity();
	world.SetFromPos(10, 0, 0);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetClassicBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_CLASSIC);

	world.SetIdentity();
	world.SetFromPos(0, 0, -10);
	m_ContextManager->SetWorldMatrix(world);
	m_ContextManager->Draw(m_DebugRender->GetPremultBlendTriangle(), CContextManager::RS_SOLID, CContextManager::DSS_OFF, CContextManager::BLEND_PREMULT);
	

	CDebugHelper::GetDebugHelper()->Render();

	m_ContextManager->EndRender();
}


