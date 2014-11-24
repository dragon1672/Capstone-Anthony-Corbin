#include <QtWidgets/QApplication>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/DebugTools/DebugMemHeader.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Entity/Entity.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Entity/Component.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Entity/Components/ScriptComponent.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Tools/MatrixInfo.h>
#include <../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/../Engine/Engine/Systems/GameObjectManager.h>

#include <CorbinGui/BasicQGLGui.h>
#include <CorbinGui/GuiSkellyTon.h>

#include <ShapeGenerator.h>
#include <Engine/Systems/Resources/Shaders/DefaultShaders.h>
#include <Engine/DebugTools/DebugMemHeader.h>

#include <Engine/Systems/GameObjectManager.h>

#include <Engine/IO/FileIO.h>

int main(int argc, char * argv[]) {
	int ret = -1;
	QApplication app(argc, argv);
	GuiSkellyTon gui;

#pragma region keyboard controller
	auto keyBoardController = resourceManager.addScript_src(Script::getScriptComponentTemplate("KeyboardController",
		//start
		"    self.KeySpeed = 9                                                      \n"
		"    self.MouseSpeed = .5                                                   \n"
		"",
		//update
		"    if(Input.getKeyDown(KeyCode.R)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getY();                    \n"
		"       tmp = tmp - Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setY(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.W)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getZ();                    \n"
		"       tmp = tmp + Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setZ(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.S)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getZ();                    \n"
		"       tmp = tmp - Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setZ(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.F)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getY();                    \n"
		"       tmp = tmp + Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setY(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.D)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getX();                    \n"
		"       tmp = tmp - Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setX(tmp);                             \n"
		"     end                                                                   \n"
		"														                    \n"
		"    if(Input.getKeyDown(KeyCode.A)) then                                   \n"
		"       local tmp = self.parent.getTrans().pos().getX();                    \n"
		"       tmp = tmp + Timer.deltaTime() * self.KeySpeed                       \n"
		"       self.parent.getTrans().pos().setX(tmp);                             \n"
		"     end                                                                   \n"
		"                                                                           \n"
		"    if(Input.getKeyDown(18)) then         \n"
		"       local x = self.parent.getTrans().rot().getX();                      \n"
		"       local y = self.parent.getTrans().rot().getY();                      \n"
		"       x = x + self.MouseSpeed * Input.getMouse().delta().getY()           \n"
		"       y = y - self.MouseSpeed * Input.getMouse().delta().getX()           \n"
		"       self.parent.getTrans().rot().setX(x);                               \n"
		"       self.parent.getTrans().rot().setY(y);                               \n"
		"    end                                                                    \n"
		"                                                                           \n"
		"                                                                           \n"
		""));
#pragma endregion

	auto BoardScript = resourceManager.addScript_src(Script::getScriptComponentTemplate("Board","self:initBoard(7,6);")+""
		"function Board:initBoard(boardWidth,boardHeight)         \n"
		"  self.width = boardWidth;                               \n"
		"  self.height = boardHeight                              \n"
		"  self._board = {} -- makin a board                      \n"
		"  for i=1,self.width do                                  \n"
		"    self._board[i] = {}                                  \n"
		"  end                                                    \n"
		"  self:clear();                                          \n"
		"end                                                      \n"
		"                                                         \n"
		"                                                         \n"
		"function Board:get (x,y)                                 \n"
		"  if(  not between(x,1,self.width)                       \n"
		"    or not between(y,1,self.height)) then                \n"
		"      return -1;                                         \n"
		"  end                                                    \n"
		"  return self._board[x][y];                              \n"
		"end;                                                     \n"
		"function Board:clear()                                   \n"
		"  for i=1,self.width do                                  \n"
		"      for j=1,self.height do                             \n"
		"        self._board[i][j] = 0                            \n"
		"      end                                                \n"
		"    end                                                  \n"
		"end                                                      \n"
		"                                                         \n"
		"                                                         \n"
		"function Board:set(x,y,val)                              \n"
		"  self._board[x][y] = val;                               \n"
		"end                                                      \n"
		"function Board:validMove(col)                            \n"
		"  if(col <= 0) then return false; end                    \n"
		"  for y=0,self.height do                                 \n"
		"    if(self:get(col,y)==0) then                          \n"
		"      return true;                                       \n"
		"    end                                                  \n"
		"  end                                                    \n"
		"  return false;                                          \n"
		"end                                                      \n"
		"                                                         \n"
		"                                                         \n"
		"                                                         \n"
		"function Board:winningMove(x,y)                          \n"
		"  local player = self:get(x,y);                          \n"
		"  local offset = List();                                 \n"
		"  offset:push(Vector2( 1, 0));                           \n"
		"  offset:push(Vector2( 0, 1));                           \n"
		"  offset:push(Vector2( 1, 1));                           \n"
		"  offset:push(Vector2( 1,-1));                           \n"
		"                                                         \n"
		"  for i=1,offset:getSize() do                            \n"
		"    local vec = offset:get(i)                            \n"
		"    local count = 0;                                     \n"
		"    for i=-3,3 do                                        \n"
		"      local posX = x + i * vec.x;                        \n"
		"      local posY = y + i * vec.y;                        \n"
		"      local tmp = self:get(posX,posY);                   \n"
		"      if (self:get(posX,posY)==player) then              \n"
		"        count = count + 1                                \n"
		"      else                                               \n"
		"        count = 0                                        \n"
		"      end                                                \n"
		"      if(count == 4) then                                \n"
		"        return true;                                     \n"
		"      end;                                               \n"
		"    end                                                  \n"
		"  end                                                    \n"
		"  return false;                                          \n"
		"end                                                      \n"
		"function Board:place(col,player)                         \n"
		"  for y=0,self.height do                                 \n"
		"    if(self:get(col,y)==0) then                          \n"
		"      self:set(col,y,player);                            \n"
		"      return self:winningMove(col,y);                    \n"
		"    end                                                  \n"
		"  end                                                    \n"
		"  return false;                                          \n"
		"end                                                      \n"
		"                                                         \n"
		"                                                         \n"
		"                                                         \n"
		"function Board:toString()                                \n"
		"  local ret = "";                                        \n"
		"    for j=self.height,1,-1 do                            \n"
		"      for i=1,self.width do                              \n"
		"        if(self._board[i][j] == 1) then                  \n"
		"          ret = ret .. 'X';                              \n"
		"        elseif(self._board[i][j] == 2) then              \n"
		"          ret = ret .. '0';                              \n"
		"        else                                             \n"
		"          ret = ret .. ' ';                              \n"
		"        end                                              \n"
		"        if(i~=self.width) then ret = ret ..'|' end       \n"
		"      end                                                \n"
		"      ret = ret .. '\n';                                 \n"
		"    end                                                  \n"
		"    for i=1,self.width do                                \n"
		"      ret = ret .. '-';                                  \n"
		"      if(i~=self.width) then ret = ret ..'+' end         \n"
		"    end                                                  \n"
		"    ret = ret .. '\n';                                   \n"
		"    for i=1,self.width do                                \n"
		"      ret = ret .. i;                                    \n"
		"      if(i~=self.width) then ret = ret ..'|' end         \n"
		"    end                                                  \n"
		"    return ret;                                          \n"
		"end                                                      \n"
		"");

	(void)BoardScript;

	resourceManager.WorkingDir("./../resources/");
	resourceManager.loadNeumontStuff();

	auto shader = resourceManager.getDefault<ShaderProgram>();

	auto dragon  = resourceManager.add2DTexture("dragon","flying_dragon_1.png");
	auto texture = resourceManager.add2DTexture("space","space.png");


	EditorGame * game = gui.Game();

	game->AddEntity("Bob");
	auto comp = game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getTrans()->pos.z =  20;
	game->currentEntity.getTrans()->pos.z =  -20;
	comp->Geo(resourceManager.getFirstMesh("NU_Cube"));
	comp->Shader(shader);
	comp->material.Diffuse(dragon);
	comp = game->currentEntity.addComponent<RenderableComponent>();
	comp->material.Diffuse(texture);
	auto sphere = resourceManager.duplicate(resourceManager.getFirstMesh("NU_Sphere"));
	sphere->scale(2,1,1);
	comp->Geo(sphere);
	comp->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.addScript_src(Script::getScriptComponentTemplate("rotator",//random
		"    self.rotSpeed = Random.RangeFloat(10,300);      \n"
		"    print('start')                                  \n"
		"    self.printInUpdate = true;                      \n"
		"    self.parent.Broadcast('update');                \n"
		"    self.printInUpdate = false;                     \n"
		"    print('start post Broad')                       \n"
		"    return true                                     \n"
		"",
		"    local x = self.parent.GetScript('rotator').parent.getTrans().rot().getX();  \n"
		"    local x = self.parent.getTrans().rot().getX();  \n"
		"    x = x + Timer.deltaTime() * self.rotSpeed       \n"
		"    self.parent.getTrans().rot().setX(x);           \n"
		"    if(self.printInUpdate) then print('update'); end \n"
		"    GameManager.getEntityFromName('Bob').getTrans().rot().setY(50)\n          "
		"")));
	(void)keyBoardController;

	game->AddEntity("Cam");
	game->currentEntity.addComponent<CameraComponent>();
	game->currentEntity.addComponent<ScriptComponent>()->myScript(keyBoardController);

	game->AddEntity("Fast Rotator");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->Geo(resourceManager.getFirstMesh("NU_Sphere"));
	game->currentEntity.getRenderable()->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.getFirstScript("rotator"));
	game->currentEntity.getTrans()->pos.x = 5;
	game->currentEntity.Parent("Bob");

	game->AddEntity("Swinger");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->Geo(resourceManager.getFirstMesh("NU_Arrow"));
	game->currentEntity.getRenderable()->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.getFirstScript("rotator"));
	game->currentEntity.getTrans()->pos.y = 5;
	game->currentEntity.getTrans()->scale = glm::vec3(.5f,.5f,.5f);
	game->currentEntity.Parent("Bob");

	game->AddEntity("Super Swing");
	game->currentEntity.addComponent<RenderableComponent>();
	game->currentEntity.getRenderable()->Geo(resourceManager.getFirstMesh("NU_Plane"));
	game->currentEntity.getRenderable()->Shader(shader);
	game->currentEntity.addComponent<ScriptComponent>()->myScript(resourceManager.getFirstScript("rotator"));
	game->currentEntity.getTrans()->pos.y = 2;
	game->currentEntity.getTrans()->scale = glm::vec3(.2f,.2f,.2f);
	game->currentEntity.Parent("Swinger");


	game->currentEntity.SetCurrent(gameManager.getEntity("Bob"));
	game->currentEntity.DuplicateCurrent();

	gui.init();

	gui.show();

	ret = app.exec();

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return ret;
}