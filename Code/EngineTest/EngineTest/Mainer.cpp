//#include <QtWidgets/QApplication>
//#include <Engine/DebugTools/DebugMemHeader.h>
#include <Engine/Entity/Entity.h>
#include <Engine/Entity/Component.h>
#include <Engine/Entity/Components/ScriptComponent.h>


int main(int argc, char * argv[]) {
	//QApplication app(argc, argv);
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Entity e;
	e.addComponent(new ScriptComponent());
	e.getComponent<ScriptComponent>()->script = ""
		"context = {}                                            \n"
		"context.exampleVar = 5;                                 \n"
		"	                                                     \n"
		"--Fill Me and init variables                            \n"
		"--Called before any update function on game start       \n"
		"context.start = function()                              \n"
		"	content.exampleVar = 20;                             \n"
		"	                                                     \n"
		"	return true -- never edit                            \n"
		"end                                                     \n"
		"	                                                     \n"
		"	                                                     \n"
		"--Called before update                                  \n"
		"context.earlyUpdate = function()                        \n"
		"	context.exampleVar = context.exampleVar+ 1;          \n"
		"	                                                     \n"
		"	return true -- never edit                            \n"
		"end                                                     \n"
		"	                                                     \n"
		"	                                                     \n"
		"context.update = function()                             \n"
		"	print(context.exampleVar)                            \n"
		"	                                                     \n"
		"	return true -- never edit                            \n"
		"end                                                     \n"
		"	                                                     \n"
		"	                                                     \n"
		"--called after all other updates                        \n"
		"context.lateUpdate = function()                         \n"
		"	                                                     \n"
		"	return true -- never edit                            \n"
		"end                                                     \n"
		"";
	e.init();
	for (int i = 0; i < 2; i++) {
		e.earlyUpdate();
		e.update();
		e.lateUpdate();
	}

	return 0;//app.exec();
}