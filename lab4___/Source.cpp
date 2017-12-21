//Clock with arrows going by system time

//#include <gl\GL.h>
#define ModelNum 5
#define PI 3.141592653589
#include <glad.h>
#include <stb_image.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opengl/filesystem.h>
#include <opengl/shader_m.h>
#include <opengl/camera.h>

#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

string MODELS_PATH = "C://Users//morga//source//repos//lab4___//Models";
string TEXTURES_PATH = "C://Users//morga//source//repos//lab4___//Textures";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
float hourAngle(tm timeinfo);
float minuteAngle(tm timeinfo);
float secondAngle(tm timeinfo);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


struct Model
{
	const double scale = 0.1;
	string name;
	double *v;
//	double *uv;
//	double *n;
	int sz;
	GLuint VBO;
	GLuint VAO;
	int Tex;
	glm::vec3 pos;
	Model() : v(nullptr), pos(0, 0, 0) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
	};
	~Model() {
		if (v)
			delete[] v;
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	};
	void readObj(string filename, int drawtype);
	void init(int drawtype);
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ShaderProgram("7.4.camera.vs", "7.4.camera.fs");
	double scale = 1;


	vector<Model> Models(ModelNum);

	Models[0].name = "hour.obj";
	Models[1].name = "minute.obj";
	Models[2].name = "second.obj";
	Models[3].name = "clock_main.obj";
	Models[4].name = "clock_cyfer.obj";



	Models[0].readObj(Models[0].name, GL_DYNAMIC_DRAW);
	Models[1].readObj(Models[1].name, GL_DYNAMIC_DRAW);
	Models[2].readObj(Models[2].name, GL_DYNAMIC_DRAW);
	Models[3].readObj(Models[3].name, GL_DYNAMIC_DRAW);
	Models[4].readObj(Models[4].name, GL_DYNAMIC_DRAW);


	glBindVertexArray(Models[0].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Models[0].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*Models[0].sz, &Models[0].v[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Models[1].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Models[1].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*Models[1].sz, &Models[1].v[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Models[2].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Models[2].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*Models[2].sz, &Models[2].v[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Models[3].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Models[3].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*Models[3].sz, &Models[3].v[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(Models[4].VAO);
	glBindBuffer(GL_ARRAY_BUFFER, Models[4].VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*Models[4].sz, &Models[4].v[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	unsigned int texWooden, texCyfer;


	glGenTextures(1, &texWooden);
	glBindTexture(GL_TEXTURE_2D, texWooden);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(GL_TRUE); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("C://Users//morga//source//repos//lab4___//lab4___//container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 2
	// ---------
	glGenTextures(1, &texCyfer);
	glBindTexture(GL_TEXTURE_2D, texCyfer);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load((TEXTURES_PATH + "//cyfer.jpg").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	ShaderProgram.use();
//	ShaderProgram.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//RENDER
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texWooden);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texCyfer);

		ShaderProgram.use();

		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ShaderProgram.setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		ShaderProgram.setMat4("view", view);

		time_t seconds = time(NULL);
		tm timeinfo;
		localtime_s(&timeinfo, &seconds);


		//clock main
		//ShaderProgram.setVec4("color", glm::vec4(0.1, 0.6, 0.1, 1.0));
		glBindVertexArray(Models[3].VAO);
		glm::mat4 model;
		model = glm::translate(model, Models[3].pos);
		ShaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, Models[3].sz);

		//change texture
		ShaderProgram.setInt("texture1", texWooden);

		//clock cyfer
	//	ShaderProgram.setVec4("color", glm::vec4(0.5, 0.4, 0.2, 1.0));
		glBindVertexArray(Models[4].VAO);
		model = glm::translate(model, Models[4].pos);
		ShaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, Models[4].sz);

		ShaderProgram.setInt("texture1", texCyfer);
		//hourArrow
		glBindVertexArray(Models[0].VAO);
		model = glm::translate(model, Models[0].pos);
		float angle = hourAngle(timeinfo);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.1f));
		ShaderProgram.setMat4("model", model);
	//	ShaderProgram.setVec4("color", glm::vec4(0.0, 0.7, 0.5, 1.0));
		glDrawArrays(GL_TRIANGLES, 0, Models[0].sz);
		model = glm::rotate(model, -glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.1f));
	//	ShaderProgram.setVec4("color", glm::vec4(0.5, 0.0, 0.5, 1.0));

		//minute arrow
		glBindVertexArray(Models[1].VAO);
		model = glm::translate(model, Models[1].pos);
		angle = minuteAngle(timeinfo);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.1f));
		ShaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, Models[1].sz);
		model = glm::rotate(model, -glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.1f));
		
		//second arrow
	//	ShaderProgram.setVec4("color", glm::vec4(0.2, 0.1, 0.7, 1.0));
		glBindVertexArray(Models[2].VAO);
		model = glm::translate(model, Models[2].pos);
		angle = secondAngle(timeinfo);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.1f));
		ShaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, Models[2].sz);
		model = glm::rotate(model, -glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.1f));

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

float hourAngle(tm timeinfo)
{
	return -((timeinfo.tm_hour % 12) * 30 + timeinfo.tm_min * 0.5);
}

float minuteAngle(tm timeinfo)
{
	return -(timeinfo.tm_min * 6 + timeinfo.tm_sec * 0.1);
}

float secondAngle(tm timeinfo)
{
	return -timeinfo.tm_sec * 6;
}

void Model::readObj(string filename, int drawtype)
{
	if (v)
		delete[] v;

	ifstream File(MODELS_PATH+"//"+ filename, ios::in);
	if (!File.is_open())
	{
		cout << "Couldn't open file." << endl;
		return;
	}

	string buff;

	vector<double> v_t;
	vector<double> uv_t;
	vector<double> n_t;
	vector<int> vIndices, nIndices, uvIndices;

	while (!File.eof())
	{
		File >> buff;
		if (buff == "v")
		{
			double vertex;
			for (int i = 0; i < 3; ++i)
			{
				File >> vertex;
				v_t.push_back(vertex);
			}
		}
		else if (buff == "vt")
		{
			double uv;
			for (int i = 0; i < 2; ++i)
			{
				File >> uv;
				uv_t.push_back(uv);
			}
		}
		else if (buff == "vn")
		{
			double normal;
			for (int i = 0; i < 3; ++i)
			{
				File >> normal;
				n_t.push_back(normal);
			}
		}
		else if (buff == "f")
		{
			unsigned int Indices[3][3];
			for (int i = 0; i <= 2; ++i)
			{
				File >> buff;
				int k = 0;
				for (int j = 0; j <= 2; ++j)
				{
					int num = 0;
					while (k < buff.size() && buff[k] != '/' && buff[k] != ' ')
					{
						num *= 10;
						num += buff[k] - '0';
						++k;
					}
					++k;
					Indices[i][j] = num;
				}
			}
			vIndices.push_back(Indices[0][0]);
			uvIndices.push_back(Indices[0][1]);
			nIndices.push_back(Indices[0][2]);
			vIndices.push_back(Indices[1][0]);
			uvIndices.push_back(Indices[1][1]);
			nIndices.push_back(Indices[1][2]);
			vIndices.push_back(Indices[2][0]);
			uvIndices.push_back(Indices[2][1]);
			nIndices.push_back(Indices[2][2]);
		}
	}

	if (vIndices.size() != nIndices.size() || vIndices.size() != uvIndices.size())
	{
		cout << "Incorrect obj" << endl;
		return;
	}
	int size = vIndices.size();
	v = new double[size*8];
	//uv = new double[size*2];
	//n = new double[size*3];
	double tmp;
	for (int i = 0; i < size; ++i)
	{
		v[8*i] = scale*v_t[3*vIndices[i]-3];
		v[8 * i + 1] = scale*v_t[3 * vIndices[i]-2];
		v[8 * i + 2] = scale*v_t[3 * vIndices[i]-1];
		v[8*i+3] = uv_t[2*uvIndices[i]-2];
	//	cout << v[8 * i + 1] << " " << v[8 * i + 2] << endl;
		v[8*i+4] = uv_t[2 * uvIndices[i] - 1];
		v[8*i+5] = n_t[3*nIndices[i]-3];
		v[8*i+6] = n_t[3*nIndices[i]-2];
		v[8*i+7] = n_t[3*nIndices[i] -1];
	}
//	cout << modf(-1.3, &tmp) << endl;
	sz = size*8;
}

void Model::init(int drawtype)
{
	glEnableVertexAttribArray(0);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(double)*sz, &v[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	//{
	//	camera.
	//}
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);

	}
//	camera.ProcessMouseScroll(yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}