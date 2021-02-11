#ifndef TEXTURE_H
#define TEXTURE_H
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class Texture : protected QOpenGLExtraFunctions
{
public:
    Texture(QString fileName);
    void bind(int texUnit);
    void unbind();
    ~Texture();

private:
    unsigned char * mp_textureData;
    unsigned int m_texture, m_texUnit;
    int m_width, m_height, m_nrChannels;
    bool uploadGL();
};

#endif // TEXTURE_H
