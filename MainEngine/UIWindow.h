
#ifndef UIWINDOW_CLASS_H
#define UIWINDOW_CLASS_H


#include "../GraphicEngine/ImGui/imgui.h"
#include "../GraphicEngine/ImGui/imgui_impl_glfw.h"
#include "../GraphicEngine/ImGui/imgui_impl_opengl3.h"
#include <map>
#include <vector>
#include <iostream>
#include "MainEngine.h"

enum UIElementType
{
	Text,
	Button,
	
};


class UIElement
{
public:
	void printst()
	{
		std::cout << "ptr\n";
	}

	UIElement(UIElementType _type)
	{
		type = _type;
		//func = printst;
	}

	void Display()
	{
		switch (type)
		{
		case Text:
			ImGui::BulletText("Camera controls : ZQSD (qwerty), WASD (azerty),\n crtl to go down, space to go up, mouse left click to orient the camera.");
			break;
		case Button:
			if (ImGui::Button("Reset"))
			{
				//func();
			}
			break;
		default:
			break;
		}
	}
	//void (*func)();
	UIElementType type;
};

class UIWindowNode
{
public:
	UIWindowNode()
	{
		UIElement e1(Text);
		UIElement e2(Button);
		Elements.emplace("1", e1);
		Elements.emplace("2", e2);
	}

	void Display() 
	{
		if (Nodes.size() != 0)
		{
			ImGui::SeparatorText("ABOUT THIS DEMO:");

			std::map<std::string, UIWindowNode>::iterator it2 = Nodes.begin();
			while (it2 != Nodes.end())
			{
				it2->second.Display();
				++it2;
			}
		}
		else
		{

			ImGui::SeparatorText("Node terminale");
			std::map<std::string, UIElement>::iterator it2 = Elements.begin();
			while (it2 != Elements.end())
			{
				it2->second.Display();
				++it2;
			}
		}
	}

protected:

	std::map<std::string, UIElement> Elements;
	std::map<std::string, UIWindowNode> Nodes;
};

class UIWindow
{
public:
	virtual void Display() 
	{
		ImGui::Begin(name.c_str());
		Root.Display();
		ImGui::End();
	}

	UIWindow(std::string _name)
	{
		name = _name;
	}
protected:
	UIWindowNode Root = UIWindowNode();
	std::string name = "default";
};





class UIWindowMain : public UIWindow
{
public:

	void Display()
	{

		ImGui::Begin("Debug");
		
		

		if (ImGui::CollapsingHeader("Help"))
		{
			ImGui::SeparatorText("ABOUT THIS DEMO:");
			ImGui::BulletText("Camera controls : ZQSD (qwerty), WASD (azerty),\n crtl to go down, space to go up, mouse left click to orient the camera.");
			ImGui::BulletText("The rotation of the camera will be fixed at the center\n of the screen but will keep rotation we the mouse reach the edges.");
			ImGui::BulletText("The blue shape indicape the forces applied to the particule");

			if (ImGui::CollapsingHeader("Help2"))
			{
				ImGui::SeparatorText("ABOUT THIS DEMO:");
				ImGui::BulletText("Camera controls : ZQSD (qwerty), WASD (azerty),\n crtl to go down, space to go up, mouse left click to orient the camera.");
				ImGui::BulletText("The rotation of the camera will be fixed at the center\n of the screen but will keep rotation we the mouse reach the edges.");
				ImGui::BulletText("The blue shape indicape the forces applied to the particule");
			}

		}

		if (ImGui::Button("Reset"))
		{
			MainEngine::GetInstance()->ToReset = !MainEngine::GetInstance()->ToReset;
		}
		ImGui::InputFloat("Speed", &(MainEngine::GetInstance()->simulationspeed), 0.01f, 1.0f, "%.3f");

		ImGui::Checkbox("Pause", &(MainEngine::GetInstance()->pauseapp));

		ImGui::Checkbox("Step", &MainEngine::GetInstance()->stepapp);

		ImGui::Checkbox("1", &MainEngine::GetInstance()->bool1);
		ImGui::Checkbox("2", &MainEngine::GetInstance()->bool2);
		ImGui::Checkbox("3", &MainEngine::GetInstance()->bool3);
		ImGui::Checkbox("4", &MainEngine::GetInstance()->bool4);
		ImGui::Checkbox("5", &MainEngine::GetInstance()->bool5);



		ImGui::End();

	}


	UIWindowMain(std::string _name) : UIWindow(_name) {};
protected:
};



class UIWindowControler
{
private:

    static UIWindowControler* _uiwindowcontroler;
public:
    static UIWindowControler* GetInstance()
    {
        if (_uiwindowcontroler == nullptr)
        {
            _uiwindowcontroler = new UIWindowControler();
        }

        return _uiwindowcontroler;
    }

    ~UIWindowControler() {};

    UIWindowControler() {};


	std::map<std::string, UIWindow*> Windows;
	

    void DisplayWindows()
    {
		std::map<std::string, UIWindow*>::iterator it = Windows.begin();
		
		while (it != Windows.end())
		{
			it->second->Display();
			++it;
		}

    };
};
#endif;