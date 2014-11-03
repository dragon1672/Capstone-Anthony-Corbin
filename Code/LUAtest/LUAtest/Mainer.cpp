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
		"context.pie = 9;\n"
		"context.start = function()\n"
		"	print(context.pie)\n"
		"	return true\n"
		"end\n"
		"context.earlyUpdate = function()\n"
		"	context.pie = context.pie+ 1;\n"
		"	return true\n"
		"end\n"
		"context.update = function()\n"
		"	print(context.pie)\n"
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
/*
#include <luacppinterface.h>
Lua LoadLua() {
	Lua lua;
	lua.LoadStandardLibraries();
	return lua;
}

Lua lua = LoadLua();

class glm::vec3 {
public:
	union {
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



#define LUA_VECTOR_MAKE_GETTER_SETTER(Uppercase,LowerCase)\
	inline float get##Uppercase##() { return LowerCase; }                 \
	inline void  set##Uppercase##(float LowerCase) { this->LowerCase = LowerCase; }
#define LUA_STRING(x) #x
#define LUA_GLEW(a,b) LUA_STRING(a##b)
#define LUA_VECTOR_BIND(vec_type,var_name,UppercaseLetter)\
	var_name.Bind(LUA_GLEW(set,UppercaseLetter),&##vec_type##::set##UppercaseLetter##);\
	var_name.Bind(LUA_GLEW(get,UppercaseLetter),&##vec_type##::get##UppercaseLetter##)

namespace wrap {

	class vec3 : public glm::vec3 {
	public:
		vec3() { }
		vec3(glm::vec3& that) { set(that); }
		inline operator glm::vec3&() { return *reinterpret_cast<glm::vec3*>(this); }
		vec3& operator=(const glm::vec3& that) { set(that); return *this; }
		LUA_VECTOR_MAKE_GETTER_SETTER(X,x);
		LUA_VECTOR_MAKE_GETTER_SETTER(Y,y);
		LUA_VECTOR_MAKE_GETTER_SETTER(Z,z);

		LUA_VECTOR_MAKE_GETTER_SETTER(R,r);
		LUA_VECTOR_MAKE_GETTER_SETTER(G,g);
		LUA_VECTOR_MAKE_GETTER_SETTER(B,b);
		void  set(const glm::vec3& that) {
			this->x = that.x;
			this->y = that.y;
			this->z = that.z;
		}
		void  set(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		inline operator LuaUserdata<vec3>() {
			MAKE_LUA_INSTANCE_RET(vec3,ret);
			LUA_VECTOR_BIND(vec3,ret,X);
			LUA_VECTOR_BIND(vec3,ret,Y);
			LUA_VECTOR_BIND(vec3,ret,Z);

			LUA_VECTOR_BIND(vec3,ret,R);
			LUA_VECTOR_BIND(vec3,ret,G);
			LUA_VECTOR_BIND(vec3,ret,B);

			return ret;
		}
	};
}

class MatrixInfo {
public:
	wrap::vec3 pos;
	wrap::vec3 scale;
	wrap::vec3 rot;

	GET_LUA_VER(wrap::vec3,pos  );
	GET_LUA_VER(wrap::vec3,scale);
	GET_LUA_VER(wrap::vec3,rot  );

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

	MatrixInfo * getMat() {
		return &transform;
	}
	

	inline operator LuaUserdata<Entity>() {
		MAKE_LUA_INSTANCE_RET(Entity,ret);
		BIND_LUA_VER(Entity,ret,transform);
		return ret;
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

void testingPram(glm::vec3& in) {
	in.x = 5;
}

void main() {
	Component t;
	t.parent = new Entity();
	//float pie = *t.parent;

	t.parent->transform.pos.x = 5;
	glm::vec3 test = t.parent->transform.pos;
	std:: cout << test.x << std::endl;

	testingPram(t.parent->transform.pos);

	std::cout << "+-----------+" << std::endl;
	std::cout << "| Lua Start |" << std::endl;
	std::cout << "+-----------+" << std::endl;
	lua.GetGlobalEnvironment().Set("t",(LuaUserdata<Entity>)*t.parent);
	auto err = lua.RunScript(""
		"print(t.transform().pos().getX());                  \n"
		"t.transform().pos().setX(6);                  \n"
		"print(t.transform().pos().getX());                  \n"
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


	glm::vec3 pickle;
	pickle.y = 5;
	t.parent->transform.pos = pickle;

	std::cout << std::endl << std::endl << std::endl << sizeof(glm::vec3) << std::endl;
}
//*/
//inplace new

#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW   new( _CLIENT_BLOCK, __FILE__, __LINE__)

#define new DEBUG_NEW

#endif // _DEBUG

#include <luacppinterface.h>

#pragma region singleton

//Place this in the class header, I prefer the first line
#define DEFINE_SINGLETON(class_name) \
	private: static class_name * _instance; \
	public: static class_name##& getInstance(); \
	public: static void delInstance(); \
	private:

//place in a CPP file
#define IMPLEMENT_SINGLETON(class_name) \
	class_name * class_name##::_instance = nullptr; \
	class_name##& class_name##::getInstance() { return *(_instance == nullptr ? _instance = new class_name() : _instance); } \
	void class_name##::delInstance() { if(_instance != nullptr) delete _instance; _instance = nullptr; }

#define DEFINE_SINGLETON_CAST(class_name,type_cast) \
	private: static class_name * _instance; \
	public: static type_cast##& getInstance(); \
	public: static void delInstance(); \
	private:

//place in a CPP file
#define IMPLEMENT_SINGLETON_CAST(class_name,type_cast) \
	class_name * class_name##::_instance = nullptr; \
	type_cast##& class_name##::getInstance() { return *(type_cast*)(_instance == nullptr ? _instance = new class_name() : _instance); } \
	void class_name##::delInstance() { if(_instance == nullptr) delete _instance; _instance = nullptr; }

#pragma endregion

#pragma region masterLua
#define LUA_INSTANCE MasterLua::getInstance().lua

class MasterLua {
public:
	Lua lua;
	MasterLua();
	DEFINE_SINGLETON(MasterLua);
};

#include <string>
#include <iostream>

//singletons

IMPLEMENT_SINGLETON(MasterLua);

MasterLua::MasterLua()
{
	lua.LoadStandardLibraries();

	lua.GetGlobalEnvironment().Set("print",lua.CreateFunction<void(std::string)>([](std::string a) -> void { std::cout << "LUA Print:" << a << std::endl; })); // make and add function

	//load class def
	lua.RunScript("function class(a,b)local c={}if not b and type(a)=='function'then b=a;a=nil elseif type(a)=='table'then for d,e in pairs(a)do c[d]=e end;c._base=a end;"
		"c.__index=c;local f={}f.__call=function(g,...)local h={}setmetatable(h,c)if b then b(h,...)else if a and a.init then a.init(h,...)end end;return h end;c.init=b;"
		"c.is_a=function(i,j)local k=getmetatable(i)while k do if k==j then return true end;k=k._base end;return false end;setmetatable(c,f)return c end");
}


#pragma endregion


#pragma region luaDefines


//------------------------------------------Notes------------------------------------------
// These functions are made to make it easier to bind complex objects in LUA
// ASSUMES THAT: operator LuaUserdata<type> has been created for each object
// all conversions happen by casting
//
// THAT MEANS
// you should use the function generators at a class level
// you should use the binding defines in operator LuaUserdata<type> function
//-----------------------------------------------------------------------------------------

#include <iostream>



//----------------core tools-----------------
// Used to make hacks and string magic work
//-------------------------------------------
//makes pram a string returning "x"
#define LUA_STRING(x) #x
#define LUA_GLEW(a,b) LUA_STRING(a##b)



#define LUA_OBJECT(class_name)									\
	struct LUA_HOLDER {											\
		LuaUserdata<class_name> val;							\
		LUA_HOLDER(LuaUserdata<class_name> init) : val(init) {}	\
	};															\
	LUA_HOLDER * LUA_HOLDER_INSTANCE

#define LUA_OBJECT_START(class_name) \
	LUA_HOLDER_INSTANCE = nullptr

#define LUA_OBJECT_END(class_name) \
	if(LUA_HOLDER_INSTANCE!=nullptr) { delete LUA_HOLDER_INSTANCE; std::cout << "del lua " << #class_name << std::endl; }

#define MAKE_DEFAULT_LUA_CONST_AND_DEST(class_name) class_name##() { LUA_OBJECT_START(class_name); } ~##class_name##() { LUA_OBJECT_END(class_name); }

//This creates a LuaUserData for your class
//it overrides the default destructor that calls delete since we are pointing to "this"
#define MAKE_LUA_INSTANCE_RET(class_name,Userdata_var_name)\
	if(LUA_HOLDER_INSTANCE == nullptr) { LUA_HOLDER_INSTANCE = new LUA_HOLDER(LUA_INSTANCE.CreateUserdata<class_name>((##class_name##*)this,[](class_name*){}));} else { return LUA_HOLDER_INSTANCE->val; } \
	LuaUserdata<class_name>& Userdata_var_name = LUA_HOLDER_INSTANCE->val;



//---------------------------overloading functions with lua counterparts---------------------------
// the following defines create functions that cast LuaUserdata version of variables and functions
// example:
// creates LuaUserdata<varType> get_lua_myAwesomeVar() { [casting magic] }
//-------------------------------------------------------------------------------------------------

//creates a getX() and setX(float)
#define LUA_VECTOR_MAKE_GETTER_SETTER(Uppercase,LowerCase)\
	inline float get##Uppercase##() { return LowerCase; }                 \
	inline void  set##Uppercase##(float LowerCase) { this->LowerCase = LowerCase; }

//creates LuaUserData function for pointer
//it will de-ref and cast as LuaUserdata
#define GET_LUA_VER_PTR(var_type,real_var) LuaUserdata<var_type>get_lua_##real_var##() { return (LuaUserdata<var_type>)(*real_var); }
//casts and returns LuaUserdata version
#define GET_LUA_VER(var_type,real_var) LuaUserdata<var_type>get_lua_##real_var##() { return (LuaUserdata<var_type>)real_var; }

//calls function de-refs and casts as LuaUserdata
#define LUA_GET_FUN_PTR(var_type,fun_name) LuaUserdata<##var_type##> fun_name##_LUA() { return (LuaUserdata<##var_type##>)*##fun_name##(); }

#define LUA_GET_FUN(var_type,fun_name) LuaUserdata<##var_type##> fun_name##_LUA() { return (LuaUserdata<##var_type##>)##fun_name##(); }



//---------------------------Bind overloaded functions into lua---------------------------
// the following defines bind into lua using original variable names
// example:
// creates bind("myAwesomeVar",get_lua_myAwesomeVar)
// call in lua: myAwesomeVar()
//----------------------------------------------------------------------------------------


//binds get_lua_var into lua
#define BIND_LUA_VER(class_name,Userdata_var_name,real_var) Userdata_var_name.Bind(#real_var,  &##class_name##::get_lua_##real_var  )

//binds getX and setX in LUA
#define LUA_VECTOR_BIND(vec_type,Userdata_var_name,UppercaseLetter)\
	Userdata_var_name.Bind(LUA_GLEW(set,UppercaseLetter),&##vec_type##::set##UppercaseLetter##);\
	Userdata_var_name.Bind(LUA_GLEW(get,UppercaseLetter),&##vec_type##::get##UppercaseLetter##)
//binds function 
#define LUA_BIND_FUN(class_name,Userdata_var_name,fun_name) Userdata_var_name.Bind(#fun_name,&##class_name##::##fun_name##_LUA)

#pragma endregion

namespace glm {
	class vec3 {
	public:
		union {
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
	};
}

namespace wrap {

	class vec3 : public glm::vec3 {
		LUA_OBJECT(vec3);
	public:
		vec3() { LUA_OBJECT_START(vec3); }
		vec3(glm::vec3& that) { set(that); LUA_OBJECT_START(vec3); }
		~vec3() { LUA_OBJECT_END(vec3); }
		inline operator glm::vec3&() { return *reinterpret_cast<glm::vec3*>(this); }
		vec3& operator=(const glm::vec3& that) { set(that); return *this; }
		LUA_VECTOR_MAKE_GETTER_SETTER(X,x);
		LUA_VECTOR_MAKE_GETTER_SETTER(Y,y);
		LUA_VECTOR_MAKE_GETTER_SETTER(Z,z);

		LUA_VECTOR_MAKE_GETTER_SETTER(R,r);
		LUA_VECTOR_MAKE_GETTER_SETTER(G,g);
		LUA_VECTOR_MAKE_GETTER_SETTER(B,b);
		void  set(const glm::vec3& that) {
			this->x = that.x;
			this->y = that.y;
			this->z = that.z;
		}
		void  set(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		inline operator LuaUserdata<vec3>&() {
			MAKE_LUA_INSTANCE_RET(vec3,ret);
			LUA_VECTOR_BIND(vec3,ret,X);
			LUA_VECTOR_BIND(vec3,ret,Y);
			LUA_VECTOR_BIND(vec3,ret,Z);

			LUA_VECTOR_BIND(vec3,ret,R);
			LUA_VECTOR_BIND(vec3,ret,G);
			LUA_VECTOR_BIND(vec3,ret,B);

			return ret;
		}
	};
}

class MatrixInfo {
	LUA_OBJECT(MatrixInfo);
public:
	MatrixInfo() { LUA_OBJECT_START(MatrixInfo); }
	~MatrixInfo() { LUA_OBJECT_END(MatrixInfo); }
	wrap::vec3 pos;
	wrap::vec3 scale;
	wrap::vec3 rot;

	GET_LUA_VER(wrap::vec3,pos  );
	GET_LUA_VER(wrap::vec3,scale);
	GET_LUA_VER(wrap::vec3,rot  );

	inline operator LuaUserdata<MatrixInfo>() {
		MAKE_LUA_INSTANCE_RET(MatrixInfo,ret);

		BIND_LUA_VER(MatrixInfo,ret,pos  ); // myObj.pos()
		BIND_LUA_VER(MatrixInfo,ret,scale);
		BIND_LUA_VER(MatrixInfo,ret,rot  );

		return ret;
	}
};

class Entity {
	LUA_OBJECT(Entity);
public:
	MAKE_DEFAULT_LUA_CONST_AND_DEST(Entity);
	MatrixInfo transform;
	GET_LUA_VER(MatrixInfo,transform);

	MatrixInfo * getMat() {
		return &transform;
	}


	inline operator LuaUserdata<Entity>&() {
		MAKE_LUA_INSTANCE_RET(Entity,ret);
		BIND_LUA_VER(Entity,ret,transform);
		return ret;
	}

};
class Component {
	LUA_OBJECT(Component);
public:
	MAKE_DEFAULT_LUA_CONST_AND_DEST(Component);

	Entity * parent;
	GET_LUA_VER_PTR(Entity,parent);

	inline operator LuaUserdata<Component>&() {
		MAKE_LUA_INSTANCE_RET(Component,ret);
		BIND_LUA_VER(Component,ret,parent);
		return ret;
	}

};

void testingPram(glm::vec3& in) {
	in.x = 5;
}

void runLua(std::string toRun) {
	std::string err = LUA_INSTANCE.RunScript(toRun);
	if(err != Lua::NO_ERRORS) {
		std::cout << err << std::endl;
	}
}


void main() {
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	LUA_INSTANCE;
	{
		Component t;
		Entity e;
		t.parent = &e;

		t.parent->transform.pos.x = 5;
		glm::vec3 test = t.parent->transform.pos;
		std::cout << test.x << std::endl;

		testingPram(t.parent->transform.pos);

		std::cout << "+-----------+" << std::endl;
		std::cout << "| Lua Start |" << std::endl;
		std::cout << "+-----------+" << std::endl;
		LUA_INSTANCE.GetGlobalEnvironment().Set("t",(LuaUserdata<Entity>)*t.parent);

		runLua("a = 1;");
		runLua("b = a + 1;");
		runLua("print(a+b);");

		for (int i = 0; i < 10; i++) {
			auto err = LUA_INSTANCE.RunScript(""
				//"print(t.transform().pos().getX());                  \n"
				"t.transform().pos().setX(6);                  \n"
				//"print(t.transform().pos().getX());                  \n"
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
			//std::cout << "+-------------------" << std::endl;
			//std::cout << "| Lua End: " << err << std::endl;
			//std::cout << "+-------------------" << std::endl;
			//std::cout << t.parent->transform.pos.x << std::endl;
		}
		//*/

		glm::vec3 pickle;
		pickle.y = 5;
		t.parent->transform.pos = pickle;

		MasterLua::delInstance();
	}

	//std::cout << std::endl << std::endl << std::endl << sizeof(glm::vec3) << std::endl;
}