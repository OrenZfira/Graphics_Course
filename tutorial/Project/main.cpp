#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "InputManager.h"
//drawinfos: 0, 1, 4, 6, 7, 8, 9 left side
//viewports: 0, 2, 3, 5 left side
//drawinfos: 2, 3, 5 right side
//viewports: 1, 4 right side
int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 45.0f;
	const float NEAR = 1.0f;
	const float FAR = 120.0f;
	const int infoIndx = 2; 
	std::list<int> x, y;
	x.push_back(DISPLAY_WIDTH/2);
	x.push_back(DISPLAY_WIDTH);
	y.push_back(DISPLAY_HEIGHT);
    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OPENGL");
    igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT/2, NEAR, FAR);
	Project *scn = new Project();  //initializing scene
	
    Init(disp,menu); //adding callback functions
	scn->Init();    //adding shaders, textures, shapes to scene
    rndr->Init(scn,x,y,3,menu); // adding scene and viewports to the renderer
    disp.SetRenderer(rndr);
	scn->cameraLocs.push_back({0,0,10});
	rndr->AddViewport(0,0,DISPLAY_WIDTH/2, DISPLAY_HEIGHT);
	rndr->AddViewport(0,0,DISPLAY_WIDTH/2, DISPLAY_HEIGHT);
	rndr->AddViewport(DISPLAY_WIDTH/2, 0,DISPLAY_WIDTH/2, DISPLAY_HEIGHT);
	rndr->AddViewport(0,0,DISPLAY_WIDTH/2, DISPLAY_HEIGHT);
	rndr->CopyDraw(1, rndr->viewport,2);
	rndr->CopyDraw(3, rndr->viewport,4);
	// rndr->AddDraw(5,0,4,0, rndr->depthTest);
	// rndr->CopyDraw(0, rndr->viewport, 1);
	
	rndr->ClearDrawFlag(4, rndr->toClear | rndr->stencilTest);
	rndr->SetDrawFlag(4, rndr->blend | rndr->inAction2 | rndr->scissorTest);
	rndr->SetDrawFlag(1, rndr->blend);
	// rndr->SetDrawFlag(2, rndr->onPicking);

	rndr->AddDraw(3, 0, 4, 0, rndr->stencil2 | rndr->stencilTest | rndr->depthTest | rndr->scaleAbit | rndr->onPicking);
	rndr->AddDraw(5,0,2,0,0);
	rndr->AddDraw(2, 0, 4, 0, rndr->stencilTest | rndr->depthTest);
	rndr->AddDraw(5,0,2,0,0);
	rndr->AddCamera(Eigen::Vector3d(0,0,10), 0, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT/2, NEAR, FAR, 3);
	scn->cameraLocs.push_back({0,0,10});
	rndr->SwitchCamera({2,3,5}, 1);

    disp.launch_rendering(rndr);

	delete scn;
	delete rndr;
	delete menu;
	
	return 0;
}

