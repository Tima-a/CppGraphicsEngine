bool invisibility = true;
float xl = 0.0f;
static void simulategame()
{
	//ellipse2d::ELLIPSE e1(0.0f, 0.0f, 1.0f, 1.0f, blue, 3600, true, true);
	triangle2d::TRIANGLE t1(15.0f, 25.0f, 20.0f, 15.0f, 0.0f, 0.0f, white, true, true);
	//vector2d::A_VECTOR v1(-10.0f, 0.0f, 5.0f, 15.0f, blue, true, 1, 1, true, true, true);
	//for (int i = 0; i < v1.px_quantity; i++)
	//{
	//	draw_pixel(v1.matrix_pixels_x_cycle[i].x, v1.matrix_pixels_x_cycle[i].y+5.0f, PIXEL_SIZE, PIXEL_SIZE, yellow);
	//}
	//for (int i = 0; i < v1.px_quantity; i++)
	//{
	//	draw_pixel(v1.matrix_pixels_y_cycle[i].x, v1.matrix_pixels_y_cycle[i].y + 10.0f, PIXEL_SIZE, PIXEL_SIZE, yellow);
	//}
	//vector2d::A_VECTOR v2(10.0f, 0.0f, 20.0f, 15.0f, blue, true, 1, 1, true, true, true);
	//vector2d::A_VECTOR v2(10.0f, 0.0f, 20.0f, 15.0f, blue, true, 1, 1, true, true, true);
	//draw_coordinate_grid(white);
	//HWND hWnd = FindWindow("Game Window Class", "Tic Tac Toe");
	//MessageBox(hWnd, "Error because of vector", "Error", 0x00000000L);
	update_screen(black);
}
