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
		"function context:start()                           \n"
		"    self.exampleVar = 5;                           \n"
		"    return true                                    \n"
		"end                                                \n"
		"                                                   \n"
		"function context:myPrint(a) print(a..' example') end\n"
		"                                                   \n"
		"function context:earlyUpdate() self:myPrint('Early') return true end     \n"
		"function context:update()                          \n"
		"    self.exampleVar = self.exampleVar + 1;         \n"
		"    print('Herro: '..self.exampleVar)              \n"
		"    self:myPrint();                                \n"
		"    return true                                    \n"
		"end                                                \n"
		"function context:lateUpdate() self:myPrint('late') return true end     \n"
		"",

		//*/
		//*random
		"function context:myPrint(a) print(a..' random') end\n"
		"                                                   \n"
		"function context:earlyUpdate() self:myPrint('Early') return true end     \n"
		"function context:update()                           \n"
		"    local x = self.parent.getTrans().pos().getX();  \n"
		"    x = x + Random.RangeFloat(1,3)                  \n"
		"    self.parent.getTrans().pos().setX(x);           \n"
		"                                                    \n"
		"    print('x'..self.parent.getTrans().pos().getX()) \n"
		"    return true                                     \n"
		"end                                                 \n"
		"function context:lateUpdate() self:myPrint('late') return true end     \n"
		"",
		//*/
		//*dt
		"function context:myPrint(a) print(a..' dt') end\n"
		"                                                   \n"
		"function context:earlyUpdate() self:myPrint('Early') return true end     \n"
		"function context:update()                         \n"
		"    print('dt: '..Timer.deltaTime())              \n"
		"    return true                                   \n"
		"end                                               \n"
		"function context:lateUpdate() self:myPrint('late') return true end     \n"
		"",
		//*/
		/*accessing script -- broken
		"function context:start() return true end                                \n"
		"                                                                        \n"
		"function context:earlyUpdate()                                          \n"
		"    self.parent.getScript().context().parent.getTrans().pos().setX(-8)  \n"
		"    return true                                                         \n"
		"end                                                                     \n"
		"context.update = function() return true end                             \n"
		"function context:lateUpdate() return true end                           \n"
		"",
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