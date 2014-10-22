#pragma once

#include <QtOpenGL/QGLWidget>
#include <Engine/Systems/GameObjectManager.h>
#include <QtCore/QTimer>

class BasicGui : public QGLWidget {
private:
	Q_OBJECT;
public: GameObjectManager meGame;
	QTimer timer;
public:
	void init() {
		meGame.init();
	}

	void initializeGL();

	void startGameLoop() {
		timer.start();
	}

	void update() {
		meGame.update();
		repaint();
	}

	void resizeGL(int w, int h)
	{
		glViewport(0, 0, w, h);
		meGame.width = w;
		meGame.height = h;
		//edit project matrix
	}

	void paintGL()
	{
		meGame.paint();
	}

};