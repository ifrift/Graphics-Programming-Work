#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    /*Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
	Audio* audioDevice();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture	
	Overlay* overlay(); //load texture
	Shader* shaderPass();
	Shader* shaderBlur();
	Shader* shaderToon();
	Shader* shaderRim();*/
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	whistle = audioDevice.loadSound("..\\res\\bang.wav");
	backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	texture.init("..\\res\\bricks.jpg"); //load texture
	texture1.init("..\\res\\water.jpg"); //load texture

	shaderPass.init("..\\res\\shaderRim.vert","..\\res\\shaderRim.frag");
	shaderBlur.init("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	shaderToon.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	shaderRim.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	shaderSkybox.init("..\\res\\shaderSkyBox.vert", "..\\res\\shaderSkyBox.frag");
	shaderCombo.init("..\\res\\combo.vert", "..\\res\\combo.frag");
	shaderMyToon.init("..\\res\\myShaderToon.vert", "..\\res\\myShaderToon.frag");
	shaderGeo.initGeo("..\\res\\geomShader.vert", "..\\res\\geomShader.geom", "..\\res\\geomShader.frag");

	//shaderRefl.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	shaderRefTest.init("..\\res\\shaderRef.vert", "..\\res\\shaderRef.frag");
	//Vertex2D vertices[] = { Vertex2D(glm::vec2(-0.5, 1.0), glm::vec2(0.0, 0.0)),
	//						Vertex2D(glm::vec2(0.5, 0.5), glm::vec2(1.0, 0.0)),
	//						Vertex2D(glm::vec2(0.5,-0.5), glm::vec2(1.0, 1.0)),
	//						Vertex2D(glm::vec2(-0.5,-0.5), glm::vec2(0.0, 1.0)) };

	vector<std::string>faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	overlay.init("..\\res\\brick.jpg");

	mesh2.loadModel("..\\res\\cap.obj");
	mesh1.loadModel("..\\res\\Mug_obj.obj");
	mesh3.loadModel("..\\res\\monkey3.obj");
	mesh4.loadModel("..\\res\\Dog.obj");

	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}



void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		//testRun = testRun = counter
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}

void MainGame::setADSLighting()
{
	modelView = transform.GetModel() * myCamera.GetView();
	
	shaderPass.setMat4("ModelViewMatrix", modelView);
	shaderPass.setMat4("ProjectionMatrix", myCamera.GetProjection()); 

	
	glm::mat4 normalMatrix = transpose(inverse(modelView));
	
	shaderPass.setMat4("NormalMatrix", normalMatrix);

	shaderPass.setVec4("Position", glm::vec4(10.0,10.0,10.0,1.0));
	shaderPass.setVec3("Intensity", glm::vec3(0.0, 0.0, 0.0));

	shaderPass.setVec3("ka", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("kd", glm::vec3(0.5, 0.5, 0.5));
	shaderPass.setVec3("ks", glm::vec3(0.5, 0.5, 0.5));

	shaderPass.setFloat("Shininess", 0.5);
}

void MainGame::setToonLighting()
{
	shaderToon.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
}

void MainGame::setMyToon()
{	
	posX = cos(counter2) * 0.5;
	posZ = sin(counter2) * 0.5;
	shaderMyToon.setVec3("lightDir", glm::vec3(posX, 0.5, posZ));
}

void MainGame::setRef1()
{
	shaderRefTest.setMat4("ModelMatrix", modelView);
	//shaderRefTest.setMat4("ViewMatrix", myCamera.GetView());
	//shaderRefTest.setMat4("ProjectionMatrix", myCamera.GetProjection());
	shaderRefTest.setVec3("cameraPosition", myCamera.getPos());	
}


void MainGame::setRimShader()
{
	shaderRim.setMat4("u_vm", myCamera.GetView());
	shaderRim.setMat4("u_pm", myCamera.GetProjection());
}

/*void MainGame::RAM()
{
	shaderToon.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	rAM.setMat4("u_vm", myCamera.GetView());
	rAM.setMat4("u_pm", myCamera.GetProjection());
}*/


void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(shaderBlur.getProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(shaderBlur.getProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(shaderBlur.getProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(shaderBlur.getProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}
//GEO SHADER
void MainGame::linkGeomShader()
{		
	shaderGeo.setFloat("time", counter);
}

void MainGame::linkComboShader()
{		
	shaderCombo.setFloat("minDist", 0.0f);
	shaderCombo.setFloat("maxDist", 20.0f);	
	shaderCombo.setVec3("fogColor", glm::vec3(1.0f, 0.8f, 0.0f));
	posX = cos(counter2) * 0.5;
	posZ = sin(counter2) * 0.5;
	shaderCombo.setVec3("lightDir", glm::vec3(posX, 0.5, posZ));
}


void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);		
	linkGeomShader();
	Skybox();	

	transform.SetPos(glm::vec3(-2.5, 1.5, 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
		
	shaderMyToon.Bind();		
	setMyToon();
	shaderMyToon.Update(transform, myCamera);
	mesh1.draw();	

	transform.SetPos(glm::vec3(sinf(counter), sinf(counter), -sinf(counter)*5));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(3, 3, 3));
	
	shaderCombo.Bind();
	linkComboShader();	
	shaderCombo.Update(transform, myCamera);	
	mesh2.draw();

	transform.SetPos(glm::vec3(4.0, 0.5, 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
		
	//shaderRefTest.Bind();
	//setRef1();
	//shaderRefTest.Update(transform, myCamera);
	

	//transform.SetPos(glm::vec3(-sinf(counter), -sinf(counter), -sinf(counter)));
	transform.SetPos(glm::vec3(2.0f, -2.0f, -1.0f));
	transform.SetRot(glm::vec3(0.0, 10.0f, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	//GEO SHADER
	//shaderGeo.Bind();		
	//shaderGeo.Update(transform, myCamera);	
	//texture.Bind(0);
	shaderRefTest.Bind();
	setRef1();
	//shaderToon.Update(transform, myCamera);
	shaderRefTest.Update(transform, myCamera);
	mesh3.draw();	

	transform.SetPos(glm::vec3(-2.0f, -2.0f, 1.0f));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	//GEO SHADER
	shaderGeo.Bind();
	shaderGeo.Update(transform, myCamera);
	texture.Bind(0);
	mesh4.draw();

	counter = counter + 0.001f;
	//counter3 = counter3 + 0.001f;
	counter2 = counter2 + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 