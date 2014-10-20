//#include <QtWidgets/QApplication>
//#include <Engine/DebugTools/DebugMemHeader.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/Component.h>
#include <Engine/Entity/Components/ScriptComponent.h>
#include <Engine/Tools/MatrixInfo.h>
#include <Engine/Systems/GameObjectManager.h>

#include <iostream>
#include <Engine/Tools/Random/StringRandom.h>
int main(int argc, char * argv[]) {
	//QApplication app(argc, argv);
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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