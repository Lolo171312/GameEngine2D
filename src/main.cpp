
#define LITE_GFX_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <litegfx.h>
#include <glfw3.h>
#include <iostream>
#include "../project/Vec2.h"
#include <sstream>
#include <cmath>
#include <stb_image.h>
#include "../project/Sprite.h"
#include "vector"
#include "../project/Collider.h"
#include "../project/World.h"

using namespace std;

struct PlayerStruct 
{
public:
	PlayerStruct(Sprite* sprite, float speed, float jumpForce) : m_sprite(sprite), m_speed(speed), m_jumpForce(jumpForce)
	{}

	~PlayerStruct()
	{
		delete m_sprite;
		m_sprite = nullptr;
	}

	Sprite* m_sprite;
	float m_speed = 125.f;
	float m_jumpForce = 250.f;

	bool m_isOnGround = true;
	bool m_isMoving = false;
	Vec2 Velocity = Vec2::ZeroVec();
};

void Escape_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

template <typename T>
std::string stringFromNumber(T val) {
	std::ostringstream stream;
	stream << std::fixed << val;
	return stream.str();
}

void PlayerInput(GLFWwindow* Window, World& ThisWorld, PlayerStruct& Player, float DeltaTime)
{
	if (Window == nullptr) return;

	if (glfwGetKey(Window, GLFW_KEY_D))
	{
		//Movimiento Derecha
		Player.Velocity._x = Player.m_speed * DeltaTime;
		Player.m_sprite->SetTexture("data/runRight.png", 6, 1);
	}
	else if (glfwGetKey(Window, GLFW_KEY_A))
	{
		//Movimiento Izquierda
		Player.Velocity._x = Player.m_speed * DeltaTime * -1;
		Player.m_sprite->SetTexture("data/runLeft.png", 6, 1);
	}
	else
	{
		//Quieto
		Player.Velocity._x = 0.f;
	}

	
	if (glfwGetKey(Window, GLFW_KEY_SPACE) && Player.m_isOnGround)
	{
		//Saltar
		Player.m_isOnGround = false;
		Player.Velocity._y = -Player.m_jumpForce;
	}

	if(!Player.m_isOnGround)
	{
		//Incluir Gravedad
		Player.Velocity._y += 9.8f * DeltaTime;
	}

	ECollisionFlag Flag = ThisWorld.MoveSprite(*Player.m_sprite, Player.Velocity);

	if(Flag == ECollisionFlag::Down)
	{
		if(!Player.m_isOnGround)
		{
			Player.m_isOnGround = true;
			Player.Velocity._y = 0.f;
		}
	}
	else if(Flag == ECollisionFlag::None)
	{
		if (Player.m_isOnGround)
		{
			Player.m_isOnGround = false;
		}
	}

	if (Player.Velocity == Vec2::ZeroVec())
	{
		if(Player.m_isMoving)
		{
			printf("Start Idle\n");
			Player.m_sprite->SetTexture("data/idle.png", 1, 1);
			Player.m_isMoving = false;
		}
	}
	else
	{
		if(!Player.m_isMoving)
		{
			printf("Start Move\n");
			Player.m_sprite->SetTexture("data/runRight.png", 6, 1);
			Player.m_isMoving = true;
		}
	}
}

const int WIDTH = 640;
const int HEIGHT = 480;

int main() 
{
	//Init librería
	if(glfwInit() == GLFW_FALSE)
	{
		glfwTerminate();
	}
	
	//Obtener tiempo
	double currentTime = glfwGetTime();

	//Init ventana
	GLFWwindow* p_myWindow = glfwCreateWindow(WIDTH, HEIGHT, "Practica7", nullptr, nullptr);

	//Obtener el contexto actual
	glfwMakeContextCurrent(p_myWindow);

	//Init librería LiteGFX
	lgfx_setup2d(WIDTH, HEIGHT);

	//Crear callback de una key (En este caso si se pulsa ESC se cierra el juego)
	glfwSetKeyCallback(p_myWindow, Escape_callback);

	//Variables
	int cloudsWidth;
	int cloudsHeight;
	ltex_t* m_pTexture = nullptr;

	uint8_t* m_pPixels = stbi_load("data/clouds.png", &cloudsWidth, &cloudsHeight, nullptr, 4);
	if (m_pPixels)
	{
		m_pTexture = ltex_alloc(cloudsWidth, cloudsHeight, 0);
		ltex_setpixels(m_pTexture, m_pPixels);
		stbi_image_free(m_pPixels);
	}
	else
	{
		printf("Error. fileName no existe");
	}

	World ThisWorld(WIDTH, HEIGHT, BackgroundData(m_pTexture, 1.0f), BackgroundData(nullptr, 0.8f), BackgroundData(nullptr, 0.6f), BackgroundData(nullptr, 0.4), 0.68f, 0.6f, 1.f);
	ThisWorld.LoadMap("data/map.tmx", 0);

	Sprite* PlayerSprite = new Sprite("data/idle.png", 1, 1, 8);
	PlayerStruct Player(PlayerSprite, 125.f, 6.f);
	Player.m_sprite->SetCollisionType(COLLISION_RECT);
	Player.m_sprite->SetPosition(128.f, ThisWorld.GetMapSize()._y - 70.f);

	ThisWorld.AddSprite(Player.m_sprite);

	//Empezar game loop
	while(glfwWindowShouldClose(p_myWindow) == false)
	{
		//Obtener deltaTime (Tiempo entre loops)
		float delta = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();

		//ACTUALIZAR LÓGICA DE JUEGO
		//Obtener input del jugador
		PlayerInput(p_myWindow, ThisWorld, Player, delta);
		//Modificar posición de la cámara
		ThisWorld.SetCameraPosition(Vec2(Player.m_sprite->GetPosition()._x - WIDTH / 2, Player.m_sprite->GetPosition()._y - HEIGHT / 2));

		//Actualizar el Mundo
		ThisWorld.Update(delta);

		//Swap buffers
		glfwSwapBuffers(p_myWindow);
		
		//Poll Events (todos los key_callback se ejecutan)
		glfwPollEvents();
	}

	//Finalizar librería (liberar almacenamiento)
	glfwTerminate();

    return 0;
}