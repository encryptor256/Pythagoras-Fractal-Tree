#include "include/glfw/glfw3.h"
#include <stdio.h>
#include <stdlib.h>

/*
	*****************************************************************************
	Structures
*/
typedef struct tagVector2D
{
	double X,Y;

}Vector2D;


/*
	*****************************************************************************
	Functions
*/
static void PythagorasTree(Vector2D * center, Vector2D * up, const unsigned long depth);
static void DrawRectangle(Vector2D * center, Vector2D * up, const unsigned long depth);
static void PythagorasTreeByLen(Vector2D * center, Vector2D * up,const unsigned long depth,const double magn,const unsigned long screenw,const unsigned long screenh);
static void initGL(GLFWwindow* window);
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void size_callback(GLFWwindow* window, int w, int h);

/*
	*****************************************************************************
	Entry: C / C++ Pythagoras Tree (Fractal) in conjunction with GLFW
*/

int main()
{
	GLFWwindow * window;

	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
		exit(EXIT_FAILURE);
	
	window = glfwCreateWindow(640, 480, "C / C++ Pythagoras Tree (Fractal)", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwSetWindowPos(window,50,50);
	
	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	
	glfwSetWindowSizeCallback(window, size_callback);
	
	initGL(window);
	
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		
		glfwGetFramebufferSize(window, &width, &height);
		
		glClear( GL_COLOR_BUFFER_BIT );
		
		Vector2D center = {width*0.5,height*0.8};
		Vector2D up = {0.0,-50.0};
	
		//PythagorasTree(&center,&up,10);
		double magn;
		glfwGetCursorPos(window,0,&magn);
		PythagorasTreeByLen(&center,&up,13,(double)magn/(double)128.0,width,height);

		glfwSwapBuffers(window);
		glfwPollEvents();
	};
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	return 0;
};

static void DrawRectangle(Vector2D * center, Vector2D *  up, const unsigned long depth)
{
	Vector2D normaln = {-up->Y,up->X};
	
	glColor3f(((1.0f / 10.0f) * (double)depth),0.3f+((0.7f / 10.0f) * (double)depth),0.0f);
	
	glBegin(GL_QUADS);
		glVertex2d(center->X-up->X-normaln.X,center->Y-up->Y-normaln.Y);
		glVertex2d(center->X-up->X+normaln.X,center->Y-up->Y+normaln.Y);
		glVertex2d(center->X+up->X+normaln.X,center->Y+up->Y+normaln.Y);
		glVertex2d(center->X+up->X-normaln.X,center->Y+up->Y-normaln.Y);
	glEnd();
};

static void PythagorasTreeByLen(Vector2D * center, Vector2D * up,const unsigned long depth,const double magn,const unsigned long screenw,const unsigned long screenh)
{
	if(depth==0||center==0||up==0) return;
	
	// Avoid if center are out of screen area
	if(center->X<0||center->Y<0||center->X>screenw||center->Y>screenh) return;
	
	Vector2D
		newnormal,
		newcenter,
		normal,
		newup,
		top,
		left,
		cross,
		crossnormal;
	
	// Draw incoming rectangle
	DrawRectangle(center,up,depth);
	
	// --------------------------------
	// Calculate one side
	//
	top =
		(Vector2D){
			center->X + up->X + up->X*magn,
			center->Y + up->Y + up->Y*magn
		};
	
	normal = (Vector2D){-up->Y,up->X};
	
	left = 
		(Vector2D){
			(center->X + up->X + normal.X) ,
			(center->Y + up->Y + normal.Y),
		};
		
	cross = 
		(Vector2D){
			(top.X - left.X) * 0.5,
			(top.Y - left.Y) * 0.5,
		};
		
	crossnormal = (Vector2D){-cross.Y,cross.X};
		
	newcenter = 
		(Vector2D){
			(left.X + cross.X + crossnormal.X) ,
			(left.Y + cross.Y + crossnormal.Y)
		};
		
	PythagorasTreeByLen(&newcenter,&crossnormal,depth-1,magn,screenw,screenh);
		
	// --------------------------------
	// Calculate the other side
	//
	normal = (Vector2D){up->Y,-up->X};
	
	left = 
		(Vector2D){
			(center->X + up->X + normal.X) ,
			(center->Y + up->Y + normal.Y),
		};
		
	cross = 
		(Vector2D){
			(top.X - left.X) * 0.5,
			(top.Y - left.Y) * 0.5,
		};
		
	crossnormal = (Vector2D){cross.Y,-cross.X};
		
	newcenter = 
		(Vector2D){
			(left.X + cross.X + crossnormal.X) ,
			(left.Y + cross.Y + crossnormal.Y)
		};
		
	PythagorasTreeByLen(&newcenter,&crossnormal,depth-1,magn,screenw,screenh);
};

static void PythagorasTree(Vector2D * center, Vector2D *  up, const unsigned long depth)
{	
	// This is static and classical Pythagoras Tree
	
	if(depth==0||center==0||up==0) return;
	//
	// Draw incoming rectangle by center and up vector
	//
	DrawRectangle(center,up,depth);
	//glVertex2d(center->X,center->Y);
	
	//
	// Draw one and the other side of the tree, in a recursive loop
	//

	Vector2D
		newcenter,
		newup,
		normal;

	unsigned char counter = 0;
	
	normal=
			(Vector2D){
				-up->Y,
				up->X
			};
			
	for(counter=0;counter<2;counter++)
	{
		newup=
			(Vector2D){
				(up->X + normal.X) * 0.5,
				(up->Y + normal.Y) * 0.5
			};
			
		newcenter=
			(Vector2D){
				center->X + up->X * 2.0 + normal.X,
				center->Y + up->Y * 2.0 + normal.Y
			};
			
		PythagorasTree(&newcenter,&newup,depth-1);
			
		normal=
			(Vector2D){
				up->Y,
				-up->X
			};
	};
};

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void size_callback(GLFWwindow* window, int w, int h)
{
	initGL(window);
}

static void initGL(GLFWwindow* window)
{
	int width, height;
	
	glfwGetFramebufferSize(window, &width, &height);
	
	glViewport(0,0,width, height);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glOrtho (0.0, width, height, 0.0, 0.0, 1.0);
	
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
};


