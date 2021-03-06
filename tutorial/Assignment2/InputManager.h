#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment2.h"
#include "imgui/imgui.h"

	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment2* scn = (Assignment2*)rndr->GetScene();
		if (action == GLFW_PRESS)
		{
			double x2, y2;
			glfwGetCursorPos(window, &x2, &y2);
			rndr->UpdatePress(x2, y2);

			scn->Intersection(Eigen::Vector3f((float)x2 / 400.0f - 1, 1 - (float)y2 / 400.0f, 0));
		}	
		else
		{
			rndr->UnPick(2);
		}
		scn->SetPress();
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			scn->SetRightPress();
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment2* scn = (Assignment2*)rndr->GetScene();
		scn->scnData.eye[2] += ((float)yoffset * 0.2);
		/*if (rndr->IsPicked())
		{
			rndr->UpdateZpos((int)yoffset);
			rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
		}
		else
		{
			rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
		}*/
		
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment2* scn = (Assignment2*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);
		//scn->x = 1-xpos/800.0;
		//scn->y = 1-ypos/800.0;
		scn->SetPosition((int)xpos, (int)ypos);
		if (rndr->CheckViewport(xpos,ypos, 0))
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{

				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				
				rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
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
		Assignment2* scn = (Assignment2*)rndr->GetScene();
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
				scn->theta-=0.1f;
				break;
			case GLFW_KEY_DOWN:
				scn->theta+=0.1f;
				break;
			case GLFW_KEY_LEFT:
				scn->phi-=0.1f;
				break;
			case GLFW_KEY_RIGHT:
				scn->phi+=0.1f;
				break;
			}
		}
	}


void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu *menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
	if(menu)
		menu->init(&display);
}
