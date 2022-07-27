#include "Project.h"
#include <iostream>
#include "Bezier.h"
#include "../../igl/PI.h"


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
	x=0;
	y=0;
	currMap = 0;
	time = 3;
	currCamera = 0;
	t = 0;
	fog = false;
}

//Project::Project(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}
void Project::Init()
{		
	unsigned int texIDs[6] = { 0 , 1, 2, 3, 4, 5};
	unsigned int slots[6] = { 0 , 1, 2, 3, 4, 5};

	bezier = new Bezier();
	
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
	AddTexture("textures/black-square.png",2);
	

	AddMaterial(texIDs,slots, 1);
	AddMaterial(texIDs+1, slots+1, 1);
	AddMaterial(texIDs+2, slots+2, 1);
	AddMaterial(texIDs + 3, slots + 3, 1);
	AddMaterial(texIDs + 4, slots + 4, 1);
	AddMaterial(texIDs + 5, slots + 5, 1);

	AddShape(Cube, -2, TRIANGLES);
	AddShape(Axis, -1, LINES, 5);
	AddShape(Plane, -2, TRIANGLES, 2);

	//right side
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Octahedron, -1, TRIANGLES,1);
	AddShape(Cube, -1, TRIANGLES, 4);
	AddShape(Axis, -1, LINES, 4);
	AddShape(Plane, -1, TRIANGLES, 4);

	SetShapeShader(1, 2);
	SetShapeShader(2, 4);
	SetShapeShader(3, 2);
	SetShapeShader(4, 2);
	SetShapeShader(5, 2);
	SetShapeShader(6, 2);
	SetShapeShader(9, 3);

	SetShapeMaterial(0, 0);
	SetShapeMaterial(2, 3);	
	SetShapeMaterial(3, 4);	
	SetShapeMaterial(4, 4);
	SetShapeMaterial(5, 4);
	SetShapeMaterial(6, 4);
	SetShapeMaterial(9, 5);

	AddShape(Cube, -1, TRIANGLES);
	SetShapeShader(10, 2);
	SetShapeMaterial(10,3);
	selected_data_index = 10;
	ShapeTransformation(zTranslate, -10, 1);
	// ShapeTransformation(xTranslate, 4, 0);



	selected_data_index = 0;
	float s = 60;
	ShapeTransformation(scaleAll, s,0);



	selected_data_index = 2;
	ShapeTransformation(zTranslate, -1.1, 1);

	selected_data_index = 6;
	SetShapeStatic(0);
	SetShapeStatic(2);
	SetShapeStatic(3);
	SetShapeStatic(4);
	SetShapeStatic(5);
	SetShapeStatic(6);
	SetShapeStatic(7);
	SetShapeStatic(8);
	SetShapeStatic(9);


	for(int i = 3 ; i < 7; i++){
		selected_data_index = i;
		data()->MyScale({0.03f, 0.03f, 0.03f});
		data()->MyTranslate(bezier->cps[i-3],0);
	
	}

	std::vector<Eigen::RowVector3d> points = bezier->GetPointsInSegment(0);
	selected_data_index = 7;
	data()->show_faces = 0;
	data()->show_lines = 0;
	data()->show_overlay = 0xFF;
	for (int i =0; i<points.size()-1; i++){
		data()->add_edges(points[i],points[i+1], Eigen::RowVector3d(0,1,1));
	}

	selected_data_index = 0;

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
	s->SetUniform1i("fog", (int)fog);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{
//		materials[shapes[pickedShape]->GetMaterial()]->Bind(textures);
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	}
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 60 / 100.0f, 99 / 100.0f, 0.5f);
		
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
		float dt = (0.02/time)*bezier->segNum;
		if (t > 1){
			Deactivate();
			t=0;
			selected_data_index = 0;
			return;
		}
		Eigen::Vector3d vel = bezier->GetVelocity(0,t, dt);
		for(int i = 0; i < data_list.size(); i++){
			if (data_list[i]->viewports & 1 && !(data_list[i]->IsStatic()) && parents[i] == -1){
				selected_data_index = i;
				data()->MyTranslate(vel, 0);
			}
		}
		t+=dt;
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

