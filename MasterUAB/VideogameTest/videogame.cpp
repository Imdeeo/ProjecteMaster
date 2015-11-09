#include <Windows.h>

#include <d3d11.h>
//#include <d3dx11.h>

#pragma comment(lib,"d3d11.lib")


#define APPLICATION_NAME		"VIDEOGAMETEST"

#define M_WIDTH_APPLICATION		800
#define M_HEIGHT_APPLICATION	600

HRESULT CreateConcept(UINT Width, UINT Height, HWND OutputWindowInstance);


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

  RegisterClassEx( &wc );

  // Calcular el tamano de nuestra ventana
	RECT rc = {
			0, 0, M_WIDTH_APPLICATION, M_HEIGHT_APPLICATION
		};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE);


  // Create the application's window
  //HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, wc.hInstance, NULL );
	// Crear la ventana en si
	HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 100, 100, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wc.hInstance, NULL);

	

	CreateConcept(M_HEIGHT_APPLICATION,M_WIDTH_APPLICATION,hWnd);

  // Añadir aquí el Init de la applicacioón

  ShowWindow( hWnd, SW_SHOWDEFAULT );

  UpdateWindow( hWnd );
  MSG msg;
  ZeroMemory( &msg, sizeof(msg) );

  // Añadir en el while la condición de salida del programa de la aplicación

  while( msg.message != WM_QUIT )
  {
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
       // Main loop: Añadir aquí el Update y Render de la aplicación principal
    }
  }
  UnregisterClass( APPLICATION_NAME, wc.hInstance );

  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}


HRESULT CreateConcept(UINT Width, UINT Height, HWND OutputWindowInstance)
{
	// Tendremos que crear y rellenar una estructura de este tipo
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	// o
	//DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Windowed = TRUE;
	// TODO:
	desc.BufferDesc.Width = Width;
	desc.BufferDesc.Height = Height;
	desc.BufferDesc.RefreshRate.Numerator = 1;
	desc.BufferDesc.RefreshRate.Denominator = 60;
	desc.OutputWindow = OutputWindowInstance;
	desc.SampleDesc.Count = 1;
	//desc.SampleDesc.Quality;
	//desc. ????
	
	// Que DirectX queremos
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	ID3D11Device *l_D3DDevice = NULL; // esta clase, el device, nos sirve para crear objetos de DirectX
	ID3D11DeviceContext *l_DeviceContext = NULL; // el contexto nos va a servir para usar objetos de DirectX
	IDXGISwapChain *l_SwapChain = NULL; // la cadena de swap

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &desc, &l_SwapChain, &l_D3DDevice, NULL, &l_DeviceContext)))
	{
		return S_FALSE;
	}
	return S_OK;
}
