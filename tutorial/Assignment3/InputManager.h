#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment3.h"
#include "imgui/imgui.h"


	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if (action == GLFW_PRESS)
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Assignment3* scn = (Assignment3*)rndr->GetScene();
			double x2, y2;
			
			glfwGetCursorPos(window, &x2, &y2);
			scn->x = x2;
			scn->y = y2;
			rndr->UpdatePress(x2, y2);
			if (rndr->Picking((int)x2, (int)y2))
			{
				rndr->UpdatePosition(x2, y2);
				if(button == GLFW_MOUSE_BUTTON_LEFT){
					rndr->Pressed();
				}
			}
			else
			{
				rndr->UnPick(2);
			}
		
		}
		else
		{
			Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			rndr->UnPick(2);
		}
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment3* scn = (Assignment3*)rndr->GetScene();
		
		if (rndr->IsPicked())
		{
			rndr->UpdateZpos((int)yoffset);
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
		}
		else
		{
			rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
		}
		
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment3* scn = (Assignment3*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);

		if (rndr->CheckViewport(xpos,ypos, 0))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{

				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				// scn->pickedShape = 1;
				float ydiff = (ypos-scn->y > 0) ? 0.05f : ((ypos-scn->y < 0) ? -0.05f : 0);
				float xdiff = (xpos-scn->x > 0) ? 0.05f : ((xpos-scn->x < 0) ? -0.05f : 0);
				scn->RotateInSystem(Eigen::Vector3d(1,0,0), ydiff);
				scn->RotateInSystem(Eigen::Vector3d(0,1,0), xdiff);
				scn->y = ypos;
				scn->x = xpos;
				// scn->ShapeTransformation(scn->yRotate, (xpos-scn->x)/10000, 0);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPicked() && rndr->IsMany())
					rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);

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
		Assignment3* scn = (Assignment3*)rndr->GetScene();
		std::vector<int> indexes;
		//rndr->FreeShapes(2);
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
				
			case GLFW_KEY_SPACE:
				if (scn->IsActive())
					scn->Deactivate();
				else
					scn->Activate();
				break;

			case GLFW_KEY_UP:
				// int k = 5;
				for(int i = 0; i< 9; i++){
					if(i == 0)
						scn->pickedShape = 13;
					if(i == 1)
						scn->pickedShape = 5;
					if(i == 2)
						scn->pickedShape = 22;
					if(i == 3)
						scn->pickedShape = 25;
					if(i == 4)
						scn->pickedShape = 23;
					if(i == 5)
						scn->pickedShape = 6;
					if(i == 6)
						scn->pickedShape = 14;
					if(i == 7)
						scn->pickedShape = 16;
					if(i == 8)
						scn->pickedShape = 8;		
					scn->ShapeTransformation(scn->zRotate, EIGEN_PI/4, 0);		
				}		
				break;
			case GLFW_KEY_DOWN:
				// scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, scn->xRotate, -0.05f);
				break;
			case GLFW_KEY_LEFT:
				rndr->MoveCamera(0, scn->yRotate, 0.05f);
				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				rndr->MoveCamera(0, scn->yRotate, -0.05f);
				break;
			case GLFW_KEY_U:
				rndr->MoveCamera(0, scn->yTranslate, 0.25f);
				break;
			case GLFW_KEY_D:
				rndr->MoveCamera(0, scn->yTranslate, -0.25f);
				break;
			case GLFW_KEY_L:
				rndr->MoveCamera(0, scn->xTranslate, -0.25f);
				break;
			
			case GLFW_KEY_R:
				indexes = scn->cube->getIndexes(2);
				for (int i = 0; i<9; i++){
					scn->pickedShape = indexes[i];
					scn->ShapeTransformation(scn->xRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->xRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->xRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->xRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->xRotate, -EIGEN_PI/10, 0);
				}
				scn->cube->rightCW();
				scn->cube->printcube();
				break;
			
			case GLFW_KEY_B:
				rndr->MoveCamera(0, scn->zTranslate, 0.5f);
				break;
			case GLFW_KEY_F:
				indexes = scn->cube->getIndexes(0);
				for (int i = 0; i<9; i++){
					scn->pickedShape = indexes[i];
					scn->ShapeTransformation(scn->zRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->zRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->zRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->zRotate, -EIGEN_PI/10, 0);
					scn->ShapeTransformation(scn->zRotate, -EIGEN_PI/10, 0);
				}
				scn->cube->frontCW();
				scn->cube->printcube();
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
}
