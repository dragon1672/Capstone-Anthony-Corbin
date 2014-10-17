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
//convert glm test
/*
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
		""
		""
		"context = {}\n"
		"context.asdf = 5;\n"
		"context.start = function()\n"
		"	print(context.asdf)\n"
		"	return true\n"
		"end\n"
		"context.earlyUpdate = function()\n"
		"	context.asdf = context.asdf+ 1;\n"
		"	return true\n"
		"end\n"
		"context.update = function()\n"
		"	print(context.asdf)\n"
		"	return true\n"
		"end\n"
		"context.lateUpdate = function()\n"
		"	--yay\n"
		"	return true\n"
		"end\n"
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
	auto obj = lua.GetGlobalEnvironment().Get<LuaTable>("context");
	obj.Get<LuaFunction<bool()>>("start").Invoke();

	//same script run 2nd time
	lua.RunScript(""
		""
		"context = {}\n"
		"context.asdf = 5;\n"
		"context.start = function()\n"
		"	print(context.asdf)\n"
		"	return true\n"
		"end\n"
		"context.earlyUpdate = function()\n"
		"	context.asdf = context.asdf+ 1;\n"
		"	return true\n"
		"end\n"
		"context.update = function()\n"
		"	print(context.asdf)\n"
		"	return true\n"
		"end\n"
		"context.lateUpdate = function()\n"
		"	--yay\n"
		"	return true\n"
		"end\n"
		""
		"");
	auto obj2 = lua.GetGlobalEnvironment().Get<LuaTable>("context");
	obj2.Get<LuaFunction<bool()>>("start").Invoke();
	for (int i=0;i<2;i++)
	{
		obj.Get<LuaFunction<bool()>>("earlyUpdate").Invoke();
		obj2.Get<LuaFunction<bool()>>("earlyUpdate").Invoke();

		obj.Get<LuaFunction<bool()>>("update").Invoke();
		obj2.Get<LuaFunction<bool()>>("update").Invoke();

		obj.Get<LuaFunction<bool()>>("lateUpdate").Invoke();
		obj2.Get<LuaFunction<bool()>>("lateUpdate").Invoke();
	}
	//std::cout << lua.GetGlobalEnvironment().Get<float>("b") << std::endl;
}
//*/
//convert glm cont
//*
#include <luacppinterface.h>
Lua LoadLua() {
	Lua lua;
	lua.LoadStandardLibraries();
	return lua;
}

Lua lua = LoadLua();

union Vec3 {
	float vals[3];
	struct {
		float r;
		float g;
		float b;
	};
	struct {
		float x;
		float y;
		float z;
	};
};

template <typename T>
class PropertyGet {
private:
	PropertyGet(const PropertyGet<T>&) {}
protected:
	T val;
	PropertyGet & operator=(const PropertyGet<T>& that) { val = that; return *this; }
public:
	PropertyGet() : val(0) {};
	explicit PropertyGet(float a) : val(a) {};
	T get() { return val; }
	inline operator T() const { return val; }
};

template <typename T>
class PropertyGetSet : public PropertyGet<T> {
public:
	PropertyGetSet() : PropertyGet() {};
	PropertyGetSet(float a) : PropertyGet(a) {};
	PropertyGetSet(const PropertyGet<T>& that) { val = that; }

	PropertyGetSet & operator=(const PropertyGet<T>& that) { val = that; return *this; }
	void set(T toSet) { val = toSet; }
	T& get() { return val; }
	inline PropertyGetSet& operator+=(const PropertyGet<T>& that){ (this->val += that); return *this; }
	inline PropertyGetSet& operator-=(const PropertyGet<T>& that){ (this->val -= that); return *this; }
	inline PropertyGetSet& operator*=(const PropertyGet<T>& that){ (this->val *= that); return *this; }
	inline PropertyGetSet& operator/=(const PropertyGet<T>& that){ (this->val /= that); return *this; }
	inline operator const PropertyGet<T>&() const { return *this; }
};

#define MAKE_LUA_INSTANCE_RET(class_name,varname) LuaUserdata<class_name> varname = lua.CreateUserdata<class_name>((##class_name##*)this,[](class_name*){}) //this disables the destructor call

#define GET_LUA_VER_PTR(var_type,real_var) LuaUserdata<var_type>get_lua_##real_var##() { return (LuaUserdata<var_type>)(*real_var); }
#define GET_LUA_VER(var_type,real_var) LuaUserdata<var_type>get_lua_##real_var##() { return (LuaUserdata<var_type>)real_var; }
#define BIND_LUA_VER(class_name,var_name,real_var) var_name.Bind(#real_var,  &##class_name##::get_lua_##real_var  )

class BoundFloat {
	float& ref;
public:
	BoundFloat(float& toWatch) : ref(toWatch) {}
	BoundFloat(float * toWatch) : ref(*toWatch) {}
	BoundFloat& operator=(const float& that) { ref = that; return *this; }
	inline operator float&() { return ref; }
};



class WrapVec3 {
private:
	Vec3 back;
public:
	union {
		struct {
			BoundFloat x;
			BoundFloat y;
			BoundFloat z;
		};
		struct {
			BoundFloat r;
			BoundFloat g;
			BoundFloat b;
		};
	};
	WrapVec3() :
		x(back.x),	r(back.x),
		y(back.y),	g(back.y),
		z(back.z),	b(back.z)
	{}
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	void  setX(float x) { this->x = x; }
	void  setY(float y) { this->y = y; }
	void  setZ(float z) { this->z = z; }
	void  set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline operator Vec3&() { return back; }

	inline operator LuaUserdata<WrapVec3>() {
		MAKE_LUA_INSTANCE_RET(WrapVec3,ret);
		ret.Bind("setX",&WrapVec3::setX);
		ret.Bind("setY",&WrapVec3::setY);
		ret.Bind("setZ",&WrapVec3::setZ);
		ret.Bind("getX",&WrapVec3::getX);
		ret.Bind("getY",&WrapVec3::getY);
		ret.Bind("getZ",&WrapVec3::getZ);
		ret.Bind("set", &WrapVec3::set);

		return ret;
	}
};

class MatrixInfo {
public:
	WrapVec3 pos;
	WrapVec3 scale;
	WrapVec3 rot;

	GET_LUA_VER(WrapVec3,pos  );
	GET_LUA_VER(WrapVec3,scale);
	GET_LUA_VER(WrapVec3,rot  );

	inline operator LuaUserdata<MatrixInfo>() {
		MAKE_LUA_INSTANCE_RET(MatrixInfo,ret);

		BIND_LUA_VER(MatrixInfo,ret,pos  ); // myObj.pos()
		BIND_LUA_VER(MatrixInfo,ret,scale);
		BIND_LUA_VER(MatrixInfo,ret,rot  );

		return ret;
	}
};

class Entity {
public:
	MatrixInfo transform;
	GET_LUA_VER(MatrixInfo,transform);

	inline operator LuaUserdata<Entity>() {
		MAKE_LUA_INSTANCE_RET(Entity,ret);
		BIND_LUA_VER(Entity,ret,transform);
		return ret;
	}
	inline operator float() {
		return 5;
	}
};

class Component {
public:
	Entity * parent;
	GET_LUA_VER_PTR(Entity,parent);

	inline operator LuaUserdata<Component>() {
		MAKE_LUA_INSTANCE_RET(Component,ret);
		BIND_LUA_VER(Component,ret,parent);
		return ret;
	}
	
};

void main() {
	Component t;
	t.parent = new Entity();
	float pie = *t.parent;

	t.parent->transform.pos.x = 5;
	Vec3 test = t.parent->transform.pos;
	std:: cout << test.x << std::endl;

	std::cout << "+-----------+" << std::endl;
	std::cout << "| Lua Start |" << std::endl;
	std::cout << "+-----------+" << std::endl;
	lua.GetGlobalEnvironment().Set("t",(LuaUserdata<Component>)t);
	auto err = lua.RunScript(""
		"print('get:' .. t.parent().transform().pos().getX());                  \n"
		"t.parent().transform().pos().setX(6);                  \n"
		"print('get:' .. t.parent().transform().pos().getX());                  \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"                                        \n"
		"");
	std::cout << "+-------------------" << std::endl;
	std::cout << "| Lua End: " << err << std::endl;
	std::cout << "+-------------------" << std::endl;
	std::cout << t.parent->transform.pos.x << std::endl;


	std::cout << std::endl << std::endl << std::endl << sizeof(Vec3) << std::endl;
}
//*/