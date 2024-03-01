#include "MazeApplication.h"
#include "AppSettings.h"


int main(int argc, char* argv[])
{
	MazeApplication application;

	Debugger::print = true;

	Mesh::normalScale = NORMALS_SIZE;
	Renderer::GetInstance().gizmoIconSize = GIZMO_ICON_SIZE;

	application.gameResolutionWidth = GAME_RESOLUTION_WIDTH;
	application.gameResolutionHeight = GAME_RESOLUTION_HEIGHT;

	application.InitializeWindow(APP_WIDTH, APP_HEIGHT);

	application.MainLoop();

	return -1;
}