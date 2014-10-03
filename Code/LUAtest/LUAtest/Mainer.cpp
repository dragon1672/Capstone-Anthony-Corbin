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
	auto t3 = lua.CreateFunction<int(int,std::string)>([](int a, std::string) -> int
	{
		return 10;
	});
	
	lua.GetGlobalEnvironment().Set("cPrint",lua.CreateFunction<void(std::string)>([](std::string a) -> void { std::cout << "cPrint:" << a << std::endl; })); // make and add function
	/*
	std::cout << t3.Invoke(5,"a") << std::endl;
	lua.RunScript(""
		"cPrint('before sleep');\n"
		"os.execute('sleep 1')\n"
		"cPrint('after sleep');\n"
		"\n"
	"");
	
	std::cout << t3.Invoke(5,"a") << std::endl;
	//*/





	
	auto newPie = lua.CreateFunction<LuaUserdata<Pie>()>(std::bind([](Lua lua) -> LuaUserdata<Pie> {
		auto pie = new Pie();
		auto pieData = lua.CreateUserdata<Pie>(pie);

		pieData.Bind("eat",  &Pie::eatPie);
		pieData.Bind("make", &Pie::makeMorePie);
		pieData.Bind("get",  &Pie::getPie);

		return pieData;
	}, lua));
	auto pieObj = lua.CreateTable();
	//const
	pieObj.Set("new", newPie);
	//make varable
	lua.GetGlobalEnvironment().Set("Pie", pieObj);
	
	auto newFoo = lua.CreateFunction<LuaUserdata<Mother>()>(std::bind([](Lua lua) -> LuaUserdata<Mother> {
		auto foo = new Mother();
		auto userData = lua.CreateUserdata<Mother>(foo);

		//register functions
		userData.Bind("reset", &Mother::resetPie);
		//userData.Bind("getPie", &Mother::getPie);

		return userData;
	}, lua));
	auto footable = lua.CreateTable();
	//const
	footable.Set("new", newFoo);
	//make varable
	lua.GetGlobalEnvironment().Set("Mother", footable);
	std::cout << 
	lua.RunScript(""
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
}