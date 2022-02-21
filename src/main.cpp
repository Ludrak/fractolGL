
#include "GLWrap.hpp"

glw::Window          *window;

glw::Texture         *texture;
glw::ShaderProgram   *shader;
glw::VertexArray     *square;

double              pos_x = -1;
double              pos_y = 0;

double              zoom_factor = 4;

bool                zooming = false;
bool                backzoom = false;

void        on_load()
{
    shader = new glw::ShaderProgram("Basic Program");
    shader->create();
    shader->addShaderFromFile("shaders/shader.vert", GL_VERTEX_SHADER);
    shader->addShaderFromFile("shaders/shader.frag", GL_FRAGMENT_SHADER);
    shader->link();
    glEnable(GL_DEPTH_TEST);
}

void        on_setup()
{
    texture = new glw::Texture();
    texture->load("ressources/tough_grass.jpeg", GL_TEXTURE_2D);
    texture->paramWrap(GL_REPEAT, GL_REPEAT);
    texture->paramFilter(GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
    texture->generateMipmap();

    float vertices[] = {
        -1, -1, -1,  255, 255, 255,   0, 1,
        1, -1, -1,   255, 0, 0,       1, 1,    
        1, 1, -1,    0, 255, 0,       1, 0,    
        -1, 1, -1,   0, 0, 255,       0, 0,    
    };

    ushort indices[] = {
        0, 2, 3,
        2, 1, 0
    };

      
    square = new glw::VertexArray();
    square->create();
    square->bind();

    square->addBufferData(1, GL_ARRAY_BUFFER,         GL_FLOAT,          vertices, sizeof(vertices), GL_DYNAMIC_DRAW);
    square->addBufferData(2, GL_ELEMENT_ARRAY_BUFFER, GL_UNSIGNED_SHORT, indices,  sizeof(indices),  GL_DYNAMIC_DRAW);
  
    square->bindBuffer(GL_ARRAY_BUFFER,1);

    square->attribPointer(0, 3, 8, 0);
    square->attribPointer(1, 3, 8, 3);
    square->attribPointer(2, 2, 8, 6);

    square->enableAttrib(0);
    square->enableAttrib(1);
    square->enableAttrib(2);
}

void        on_draw()
{
    /* modiffiing mesh */

    shader->use();
    texture->activate(GL_TEXTURE0);
    shader->setUniform1f("time", glfwGetTime());
    shader->setUniform1i("tex_sampler", 0);
    shader->setUniform1d("pos_x", pos_x);
    shader->setUniform1d("pos_y", pos_y);
    shader->setUniform1d("zoom_factor", zoom_factor);
    square->draw();

    //zoom_factor = 4.0f / exp(glfwGetTime() * 0.2f);
    if (zooming)
        zoom_factor *= 0.993f;
    else if (backzoom)
        zoom_factor *= 1.007f;
    if (zoom_factor > 4)
    { 
        zoom_factor = 4;
        backzoom = false;
    }
}

void onKeyPress(int key)
{
    if (key ==  GLFW_KEY_UP)
    {
        if (backzoom)
            backzoom = !backzoom;
        else
            zooming = !zooming;
    }
    else if (key == GLFW_KEY_DOWN)
    {
        if (zooming)
            zooming = !zooming;
        else
            backzoom = !backzoom;
    }
    else if (key == GLFW_KEY_SPACE)
    {
        zooming = false;
        backzoom = false;
    }
}

void onKeyRelease(int key)
{
    (void)key;
}


void onMouseScroll(double x, double y)
{
    pos_x += zoom_factor * (-x * 0.1f);
    pos_y += zoom_factor * (-y * 0.1f);
    if (pos_x < -2)
        pos_x = -2;
    if (pos_x > 1)
        pos_x = 1;
    if (pos_y < -1.5f)
        pos_y = -1.5f;
    if (pos_y > 1.5f)
        pos_y = 1.5f;
}

int main()
{
    window = new  glw::Window(720, 720, "GLwrap example");
    window->mapEvent(WIN_LOAD, on_load);
    window->mapEvent(WIN_SETUP, on_setup);
    window->mapEvent(WIN_DRAW, on_draw);
    window->init();
    window->mapInputEvent(WIN_KEYPRESS, onKeyPress);
    window->mapInputEvent(WIN_KEYRELEASE, onKeyRelease);
    window->mapInputEvent(WIN_MOUSESCROLL, onMouseScroll);
    window->run();
    delete square;
    delete shader;
    delete window;
    delete texture;
    return (0);
}