#include "Background.h"
#include <litegfx.h>
#include <glfw3.h>
#include <iostream>

Background::Background(ltex_t * _Texture, float _Scroll, Vec2 _AutoSpeed, Vec2 _InitialPosition) : Texture(_Texture), AutomaticSpeed(_AutoSpeed), Scroll(_Scroll), Position(_InitialPosition), InitialPosition(_InitialPosition), NumberOfHorizontalNeighbours(0), NumberOfVerticalNeighbours(0)
{}

void Background::Update(float Delta, const Vec2& CameraPosition, const Vec2& ScreenSize)
{
	//Hacer el movimiento por Scroll
	Position._x = InitialPosition._x - CameraPosition._x * (1.f - Scroll);
	Position._y = InitialPosition._y;

	//Hacer el movimiento por AutoSpeed
	InitialPosition._x += AutomaticSpeed._x * Delta;
	InitialPosition._y += AutomaticSpeed._y * Delta;

	//Check de los límites
	if (Position._x + Texture->width + Texture->width < 0 + CameraPosition._x)//Límite en izquierda
	{
		InitialPosition._x += Texture->width * NumberOfHorizontalNeighbours;
	}
	else if (Position._x - Texture->width > ScreenSize._x + CameraPosition._x)//Límite en derecha
	{
		InitialPosition._x -= Texture->width * NumberOfHorizontalNeighbours;
	}

	if (Position._y + Texture->height + Texture->height < 0 + CameraPosition._y)//Límite por arriba
	{
		InitialPosition._y += Texture->height * NumberOfVerticalNeighbours;
	}
	else if(Position._y - Texture->height > ScreenSize._y + CameraPosition._y)//Límite por abajo
	{
		InitialPosition._y -= Texture->height * NumberOfVerticalNeighbours;
	}
}

void Background::Draw()
{
	//Dibujar el fondo en su posición y con su tamaño
	ltex_drawrotsized(Texture, Position._x, Position._y, 0.f, 0.f, 0.f, Texture->width, Texture->height, 0.f, 0.f, 1.f, 1.f);
}