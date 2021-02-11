#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(QString fileName)
{
    QDir imagePath = QDir(QApplication::applicationDirPath() + "/resources/" +fileName);
    mp_textureData = stbi_load(imagePath.absolutePath().toUtf8(), &m_width, &m_height, &m_nrChannels, 0);
    qDebug() << sizeof(mp_textureData) << m_width << m_height << m_nrChannels;
    uploadGL();
}

bool Texture::uploadGL(){
    initializeOpenGLFunctions();
    if (!mp_textureData) {
        qDebug() << "Failed to load texture";
        return false;
    }
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, mp_textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(mp_textureData);
    return true;
}

void Texture::bind(int texUnit){
    m_texUnit = GL_TEXTURE0 + texUnit;
    glActiveTexture(m_texUnit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::unbind(){
    glActiveTexture(m_texUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture(){
    unbind();
    glDeleteTextures(1, &m_texture);
}
