#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stopWatch.h"
#include <iostream>
#include <string>
#include <fstream>
#include "gldxt.h"




/*加载指定路径的图片，并且上传到显存中*/
Texture2D* Texture2D::loadImageFromFile(const std::string& image_file_path)
{
	Texture2D* texture2d = new Texture2D();

	stbi_set_flip_vertically_on_load(true);//翻转图片，解析出来的图片数据从左下角开始，这是因为OpenGL的纹理坐标起始点为左下角。
	int channels_in_file;//通道数

	/*timetool::StopWatch stopwatch;
	stopwatch.start();*/
	unsigned char* data = stbi_load(image_file_path.c_str(), &(texture2d->width), &(texture2d->height), &channels_in_file, 0);
	//stopwatch.stop();
	//std::int64_t decompress_jpg_cost = stopwatch.milliseconds();

	int image_data_format = GL_RGB;
	if (data != nullptr)
	{
		//根据颜色通道数，判断颜色格式。
		switch (channels_in_file) {
		case 1:
		{
			image_data_format = GL_ALPHA;
			break;
		}
		case 3:
		{
			image_data_format = GL_RGB;
			texture2d->gl_texture_format_ = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;
		}
		case 4:
		{
			image_data_format = GL_RGBA;
			texture2d->gl_texture_format_ = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		}
		}
	}

	std::cout << &texture2d->gl_texture_id_ << std::endl;
	//1. 通知显卡创建纹理对象，返回句柄;
	glGenTextures(1, &(texture2d->gl_texture_id_));
	std::cout << &texture2d->gl_texture_id_ << std::endl;
	//2. 将纹理绑定到特定纹理目标;
	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);

	//stopwatch.restart();

	//3. 将图片rgb数据上传到GPU;并进行压缩。
	glTexImage2D(GL_TEXTURE_2D, texture2d->mipmapLevel, texture2d->gl_texture_format_, texture2d->width, texture2d->height, 0, image_data_format, GL_UNSIGNED_BYTE, data);
	//stopwatch.stop();
	//std::int64_t upload_and_compress_cost = stopwatch.milliseconds();

	//4. 指定放大，缩小滤波方式，线性滤波，即放大缩小的插值方式;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//释放图片文件内存
	stbi_image_free(data);

	return texture2d;
}


/*
*@使用该方法加载图片到显存的过程是，从硬盘中加载jpg图片数据->将jpg数据进行解压，解压成为压缩的RGB数据->RGB数据进行压缩上传到GPU内存中。
* 以上三个过程都需要消耗一定的时间-
*/
Texture2D* Texture2D::loadFromJPGFile(const std::string& image_file_path)
{
	Texture2D* texture2d = new Texture2D();
	/*timetool::StopWatch stopWatch;
	stopWatch.start();*/
	stbi_set_flip_vertically_on_load(true);//翻转图片，解析出来的图片数据从左下角开始，这是因为OpenGL的纹理坐标起始点为左下角。
	int channels_in_file;//通道数
	unsigned char* data = stbi_load(image_file_path.c_str(), &(texture2d->width), &(texture2d->height), &channels_in_file, 0);
	//stopWatch.stop();
	//auto costTime = stopWatch.milliseconds();
	if (data != nullptr)
	{
		//根据颜色通道数，判断颜色格式。
		switch (channels_in_file) {
		case 1:
		{
			texture2d->gl_texture_format_ = GL_ALPHA;
			break;
		}
		case 3:
		{
			texture2d->gl_texture_format_ = GL_RGB;
			break;
		}
		case 4:
		{
			texture2d->gl_texture_format_ = GL_RGBA;
			break;
		}
		}
	}

	//1. 通知显卡创建纹理对象，返回句柄;

	GLuint texId;
	glGenTextures(1, &texId);
	std::cout << "Hello" << std::endl;
	//2. 将纹理绑定到特定纹理目标;
	glBindTexture(GL_TEXTURE_2D, texId);

	//stopWatch.restart();
	//3. 将图片rgb数据上传到GPU;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture2d->width, texture2d->height, 0, texture2d->gl_texture_format_, GL_UNSIGNED_BYTE, data);

	//stopWatch.stop();
	//auto upload_cost = stopWatch.milliseconds();
	//4. 指定放大，缩小滤波方式，线性滤波，即放大缩小的插值方式;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//释放图片文件内存
	stbi_image_free(data);

	return texture2d;
}

Texture2D* Texture2D::loadFromFile(const std::string& image_file_path)
{
	Texture2D* texture2d = new Texture2D();
	/*timetool::StopWatch stopwatch;
	stopwatch.start();*/
	//读取 cpt 压缩纹理文件
	std::ifstream input_file_stream(image_file_path, std::ios::in | std::ios::binary);
	QYHSHeader tcp_file_head;
	input_file_stream.read((char*)&tcp_file_head, sizeof(QYHSHeader));
	char* data = (char*)malloc(tcp_file_head.compressSize);
	input_file_stream.read((char*)data, tcp_file_head.compressSize);
	input_file_stream.close();
	//stopwatch.stop();
	//std::int64_t decompress_jpg_cost = stopwatch.milliseconds();
	texture2d->gl_texture_format_ = tcp_file_head.format;
	texture2d->width = tcp_file_head.width;
	texture2d->height = tcp_file_head.height;



	glGenTextures(1, &(texture2d->gl_texture_id_));
	glBindTexture(GL_TEXTURE_2D, texture2d->gl_texture_id_);
	//stopwatch.restart();
	{
		//3. 将压缩纹理数据上传到GPU;
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, texture2d->gl_texture_format_, texture2d->width, texture2d->height, 0, tcp_file_head.compressSize, data);
	}
	//stopwatch.stop();
	//std::int64_t upload_cpt_cost = stopwatch.milliseconds();
	//4. 指定放大，缩小滤波方式，线性滤波，即放大缩小的插值方式;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	delete (data);
	return texture2d;
}






void Texture2D::compressImageFile(const std::string& sourcePath, const std::string& dirPath)
{
	//先将硬盘中的jpg图片加载到显存中来
	Texture2D* image = loadImageFromFile(sourcePath);

	GLint compress_success = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compress_success);

	GLint compress_size = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compress_size);

	GLint compressed_format = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &compressed_format);

	//从显存中，将已经保存好的压缩好的纹理数据加载到内存中来。
	void* img = malloc(compress_size);
	glGetCompressedTexImage(GL_TEXTURE_2D, 0, img);

	QYHSHeader qyhs_header;
	qyhs_header.head[0] = 'q';
	qyhs_header.head[1] = 'y';
	qyhs_header.head[2] = 'h';
	qyhs_header.head[3] = 's';
	qyhs_header.width = image->width;
	qyhs_header.height = image->height;
	qyhs_header.compressSize = compress_size;
	qyhs_header.format = compressed_format;
	qyhs_header.mipmapLevel = image->mipmapLevel;


	//将RGB数据读取到文件中来。
	std::ofstream output_file_stream(dirPath, std::ios::out | std::ios::binary);
	output_file_stream.write((char*)(&qyhs_header), sizeof(qyhs_header));
	output_file_stream.write((char*)img, compress_size);
	output_file_stream.close();
}
