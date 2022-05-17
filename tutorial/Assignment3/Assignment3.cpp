#include "Assignment3.h"
#include <iostream>


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

Assignment3::Assignment3()
{
}

//Assignment3::Assignment3(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Assignment3::Init()
{		
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
	
	AddShader("shaders/pickingShader");
	AddShader("shaders/cubemapShader");
	AddShader("shaders/basicShader");
	AddShader("shaders/pickingShader");
	
	AddTexture("textures/box0.bmp",2);
	AddTexture("textures/cubemaps/Daylight Box_", 3);
	AddTexture("textures/plane.png", 2);
	//AddTexture("../res/textures/Cat_bump.jpg", 2);

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	
	AddShape(Cube, -2, TRIANGLES);
	AddShape(Cube, -1, TRIANGLES);
	SetShapeShader(1, 2);
	SetShapeMaterial(1, 2);

	SetShapeMaterial(0, 1);
	for (int i = 2; i < 28; i++){
		AddShape(Cube, 1, TRIANGLES);
		SetShapeShader(i, 2);
		SetShapeMaterial(i, 2);
	}
	
	
	pickedShape = 0;
	float s = 60;
	ShapeTransformation(scaleAll, s,0);
	pickedShape = 27;
	ShapeTransformation(xTranslate, 1,0);
	pickedShape = 2;
	ShapeTransformation(xTranslate, -1,0);
	pickedShape = 3;
	ShapeTransformation(xTranslate, 1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 4;
	ShapeTransformation(xTranslate, -1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 5;
	ShapeTransformation(xTranslate, 1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 6;
	ShapeTransformation(xTranslate, -1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 7;
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 8;
	ShapeTransformation(zTranslate, -1,0);

	for(int i = 9; i < 18;i++){
		pickedShape = i;
		ShapeTransformation(yTranslate, 1,0);
	}
	pickedShape = 9;
	ShapeTransformation(xTranslate, 1,0);
	pickedShape = 10;
	ShapeTransformation(xTranslate, -1,0);
	pickedShape = 11;
	ShapeTransformation(xTranslate, 1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 12;
	ShapeTransformation(xTranslate, -1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 13;
	ShapeTransformation(xTranslate, 1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 14;
	ShapeTransformation(xTranslate, -1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 15;
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 16;
	ShapeTransformation(zTranslate, -1,0);

	pickedShape = 18;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(xTranslate, 1,0);
	pickedShape = 19;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(xTranslate, -1,0);
	pickedShape = 20;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(xTranslate, 1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 21;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(xTranslate, -1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 22;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(xTranslate, 1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 23;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(xTranslate, -1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 24;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(zTranslate, 1,0);
	pickedShape = 25;
	ShapeTransformation(yTranslate, -1,0);
	ShapeTransformation(zTranslate, -1,0);
	pickedShape = 26;
	ShapeTransformation(yTranslate, -1,0);
	pickedShape = -1;

	SetShapeStatic(0);
//	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Assignment3::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


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
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
//	if(shaderIndx == 0)
//		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
//	else 
//		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}


void Assignment3::WhenRotate()
{
}

void Assignment3::WhenTranslate()
{
}

void Assignment3::Animate() {
    if(isActive)
	{
		
	}
}

void Assignment3::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Assignment3::~Assignment3(void)
{
}

