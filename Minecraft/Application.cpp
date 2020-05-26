#include "Application.h"

Application* Application::app = nullptr;

Application& Application::Get() {
	return *app;
}

void Application::SetPostProcessEffect(const PostProcessEffect& effect) {
	app->effect = effect;
}

void Application::OnStart() {
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Load default shaders
	ShaderManager::InitShaders();

	app = this;

	effect = PostProcessEffect::NORMAL;
	lastResolution = Engine::GetWindowSize();
	
	// Create World
	printf("World Size: %llu\n", sizeof(World));
	world = new World();

	// Post processing
	postProcessing = new PostProcess("postProcess", 1280, 720, FramebufferType::COLOUR_AND_DEPTH);
	postProcessing->GetShader().SetTextureUnit("screenColour", 0);
	postProcessing->GetShader().SetTextureUnit("screenDepth", 1);

	// Framebuffers
	framebuffer = new Framebuffer(glm::ivec2(1280, 720), FramebufferType::DEPTH);
	blockBuffer = new Framebuffer(glm::ivec2(160, 160), FramebufferType::COLOUR);

	camera2D = new Camera2D(glm::vec3(0, 4, -10), glm::vec3(0), -1.6f, 1.6f, -1.6f, 1.6f, 0.1f, 100.0f);

	SetupObjects();


	// Set up block highlighting
	std::vector<float>highlight_verts = std::vector<float>({
		// TOP
		-0.5f, 0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f,-0.5f,

		// BOT
		-0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f,-0.5f,

		// CORNERS
		-0.5f, 0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		 0.5f, 0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f
	});
	std::vector<int> highlight_attributes = std::vector<int>({
		3
	});

	blockHighlight = new Object3D(glm::vec3(0), highlight_verts, highlight_attributes);
	blockHighlight->SetRender(Object3D::RenderType::LINES);
	blockHighlight->SetShader(ShaderManager::AddShader("lineColour"));
	blockHighlight->GetShader()->SetVector3("colour", glm::vec3(1, 1, 1));
	blockHighlight->GetTransform().SetScale(1.0025f);

	// Turn off cursor
	Input::DisableCursor(true);
}

void Application::OnEnd() {
	ShaderManager::DestroyShaders();
	
	delete world;
	
	delete blockHighlight;
	delete camera2D;
	delete selectedBlock;

	delete framebuffer;
	delete blockBuffer;
	delete postProcessing;
}

void Application::OnUpdate() {
	// DEBUG
	if(Input::IsKeyPressed(GLFW_KEY_2) || Input::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		lockCursor = !lockCursor;
		Input::DisableCursor(lockCursor);
	}
	
	// Update framebuffer resolution
	glm::vec2 resolution = Engine::GetWindowSize();
	if(lastResolution != resolution) {
		lastResolution = resolution;
		postProcessing->GetBuffer().SetSize(resolution);
	}
	
	if((BlockIDs)currentBlock != World::GetPlayer().GetHeldBlock()) {
		currentBlock = (int)World::GetPlayer().GetHeldBlock();
	
		float textureIndex[6];
		for(int i = 0; i < 6; i++)
			textureIndex[i] = BlockManager::GetBlockData((BlockIDs)currentBlock)->texture[i];
	
		selectedBlock->GetShader()->SetFloatArray("faceIndex", 6, *textureIndex);
	}

	world->Update();
}

void Application::OnRender() {
	// Render the world into the buffer to add post processing effects to it
	postProcessing->Begin();
	world->Render();
	
	// Highlight
	Ray::RayHit hit = Ray::RaycastFromCamera(10.0f, *World::GetPlayer().Camera());
	if(hit.hit) {
		blockHighlight->GetTransform().SetPosition(hit.hitPosition + glm::vec3(0.5f));
		blockHighlight->Render(*World::GetPlayer().Camera());
	}		
	
	postProcessing->End();
	
	// Set shader settings
	postProcessing->GetShader().SetInt("effectType", (int)effect);
	postProcessing->GetBuffer().BindTextures(0);
	
	// Render screen quad
	postProcessing->Render();
	
	// Selecteed Block Renderer
	glClearColor(0, 0, 0, 0);
	blockBuffer->Render_Begin();
	selectedBlock->GetTransform().RotateY(Time::DeltaTime() * 30.0f);
	selectedBlock->Render(*camera2D);
	blockBuffer->Render_End();
	
	
	// GUI
	glm::vec2 size = Engine::GetWindowSize();
	ImGui::SetNextWindowPos(ImVec2(size.x - 160 - 32, 16));
	ImGui::Begin("Block Selection", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
	ImGui::SetCursorPos(ImVec2(80.0f - 49.0f, 0.0f));
	ImGui::Text(" Selected Block");
	ImGui::Image((void*)blockBuffer->GetTexture(), ImVec2(160, 160), ImVec2(0, 0), ImVec2(1, 1));
	
	const char* name = BlockManager::GetBlockData((BlockIDs)currentBlock)->name;
	int nameSize = strlen(name) * 7;
	
	ImGui::SetCursorPos(ImVec2(80.0f - (nameSize / 2.0f), 175.0f));
	ImGui::Text(name);
	ImGui::End();
}

void Application::SetupObjects() {
	// Current block being held
	std::vector<float> cube_verts = std::vector<float>({
		// BACK
		 1.0f,-1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
	    -1.0f,-1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		
		-1.0f, 1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,		 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		// RIGHT				
		1.0f,-1.0f,-1.0f,		 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,		 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		
		1.0f, 1.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,-1.0f,		 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		1.0f,-1.0f, 1.0f,		 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		// FRONT				
		-1.0f,-1.0f,-1.0f,		 0.0f, 0.0f,-1.0f, 1.0f, 1.0f, 2.0f,
		 1.0f,-1.0f,-1.0f,		 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 2.0f,
		-1.0f, 1.0f,-1.0f,		 0.0f, 0.0f,-1.0f, 1.0f, 0.0f, 2.0f,

		 1.0f, 1.0f,-1.0f,		 0.0f, 0.0f,-1.0f, 0.0f, 0.0f, 2.0f,
		-1.0f, 1.0f,-1.0f,		 0.0f, 0.0f,-1.0f, 1.0f, 0.0f, 2.0f,
		 1.0f,-1.0f,-1.0f,		 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 2.0f,

		  //LEFT DONE			
		-1.0f,-1.0f, 1.0f,		-1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 3.0f,
		-1.0f,-1.0f,-1.0f,		-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 3.0f,
		-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f,

		-1.0f, 1.0f,-1.0f,		-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f,
		-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 3.0f,
		-1.0f,-1.0f,-1.0f,		-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 3.0f,

	  // TOP					
		-1.0f, 1.0f, 1.0f,		 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 4.0f,
		-1.0f, 1.0f,-1.0f,		 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 4.0f,
		 1.0f, 1.0f, 1.0f,		 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f,

		 1.0f, 1.0f,-1.0f,		 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 4.0f,
		 1.0f, 1.0f, 1.0f,		 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 4.0f,
		-1.0f, 1.0f,-1.0f,		 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 4.0f,

	 // BOT				
		-1.0f,-1.0f,-1.0f,		 0.0f,-1.0f, 0.0f, 0.0f, 0.0f, 5.0f,
		-1.0f,-1.0f, 1.0f,		 0.0f,-1.0f, 0.0f, 0.0f, 1.0f, 5.0f,
		 1.0f,-1.0f,-1.0f,		 0.0f,-1.0f, 0.0f, 1.0f, 0.0f, 5.0f,

		 1.0f,-1.0f, 1.0f,		 0.0f,-1.0f, 0.0f, 1.0f, 1.0f, 5.0f,
		 1.0f,-1.0f,-1.0f,		 0.0f,-1.0f, 0.0f, 1.0f, 0.0f, 5.0f,
		-1.0f,-1.0f, 1.0f,		 0.0f,-1.0f, 0.0f, 0.0f, 1.0f, 5.0f,
													   });
	std::vector<int> attributes = std::vector<int>({
		3, 3, 2, 1
												   });

	selectedBlock = new Object3D(glm::vec3(0), cube_verts, attributes);
	selectedBlock->SetShader(ShaderManager::AddShader("voxelSelected"));
	selectedBlock->GetShader()->SetTextureUnit("texture1", 3);

	currentBlock = (int)BlockIDs::DIRT;
	float textureIndex[6];
	for(int i = 0; i < 6; i++)
		textureIndex[i] = BlockManager::GetBlockData((BlockIDs)currentBlock)->texture[i];
	selectedBlock->GetShader()->SetFloatArray("faceIndex", 6, *textureIndex);
}
