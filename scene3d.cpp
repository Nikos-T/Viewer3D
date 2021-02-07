#include "scene3d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <QDir>
#include <QApplication>

Scene3D::Scene3D(QWidget *parent) : QGLWidget(parent)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	float vertices[] = {
			// positions          // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
	
	// vertex shader
	m_vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);\n"
		"	ourColor = aColor;\n"
		"	TexCoord = aTexCoord;\n"
		"}\0";
		
	// fragment shader
	m_fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"//	FragColor = vec4(ourColor, 1.f);\n"
		"	FragColor = texture(ourTexture, TexCoord);\n"
		"}\n\0";

	// texture
	QDir imagePath = QDir(QApplication::applicationDirPath() + "/../resources/container.jpg");
	mp_textureData = stbi_load(imagePath.absolutePath().toUtf8(), &m_width, &m_height, &m_nrChannels, 0);
	qDebug() << sizeof(mp_textureData) << m_width << m_height << m_nrChannels;
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

	mp_timer = new QTimer(this);
	mp_timer->setSingleShot(false);
	mp_timer->setInterval(1000.0/60.0);
	connect(mp_timer, &QTimer::timeout, this, &Scene3D::paintGL);
	m_green = 0;

	initializeOpenGLFunctions();
	qDebug() << reinterpret_cast<const char *>(glGetString(GL_RENDERER));
	// glViewport(0, 0, 800, 600);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &m_vertexShaderSource, NULL);
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
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &m_fragmentShaderSource, NULL);
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
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	
	if (mp_textureData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, mp_textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		qDebug() << "Failed to load texture";
	}
	glGenTextures(1, &m_texture);  
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, mp_textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mp_vertices), mp_vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mp_indices), mp_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, N_AXES, GL_FLOAT, GL_FALSE, (N_AXES+N_COLORARGS) * sizeof(float), reinterpret_cast<void *>(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, N_COLORARGS, GL_FLOAT, GL_FALSE, (N_AXES+N_COLORARGS) * sizeof(float), reinterpret_cast<void *>(N_AXES*sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (N_AXES+N_COLORARGS+N_TEXAXES) * sizeof(float), reinterpret_cast<void *>((N_AXES+N_COLORARGS)*sizeof(float)));
	glEnableVertexAttribArray(2); 

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); 


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); 
	stbi_image_free(mp_textureData);
	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0); 
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// mp_timer->start();
	connect(mp_timer, SIGNAL(timeout()), this, SLOT(update()));
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

	// glBindVertexArray(m_vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawElements(GL_TRIANGLES, N_ELEMENTS*N_ELEMENTITEMS, GL_UNSIGNED_INT, 0);
	// glDrawArrays(GL_TRIANGLES, 0, sizeof(mp_vertices)/(3*sizeof(float)));
	// glBindVertexArray(0);
}

void Scene3D::resizeGL(int w, int h)
{
	int side = qMin(w, h);
	glViewport((w - side) / 2, (h - side) / 2, side, side);
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