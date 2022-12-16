#pragma once

#include <string>

#include <glad/glad.h>
class Texture2D
{

public:
	Texture2D() :mipmapLevel(0) {}

	struct QYHSHeader
	{
		char head[4];
		int width;
		int height;
		int format;
		int mipmapLevel;
		int compressSize;
	};

	static Texture2D* loadImageFromFile(const std::string& image_file_path);
	static Texture2D* loadFromFile(const std::string& image_file_path);
	GLuint getTextureID() { return gl_texture_id_; }

	static Texture2D* loadFromJPGFile(const std::string& jpg_file_path);
	static void compressImageFile(const std::string& sourcePath, const std::string& dirPath);
private:
	int width;
	int height;
	int mipmapLevel;
	GLenum gl_texture_format_;//纹理ID
	GLuint gl_texture_id_;//纹理ID
};