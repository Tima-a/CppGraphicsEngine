int h = 0;
static void simulategame()
{
	//ellipse2d::ELLIPSE e1(0.0f, 0.0f, 30.0f, 20.0f, blue, 3600, true, true, false, false, true);
	//triangle2d::TRIANGLE t1(6.0f, 4.0f, 10.0f, 5.0f, 0.0f, 0.0f, blue, true);
	vector2d::VECTOR v1(0.0f, 0.0f, 10.0f, 0.0f, blue, true, 1, 1, true, true, true, true); v1.draw();
	//draw_coordinate_grid(white);
	//float n = make_float_divisible(10.0f, 3.0f);
	//HWND hWnd = FindWindow("Game Window Class", "Tic Tac Toe");
	//MessageBox(hWnd, "Error because of vector", "Error", 0x00000000L);

	h += 0.1f;
}

