#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	for (int i = 0; i < 46; i++)
	{
		m_pMeshes[i] = new MyMesh();
		m_pMeshes[i]->GenerateCube(1.0f, C_WHITE);
	}
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();


	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f, 2.0f, 2.0f));
	static float value = 0.0f;
	matrix4 m4Translate[46];
	m4Translate[0] = glm::translate(IDENTITY_M4, vector3(value - 5.0f, 0.0f, 3.0f));
	m4Translate[1] = glm::translate(IDENTITY_M4, vector3(value - 5.0f, -1.0f, 3.0f));
	m4Translate[2] = glm::translate(IDENTITY_M4, vector3(value - 5.0f, -2.0f, 3.0f));
	m4Translate[3] = glm::translate(IDENTITY_M4, vector3(value - 4.0f, 1.0f, 3.0f));
	m4Translate[4] = glm::translate(IDENTITY_M4, vector3(value - 4.0f, 0.0f, 3.0f));
	m4Translate[5] = glm::translate(IDENTITY_M4, vector3(value - 3.0f, 4.0f, 3.0f));
	m4Translate[6] = glm::translate(IDENTITY_M4, vector3(value - 3.0f, 2.0f, 3.0f));
	m4Translate[7] = glm::translate(IDENTITY_M4, vector3(value - 3.0f, 1.0f, 3.0f));
	m4Translate[8] = glm::translate(IDENTITY_M4, vector3(value - 3.0f, 0.0f, 3.0f));
	m4Translate[9] = glm::translate(IDENTITY_M4, vector3(value - 3.0f, -1.0f, 3.0f));
	m4Translate[10] = glm::translate(IDENTITY_M4, vector3(value - 3.0f, -2.0f, 3.0f));
	m4Translate[11] = glm::translate(IDENTITY_M4, vector3(value - 2.0f, 3.0f, 3.0f));
	m4Translate[12] = glm::translate(IDENTITY_M4, vector3(value - 2.0f, 2.0f, 3.0f));
	m4Translate[13] = glm::translate(IDENTITY_M4, vector3(value - 2.0f, 0.0f, 3.0f));
	m4Translate[14] = glm::translate(IDENTITY_M4, vector3(value - 2.0f, -1.0f, 3.0f));
	m4Translate[15] = glm::translate(IDENTITY_M4, vector3(value - 2.0f, -3.0f, 3.0f));
	m4Translate[16] = glm::translate(IDENTITY_M4, vector3(value - 1.0f, 2.0f, 3.0f));
	m4Translate[17] = glm::translate(IDENTITY_M4, vector3(value - 1.0f, 1.0f, 3.0f));
	m4Translate[18] = glm::translate(IDENTITY_M4, vector3(value - 1.0f, 0.0f, 3.0f));
	m4Translate[19] = glm::translate(IDENTITY_M4, vector3(value - 1.0f, -1.0f, 3.0f));
	m4Translate[20] = glm::translate(IDENTITY_M4, vector3(value - 1.0f, -3.0f, 3.0f));
	m4Translate[21] = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	m4Translate[22] = glm::translate(IDENTITY_M4, vector3(value, 1.0f, 3.0f));
	m4Translate[23] = glm::translate(IDENTITY_M4, vector3(value, 0.0f, 3.0f));
	m4Translate[24] = glm::translate(IDENTITY_M4, vector3(value, -1.0f, 3.0f));
	m4Translate[25] = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 2.0f, 3.0f));
	m4Translate[26] = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 1.0f, 3.0f));
	m4Translate[27] = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 0.0f, 3.0f));
	m4Translate[28] = glm::translate(IDENTITY_M4, vector3(value + 1.0f, -1.0f, 3.0f));
	m4Translate[29] = glm::translate(IDENTITY_M4, vector3(value + 1.0f, -3.0f, 3.0f));
	m4Translate[30] = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 3.0f, 3.0f));
	m4Translate[31] = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 2.0f, 3.0f));
	m4Translate[32] = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 0.0f, 3.0f));
	m4Translate[33] = glm::translate(IDENTITY_M4, vector3(value + 2.0f, -1.0f, 3.0f));
	m4Translate[34] = glm::translate(IDENTITY_M4, vector3(value + 2.0f, -3.0f, 3.0f));
	m4Translate[35] = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 4.0f, 3.0f));
	m4Translate[36] = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 2.0f, 3.0f));
	m4Translate[37] = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 1.0f, 3.0f));
	m4Translate[38] = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 0.0f, 3.0f));
	m4Translate[39] = glm::translate(IDENTITY_M4, vector3(value + 3.0f, -1.0f, 3.0f));
	m4Translate[40] = glm::translate(IDENTITY_M4, vector3(value + 3.0f, -2.0f, 3.0f));
	m4Translate[41] = glm::translate(IDENTITY_M4, vector3(value + 4.0f, 1.0f, 3.0f));
	m4Translate[42] = glm::translate(IDENTITY_M4, vector3(value + 4.0f, 0.0f, 3.0f));
	m4Translate[43] = glm::translate(IDENTITY_M4, vector3(value + 5.0f, 0.0f, 3.0f));
	m4Translate[44] = glm::translate(IDENTITY_M4, vector3(value + 5.0f, -1.0f, 3.0f));
	m4Translate[45] = glm::translate(IDENTITY_M4, vector3(value + 5.0f, -2.0f, 3.0f));

	value += 0.001f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model[46];
	for (int i = 0; i < 46; i++)
	{
		m4Model[i] = m4Scale * m4Translate[i];
	}
	
	for (int i = 0; i < 46; i++)
	{
		m_pMeshes[i]->Render(m4Projection, m4View, m4Model[i]);
	}
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	for (int i = 0; i < 46; i++) 
	{
		SafeDelete(m_pMeshes[i]);
	}

	//release GUI
	ShutdownGUI();
}