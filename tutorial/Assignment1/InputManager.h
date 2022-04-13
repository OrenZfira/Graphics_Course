#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment1.h"
#include "imgui/imgui.h"


	void glfw_mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
			Assignment1* scn = (Assignment1*)rndr->GetScene();
			double x2, y2;
				
			glfwGetCursorPos(window, &x2, &y2);
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
		{
		
			scn->initcx = x2;
			scn->initcy = y2;
			scn->initx = scn->x*400;
			scn->inity = scn->y*400;
			rndr->Pressed();
			
		}
		else if(action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT){
			rndr->Pressed();
		}
	}
	
	void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
		if (yoffset > 0){
			if (scn->scroll > 0.1)
				scn->scroll -= 0.01;
		}
		else
			scn->scroll += 0.01;
		std::cout << "pixel width:" << 1.0/800 * (1/scn->scroll) << std::endl;
		
	}
	
	void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();

		rndr->UpdatePosition((float)xpos,(float)ypos);
		if (rndr->IsPressed())
		{
			scn->x = (scn->initx + xpos - scn->initcx)/400.0;
			scn->y = (scn->inity + ypos - scn->initcy)/400.0;
		}
	}

	void glfw_window_size_callback(GLFWwindow* window, int width, int height)
	{
		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

        rndr->resize(window,width,height);
		
	}
	
	void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	    using namespace std::complex_literals;

		Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
		Assignment1* scn = (Assignment1*)rndr->GetScene();
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
				scn->coeffs[scn->selected]+=0.01;
				
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				scn->coeffs[scn->selected]-=0.01;
				break;
			case GLFW_KEY_LEFT:
				if (scn->iter > 1)
					scn->iter--;
				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				scn->iter++;
				break;
			case GLFW_KEY_1:
				scn->selected = 0;
				break;
			case GLFW_KEY_2:
				scn->selected = 1;
				break;
			case GLFW_KEY_3:
				scn->selected = 2;
				break;
			case GLFW_KEY_4:
				scn->selected = 3;
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
    if (menu != NULL)
		menu->init(&display);
}
