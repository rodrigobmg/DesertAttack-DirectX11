////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = NULL;
	m_Camera = NULL;
	m_Sun = NULL;
	m_Sun2 = NULL;
	m_Cadena = NULL;
	m_LightShader = NULL;
	m_Light = NULL;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model object.
	m_Sun = new Esfera;
	if(!m_Sun)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Sun->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/sun.jpg");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Sun2 = new Esfera;
	if (!m_Sun2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Sun2->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/grass.jpg");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd, L"../Engine/light_vs.hlsl", L"../Engine/light_ps.hlsl");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	
	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	m_TextDirt = new TextureClass();

	if (!m_TextDirt)
	{
		return false;
	}

	result = m_TextDirt->Initialize(m_D3D->GetDevice(), L"../Engine/data/dirt.jpg");

	if (!result)
	{
		return false;
	}

	m_Blend = new TextureClass();

	if (!m_Blend)
	{
		return false;
	}

	result = m_Blend->Initialize(m_D3D->GetDevice(), L"../Engine/data/alpha.jpg");

	if (!result)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	

	// Release the model object.
	if(m_Sun)
	{
		m_Sun->Shutdown();
		delete m_Sun;
		m_Sun = NULL;
	}

	// Release the model object.
	
	
	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, newMatrix;
	bool result;



	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	D3DXMatrixRotationY(&worldMatrix, rotation);
	D3DXMatrixTranslation(&newMatrix, 1.5, 0,0);

	worldMatrix = newMatrix * worldMatrix;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Sun->Render(m_D3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Sun->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,newMatrix,
		m_Sun->GetTexture(),m_TextDirt->GetTexture(),m_Blend->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	
	if(!result)
	{
		return false;
	}

	/////////////////////////////////////////////////////////////////////// 
	//Dibujando un toroide
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.

	

	if (!result)
	{
		return false;
	}
	
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

