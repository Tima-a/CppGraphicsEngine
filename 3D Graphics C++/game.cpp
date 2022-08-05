static void simulategame()
{
    //Texture t1("sky.png");
    //Sprite s1(t1, 0.0f, -1.0f, 1.9f, 1.5f, true);
    const char* a = ToString(914);
    text2d::text txt(a,0.0f, 0.0f, 2.0f, 2.0f, 1.0f, red, true);
    //ellipse2d::ELLIPSE e1(0.0f, 0.0f, 10.0f, 10.0f, yellow, 0, 3600, true, true);
    //vector2d::fvector v1(0.0f, 0.0f,40.0f, 40.0f, red, true);
    draw_coordinate_grid(white);
    //HWND hWnd = FindWindow("Game Window Class", "Tic Tac Toe");
    //MessageBox(hWnd, "Error because of vector", "Error", 0x00000000L);
    update_screen(black);

}
