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

int main(int argc, char * argv[]) {
	QApplication app(argc, argv);
	GuiSkellyTon gui;


	auto keyBoardController = resourceManager.addScript_src("KeyboardController",
		"function context:start()                                                   \n"
		"    self.KeySpeed = 9                                                      \n"
		"    self.MouseSpeed = .5                                                   \n"
		"    return true                                                            \n"
		"end                                                                        \n"
		"function context:update()                                                  \n"
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
		"print('yus')\n"
		"       local x = self.parent.getTrans().rot().getX();                      \n"
		"       local y = self.parent.getTrans().rot().getY();                      \n"
		"       x = x + self.MouseSpeed * Input.getMouse().delta().getY()           \n"
		"       y = y + self.MouseSpeed * Input.getMouse().delta().getX()           \n"
		"       self.parent.getTrans().rot().setX(x);                               \n"
		"       self.parent.getTrans().rot().setY(y);                               \n"
		"    end                                                                    \n"
		"                                                                           \n"
		"                                                                           \n"
		//"    print(self.parent.getTrans().rot().getX());                            \n"
		"    return true                                                            \n"
		"end                                                                        \n"
		"");

	resourceManager.WorkingDir("./../resources/");

	auto geo = resourceManager.addMesh("cube",Neumont::ShapeGenerator::makeCube());
	auto shader = resourceManager.getDefault<ShaderProgram>();
	auto sphere = resourceManager.addMesh("cube",Neumont::ShapeGenerator::makeSphere(30));
	sphere->initUVData();

	auto dragon  = resourceManager.add2DTexture("dragon","flying_dragon_1.png");
	auto texture = resourceManager.add2DTexture("dragon","space.png");


	sphere->scale(2,1,1);

	EditorGame * game = gui.Game();

	game->AddEntity("Bob");
	auto comp = game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getTrans()->pos.z =  -20;
	comp->geo = geo;
	comp->shader = shader;
	comp->material.Diffuse(dragon);
	comp = game->currentEntity.addComponent<RenderableComponent>();
	comp->material.Diffuse(texture);
	comp->geo = sphere;
	comp->shader = shader;
	game->currentEntity.addComponent<ScriptComponent>()->script = resourceManager.addScript_src("rotator",//random
		"function context:start()                            \n"
		"    self.rotSpeed = Random.RangeFloat(10,300);      \n"
		"    return true                                     \n"
		"end                                                 \n"
		"function context:update()                           \n"
		"    local x = self.parent.getTrans().rot().getX();  \n"
		"    x = x + Timer.deltaTime() * self.rotSpeed       \n"
		"    self.parent.getTrans().rot().setX(x);           \n"
		//"    print(self.parent.getTrans().rot().getX());     \n"
		"    return true                                     \n"
		"end                                                 \n"
		"");
	(void)keyBoardController;

	game->AddEntity("Cam");
	game->currentEntity.addComponent<CameraComponent>();
	game->currentEntity.addComponent<ScriptComponent>()->script = keyBoardController;

	game->AddEntity("Fast Rotator");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->geo = geo;
	game->currentEntity.getRenderable()->shader = shader;
	game->currentEntity.addComponent<ScriptComponent>()->script = resourceManager.getFirstScript("rotator");
	game->currentEntity.getTrans()->pos.x = 5;
	game->currentEntity.Parent("Bob");

	game->AddEntity("Swinger");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->geo = geo;
	game->currentEntity.getRenderable()->shader = shader;
	game->currentEntity.addComponent<ScriptComponent>()->script = resourceManager.getFirstScript("rotator");
	game->currentEntity.getTrans()->pos.y = 5;
	game->currentEntity.getTrans()->scale = glm::vec3(.5f,.5f,.5f);
	game->currentEntity.Parent("Bob");

	game->AddEntity("Super Swing");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->geo = geo;
	game->currentEntity.getRenderable()->shader = shader;
	game->currentEntity.addComponent<ScriptComponent>()->script = resourceManager.getFirstScript("rotator");
	game->currentEntity.getTrans()->pos.y = 2;
	game->currentEntity.getTrans()->scale = glm::vec3(.2f,.2f,.2f);
	game->currentEntity.Parent("Swinger");

	gui.init();

	//gui.startGameLoop();

	gui.show();

	return app.exec();
}

//Script Testing
/*
#include <iostream>
#include <Engine/Tools/Random/StringRandom.h>
int main(int argc, char * argv[]) {

GameObjectManager game;

Entity * e = game.AddEntity("Steve");
e->addComponent<MatrixInfo>();
e->getTrans()->pos.x = -10;
Script * scripts[] = {
resourceManager.addScript_src("name",//hello world
"function context:start()                           \n"
"    self.exampleVar = 5;                           \n"
"    return true                                    \n"
"end                                                \n"
"                                                   \n"
"function context:update()                          \n"
"    self.exampleVar = self.exampleVar + 1;         \n"
"    print('Herro: '..self.exampleVar)              \n"
"    self:myPrint();                                \n"
"    return true                                    \n"
"end                                                \n"
""),


resourceManager.addScript_src("name",//random
"function context:update()                           \n"
"    local x = self.parent.getTrans().pos().getX();  \n"
"    x = x + Random.RangeFloat(1,3)                  \n"
"    self.parent.getTrans().pos().setX(x);           \n"
"                                                    \n"
"    print(Random.String.Letters(5)..':'..self.parent.getTrans().pos().getX()) \n"
"    return true                                     \n"
"end                                                 \n"
""),

resourceManager.addScript_src("name",//dt
"function context:update()                         \n"
"    print('dt: '..Timer.deltaTime())              \n"
"    return true                                   \n"
"end                                               \n"
""),

resourceManager.addScript_src("name",//accessing script -- broken
"function context:earlyUpdate()                                          \n"
"    self.parent.getScript().context().parent.getTrans().pos().setX(-8)  \n"
"    return true                                                         \n"
"end                                                                     \n"
""),
};
int size = sizeof(scripts)/sizeof(*scripts);
size--; // skipping the last one

Entity * f = game.AddEntity("Bob");
f->addComponent<MatrixInfo>();
f->getTrans()->pos.x = -10;
for (int i = 0; i < sizeof(scripts)/sizeof(*scripts); i++)
{
e->addComponent<ScriptComponent>(scripts[i]->getID());
f->addComponent<ScriptComponent>(scripts[i]->getID());
}
//game.init();
//for (int i = 0; i < 5; i++) {
//	std::cout << std::endl;
//	game.update();
//}
//std::cout << e->getTrans()->pos.x << std::endl;


for (int i = 0; i < 1000; i++)
{
std::cout << Random::rString::Letters(5) << std::endl;
}

return 0;//app.exec();
}
//*/