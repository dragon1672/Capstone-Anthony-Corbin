#include <iostream>
#include <luacppinterface.h>

class Scriptable {
	LuaTable scriptVersion;
	virtual void constructor() = 0;

};

class Pie {
public:
	int pie;
	void makeMorePie() {this->pie++;}
	void eatPie() {this->pie--;}
	int getPie() { return pie; }
};
class Mother {
public:
	Pie supply;
	void resetPie() {
		supply = Pie();
	}
	Pie getPie() {
		return supply;
	}
};

void main() {
	std::cout << "Hello World" << std::endl;

	Lua lua;
	lua.LoadStandardLibraries();
	lua.GetGlobalEnvironment().Set("cPrint",lua.CreateFunction<void(std::string)>([](std::string a) -> void { std::cout << "cPrint:" << a << std::endl; })); // make and add function

	
	auto newPie = lua.CreateFunction<LuaUserdata<Pie>()>(std::bind([](Lua lua) -> LuaUserdata<Pie> {
		auto pie = new Pie();
		auto pieData = lua.CreateUserdata<Pie>(pie);

		pieData.Bind("eat",  &Pie::eatPie);
		pieData.Bind("make", &Pie::makeMorePie);
		pieData.Bind("get",  &Pie::getPie);

		return pieData;
	}, lua));

	LuaTable pieObj = lua.CreateTable();
	//const
	pieObj.Set("new", newPie);
	//make varable
	lua.GetGlobalEnvironment().Set("Pie", pieObj);
	
	auto newFoo = lua.CreateFunction<LuaUserdata<Mother>()>(std::bind([](Lua lua) -> LuaUserdata<Mother> {
		auto foo = new Mother();
		auto userData = lua.CreateUserdata<Mother>(foo);

		userData.Bind("reset", &Mother::resetPie);
		//userData.Bind("getPie", &Mother::getPie); // throws error because it isn't allowed type (primitive)

		return userData;
	}, lua));
	LuaTable motherObject = lua.CreateTable();
	motherObject.Set("new", newFoo);
	lua.GetGlobalEnvironment().Set("Mother", motherObject);
	std::string err = lua.RunScript(""
		/* trying out mother object
			"local a = Mother:new();\n"
			"a.getPie().make()\n"
			"a.getPie().eat()\n"
			"a.getPie().make()\n"
			"cPrint(a.getPie().getPie());\n"
			"a.reset();\n"
			"cPrint(a.getPie().getPie());\n"
			"\n"
		/*/ // testing pie object, WORKS!
			"local a = Pie:new()\n"
			"local b = Pie:new()\n"
			"a.make();\n"
			"b.make();\n"
			"a.make();\n"
			"b.make();\n"
			"a.make();\n"
			"b.make();\n"
			"a.eat()\n"
			"cPrint(a.get())\n"
			"cPrint(b.get())\n"
		//*/
		"");
	std::cout << err << std::endl;
}