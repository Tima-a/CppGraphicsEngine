class Texture
{
public:
    const char* path;
    float x;
    float y;
    bool visible;
    int img_width;
    int img_height;
    int rgb_channels;
    Texture(const char* path_, float x_, float y_)
    {
        path = path_;
        visible = true;
        x = x_;
        y = y_;
    }
    Texture()
    {

    }
    void show_image()
    {
        const char* filename = "sky.jpg";
        int img_width_, img_height_, img_channels_;
        unsigned char* data = stbi_load(filename, &img_width_, &img_height_, &img_channels_, 0);
        img_width = img_width_;
        img_height = img_height_;
        rgb_channels = img_channels_;
        // ... process data if not NULL ..
        int index = 0;
        if (data != nullptr && img_height > 0 && img_width > 0)
        {
            for (int i = 0; i < img_height; i++)
            {
                for (int j = 0; j < img_width; j++)
                {
                    index++;
                    //draw_pixel(x + j * 0.1f, y + i * 0.1f, rgb(data[index], data[index + 1], data[index + 2]));
                    //std::cout << index << " pixel: RGB " << static_cast<int>(data[index]) << " " << static_cast<int>(data[index + 1]) << " " << static_cast<int>(data[index + 2]) << std::endl;
                }
            }
        }
        stbi_image_free(data);
    }
};
static void simulategame()
{
	Texture t1("sky.jpg", 0.0f, 0.0f);
	t1.show_image();
    //text2d::text txt("PLAY CHESS",0.0f, 17.0f, 2.5f, 3.0f, 1.0f, white, true);
	//vector2d::VECTOR v1(0.0f, 0.0f,4.0f, 0.0f, red, true);
	//draw_coordinate_grid(white);
	//HWND hWnd = FindWindow("Game Window Class", "Tic Tac Toe");
	//MessageBox(hWnd, "Error because of vector", "Error", 0x00000000L);
	update_screen(black);

}
