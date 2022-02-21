#version 410 core

out vec4 FragColor;
in  vec3 color;

uniform double		pos_x;
uniform double		pos_y;
uniform	double		zoom_factor;

in  vec2 coord;

uniform sampler2D   tex_sampler;


//in float time_val;

#define WIDTH           720
#define HEIGHT          720

#define TEX_SIZE        2400

#define MAX_ITERATION   256
#define INF             10000
#define LOG2            0.693147

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4	mandelbrot(double x0, double y0)
{
	double	x;
	double	y;
	double	x_squared;
	double	y_squared;
	double	iterations;

	iterations = 0;
	x = x0;
	y = y0;
	while (abs(x + y) < INF && iterations < MAX_ITERATION)
	{
		x_squared = x * x - y * y;
		y_squared = 2.0 * x * y;
		x = x_squared + x0;
		y = y_squared + y0;
		iterations++;
	}
	if (iterations < MAX_ITERATION)
    {
        float log_zn = log(float(x * x + y * y)) / 2.0;
        float nu = log(log_zn / LOG2) / LOG2;
        iterations = iterations + 1.0 - nu;
        //vec4 c1 = texture(tex_sampler, vec2(iterations / double(MAX_ITERATION), 0));
        //vec4 c2 = texture(tex_sampler, vec2((iterations) / double(MAX_ITERATION) + 1 , 0));
	    vec3 rgb = hsv2rgb(vec3(iterations / double(MAX_ITERATION), 1.0, 1.0));
    	return (vec4(rgb, 1.0));//vec4(mix(c2, c1, (iterations - float(int(iterations)))).xyz, 1));
    }
	else
    {
		return (vec4(0));
    }
}

void main()
{
    FragColor = mandelbrot( (coord.x * zoom_factor) - (zoom_factor * 0.5) + pos_x, 
							(coord.y * zoom_factor) - (zoom_factor * 0.5) + pos_y);
}