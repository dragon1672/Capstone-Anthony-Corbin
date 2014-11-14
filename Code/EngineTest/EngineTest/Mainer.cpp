#include <QtWidgets/QApplication>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/DebugTools/DebugMemHeader.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Entity/Entity.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Entity/Component.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Entity/Components/ScriptComponent.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Tools/MatrixInfo.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Systems/GameObjectManager.h>

#include <CorbinGui/BasicQGLGui.h>
#include <CorbinGui/GuiSkellyTon.h>

#include <ShapeGenerator.h>
#include <Engine/Systems/Resources/Shaders/DefaultShaders.h>
#include <Engine/DebugTools/DebugMemHeader.h>

#include <Engine/IO/FileIO.h>

int main(int argc, char * argv[]) {

	int ret = -1;
	QApplication app(argc, argv);
	GuiSkellyTon gui;

	auto keyBoardController = resourceManager.addScript_src(Script::getClassTemplate("KeyboardController",
		//start
		"    self.KeySpeed = 9                                                      \n"
		"    self.MouseSpeed = .5                                                   \n"
		"",
		//update
		"    if(Input.getKeyDown(KeyCode.R)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getY();                    \n"
		"       tmp = tmp - Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setY(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.W)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getZ();                    \n"
		"       tmp = tmp + Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setZ(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.S)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getZ();                    \n"
		"       tmp = tmp - Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setZ(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.F)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getY();                    \n"
		"       tmp = tmp + Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setY(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.D)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getX();                    \n"
		"       tmp = tmp - Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setX(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.A)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getX();                    \n"
		"       tmp = tmp + Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setX(tmp);                             \n"
		"     end                                                                   \n"
		"                                                                           \n"
		"    if(Input.getKeyDown(18)) then         \n"
		"       local x = self.parent.getTrans().rot().getX();                      \n"
		"       local y = self.parent.getTrans().rot().getY();                      \n"
		"       x = x + self.MouseSpeed * Input.getMouse().delta().getY()           \n"
		"       y = y - self.MouseSpeed * Input.getMouse().delta().getX()           \n"
		"       self.parent.getTrans().rot().setX(x);                               \n"
		"       self.parent.getTrans().rot().setY(y);                               \n"
		"    end                                                                    \n"
		"                                                                           \n"
		"                                                                           \n"
		""));

	resourceManager.WorkingDir("./../resources/");
	resourceManager.loadNeumontStuff();

	auto shader = resourceManager.getDefault<ShaderProgram>();

	auto dragon  = resourceManager.add2DTexture("dragon","flying_dragon_1.png");
	auto texture = resourceManager.add2DTexture("space","space.png");


	EditorGame * game = gui.Game();

	game->AddEntity("Bob");
	auto comp = game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getTrans()->pos.z =  20;
	game->currentEntity.getTrans()->pos.z =  -20;
	comp->Geo(resourceManager.getFirstMesh("NU_Cube"));
	comp->Shader(shader);
	comp->material.Diffuse(dragon);
	comp = game->currentEntity.addComponent<RenderableComponent>();
	comp->material.Diffuse(texture);
	auto sphere = resourceManager.duplicate(resourceManager.getFirstMesh("NU_Sphere"));
	sphere->scale(2,1,1);
	comp->Geo(sphere);
	comp->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.addScript_src(Script::getClassTemplate("rotator",//random
		"    self.rotSpeed = Random.RangeFloat(10,300);      \n"
		"    return true                                     \n"
		"",
		"    local x = self.parent.GetScript('rotator').parent.getTrans().rot().getX();  \n"
		//"    local x = self.parent.getTrans().rot().getX();  \n"
		"    x = x + Timer.deltaTime() * self.rotSpeed       \n"
		"    self.parent.getTrans().rot().setX(x);           \n"
		"")));
	(void)keyBoardController;

	game->AddEntity("Cam");
	game->currentEntity.addComponent<CameraComponent>();
	game->currentEntity.addComponent<ScriptComponent>()->myScript(keyBoardController);

	game->AddEntity("Fast Rotator");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->Geo(resourceManager.getFirstMesh("NU_Sphere"));
	game->currentEntity.getRenderable()->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.getFirstScript("rotator"));
	game->currentEntity.getTrans()->pos.x = 5;
	game->currentEntity.Parent("Bob");

	game->AddEntity("Swinger");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->Geo(resourceManager.getFirstMesh("NU_Arrow"));
	game->currentEntity.getRenderable()->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.getFirstScript("rotator"));
	game->currentEntity.getTrans()->pos.y = 5;
	game->currentEntity.getTrans()->scale = glm::vec3(.5f,.5f,.5f);
	game->currentEntity.Parent("Bob");

	game->AddEntity("Super Swing");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->Geo(resourceManager.getFirstMesh("NU_Plane"));
	game->currentEntity.getRenderable()->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.getFirstScript("rotator"));
	game->currentEntity.getTrans()->pos.y = 2;
	game->currentEntity.getTrans()->scale = glm::vec3(.2f,.2f,.2f);
	game->currentEntity.Parent("Swinger");

	gui.init();

	gui.show();

	ret = app.exec();

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return ret;
}