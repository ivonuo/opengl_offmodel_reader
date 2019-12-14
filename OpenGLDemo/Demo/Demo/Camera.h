#pragma once
#include "vec3.h"

class Camera {
public:
	vec3 pos;
	vec3 front_dir, up_dir, right_dir, world_up;
	float yaw, pitch;
	float velocity, sensitivity;
	Camera(vec3 pos = { 0,0,0 }, float velocity = 20.f, float sensitivity = .01f)
	{
		this->pos = pos;
		this->velocity = velocity;
		this->sensitivity = sensitivity;
		yaw = -90.f;
		world_up = { 0,1,0 };
		//update_camera_vectors();
	}
	void process_movement(int x_offset, int y_offset)
	{
		yaw += x_offset * sensitivity;
		pitch += y_offset * sensitivity;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		update_camera_vectors();
	}
	float radians(float angle)
	{
		return angle / (2 * 3.14159f);
	}
	void update_camera_vectors()
	{
		vec3 front = { cos(radians(yaw)) * cos(radians(pitch)) , sin(radians(pitch)) , sin(radians(yaw)) * cos(radians(pitch)) };
		front_dir = unit_vector(front);
		right_dir = unit_vector(cross(front_dir, world_up));
		up_dir = unit_vector(cross(right_dir, front_dir));
	}
	void ProcessKeyboard(unsigned char direction)
	{
		if (direction == 'w')
			pos += front_dir * velocity;
		if (direction == 's')
			pos -= front_dir * velocity;
		if (direction == 'a')
			pos -= right_dir * velocity;
		if (direction == 'd')
			pos += right_dir * velocity;
		if (direction == 'e')
			pos += world_up * velocity;
		if (direction == 'c')
			pos -= world_up * velocity;
	}
};