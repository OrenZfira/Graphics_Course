#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Assignment2.h"
#include "imgui/imgui.h"
#include <math.h>
	int reverse = 1;

	float find_phi(float x, float y){
		float pi = (float)atan(1)*4;
		if(x > 0) return atan(y/x);
		else if(x < 0){
			if(y >= 0)
				return atan(y/x) + pi;
			else
				return atan(y/x) - pi;
		}
		else
			if (y>0)
				return pi/2;
			else if(y<0)
				return -pi/2;
	}

	void rotate(Assignment2* scn, float theta_add, float phi_add,int i){
		float pi = (float)atan(1)*4;
		float x = scn->scnData.objects[i][0] - scn->scnData.eye[0];
		float y = scn->scnData.objects[i][1] - scn->scnData.eye[1];
		float z = scn->scnData.objects[i][2] - scn->scnData.eye[2];
		float r = sqrt(x*x+y*y+z*z);
		float theta = acos(y/r);
		float phi = find_phi(x,z);
		if (abs(x) <= 0.1f && abs(z) <= 0.1f ){
			if (theta > pi/2 && theta_add < 0)
				return;
			if (theta < pi/2 && theta_add > 0)
				return;
		}
		
		theta+=reverse*theta_add;
		phi += phi_add;
		float newx = r*sin(theta)*cos(phi) + scn->scnData.eye[0];
		float newz = r*sin(theta)*sin(phi) + scn->scnData.eye[2];
		float newy = r*cos(theta) + scn->scnData.eye[1];
		scn->scnData.objects[i][0] = newx;
		scn->scnData.objects[i][1] = newy;
		scn->scnData.objects[i][2] = newz;
					
	}

	
	void rotatelights(Assignment2* scn, float theta_add, float phi_add,int i){
		float pi = (float)atan(1)*4;
		float x = scn->scnData.directions[i][0] - scn->scnData.eye[0];
		float y = scn->scnData.directions[i][1] - scn->scnData.eye[1];
		float z = scn->scnData.directions[i][2] - scn->scnData.eye[2];
		float r = sqrt(x*x+y*y+z*z);
		float theta = acos(y/r);
		float phi = find_phi(x,z);
		if (abs(x) <= 0.1f && abs(z) <= 0.1f ){
			if (theta > pi/2 && theta_add < 0)
				return;
			if (theta < pi/2 && theta_add > 0)
				return;
		}
		
		theta+=theta_add;
		phi += phi_add;
		float newx = r*sin(theta)*cos(phi) + scn->scnData.eye[0];
		float newz = r*sin(theta)*sin(phi) + scn->scnData.eye[2];
		float newy = r*cos(theta) + scn->scnData.eye[1];
		scn->scnData.directions[i][0] = newx;
		scn->scnData.directions[i][1] = newy;
		scn->scnData.directions[i][2] = newz;
					
	}

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
				//rndr->MoveCamera(0, scn->xRotate, 0.05f);
				// scn->RotateEye(0.05f, true);
				for(int i=0; i<scn->scnData.objects.size(); i++){
					rotate(scn, 0.01f, 0, i);
				}
				for(int i=0; i< scn->scnData.directions.size(); i++){
					rotatelights(scn, 0.01f, 0, i);
				}
				break;
			case GLFW_KEY_DOWN:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				for(int i=0; i<scn->scnData.objects.size(); i++){
					rotate(scn, -0.01f, 0, i);
				}
				for(int i=0; i<scn->scnData.directions.size(); i++){
					rotatelights(scn, -0.01f, 0, i);
				}
				break;
			case GLFW_KEY_LEFT:
				for(int i=0; i<scn->scnData.objects.size(); i++){
					rotate(scn, 0,0.1f, i);
				}
				for(int i=0; i<scn->scnData.directions.size(); i++){
					rotatelights(scn, 0,0.1f, i);
				}

				break;
			case GLFW_KEY_RIGHT:
				//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//cout<< "down: "<<endl;
				for(int i=0; i<scn->scnData.objects.size(); i++){
					rotate(scn, 0, -0.1f, i);
				}
				for(int i=0; i<scn->scnData.directions.size(); i++){
					rotatelights(scn, 0, -0.1f, i);
				}
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
				rndr->MoveCamera(0, scn->xTranslate, 0.25f);
				break;
			
			case GLFW_KEY_B:
				rndr->MoveCamera(0, scn->zTranslate, 0.5f);
				break;
			case GLFW_KEY_F:
				rndr->MoveCamera(0, scn->zTranslate, -0.5f);
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
	if(menu)
		menu->init(&display);
}
