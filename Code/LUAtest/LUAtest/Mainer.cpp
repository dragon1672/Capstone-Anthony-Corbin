#include <iostream>
#include <luacppinterface.h>
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

class Vec {
public:
	float x,y,z;
};
class LuaVec : public Vec {
public:
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

	float setX(float toSet) { x = toSet; }
	float setY(float toSet) { y = toSet; }
	float setZ(float toSet) { z = toSet; }
};

LuaUserdata<Vec> convertVec(Vec& toConvert) {
	auto ret = lua.CreateUserdata<Vec>(&toConvert);
	
	ret.Bind("x",&Vec::x);

	return ret;
}
class Matrix {
public:
	Vec pos;
	Vec scale;
	Vec rot;
public:
	Vec& getPos()   { return pos;   }
	LuaUserdata<Vec> getLuaPos()   { return convertVec(pos); }
	Vec& getScale() { return scale; }
	LuaUserdata<Vec> getLuaScale() { return convertVec(scale); }
	Vec& getRot()   { return rot;   }
	LuaUserdata<Vec> getLuaRot()   { return convertVec(rot); }
};
LuaUserdata<Matrix> convertMat(Matrix& toConvert) {
	auto ret = lua.CreateUserdata<Matrix>(&toConvert);
	ret.Bind("getPos",  &Matrix::getLuaPos);
	ret.Bind("getScale",&Matrix::getLuaScale);
	ret.Bind("getRot",  &Matrix::getLuaRot);
	return ret;
}


class Entity {
public:
	Matrix mat;
	Matrix getMat();
	LuaUserdata<Matrix> getLuaMat() { return convertMat(mat); }
	static void registerToLua() {
		auto constructor = lua.CreateFunction<LuaUserdata<Entity>()>(std::bind([](Lua lua) -> LuaUserdata<Entity> {
			auto foo = new Entity();
			auto userData = lua.CreateUserdata<Entity>(foo);

			userData.Bind("getMat", &Entity::getLuaMat);

			return userData;
		}, lua));
		LuaTable classObj = lua.CreateTable();
		classObj.Set("new", constructor);
		lua.GetGlobalEnvironment().Set("Entity", classObj);
	}
};

void main() {
	Entity::registerToLua();

	std::string err = lua.RunScript(""
		"local a = Entity:new();\n"
		"print(a.getMat().getPos())\n"
		"print(a.getMat().getPos().x)\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"");
	std::cout << err << std::endl;
}