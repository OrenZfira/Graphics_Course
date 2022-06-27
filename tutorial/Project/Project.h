#pragma once
#include "igl/opengl/glfw/Viewer.h"

class Project : public igl::opengl::glfw::Viewer
{
	int currMap;	
public:
	
	Project();
//	Project(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ChangeBackground() override;
	void ScaleAllShapes(float amt, int viewportIndx);
	int selected;
	~Project(void);
};


