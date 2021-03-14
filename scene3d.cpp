#include "scene3d.h"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Scene3D::Scene3D(QWidget *parent) : QGLWidget(parent)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	float vertices[] = {
			// positions       // colors         // texture coords
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// top right
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// bottom left
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f	// top left 
	};
	if (sizeof(vertices) == sizeof(mp_vertices)) {
		std::copy(std::begin(vertices), std::end(vertices), mp_vertices); 
	} else {
		qDebug() << "Vertices sizes not equal";
	}
	
	unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
	};
	if (sizeof(indices) == sizeof(mp_indices)) {
		std::copy(std::begin(indices), std::end(indices), mp_indices);
	} else {
		qDebug() << "Indices sizes not equal";
	}
	
	// shaders
	m_vertexDir = QString::fromUtf8("./resources/shaders/default.vert");
	m_fragmentDir = QString::fromUtf8("./resources/shaders/default.frag");

	// textures
	QDir imagePath = QDir(QApplication::applicationDirPath() + "/resources/images");
	QStringList imageFileExtensions = {"*.jpg", "*.png"};
	QStringList images = imagePath.entryList(imageFileExtensions, QDir::Files);
	while (images.length() > 32) {
		qDebug () << "Cannot load more than 32 Textures in a single program";
		images.removeLast();
	}
	stbi_set_flip_vertically_on_load(true);
	foreach(QString fileName, images) {
		QString filePath = QDir::toNativeSeparators(imagePath.absolutePath() + "/" + fileName);
		TextureStruct texData;
		texData.data = stbi_load(filePath.toLatin1(), &texData.width, &texData.height, &texData.nrChannels, 0);
		m_textures.append(texData);
		#ifdef GLDEBUG
		qDebug().nospace() << fileName << " texture loaded: textureDataSize=" << sizeof(texData.data) <<", size=" << texData.width << "x" << texData.height << ", channels=" << texData.nrChannels;
		#endif
	}
}

Scene3D::~Scene3D()
{
	
}

QSize Scene3D::sizeHint() const
{
	return QSize(400, 400);
}

void Scene3D::initializeGL()
{

	// mp_timer = new QTimer(this);
	// mp_timer->setSingleShot(false);
	// mp_timer->setInterval(1000.0/60.0);
	// connect(mp_timer, &QTimer::timeout, this, &Scene3D::paintGL);
	// m_green = 0;

	initializeOpenGLFunctions();	//once
	#ifdef GLDEBUG
	qDebug().nospace() << "GLfunctions initialized. Renderer=" << reinterpret_cast<const char *>(glGetString(GL_RENDERER));
	#endif

	compileShaders(); // uses shader and vertex source to compile gl program

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_vao);	// if unbind remember to bind it in paintGL

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);	// this we can unbind. see comment on unbind
	glBufferData(GL_ARRAY_BUFFER, sizeof(mp_vertices), mp_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mp_indices), mp_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, N_AXES, GL_FLOAT, GL_FALSE, (N_AXES+N_COLORARGS+N_TEXAXES) * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, N_COLORARGS, GL_FLOAT, GL_FALSE, (N_AXES+N_COLORARGS+N_TEXAXES) * sizeof(float), reinterpret_cast<void *>(N_AXES*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, N_TEXAXES, GL_FLOAT, GL_FALSE, (N_AXES+N_COLORARGS+N_TEXAXES) * sizeof(float), reinterpret_cast<void *>((N_AXES+N_COLORARGS)*sizeof(float)));
	glEnableVertexAttribArray(2); 

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
 
	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0); 
	
	unsigned int i = 0;
	foreach (TextureStruct tex, m_textures) {
		glGenTextures(1, &tex.texture);
		// qDebug() << tex.texture;
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, tex.texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (tex.data) {
			switch (tex.nrChannels) {
			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.width, tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex.data);
				break;
			case 4:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data);
				break;
			default:
				qDebug().nospace() << "Channel number of " << tex.nrChannels << " not implemented yet.";
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			qDebug().nospace() << "tex.data[" << QString::number(i) << "] failed to load.";
		}
		stbi_image_free(tex.data);
		i++;
	}
	// We also have to tell OpenGL to which texture unit each shader sampler belongs to by setting each sampler using glUniform1i. We only have to set this once, so we can do this before we enter the render loop: 
	glUseProgram(m_shaderProgram);
	do {
		i--;
		glUniform1i(glGetUniformLocation(m_shaderProgram, (QString("texture")+QString::number(i)).toLatin1()), i);
	} while (i != 0); //By setting the samplers via glUniform1i we make sure each uniform sampler corresponds to the proper texture unit.

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	/* testing transform */
	m_trans = glm::mat4(1.0f);
	m_trans = glm::translate(m_trans, glm::vec3(0.5, -0.5, 0.));

	m_transformLoc = glGetUniformLocation(m_shaderProgram, "transform");
	glUniformMatrix4fv(m_transformLoc, 1, GL_FALSE, glm::value_ptr(m_trans));
	// mp_timer->start();
	// connect(mp_timer, SIGNAL(timeout()), this, SLOT(update()));
} 

void Scene3D::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shaderProgram);
	// if (m_green < 1.f) {
	// 	m_green += 0.005f;
	// } else {
	// 	m_green = 0.f;
	// }
	// int vertexColorLocation = glGetUniformLocation(m_shaderProgram, "ourColor");
	// if (vertexColorLocation != -1) {
	// 	glUniform4f(vertexColorLocation, 0.0f, m_green, 0.0f, 1.0f);
	// } else {
	// 	qDebug() << "Couldn't find \"ourColor\"";
	// }
	m_trans = glm::rotate(m_trans, 0.1f, glm::vec3(0.0, 0.0, 1.0f));
	glUniformMatrix4fv(m_transformLoc, 1, GL_FALSE, glm::value_ptr(m_trans));

	// glBindTexture(GL_TEXTURE_2D, m_texture);
	// glBindVertexArray(m_vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawElements(GL_TRIANGLES, N_ELEMENTS*N_ELEMENTITEMS, GL_UNSIGNED_INT, 0);
	// glDrawArrays(GL_TRIANGLES, 0, sizeof(mp_vertices)/(3*sizeof(float)));
	// glBindVertexArray(0);
}

void Scene3D::resizeGL(int w, int h)
{
	int side = qMin(w, h);
	// glViewport((w - side) / 2, (h - side) / 2, side, side);
	glViewport(0, 0, w, h);
}

void Scene3D::closeEvent(QCloseEvent *event)
{
	makeCurrent();
	// here we delete the opengl arrays
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteProgram(m_shaderProgram);
	doneCurrent();	
}

void Scene3D::compileShaders()
{
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	QString vertexSource = sourceFromFile(m_vertexDir);
	const char * vertexSourceC = vertexSource.toLatin1();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSourceC, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	#ifdef GLDEBUG
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
	}
	#endif
	// fragment shader
	QString fragmentSource = sourceFromFile(m_fragmentDir);
	const char * fragmentSourceC = fragmentSource.toLatin1();
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSourceC, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	#ifdef GLDEBUG
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
	}
	#endif
	// link shaders
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);
	// check for linking errors
	#ifdef GLDEBUG
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
	}
	#endif

	glDetachShader(m_shaderProgram, vertexShader);
	glDetachShader(m_shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

QString Scene3D::sourceFromFile(const QString &fileDir) const
{
	QFile file(QDir::toNativeSeparators(fileDir));
	file.open(QIODevice::ReadOnly);	// file is closed automatically when deleted.
	return QString(file.readAll());
}