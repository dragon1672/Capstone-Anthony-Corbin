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
		"context = {}                                          \n"
		"--keep all vars within scope of context               \n"
		"context.exampleVar = 5;                               \n"
		"                                                      \n"
		"--called first                                        \n"
		"context.start = function()                            \n"
		"    print(context.exampleVar)                         \n"
		"    return true                                       \n"
		"end                                                   \n"
		"                                                      \n"
		"--updates are called after start                      \n"
		"context.earlyUpdate = function()                      \n"
		"    context.exampleVar = context.exampleVar + 1;      \n"
		"    return true                                       \n"
		"end                                                   \n"
		"context.update = function()                           \n"
		"    print(context.exampleVar)                         \n"
		"    return true                                       \n"
		"end                                                   \n"
		"context.lateUpdate = function()                       \n"
		"    --yay                                             \n"
		"    return true                                       \n"
		"end                                                   \n"
		"";
	e.init();
	for (int i = 0; i < 2; i++) {
		e.earlyUpdate();
		e.update();
		e.lateUpdate();
	}

	return 0;//app.exec();
}