#include <QtWidgets/QApplication>
#include <../Engine/Engine/DebugTools/DebugMemHeader.h>
#include <../Engine/Engine/Entity/Entity.h>
#include <../Engine/Engine/Entity/Component.h>
#include <../Engine/Engine/Entity/Components/ScriptComponent.h>
#include <../Engine/Engine/Tools/MatrixInfo.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Systems/GameObjectManager.h>

#include <CorbinGui/BasicQGLGui.h>
#include <CorbinGui/GuiSkellyTon.h>

#include <ShapeGenerator.h>
#include <Engine/Systems/Resources/Shaders/DefaultShaders.h>

#pragma region shaderCode
const char * defaultVertShader = "#version 400					  \n"
	"															  \n"
	"in layout(location=0) vec3 pos;							  \n"
	"in layout(location=1) vec3 col;							  \n"
	"in layout(location=3) vec2 uv;								  \n"
	"															  \n"
	"out vec2 fragUv;											  \n"
	"out vec3 fragCol;											  \n"
	"out vec3 fragPos;											  \n"
	"out vec3 fragWorldPos;										  \n"
	"															  \n"
	"//mats														  \n"
	"uniform mat4x4 viewTransform;								  \n"
	"uniform mat4x4 model2WorldTransform;						  \n"
	"															  \n"
	"void main() {												  \n"
	"	vec4 transformedPos = model2WorldTransform * vec4(pos,1); \n"
	"	gl_Position = viewTransform * transformedPos;			  \n"
	"	fragUv = uv;											  \n"
	"	fragCol = col;											  \n"
	"	fragPos = pos;											  \n"
	"	fragWorldPos = vec3(transformedPos);					  \n"
	"}															  \n"
	"															  \n";
//color
const char * defaultFragShader_Color = "#version 400\n"
	"												\n"
	"in vec3 fragCol;								\n"
	"												\n"
	"void main() {									\n"
	"	gl_FragColor = vec4(fragCol,1);				\n"
	"}												\n"
	"";
//texture
const char * defaultFragShader_Texture = "#version 400\n"
	"												  \n"
	"in vec2 fragUv;								  \n"
	"uniform sampler2D myTexture;					  \n"
	"												  \n"
	"void main() {									  \n"
	"	gl_FragColor = texture(myTexture,fragUv);	  \n"
	"}												  \n"
	"";
//Cube map withModeldPos
const char * defaultFragShader_CubeMapModel= "#version 400\n"
	"													  \n"
	"in vec3 fragPos;									  \n"
	"uniform samplerCube worldMap;						  \n"
	"													  \n"
	"void main() {										  \n"
	"	gl_FragColor = texture(worldMap,fragPos);		  \n"
	"}													  \n"
	"";
//Cube map withWorldPos
const char * defaultFragShader_CubeMapWorld = "#version 400\n"
	"													   \n"
	"in vec3 fragWorldPos;								   \n"
	"uniform samplerCube worldMap;						   \n"
	"													   \n"
	"void main() {										   \n"
	"	gl_FragColor = texture(worldMap,fragWorldPos);	   \n"
	"}													   \n"
	"													   \n";
#pragma endregion

int main(int argc, char * argv[]) {
	QApplication app(argc, argv);
	GuiSkellyTon gui;


	auto keyBoardController = resourceManager.addScript_src("KeyboardController",
		"function context:start()                           \n"
		"    self.Speed = 5  \n"
		"    return true                                    \n"
		"end                                                \n"
		"function context:update()                           \n"
		"    if(Input.getKeyDown(KeyCode.W)) then                    \n"
		"       local tmp = self.parent.getTrans().pos().getY();  \n"
		"       tmp = tmp + Timer.deltaTime() * self.Speed        \n"
		"       self.parent.getTrans().pos().setY(tmp);           \n"
		"     end                                                 \n"

		"    if(Input.getKeyDown(KeyCode.S)) then                    \n"
		"       local tmp = self.parent.getTrans().pos().getY();  \n"
		"       tmp = tmp - Timer.deltaTime() * self.Speed        \n"
		"       self.parent.getTrans().pos().setY(tmp);           \n"
		"     end                                                 \n"

		"    if(Input.getKeyDown(KeyCode.A)) then                    \n"
		"       local tmp = self.parent.getTrans().pos().getX();  \n"
		"       tmp = tmp - Timer.deltaTime() * self.Speed        \n"
		"       self.parent.getTrans().pos().setX(tmp);           \n"
		"     end                                                 \n"

		"    if(Input.getKeyDown(KeyCode.D)) then                    \n"
		"       local tmp = self.parent.getTrans().pos().getX();  \n"
		"       tmp = tmp + Timer.deltaTime() * self.Speed        \n"
		"       self.parent.getTrans().pos().setX(tmp);           \n"
		"     end                                                 \n"
		//"    print(self.parent.getTrans().rot().getX());     \n"
		"    return true                                     \n"
		"end                                                 \n"
		"");

	auto geo = resourceManager.addMesh("cube",Neumont::ShapeGenerator::makeCube());
	auto shader = resourceManager.addShader_src("basic Shader",DefaultShaders::VertexShader::DefaultVertShader(),DefaultShaders::FragShader::FragModelColor());
	auto tmp = gui.addEntity("Bob");
	auto comp = tmp->addComponent<RenderableComponent>();
	tmp->getTrans()->pos.z =  -20;
	comp->whatGeo = geo;
	comp->howShader = shader;
	tmp->addComponent<ScriptComponent>()->script = resourceManager.addScript_src("rotator",//random
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

	auto cam = gui.addEntity("Cam");
	cam->addComponent<CameraComponent>();
	cam->addComponent<ScriptComponent>()->script = keyBoardController;

	auto obj2 = gui.addEntity("Fast Rotator");
	obj2->addComponent<RenderableComponent>();
	obj2->getRenderable()->whatGeo = geo;
	obj2->getRenderable()->howShader = shader;
	obj2->addComponent<ScriptComponent>()->script = resourceManager.getFirstScript("rotator");
	//obj2->addComponent<ScriptComponent>(keyBoardController->getID());
	obj2->getTrans()->pos.x = 5;
	obj2->Parent(tmp);

	auto obj3 = gui.addEntity("Swinger");
	obj3->addComponent<RenderableComponent>();
	obj3->getRenderable()->whatGeo = geo;
	obj3->getRenderable()->howShader = shader;
	obj3->addComponent<ScriptComponent>()->script = resourceManager.getFirstScript("rotator");
	obj3->getTrans()->pos.y = 5;
	obj3->getTrans()->scale = glm::vec3(.5f,.5f,.5f);
	obj3->Parent(tmp);

	auto obj4 = gui.addEntity("Super Swing");
	obj4->addComponent<RenderableComponent>();
	obj4->getRenderable()->whatGeo = geo;
	obj4->getRenderable()->howShader = shader;
	obj4->addComponent<ScriptComponent>()->script = resourceManager.getFirstScript("rotator");
	obj4->getTrans()->pos.y = 2;
	obj4->getTrans()->scale = glm::vec3(.2f,.2f,.2f);
	obj4->Parent(obj3);



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