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

	auto geo = resourceManager.addMesh("cube",Neumont::ShapeGenerator::makeCube());
	auto shader = resourceManager.addShader_src("basic Shader",DefaultShaders::VertexShader::DefaultVertShader(),DefaultShaders::FragShader::FragModelColor());
	auto tmp = gui.addEntity("Bob");
	auto comp = tmp->addComponent<RenderableComponent>();
	tmp->getTrans()->rot.x = 5;
	comp->whatGeo = geo;
	comp->howShader = shader;
	tmp->addComponent<ScriptComponent>(resourceManager.addScript_src("name",//random
		"function context:start()                           \n"
		"    self.rotSpeed = Random.RangeFloat(10,300);     \n"
		"    return true                                    \n"
		"end                                                \n"
		"function context:update()                           \n"
		"    local x = self.parent.getTrans().rot().getX();  \n"
		"    x = x + Timer.deltaTime() * self.rotSpeed                  \n"
		"    self.parent.getTrans().rot().setX(x);           \n"
		//"    print(self.parent.getTrans().rot().getX());     \n"
		"    return true                                     \n"
		"end                                                 \n"
		"")->getID());

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