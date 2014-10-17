//#include <QtWidgets/QApplication>
//#include <Engine/DebugTools/DebugMemHeader.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/Component.h>
#include <Engine/Entity/Components/ScriptComponent.h>
#include <Engine/Tools/MatrixInfo.h>
#include <Engine/Systems/GameObjectManager.h>

#include <iostream>
int main(int argc, char * argv[]) {
	//QApplication app(argc, argv);
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GameObjectManager game;

	Entity * e = game.AddEntity("Steve");
	e->addComponent(new MatrixInfo());
	e->getTrans()->pos.x = -10;
	std::string scripts[] = {
		//*hello world
		"context.exampleVar = 5                                     \n"
		"context.start = function()                                 \n"
		"    print(context.exampleVar)                              \n"
		"    return true                                            \n"
		"end                                                        \n"
		"                                                           \n"
		"context.earlyUpdate = function()                           \n"
		"    context.exampleVar = context.exampleVar + 1;           \n"
		"    return true                                            \n"
		"end                                                        \n"
		"context.update = function()                                \n"
		"    print(context.exampleVar)                              \n"
		"    return true                                            \n"
		"end                                                        \n"
		"context.lateUpdate = function() return true end            \n"
		"",

		//*/
		//*random
		"context.start = function() return true end                 \n"
		"                                                           \n"
		"context.earlyUpdate = function() return true end           \n"
		"context.update = function()                                \n"
		"    local x = context.parent.getTrans().pos().getX();      \n"
		"    x = x + Random.RangeFloat(1,3)                         \n"
		"    context.parent.getTrans().pos().setX(x);               \n"
		"    return true                                            \n"
		"end                                                        \n"
		"context.lateUpdate = function()                            \n"
		"    print('x'..context.parent.getTrans().pos().getX())     \n"
		"    return true                                            \n"
		"end                                                        \n"
		"",
		//*/
		//*dt
		"context.start = function() return true end                 \n"
		"                                                           \n"
		"context.earlyUpdate = function() return true end           \n"
		"context.update = function()                                \n"
		"    print('dt: '..Timer.deltaTime())                       \n"
		"    return true                                            \n"
		"end                                                        \n"
		"context.lateUpdate = function() return true end            \n"
		"",
		//*/
		/*accessing script
		"context.start = function() return true; end                \n"
		"    return true                                            \n"
		"end                                                        \n"
		"                                                           \n"
		"context.earlyUpdate = function()                           \n"
		"    context.parent.getScript().context().parent.getTrans().pos().setX(-8) \n"
		"    return true                                            \n"
		"end                                                        \n"
		"context.update = function() return true end                \n"
		"context.lateUpdate = function() return true end            \n"
		//*/ //*
	};
	for (int i = 0; i < sizeof(scripts)/sizeof(*scripts); i++)
	{
		e->addComponent(new ScriptComponent(scripts[i].c_str()));
	}
	e->getScript();
	game.init();
	e->getScript();
	for (int i = 0; i < 5; i++) {
		game.update();
	}
	std::cout << e->getTrans()->pos.x << std::endl;

	return 0;//app.exec();
}