#include "framework.h"
#include "Scene230221.h"
#include "Planet230221.h"
#include "Grid230221.h"
Scene230221::Scene230221()
{
	sun = new Planet230221(L"Planets/Sun.jpg", Vector3(10.0f, 10.0f, 10.0f));
	sun->SetRotSpeedY(1.0f);

	mercury = new Planet230221(L"Planets/Mercury.jpg", Vector3(0.6f, 0.6f, 0.6f));
	mercury->SetAnchor(sun);
	mercury->SetFarAchor(Vector3(18.0f, 0.0f, 0.0f));
	mercury->SetRotSpeedY(3.65f);
	mercury->SetRotSpeedA(4.0f);

	venus = new Planet230221(L"Planets/Venus.jpg", Vector3(0.9f, 0.9f, 0.9f));
	venus->SetAnchor(sun);
	venus->SetFarAchor(Vector3(25.0f, 0.0f, 0.0f));
	venus->SetRotSpeedY(3.65f);
	venus->SetRotSpeedA(1.6f);

	earth = new Planet230221(L"Planets/Earth.jpg");
	earth->SetAnchor(sun);
	earth->SetFarAchor(Vector3(30.0f, 0.0f, 0.0f));
	earth->SetRotSpeedY(3.65f);

	moon = new Planet230221(L"Planets/Moon.jpg", Vector3(0.3f, 0.3f, 0.3f));
	moon->SetAnchor(earth);
	moon->SetFarAchor(Vector3(3.0f, 0.0f, 0.0f));
	moon->SetRotSpeedY(0.03f);
	moon->SetRotSpeedA(13.5f);

	Mars = new Planet230221(L"Planets/Mars.jpg");
	Mars->SetAnchor(sun);
	Mars->SetFarAchor(Vector3(45.0f, 0.0f, 0.0f));
	Mars->SetRotSpeedY(3.65f);
	Mars->SetRotSpeedA(0.5f);

	jupiter = new Planet230221(L"Planets/Jupiter.jpg", Vector3(5.0f, 5.0f, 5.0f));
	jupiter->SetAnchor(sun);
	jupiter->SetFarAchor(Vector3(60.0f, 0.0f, 0.0f));
	jupiter->SetRotSpeedY(3.65f);
	jupiter->SetRotSpeedA(0.08f);

	grid = new Grid230221;
}

Scene230221::~Scene230221()
{
	delete sun;
	delete mercury;
	delete venus;
	delete earth;
	delete moon;
	delete Mars;
	delete jupiter;
	delete grid;
}

void Scene230221::Update()
{
	sun->Update();
	mercury->Update();
	venus->Update();
	earth->Update();
	moon->Update();
	Mars->Update();
	jupiter->Update();
}

void Scene230221::PreRender()
{
}

void Scene230221::Render()
{
	sun->Render();
	mercury->Render();
	venus->Render();
	earth->Render();
	moon->Render();
	Mars->Render();
	jupiter->Render();

	grid->Render();
}

void Scene230221::PostRender()
{
}

void Scene230221::GUIRender()
{
}
