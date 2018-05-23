#pragma once

#include "e_math.hxx"

class camera
{
public:
	camera();
	~camera();

	void init(size_t window_width, size_t window_height);

	//setters
	void set_position(glm::vec2& newPosition);
	void set_scale(float new_scale);

	//getters
	om::vec2 get_position() const;
	float get_scale() const;
	om::mat2x3 get_camera_matrix() const;

	void update();
private:
	om::vec2 screen;
	bool need_matrix_update;
	float scale;
	om::vec2 position;
	om::mat2x3 camera_matrix;
	om::mat2x3 ortho_matrix;
};

camera::camera() :
		screen(500.0f, 500.0f),
		need_matrix_update(true),
		position(0.0f, 0.0f)
{
}

void camera::init(size_t window_width, size_t window_height)
{
	screen.x = static_cast<float>(window_width);
	screen.y = static_cast<float>(window_height);
	glm::mat2x3 mat = glm::ortho(0.0f, screen.x, 0.0f, screen.y);
	ortho_matrix = mat[0];
}


void camera::update()
{
	if(need_matrix_update)
	{
		glm::vec3 translate(-position.x, -position.y, 0.0f);
		glm::mat4 mat = glm::translate(ortho_matrix, translate);
		camera_matrix =
	}
}

camera::~camera()
{
}

void camera::set_position(glm::vec2& new_position)
{
	position = new_position;
}

glm::vec2 camera::get_position() const
{
	return position;
}

void camera::set_scale(float new_scale)
{
	scale = new_scale;
}

float camera::get_scale() const
{
	return scale;
}

glm::mat4 camera::get_camera_matrix() const
{
	return camera_matrix;
}
