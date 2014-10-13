#include <iostream>
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <luacppinterface.h>
#pragma  warning( pop )
/*
	Lua lua;
	bool LoadLua() {
		lua.LoadStandardLibraries();
		return true;
	}
	bool luaLoaded = LoadLua();

class Pie {
public:
	int pie;
	void makeMorePie() {this->pie++;}
	void eatPie() {this->pie--;}
	int getPie() { return pie; }

	LuaUserdata<Pie> getLuaInstance() {
		auto ret = lua.CreateUserdata<Pie>(this);
		ret.Bind("eat",  &Pie::eatPie);
		ret.Bind("make", &Pie::makeMorePie);
		ret.Bind("get",  &Pie::getPie);
		return ret;
	}
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
	LuaUserdata<Pie> getPieLua() {
		return supply.getLuaInstance();
	}
};

void main() {
	std::cout << "Hello World" << std::endl;
	
	
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
	//make variable
	lua.GetGlobalEnvironment().Set("Pie", pieObj);
	
	auto newFoo = lua.CreateFunction<LuaUserdata<Mother>()>(std::bind([](Lua lua) -> LuaUserdata<Mother> {
		auto foo = new Mother();
		auto userData = lua.CreateUserdata<Mother>(foo);

		userData.Bind("reset", &Mother::resetPie);
		userData.Bind("getPie", &Mother::getPieLua);

		return userData;
	}, lua));
	LuaTable motherObject = lua.CreateTable();
	motherObject.Set("new", newFoo);
	lua.GetGlobalEnvironment().Set("Mother", motherObject);
	std::string err = lua.RunScript(""
		//* trying out mother object
			"local a = Mother:new();\n"
			"cPrint('yus1')\n"
			"a.getPie().make()\n"
			"cPrint('yus2')\n"
			"a.getPie().eat()\n"
			"cPrint('yus3')\n"
			"a.getPie().make()\n"
			"cPrint('yus4')\n"
			"cPrint(a.getPie().get());\n"
			"cPrint('yus5')\n"
			"a.reset();\n"
			"cPrint('yus6')\n"
			"cPrint(a.getPie().get());\n"
			"cPrint('yus7')\n"
			"\n"
			"local aP = Pie:new()\n"
			"local bP = Pie:new()\n"
			"aP.make();\n"
			"bP.make();\n"
			"aP.make();\n"
			"bP.make();\n"
			"aP.make();\n"
			"bP.make();\n"
			"aP.eat()\n"
			"cPrint(aP.get())\n"
			"cPrint(bP.get())\n"
		"");
	std::cout << err << std::endl;
}
//*/

Lua lua;
bool LoadLua() {
	lua.LoadStandardLibraries();
	return true;
}
bool luaLoaded = LoadLua();

//represents class from external lib that I cannot edit
class Vec {
public:
	float x,y,z;
};

//binding class to wrap not sure how else to approach this
//since I need getters and setters
class LuaVec {
	Vec& base;
public:
	LuaVec(Vec& base) : base(base) { }
	float getX() { return base.x; }
	float getY() { return base.y; }
	float getZ() { return base.z; }

	void setX(float toSet) { base.x = toSet; }
	void setY(float toSet) { base.y = toSet; }
	void setZ(float toSet) { base.z = toSet; }
};

LuaUserdata<LuaVec> convertVec(Vec& toConvert) {
	auto ret = lua.CreateUserdata<LuaVec>(new LuaVec(toConvert));
	
	ret.Bind("getX",&LuaVec::getX);
	ret.Bind("getY",&LuaVec::getY);
	ret.Bind("getZ",&LuaVec::getZ);

	ret.Bind("setX",&LuaVec::setX);
	ret.Bind("setY",&LuaVec::setY);
	ret.Bind("setZ",&LuaVec::setZ);

	return ret;
}
class Matrix {
public:
	Vec pos;
	Vec scale;
	Vec rot;
public:
	Vec& getPos()   { return pos;   }
	LuaUserdata<LuaVec> getLuaPos()   { return convertVec(pos); }
	Vec& getScale() { return scale; }
	LuaUserdata<LuaVec> getLuaScale() { return convertVec(scale); }
	Vec& getRot()   { return rot;   }
	LuaUserdata<LuaVec> getLuaRot()   { return convertVec(rot); }

	static LuaUserdata<Matrix> getLuaVersion(Matrix& toConvert) {
		auto ret = lua.CreateUserdata<Matrix>(&toConvert);
		ret.Bind("getPos",  &Matrix::getLuaPos);
		ret.Bind("getScale",&Matrix::getLuaScale);
		ret.Bind("getRot",  &Matrix::getLuaRot);
		return ret;
	}
};


class Entity {
public:
	Matrix mat;
	Matrix& getMat() { return mat; }
	LuaUserdata<Matrix> getLuaMat() { return Matrix::getLuaVersion(mat); }
	static LuaUserdata<Entity> getLuaVersion(Entity& toConvert) {
		return getLuaVersion(&toConvert);
	}
	static LuaUserdata<Entity> getLuaVersion(Entity * toConvert) {
		auto ret = lua.CreateUserdata<Entity>(toConvert);
		ret.Bind("getMat", &Entity::getLuaMat);
		return ret;
	}
	static void registerToLua() {
		auto constructor = lua.CreateFunction<LuaUserdata<Entity>()>(std::bind([](Lua lua) -> LuaUserdata<Entity> {
			return Entity::getLuaVersion(new Entity());
		}, lua));
		LuaTable classObj = lua.CreateTable();
		classObj.Set("new", constructor);
		lua.GetGlobalEnvironment().Set("Entity", classObj);
	}
};

void main() {
	Entity::registerToLua();

	Entity a;

	lua.GetGlobalEnvironment().Set("a",Entity::getLuaVersion(a));

	std::string err = lua.RunScript(""
		//"local a = Entity:new();\n"
		//"print(a.getMat().getPos())\n"
		//"print(a.getMat().getPos().getX())\n"
		"a.getMat().getPos().setX(5)\n"
		//"print(a.getMat().getPos().getX())\n"
		"local b = 5;\n"
		"function addB(a)\n"
		"b = b + 1;\n"
		"  return a+b\n"
		"end\n"

		"function ScriptComponent()			\n"
		"return {							\n"
		"	start = function()				\n"
		"		--fill me!!!!				\n"
		"		end							\n"
		"		earlyUpdate = function()	\n"
		"		--fill me!!!!				\n"
		"		end							\n"
		"		update = function()			\n"
		"		--fill me!!!!				\n"
		"		end							\n"
		"		lateupdate = function()		\n"
		"		--fill me!!!!				\n"
		"		end							\n"
		"}									\n"
		"end								\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"");
	std::cout << err << std::endl;
	//auto test = lua.GetGlobalEnvironment().Get<float>("a.getMat().getPos().getX()");
	//auto test = lua.GetGlobalEnvironment().Get<float>("b");
	//std::cout << test << std::endl;
	//std::cout << a.mat.pos.x << std::endl;
	//std::cout << lua.GetGlobalEnvironment().Get<float>("player.pos.X") << std::endl;
	std::cout << lua.GetGlobalEnvironment().Get< LuaFunction<int(int)> >("addB").Invoke(4) << std::endl;
	std::cout << lua.GetGlobalEnvironment().Get< LuaFunction<int(int)> >("addB").Invoke(4) << std::endl;
	//std::cout << lua.GetGlobalEnvironment().Get<float>("b") << std::endl;
}