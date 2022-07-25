#include "Project.h"
#include <iostream>
#include "Bezier.h"


static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

Project::Project()
{
}

//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Project::Init()
{		
	unsigned int texIDs[5] = { 0 , 1, 2, 3, 4};
	unsigned int slots[5] = { 0 , 1, 2, 3, 4 };

	Bezier* bezier = new Bezier();
	
	AddShader("shaders/pickingShader");
	AddShader("shaders/cubemapShader");
	AddShader("shaders/basicShaderTex");
	AddShader("shaders/basicShader1");
	AddShader("shaders/pickingShader");

	
	AddTexture("textures/cubemaps/Daylight Box_", 3);
	AddTexture("textures/cubemaps/a_", 3);
	AddTexture("textures/plane.png",2);
	AddTexture("textures/grass.bmp", 2);
	AddTexture("textures/snake1.png",2);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs+2, slots+2, 1);
	AddMaterial(texIDs + 3, slots + 3, 1);
	AddMaterial(texIDs + 4, slots + 4, 1);

	AddShape(Cube, -2, TRIANGLES);
	AddShape(zCylinder, -1, TRIANGLES);
	AddShape(zCylinder, 1, TRIANGLES);
	AddShape(zCylinder, 2, TRIANGLES);
	AddShape(Axis, -1, LINES, 4);
	// AddShape(zCylinder, -1, TRIANGLES,1);

	AddShape(Plane, -2, TRIANGLES, 2);
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Octahedron, -1, TRIANGLES,1);

	AddShape(Cube, -2, TRIANGLES, 1);




	SetShapeShader(1, 2);
	SetShapeShader(2, 2);
	SetShapeShader(3, 2);
	SetShapeShader(4, 2);
	SetShapeShader(5, 4);
	SetShapeShader(6, 2);
	SetShapeShader(7, 2);
	SetShapeShader(8, 2);
	SetShapeShader(9, 2);




	SetShapeMaterial(1, 3);
	SetShapeMaterial(2, 3);	
	SetShapeMaterial(3, 3);	
	SetShapeMaterial(4, 3);
	SetShapeMaterial(5, 0);
	SetShapeMaterial(6, 4);
	SetShapeMaterial(7, 4);
	SetShapeMaterial(8, 4);
	SetShapeMaterial(9, 4);

	SetShapeMaterial(0, 0);
	SetShapeMaterial(10, 1);


	selected_data_index = 0;
	float cylinderLen = 1.6f;
	float s = 60;
	ShapeTransformation(scaleAll, s,0);
	selected_data_index = 10;
	ShapeTransformation(scaleAll, s,0);
	selected_data_index = 1;
	data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	ShapeTransformation(zTranslate, cylinderLen / 2.0, 1);
	
	selected_data_index = 2;
	ShapeTransformation(zTranslate, cylinderLen , 1);
	data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));
	
	selected_data_index = 3;
	ShapeTransformation(zTranslate, cylinderLen, 1);
	data()->SetCenterOfRotation(Eigen::Vector3d(0, 0, -cylinderLen / 2.0));

	selected_data_index = 5;
	ShapeTransformation(zTranslate, -1.1, 1);
	selected_data_index = 0;
	SetShapeStatic(0);
	SetShapeStatic(5);
SetShapeStatic(10);
	for(int i = 6 ; i < 10; i++){
		selected_data_index = i;
		ShapeTransformation(scaleAll, 0.3f, 0);
		data()->MyTranslate(bezier->segments[0].row(i-6).head(3),0);
		// std::cout << "Row " << i << " is " << bezier->segments[0].row(i-6) << std::endl;
	
	}
	// ShapeTransformation(scaleAll, 0.5f, 0);
	selected_data_index = 0;

	currMap = 0;
	time = 3;
	currCamera = 0;
	// SetShapeViewport(6, 1);
	// ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = (shapeIndx& 0x000000FF) >>  0;
	int g = (shapeIndx& 0x0000FF00) >>  8;
	int b = (shapeIndx& 0x00FF0000) >> 16;
		s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);
	//textures[0]->Bind(0);

	
	

	//s->SetUniform1i("sampler2", materials[shapes[pickedShape]->GetMaterial()]->GetSlot(1));
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Project::WhenRotate()
{
}

void Project::WhenTranslate()
{
}

void Project::Animate() {
    if(isActive)
	{
		if (time <= 0){
			Deactivate();
			time = 3;
			selected_data_index = 0;
			return;
		}
		time-=0.05f;
		selected_data_index = selected;
		if (selected_data_index > 0) {
			std:: cout << "animating" << std::endl;
			// data()->MyRotate(Eigen::Vector3d(0, 1, 0), 0.01);
			ShapeTransformation(xTranslate, 0.1f, 1);
		}
	}
}

void Project::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

void Project::ChangeBackground() {
	currMap ^= 1;
	SetShapeMaterial(0, currMap);
}

Project::~Project(void)
{
}

