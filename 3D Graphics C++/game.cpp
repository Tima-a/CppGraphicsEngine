WINDOW window3("Graphics Engine", 300, 100, 1620, 980);
Camera c1(window3, 0.0f, 0.0f, _win.win_width_px[window3.wnd_param], _win.win_height_px[window3.wnd_param]);
audio a1("test.mp3");
static void simulategame()
{
    c1.Init();
    refresh_screen(colors.blue, window3);
    //p1.ApplyPhysicsComponent(1.0f, r1);
    //p1.wind_type = ACCELERATING_WIND;
    //p1.object_throw_angle = 25.0f;
    //p1.wind_acceleration = 4.0f;
    //p1.initial_velocity = 20.0f;
    //p1.ar_coefficent = 0.0099f;
    //p1.drag_coefficent = 0.0009f;
    //p1.SetWindAngle(90.0f);
    //p1.wind_speed = 2.0f;
    //p1.SetAirDensityParametersHeight(DRY_AIR, 273.5f, 10000, 100.0f, 1.0f);
    //p1.StartPhysicsSimulation();
    //p1.ApplyPhysicsTransformation(r1);
    //r1.draw();
    //Sprite s1(window3, "sea.jpg", _win.win_width_px[window3.wnd_param] / 2 + 0.0f, _win.win_height_px[window3.wnd_param] / 2 + 0.0f, 0.5f, 0.5f, true);
    //s1.grayscalefilter = true;
    ////s1.redfilter = true;
    //s1.draw_sprite();    
    //rectangle::rectd f(window3, _win.win_width_px[window3.wnd_param] / 2, _win.win_height_px[window3.wnd_param] / 2, 50.f, 5.0f, colors.white, true, true);
    //vector2d::fvector r1(window3, _win.win_width_px[window3.wnd_param] / 2+20.0f, _win.win_height_px[window3.wnd_param] / 2+20.0f, _win.win_width_px[window3.wnd_param] / 2+100.0f, _win.win_height_px[window3.wnd_param] / 2+0.0f, colors.red, true);
    //f.Rotate(d, center, clockwise);
    //f.draw();
    // r1.draw();
    //triangle2d::triangle t1(window3, 100.0f, 200.0f, 200.0f, 150.0f, _win.win_width_px[window3.wnd_param] / 2 - 150, _win.win_height_px[window3.wnd_param] / 2 - 200, colors.white, true, true);
    //rectangle::rectf r1(window3, _win.win_width_px[window3.wnd_param] / 2-100.0f, _win.win_height_px[window3.wnd_param] / 2+50.0f, 100.0f, 50.0f, colors.white, true, true);
    //t1.Rotate(d, startpos, counter_clockwise);
    //t1.draw();
    //p1.StartPhysicsSimulation();
    //p1.ApplyPhysicsTransformation(r1);
    //r1.draw();
    /*ellipse2d::ellipsef e1(window3, _win.win_width_px[window3.wnd_param]/2+200, _win.win_height_px[window3.wnd_param]/2+100, 50.0f, 75.0f, colors.white, 0, 3600, true, true);
    e1.draw();*/
    /*float x = 0;
    float y = 0;
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    ScreenToClient(hWnd, &cursorPos);
    x = cursorPos.x;
    y = cursorPos.y;*/
    //Sprite s1(window3, "sea.jpg", _win.win_width_px[window3.wnd_param] / 2 + 20.0f, _win.win_height_px[window3.wnd_param] / 2 + 20.0f, 1.0f, 1.0f, true);
    //s1.flip_horizontally = true;
    //s1.flip_vertically = true;
    //s1.draw_sprite();
    //rectangle::rectf r1(window3, _win.win_width_px[window3.wnd_param] / 2.0f, _win.win_height_px[window3.wnd_param], 100.0f, 50.0f, colors.red, true, true); r1.draw();
    //Button b1(window3, _win.win_width_px[window3.wnd_param] / 2.0f + 50.0f, _win.win_width_px[window3.wnd_param] / 2.0f - 50.0f, _win.win_height_px[window3.wnd_param] / 2.0f + 25.0f, _win.win_height_px[window3.wnd_param] / 2.0f - 25.0f);
    //b1.InitializeButton();
    if (Input::KeyPressed(Input::Keys.D))
    {
        c1.x++;
        a1.play();
        a1.set_volume(100, AudioLeft);

    }
    //bool k = ch_equals("Salam", "salam");
    //const char* k = sub_char("Salam want to go home!", 1, 8);
    //text2d::text t1(window3, tocnstch(0.1001231), _win.win_width_px[window3.wnd_param] / 2 + 200.0f, _win.win_height_px[window3.wnd_param] / 2+200, 1.5f, 1.5f, 12.0f, 50.0f, 3.0f, colors.red, true); t1.draw();
    //text2d::text t2(window3, tocnstch(dtoi(b1.cursory)), _win.win_width_px[window3.wnd_param] / 2 - 200.0f, _win.win_height_px[window3.wnd_param] / 2+200, 1.5f, 1.5f, 12.0f, 50.0f, 3.0f, colors.red, true); t2.draw();
    
    draw_coordinate_grid(colors.yellow, window3);
}
