#include "Assignment1.h"
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

Assignment1::Assignment1()
{
	
	iter = 1; 
	coeffs = Eigen::Vector4cf::Ones();
	coeffs[0] = 1;
	coeffs[1] = 0;
	coeffs[2] = 0;
	coeffs[3] = -1;
	selected = 0;
	scroll = 1;
	x = 0;
	y = 0;
}

//Assignment1::Assignment1(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

void Assignment1::Init()
{		
	unsigned int texIDs[3] = { 0 , 1, 2};
	unsigned int slots[3] = { 0 , 1, 2 };
	
	AddShader("shaders/pickingShader");
	AddShader("shaders/exampleShader");
	
	AddTexture("textures/box0.bmp",2);
	AddTexture("textures/grass.bmp", 2);

	AddMaterial(texIDs,slots, 2);
	AddMaterial(texIDs+1, slots+1, 1);
	
	AddShape(Plane, -1, TRIANGLES,0);
	
	SetShapeShader(0,1);
	SetShapeMaterial(0, 0);
	pickedShape = 0;
	ShapeTransformation(scaleAll,5,0);
	pickedShape = -1;
	SetShapeStatic(0);
}

void Assignment1::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


		s->Bind();
		s->SetUniformMat4f("Proj", Proj);
		s->SetUniformMat4f("View", View);
		s->SetUniformMat4f("Model", Model);
		s->SetUniform4f("real", coeffs[0].real(), coeffs[1].real(),coeffs[2].real() , coeffs[3].real());
		s->SetUniform4f("img", coeffs[0].imag(), coeffs[1].imag(), coeffs[2].imag(), coeffs[3].imag());
		s->SetUniform1i("iter", iter);
		s->SetUniform1f("scroll", scroll);
		s->SetUniform1f("x",x);
		s->SetUniform1f("y",y);
		Eigen::Vector3cf roots =  FindCubicRoots();
		s->SetUniform4f("real_roots", roots[0].real(), roots[1].real(), roots[2].real(), 0.0f);
		s->SetUniform4f("img_roots", roots[0].imag(), roots[1].imag(), roots[2].imag(), 0.0f);
		// s->SetUniform4f("real_roots", 1, -0.5, -0.5, 0);
		// s->SetUniform4f("img_roots", 0, sqrt(3)/2, -sqrt(3)/2, 0);
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


void Assignment1::WhenRotate()
{
}

void Assignment1::WhenTranslate()
{
}

void Assignment1::Animate() {
    if(isActive)
	{
		
	}
}

void Assignment1::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Eigen::Vector3cf Assignment1::FindCubicRoots() 
{
	Eigen::Vector2cf reduceCoeffs = Eigen::Vector2cf::Zero();
	Eigen::Vector3cf roots;
	std::complex<float> bOver3a = (coeffs[1]/coeffs[0])/3.0f;
	reduceCoeffs[0] = coeffs[2]/coeffs[0] - 3.0f*bOver3a*bOver3a;
	reduceCoeffs[1] = coeffs[2]/coeffs[0]*bOver3a - coeffs[3]/coeffs[0] - 2.0f*bOver3a*bOver3a*bOver3a;
	// std::cout<<"reduced\n"<<reduceCoeffs<<std::endl;
	if(reduceCoeffs.norm()>0.000001)
	{
		roots =	FindRootsOfReduceEquation(reduceCoeffs);
		roots[0] -= bOver3a;
		roots[1] -= bOver3a;
		roots[2] -= bOver3a;
	}
	else
	{
		roots[0] = -1.0f*bOver3a;
		roots[1] = std::complex<float>(std::cosf(3.14159f/3.0f),std::sinf(3.14159f/3.0f))*bOver3a;
		roots[2] = std::complex<float>(std::cosf(2.0f*3.14159f/3.0f),std::sinf(2*3.14159f/3.0f))*bOver3a;
	}

	return roots;
}

std::complex<float> Assignment1::NewtonCubicRoot(std::complex<float> num)
{		
	std::complex<float> root = num ;
	const int iter = 9;
	bool isSmall = false;
	if(std::abs(num)<1e-3)
	{
		if(std::abs(num)==0)
			return num;
		isSmall = true;
		num = num*1e6f;
		root = num;
	}
	else 
		if(std::abs(num)<0.9f) 
		 	root = 1;
	for (int k = 0; k < iter; k++)
	{
		root = (2.0f*root*root*root + num)/root/root/3.0f;  
	}	
	if(isSmall)
		root = root/100.0f;
	return root;
}

Eigen::Vector3cf Assignment1::FindRootsOfReduceEquation(Eigen::Vector2cf reduceCoeffs)
{		
	Eigen::Vector3cf roots = Eigen::Vector3cf::Zero();
	std::complex<float> sqroot = std::sqrt(reduceCoeffs[0]*reduceCoeffs[0]*reduceCoeffs[0]/27.0f + reduceCoeffs[1]*reduceCoeffs[1]/4.0f);
	std::complex<float> p = NewtonCubicRoot(reduceCoeffs[1]/2.0f + sqroot);
	std::complex<float> n = NewtonCubicRoot(reduceCoeffs[1]/2.0f - sqroot);
	roots[0] = p + n;
	roots[1] = p *std::complex<float>(std::cosf(2.0f*3.14159f/3.0f),std::sinf(2*3.14159f/3.0f)) - n * std::complex<float>(std::cosf(1.0f*3.14159f/3.0f),std::sinf(1*3.14159f/3.0f));
	roots[2] = -p * std::complex<float>(std::cosf(1.0f*3.14159f/3.0f),std::sinf(1*3.14159f/3.0f)) + n * std::complex<float>(std::cosf(2.0f*3.14159f/3.0f),std::sinf(2*3.14159f/3.0f));
	return roots;
}	

Assignment1::~Assignment1(void)
{
}

