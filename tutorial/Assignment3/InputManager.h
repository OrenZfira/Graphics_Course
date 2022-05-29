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
			if (button == GLFW_MOUSE_BUTTON_RIGHT && rndr->Picking((int)x2, (int)y2))
			{
				scn->actions.push(scn->pickedShape);
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
				float ydiff = (ypos-scn->y > 0) ? 0.05f : ((ypos-scn->y < 0) ? -0.05f : 0);
				float xdiff = (xpos-scn->x > 0) ? 0.05f : ((xpos-scn->x < 0) ? -0.05f : 0);
				scn->RotateInSystem(Eigen::Vector3d(1,0,0), ydiff);
				scn->RotateInSystem(Eigen::Vector3d(0,1,0), xdiff);
				scn->y = ypos;
				scn->x = xpos;
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
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
				
			case GLFW_KEY_SPACE:
				scn->actions.push(6);
				break;

			case GLFW_KEY_UP:
				rndr->MoveCamera(0, scn->xRotate, 0.05f);		
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
				scn->actions.push(4);
				break;
			case GLFW_KEY_D:
				scn->actions.push(5);
				break;
			case GLFW_KEY_L:
				scn->actions.push(3);
				break;
			case GLFW_KEY_R:
				scn->actions.push(2);
				break;
			case GLFW_KEY_B:
				scn->actions.push(1);
				break;
			case GLFW_KEY_F:
				scn->actions.push(0);
				break;
			case GLFW_KEY_A:
				scn->actions.push(7);
				break;
			case GLFW_KEY_Z:
				scn->actions.push(8);
				break;
			case GLFW_KEY_M:
				scn->Randomise();
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
