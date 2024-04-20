




#include "MainEngine/MainEngine.h"
#include "GraphicEngine/GraphicEngine.h"
#include "MainEngine/UIWindow.h"


/* -- static init -- */

MainEngine* MainEngine::_mainmanagerptr = NULL;
GraphicEngine* GraphicEngine::_graphicengineptr = NULL;
UIWindowControler* UIWindowControler::_uiwindowcontroler = NULL;


/* -- ----------- -- */


bool b;


void DisplayUI()
{



	UIWindowMain* mainui = new UIWindowMain("main");
	UIWindowControler::GetInstance()->Windows.emplace("main", mainui);

	UIWindowControler::GetInstance()->DisplayWindows();
	
	
	//ImGui::ShowDemoWindow(&b);
}




int main()
{





	GraphicEngine::GetInstance()->Init();

	MainEngine::GetInstance()->Init();


	while (!GraphicEngine::GetInstance()->ShouldCloseMainWindow())
	{
		
		//Graphic
		GraphicEngine::GetInstance()->Display(DisplayUI);

	}


	GraphicEngine::GetInstance()->Delete();
	delete(GraphicEngine::GetInstance());


	return 0;

}
