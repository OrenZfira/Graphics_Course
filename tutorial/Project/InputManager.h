#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "imgui/imgui.h"

static void printMat(const Eigen::Matrix4d& mat)
{

}

	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		double x2, y2;
		glfwGetCursorPos(window, &x2, &y2);
		scn->x = x2;
		scn->y = y2;
		if (action == GLFW_PRESS)
		{
			scn->selected = scn->selected_data_index;
			scn->selectedShapes = scn->pShapes;
			rndr->UpdatePress(x2, y2);
			if(rndr->IsMany()){
				scn->selected = -1; //to know that we picked many shapes for the menu
				rndr->ClearPickedShapes(3);
			}
			else if (rndr->IsPicked())
				rndr->UnPick(3);
			int vpid = rndr->CheckViewport((int)x2, (int) y2, 0) ? 0 : 1; 
			if (button == GLFW_MOUSE_BUTTON_LEFT && rndr->Picking((int)x2, (int)y2, vpid))
			{
				rndr->UpdatePosition(x2, y2);
			}	
			if(button == GLFW_MOUSE_BUTTON_RIGHT && rndr->CheckViewport((int)x2, (int) y2, 0))
				rndr->Pressed();
		}
		else
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			if(button == GLFW_MOUSE_BUTTON_RIGHT){
				rndr->PickMany(3);
				rndr->Pressed();
			}
			// rndr->UnPick(2);
		}
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		
		if (rndr->IsPicked())
		{
			rndr->UpdateZpos((int)yoffset);
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
			bool toMoveCam = false;
			for(int i = 0; i<scn->cameraShapes.size(); i++){
				if(scn->cameraShapes[i] == scn->selected_data_index){
					Eigen::Matrix4d tmp = scn->data()->MakeTransd();
					rndr->MoveCamera(i+2, rndr->resetPos,0);//to zero
					rndr->MoveCamera(i+2, scn->xTranslate, tmp(0,3));
					rndr->MoveCamera(i+2, scn->yTranslate, tmp(1,3));
					rndr->MoveCamera(i+2, scn->zTranslate, tmp(2,3));
					break;
				}
			}
		}
		else
		{
			rndr->MoveCamera(scn->currCamera, rndr->zTranslate, (float)yoffset);
			if (scn->currCamera > 1){
				scn->data_list[scn->cameraShapes[scn->currCamera - 2]]->MyTranslate({0,0,yoffset}, true);
			}
			scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({0, 0, yoffset});

		}
		
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);
		if (rndr->CheckViewport(xpos,ypos, 0))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT, scn->currCamera);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				if(scn->cameraShapesMap.find(scn->selected_data_index) == scn->cameraShapesMap.end())
					rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT, scn->currCamera);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany()){
				float ydiff = (- ypos + scn->y)/400.0;
				float xdiff = (xpos-scn->x)/400.0; 
				for(int i : scn->pShapes){
					scn->selected_data_index = i;
					scn->data()->MyTranslate(Eigen::Vector3d(xdiff,ydiff,0), 1);
					auto it = scn->cameraShapesMap.find(i);
					if(it != scn->cameraShapesMap.end()){
						rndr->MoveCamera(it->second, scn->xTranslate, xdiff);
						rndr->MoveCamera(it->second, scn->yTranslate, ydiff);
					}
				}
				scn->y = ypos;
				scn->x = xpos;				
			}	
		}
		else
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				float ydiff = (- ypos + scn->y)/400.0;
				float xdiff = (xpos-scn->x)/400.0;
				Eigen::Vector3d temp = Eigen::Vector3d(xdiff,ydiff,0);
				if (scn->selected_data_index >= 3 && scn->selected_data_index <= 6){
					scn->data()->MyTranslate(temp, false);
					scn->bezier->ChangeCpt(scn->selected_data_index-3, temp);
					int index = scn->selected_data_index;
					scn->selected_data_index = 7;
					scn->data()->clear_edges();
					std::vector<Eigen::RowVector3d> points = scn->bezier->GetPointsInSegment(0);
					for (int i =0; i<points.size()-1; i++){
						scn->data()->add_edges(points[i],points[i+1], Eigen::RowVector3d(0,1,1));
					}
					scn->selected_data_index = index;
				}
				scn->y = ypos;
				scn->x = xpos;				

				
			}
		}
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

        rndr->resize(window,width,height);
		
	}
	
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Project* scn = (Project*)rndr->GetScene();
		std::vector<int> tmp = {0,1, 4,6,7,8};
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
				
			case GLFW_KEY_SPACE:
				if(scn->currCamera == 0)
					scn->currCamera = 2;
				else
					scn->currCamera =0;
				rndr->SwitchCamera(tmp, scn->currCamera);
				break;
			case GLFW_KEY_UP:
				rndr->MoveCamera(scn->currCamera, scn->xRotate, 0.05f);
				if(scn->currCamera>1)
					scn->selected_data_index =  scn->cameraShapes[scn->currCamera-2];
					scn->ShapeTransformation(scn->xRotate, 0.05f, 1);
				break;
			case GLFW_KEY_DOWN:
				rndr->MoveCamera(scn->currCamera, scn->xRotate, -0.05f);
				if(scn->currCamera>1)
					scn->selected_data_index =  scn->cameraShapes[scn->currCamera-2];
					scn->ShapeTransformation(scn->xRotate, -0.05f, 1);
				break;
			case GLFW_KEY_LEFT:
				rndr->MoveCamera(scn->currCamera, scn->yRotate, 0.05f);
				if(scn->currCamera>1)
					scn->selected_data_index =  scn->cameraShapes[scn->currCamera-2];
					scn->ShapeTransformation(scn->yRotate, 0.05f, 1);
				break;
			case GLFW_KEY_RIGHT:
				rndr->MoveCamera(scn->currCamera, scn->yRotate, -0.05f);
				if(scn->currCamera>1)
					scn->selected_data_index =  scn->cameraShapes[scn->currCamera-2];
					scn->ShapeTransformation(scn->yRotate, -0.05f, 1);
				break;
			case GLFW_KEY_U:
				rndr->MoveCamera(scn->currCamera, scn->yTranslate, 0.25f);
				if(scn->currCamera>1)
					scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({0, 0.25f,0}, 1);
				scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({0, 0.25, 0});
				break;
			case GLFW_KEY_D:
				rndr->MoveCamera(scn->currCamera, scn->yTranslate, -0.25f);
				if(scn->currCamera>1)
					scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({0, -0.25f,0}, 1);
				scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({0, -0.25, 0});
				break;
			case GLFW_KEY_L:
				rndr->MoveCamera(scn->currCamera, scn->xTranslate, -0.25f);
				if(scn->currCamera>1)
					scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({-0.25f, 0,0}, 1);
				scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({-0.25, 0, 0});
				break;
			case GLFW_KEY_R:
				rndr->MoveCamera(scn->currCamera, scn->xTranslate, 0.25f);
				if(scn->currCamera>1)
					scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({0.25f, 0,0}, 1);
				scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({0.25f, 0, 0});
				break;
			case GLFW_KEY_B:
				rndr->MoveCamera(scn->currCamera, scn->zTranslate, 0.5f);
				if(scn->currCamera>1)
					scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({0, 0,0.5f}, 1);
				scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({0, 0, 0.5f});
				break;
			case GLFW_KEY_F:
				rndr->MoveCamera(scn->currCamera, scn->zTranslate, -0.5f);
				if(scn->currCamera>1)
					scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({0, 0, -0.5f}, 1);
				scn->cameraLocs[scn->currCamera]+=Eigen::Vector3d({0, 0, -0.5f});
				break;
			case GLFW_KEY_Z:
				if(rndr->IsPicked() && !(rndr->IsMany()) && scn->selected_data_index > 9){
					Eigen::Matrix4d tmp = scn->data()->MakeTransd();
					rndr->MoveCamera(scn->currCamera, rndr->resetPos,0);//to zero
					rndr->MoveCamera(scn->currCamera, scn->xTranslate, tmp(0,3));
					rndr->MoveCamera(scn->currCamera, scn->yTranslate, tmp(1,3));
					rndr->MoveCamera(scn->currCamera, scn->zTranslate, tmp(2,3));
					rndr->MoveCamera(scn->currCamera, scn->zTranslate, 5.0f);
					if(scn->currCamera>1)
						scn->data_list[scn->cameraShapes[scn->currCamera-2]]->MyTranslate({tmp(3,0), tmp(1,3), tmp(2,3)+5}, 1);
					scn->cameraLocs[scn->currCamera]=Eigen::Vector3d({tmp(3,0), tmp(1,3), tmp(2,3)+5});
				}
				break;

			default:
				break;

			}
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    menu->init(&display);
}
