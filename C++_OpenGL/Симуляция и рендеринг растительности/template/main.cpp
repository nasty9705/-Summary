#include <iostream>
#include <vector>

#include "Utility.h"
#include "SOIL.h"

using namespace std;

const uint GRASS_INSTANCES = 10000; // Количество травинок
const uint GRASS_INSTANCES_1 = 10000; // Количество травинок1
const uint GRASS_INSTANCES_2 = 500; // Количество травинок2
const uint GRASS_INSTANCES_3 = 500; // Количество травинок3

GL::Camera camera;               // Мы предоставляем Вам реализацию камеры. В OpenGL камера - это просто 2 матрицы. Модельно-видовая матрица и матрица проекции. // ###
                                 // Задача этого класса только в том чтобы обработать ввод с клавиатуры и правильно сформировать эти матрицы.
                                 // Вы можете просто пользоваться этим классом для расчёта указанных матриц.


GLuint grassPointsCount; // Количество вершин у модели травинки
GLuint grassPointsCount1;
GLuint grassPointsCount2;
GLuint grassPointsCount3;
GLuint grassShader;      // Шейдер, рисующий траву
GLuint grassShader1;
GLuint grassShader2;
GLuint grassShader3;
GLuint grassTexture;
GLuint grassTexture1;
GLuint grassTexture2;
GLuint grassTexture3;
GLuint grassVAO;         // VAO для травы (что такое VAO почитайте в доках)
GLuint grassVAO1;
GLuint grassVAO2;
GLuint grassVAO3;
GLuint grassVariance;    // Буфер для смещения координат травинок
GLuint grassVariance1;
GLuint grassVariance2;
GLuint grassVariance3;
vector<VM::vec4> grassVarianceData(GRASS_INSTANCES); // Вектор со смещениями для координат травинок
vector<VM::vec4> grassVarianceData1(GRASS_INSTANCES_1);
vector<VM::vec4> grassVarianceData2(GRASS_INSTANCES_2);
vector<VM::vec4> grassVarianceData3(GRASS_INSTANCES_3);

GLuint groundTexture;
GLuint groundShader; // Шейдер для земли
GLuint groundVAO;    // VAO для земли

GLuint grassTBO = -1;// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint grassTBO2 = -1;// аналогично

GLuint grass1TBO = -1;// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint grass1TBO2 = -1;// аналогично

GLuint grass2TBO = -1;// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint grass2TBO2 = -1;// аналогично

GLuint grass3TBO = -1;// Это "фиктивная" текстура для чтения из буфера. На самом деле никакой текстуры не создаётся.
GLuint grass3TBO2 = -1;// аналогично


// Размеры экрана
uint screenWidth = 800;
uint screenHeight = 600;

// Это для захвата мышки. Вам это не потребуется (это не значит, что нужно удалять эту строку)
bool captureMouse = true;


// Функция, для привязки к шейдеру TBO - буфера через текстуру
void bindTextureBuffer(GLuint program, GLint unit, const GLchar *name, GLuint texture) {

  glActiveTexture(GL_TEXTURE0 + unit);       CHECK_GL_ERRORS; // делаем активным определённый текстурныю юнит/блок
  glBindTexture(GL_TEXTURE_BUFFER, texture); CHECK_GL_ERRORS; // делаем текущей текстуру отвечающую за наш буфер

  GLint location = glGetUniformLocation(program, name);   CHECK_GL_ERRORS;  // привязываем текущую текстуру (которая фикктивная) и теущим текстурным блоком к имени в шейдере
  if (location >= 0)                                                        // на следующей строчке
    glUniform1i(location, unit);                                            // да да вот тут :)
}


// Функция, рисующая землю
void DrawGround() {

    // Используем шейдер для земли
    glUseProgram(groundShader);                                                  CHECK_GL_ERRORS

    // Устанавливаем юниформ для шейдера. В данном случае передадим перспективную матрицу камеры
    // Находим локацию юниформа 'camera' в шейдере
    GLint cameraLocation = glGetUniformLocation(groundShader, "camera");         CHECK_GL_ERRORS
    // Устанавливаем юниформ (загружаем на GPU матрицу проекции?)                                                     // ###
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS

    // Подключаем VAO, который содержит буферы, необходимые для отрисовки земли
    glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glUniform1i(glGetUniformLocation(groundShader, "point"), 0);

    // Рисуем землю: 2 треугольника (6 вершин)
    glDrawArrays(GL_TRIANGLES, 0, 6);                                            CHECK_GL_ERRORS

    // Отсоединяем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отключаем шейдер
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

// Обновление смещения травинок
float T = 0;
void UpdateGrassVariance() {
    // Генерация случайных смещений
	T = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //возвращает время в миллисекундах, прошедшее от вызова функции glutInit.
										     //При делении на 1000 получаем время в секундах
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        //grassVarianceData[i].x = (float)rand() / RAND_MAX / 5;
        //grassVarianceData[i].z = (float)rand() / RAND_MAX / 5;
		grassVarianceData[i].x = 0.03*(1 - cos(2.5 * T));
		grassVarianceData[i].z = 0.03*(1 - sin(1.5 * T));
    }

    // Привязываем буфер, содержащий смещения
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                                CHECK_GL_ERRORS
    // Загружаем данные в видеопамять
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Обновление смещения травинок1
float T1 = 0;
void UpdateGrassVariance1() {
	// Генерация случайных смещений
	T1 = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //возвращает время в миллисекундах, прошедшее от вызова функции glutInit.
											 //При делении на 1000 получаем время в секундах
	for (uint i = 0; i < GRASS_INSTANCES_1; ++i) {
		grassVarianceData1[i].x = 0.03*(1 - cos(2.5 * T1));
		grassVarianceData1[i].z = 0.05*(1 - cos(1.5 * T1));
	}

	// Привязываем буфер, содержащий смещения
	glBindBuffer(GL_ARRAY_BUFFER, grassVariance1);                                CHECK_GL_ERRORS
	// Загружаем данные в видеопамять
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES_1, grassVarianceData1.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
	// Отвязываем буфер
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Обновление смещения травинок2
float T2 = 0;
void UpdateGrassVariance2() {
	// Генерация случайных смещений
	T2 = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //возвращает время в миллисекундах, прошедшее от вызова функции glutInit.
											  //При делении на 1000 получаем время в секундах
	for (uint i = 0; i < GRASS_INSTANCES_2; ++i) {
		grassVarianceData2[i].x = 0.02*(1 - cos(2.5 * T1));
		grassVarianceData2[i].z = 0.03*(1 - sin(1.5 * T1));
	}

	// Привязываем буфер, содержащий смещения
	glBindBuffer(GL_ARRAY_BUFFER, grassVariance2);                                CHECK_GL_ERRORS
		// Загружаем данные в видеопамять
		glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES_2, grassVarianceData2.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
		// Отвязываем буфер
		glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Обновление смещения травинок3
float T3 = 0;
void UpdateGrassVariance3() {
	// Генерация случайных смещений
	T3 = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //возвращает время в миллисекундах, прошедшее от вызова функции glutInit.
											  //При делении на 1000 получаем время в секундах
	for (uint i = 0; i < GRASS_INSTANCES_3; ++i) {
		grassVarianceData3[i].x = 0.02*(1 - cos(2.5 * T1));
		grassVarianceData3[i].z = 0.03*(1 - sin(1.5 * T1));
	}

	// Привязываем буфер, содержащий смещения
	glBindBuffer(GL_ARRAY_BUFFER, grassVariance3);                                CHECK_GL_ERRORS
	// Загружаем данные в видеопамять
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES_3, grassVarianceData3.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS
	// Отвязываем буфер
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

// Рисование травы
void DrawGrass() {
    // Тут то же самое, что и в рисовании земли
    glUseProgram(grassShader);                                                   CHECK_GL_ERRORS
    GLint cameraLocation = glGetUniformLocation(grassShader, "camera");          CHECK_GL_ERRORS
    glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS
    // Обновляем смещения для травы
    UpdateGrassVariance();
    // Отрисовка травинок в количестве GRASS_INSTANCES

	glBindTexture(GL_TEXTURE_2D, grassTexture);                                  CHECK_GL_ERRORS
	glUniform1i(glGetUniformLocation(grassShader, "point"), 0);                  CHECK_GL_ERRORS

    bindTextureBuffer(grassShader, 2, "tboSampler",  grassTBO); // привязываем TBO к имени в шейдере
    bindTextureBuffer(grassShader, 3, "tboSampler2", grassTBO2);// привязываем TBO к имени в шейдере

    glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount, GRASS_INSTANCES);   CHECK_GL_ERRORS
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glUseProgram(0);                                                             CHECK_GL_ERRORS
}

//Рисование травы1
void DrawGrass1() {
	// Тут то же самое, что и в рисовании земли
	glUseProgram(grassShader1);                                                   CHECK_GL_ERRORS
	GLint cameraLocation = glGetUniformLocation(grassShader1, "camera");          CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
	glBindVertexArray(grassVAO1);                                                 CHECK_GL_ERRORS
	// Обновляем смещения для травы
	UpdateGrassVariance1();
	// Отрисовка травинок в количестве GRASS_INSTANCES

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture1);                                  CHECK_GL_ERRORS
	glUniform1i(glGetUniformLocation(grassShader1, "point"), 0);                  CHECK_GL_ERRORS

	bindTextureBuffer(grassShader1, 2, "tboSampler", grass1TBO); // привязываем TBO к имени в шейдере
	bindTextureBuffer(grassShader1, 3, "tboSampler2", grass1TBO2);// привязываем TBO к имени в шейдере

	glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount1, GRASS_INSTANCES_1);   CHECK_GL_ERRORS
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	glUseProgram(0);                                                             CHECK_GL_ERRORS
}

//Рисование травы2
void DrawGrass2() {
	// Тут то же самое, что и в рисовании земли
	glUseProgram(grassShader2);                                                   CHECK_GL_ERRORS
	GLint cameraLocation = glGetUniformLocation(grassShader2, "camera");          CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
	glBindVertexArray(grassVAO2);                                                 CHECK_GL_ERRORS
	// Обновляем смещения для травы
	UpdateGrassVariance2();
	// Отрисовка травинок в количестве GRASS_INSTANCES

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture2);                                  CHECK_GL_ERRORS
	glUniform1i(glGetUniformLocation(grassShader2, "point"), 0);                  CHECK_GL_ERRORS

	bindTextureBuffer(grassShader2, 2, "tboSampler", grass2TBO); // привязываем TBO к имени в шейдере
	bindTextureBuffer(grassShader2, 3, "tboSampler2", grass2TBO2);// привязываем TBO к имени в шейдере

	glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount2, GRASS_INSTANCES_1);   CHECK_GL_ERRORS
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	glUseProgram(0);                                                             CHECK_GL_ERRORS
}

//Рисование травы3
void DrawGrass3() {
	// Тут то же самое, что и в рисовании земли
	glUseProgram(grassShader3);                                                   CHECK_GL_ERRORS
	GLint cameraLocation = glGetUniformLocation(grassShader3, "camera");          CHECK_GL_ERRORS
	glUniformMatrix4fv(cameraLocation, 1, GL_TRUE, camera.getMatrix().data().data()); CHECK_GL_ERRORS
	glBindVertexArray(grassVAO3);                                                 CHECK_GL_ERRORS
	// Обновляем смещения для травы
	UpdateGrassVariance3();
	// Отрисовка травинок в количестве GRASS_INSTANCES

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture3);                                  CHECK_GL_ERRORS
	glUniform1i(glGetUniformLocation(grassShader3, "point"), 0);                  CHECK_GL_ERRORS

	bindTextureBuffer(grassShader3, 2, "tboSampler", grass3TBO); // привязываем TBO к имени в шейдере
	bindTextureBuffer(grassShader3, 3, "tboSampler2", grass3TBO2);// привязываем TBO к имени в шейдере

	glDrawArraysInstanced(GL_TRIANGLES, 0, grassPointsCount3, GRASS_INSTANCES_3);   CHECK_GL_ERRORS
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	glUseProgram(0);                                                             CHECK_GL_ERRORS
}

// Эта функция вызывается для обновления экрана
void RenderLayouts() {
    // Включение буфера глубины
    glEnable(GL_DEPTH_TEST);
    // Очистка буфера глубины и цветового буфера
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Рисуем меши
    DrawGrass();
	DrawGrass1();
	DrawGrass2();
	DrawGrass3();
    DrawGround();
    glutSwapBuffers();
}

// Завершение программы
void FinishProgram() {
    glutDestroyWindow(glutGetWindow());
}

// Обработка события нажатия клавиши (специальные клавиши обрабатываются в функции SpecialButtons)
bool MSAA = false;
void KeyboardEvents(unsigned char key, int x, int y) {
    if (key == 27) {
        FinishProgram();
    } else if (key == 'w') {
        camera.goForward();
    } else if (key == 's') {
        camera.goBack();
    } else if (key == 'm') {
        captureMouse = !captureMouse;
        if (captureMouse) {
            glutWarpPointer(screenWidth / 2, screenHeight / 2);
            glutSetCursor(GLUT_CURSOR_NONE);
        } else {
            glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
        }
	} else if (key == 'a') {
		if (MSAA) {
			glDisable(GL_MULTISAMPLE);
			MSAA = false;
		}
		else {
			glEnable(GL_MULTISAMPLE);
			MSAA = true;
		}

	}
}

// Обработка события нажатия специальных клавиш
void SpecialButtons(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        camera.rotateY(0.02);
    } else if (key == GLUT_KEY_LEFT) {
        camera.rotateY(-0.02);
    } else if (key == GLUT_KEY_UP) {
        camera.rotateTop(-0.02);
    } else if (key == GLUT_KEY_DOWN) {
        camera.rotateTop(0.02);
    }
}

void IdleFunc() {
    glutPostRedisplay();
}

// Обработка события движения мыши
void MouseMove(int x, int y) {
    if (captureMouse) {
        int centerX = screenWidth / 2,
            centerY = screenHeight / 2;
        if (x != centerX || y != centerY) {
            camera.rotateY((x - centerX) / 1000.0f);
            camera.rotateTop((y - centerY) / 1000.0f);
            glutWarpPointer(centerX, centerY);
        }
    }
}

// Обработка нажатия кнопки мыши
void MouseClick(int button, int state, int x, int y) {
}

// Событие изменение размера окна
void windowReshapeFunc(GLint newWidth, GLint newHeight) {
    glViewport(0, 0, newWidth, newHeight);
    screenWidth = newWidth;
    screenHeight = newHeight;

    camera.screenRatio = (float)screenWidth / screenHeight;
}

// Инициализация окна
void InitializeGLUT(int argc, char **argv) {

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitContextVersion(3, 1);
    glutInitWindowPosition(-1, -1);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Computer Graphics 3");
    glutWarpPointer(400, 300);
    glutSetCursor(GLUT_CURSOR_NONE);

    glutDisplayFunc(RenderLayouts);
    glutKeyboardFunc(KeyboardEvents);
    glutSpecialFunc(SpecialButtons);
    glutIdleFunc(IdleFunc);
    glutPassiveMotionFunc(MouseMove);
    glutMouseFunc(MouseClick);
    glutReshapeFunc(windowReshapeFunc);
}

// Генерация позиций травинок (эту функцию вам придётся переписать)
vector<VM::vec2> GenerateGrassPositions() {
    vector<VM::vec2> grassPositions(GRASS_INSTANCES);
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        //grassPositions[i] = VM::vec2((i % 4) / 4.0, (i / 4) / 4.0) + VM::vec2(1, 1) / 8;
		grassPositions[i] = VM::vec2((float)rand()*3/RAND_MAX, (float)rand()*3/RAND_MAX);
    }
    return grassPositions;
}

// Генерация позиций травинок1
vector<VM::vec2> GenerateGrassPositions1() {
	vector<VM::vec2> grassPositions1(GRASS_INSTANCES_1);
	for (uint i = 0; i < GRASS_INSTANCES_1; ++i) {
		grassPositions1[i] = VM::vec2((float)rand() * 3 / RAND_MAX, (float)rand() * 3 / RAND_MAX);
	}
	return grassPositions1;
}

// Генерация позиций травинок2
vector<VM::vec2> GenerateGrassPositions2() {
	vector<VM::vec2> grassPositions2(GRASS_INSTANCES_2);
	for (uint i = 0; i < GRASS_INSTANCES_2; ++i) {
		grassPositions2[i] = VM::vec2((float)rand() * 3 / RAND_MAX, (float)rand() * 3 / RAND_MAX);
	}
	return grassPositions2;
}

// Генерация позиций травинок3
vector<VM::vec2> GenerateGrassPositions3() {
	vector<VM::vec2> grassPositions3(GRASS_INSTANCES_3);
	for (uint i = 0; i < GRASS_INSTANCES_3; ++i) {
		grassPositions3[i] = VM::vec2((float)rand() * 3 / RAND_MAX, (float)rand() * 3 / RAND_MAX);
	}
	return grassPositions3;
}

// Здесь вам нужно будет генерировать меш
vector<VM::vec4> GenMesh(uint n) {
    return {
        /*VM::vec4(0, 0, 0, 1),
        VM::vec4(1, 0, 0, 1),
        VM::vec4(0.5, 1, 0, 1),*/
		VM::vec4(0,0,0,1),
		VM::vec4(0.7,0,0,1),
		VM::vec4(0,0.3,0,1),

		VM::vec4(0.7,0,0,1),
		VM::vec4(0,0.3,0,1),
		VM::vec4(0.7,0.3,0,1),

		VM::vec4(0,0.3,0,1),
		VM::vec4(0.7,0.3,0,1),
		VM::vec4(0,0.6,0,1),

		VM::vec4(0,0.6,0,1),
		VM::vec4(0.7,0.3,0,1),
		VM::vec4(0.7,0.6,0,1),

		VM::vec4(0,0.6,0,1),
		VM::vec4(0.7,0.6,0,1),
		VM::vec4(0.4,0.9,0,1),
    };
}

vector<VM::vec4> GenMesh1(uint n) {
	return{
		/*VM::vec4(0, 0, 0, 1),
		VM::vec4(1, 0, 0, 1),
		VM::vec4(0.5, 1, 0, 1),*/
		VM::vec4(0,0,0,1),
		VM::vec4(0.5,0,0,1),
		VM::vec4(0.5,0.2,0,1),

		VM::vec4(0,0,0,1),
		VM::vec4(0,0.2,0,1),
		VM::vec4(0.5,0.2,0,1),

		VM::vec4(0,0.2,0,1),
		VM::vec4(0.35,0.4,0,1),
		VM::vec4(0.5,0.2,0,1),

		VM::vec4(0.5,0.2,0,1),
		VM::vec4(0.35,0.4,0,1),
		VM::vec4(0.85,0.4,0,1),

		VM::vec4(0.35,0.4,0,1),
		VM::vec4(0.85,0.4,0,1),
		VM::vec4(0.95,0.6,0,1),
	};
}

vector<VM::vec4> GenMesh2(uint n) {
	return{
		VM::vec4(0,0,0,1),
		VM::vec4(0.2,0,0,1),
		VM::vec4(0.2,0.3,0,1),

		VM::vec4(0,0,0,1),
		VM::vec4(0.2,0.3,0,1),
		VM::vec4(0,0.3,0,1),

		VM::vec4(0,0.3,0,1),
		VM::vec4(0.2,0.3,0,1),
		VM::vec4(0,0.6,0,1),

		VM::vec4(0.2,0.3,0,1),
		VM::vec4(0,0.6,0,1),
		VM::vec4(0.2,0.6,0,1),

		VM::vec4(0,0.6,0,1),
		VM::vec4(0.2,0.6,0,1),
		VM::vec4(0.4,1,0,1),

		VM::vec4(0,0.6,0,1),
		VM::vec4(0.4,1,0,1),
		VM::vec4(-0.2,1,0,1),

		VM::vec4(0.4,1,0,1),
		VM::vec4(-0.2,1,0,1),
		VM::vec4(0,1,0,1), //

		VM::vec4(0.4,1,0,1),
		VM::vec4(0.05,1,0,1),
		VM::vec4(0.2,1,0,1), //

		VM::vec4(0.2,0.6,0,1),
		VM::vec4(0.4,0.6,0,1),
		VM::vec4(0.4,1,0,1),

		VM::vec4(0,0.6,0,1),
		VM::vec4(-0.2,0.6,0,1),
		VM::vec4(-0.2,1,0,1),

		VM::vec4(0.15,1,0,1),
		VM::vec4(0.15,1.15,0,1),
		VM::vec4(0,1.15,0,1),

		VM::vec4(0.15,1,0,1),
		VM::vec4(0.15,1.15,0,1),
		VM::vec4(0,1,0,1),
	};
}

vector<VM::vec4> GenMesh3(uint n) {
	return{
		VM::vec4(0,0,0,1),
		VM::vec4(-2.4,0,0,1),
		VM::vec4(-1.8,0.05,0,1),

		VM::vec4(-2.4,0,0,1),
		VM::vec4(-1.8,0.05,0,1),
		VM::vec4(-2.4,0.05,0,1),

		VM::vec4(0,0,0,1),
		VM::vec4(-1.2,0.075,0,1),
		VM::vec4(0,0.125,0,1),

		VM::vec4(-1.2,0.075,0,1),
		VM::vec4(0,0.125,0,1),
		VM::vec4(-1.2,0.125,0,1),

		VM::vec4(0,0,0,1),
		VM::vec4(0.4,0.125,0,1),
		VM::vec4(1.8,0.1,0,1),

		VM::vec4(0.6,0.125,0,1),
		VM::vec4(1.8,0.1,0,1),
		VM::vec4(1.8,0.125,0,1),

		VM::vec4(0,0,0,1),
		VM::vec4(1.8,0.075,0,1),
		VM::vec4(2.4,0.025,0,1),

		VM::vec4(1.2,0.15,0,1),
		VM::vec4(1.6,0.05,0,1),
		VM::vec4(1.6,0.15,0,1),
	};
}

// Создание травы
void CreateGrass() {
    uint LOD = 1;

	grassTexture = SOIL_load_OGL_texture("Texture/grass.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS

    // Создаём меш
    vector<VM::vec4> grassPoints = GenMesh(LOD);
    // Сохраняем количество вершин в меше травы
    grassPointsCount = grassPoints.size();
    // Создаём позиции для травинок
    vector<VM::vec2> grassPositions = GenerateGrassPositions();
    // Инициализация смещений для травинок
    for (uint i = 0; i < GRASS_INSTANCES; ++i) {
        grassVarianceData[i] = VM::vec4(0, 0, 0, 0);
    }

    /* Компилируем шейдеры
    Эта функция принимает на вход название шейдера 'shaderName',
    читает файлы shaders/{shaderName}.vert - вершинный шейдер
    и shaders/{shaderName}.frag - фрагментный шейдер,
    компилирует их и линкует.
    */
    grassShader = GL::CompileShaderProgram("grass");

    // Здесь создаём буфер
    GLuint pointsBuffer;
    // Это генерация одного буфера (в pointsBuffer хранится идентификатор буфера)
    glGenBuffers(1, &pointsBuffer);                                              CHECK_GL_ERRORS
    // Привязываем сгенерированный буфер
    glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
    // Заполняем буфер данными из вектора
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints.size(), grassPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    // Создание VAO
    // Генерация VAO
    glGenVertexArrays(1, &grassVAO);                                             CHECK_GL_ERRORS
    // Привязка VAO
    glBindVertexArray(grassVAO);                                                 CHECK_GL_ERRORS

    // Получение локации параметра 'point' в шейдере
    GLuint pointsLocation = glGetAttribLocation(grassShader, "point");           CHECK_GL_ERRORS
    // Подключаем массив атрибутов к данной локации
    glEnableVertexAttribArray(pointsLocation);                                   CHECK_GL_ERRORS
    // Устанавливаем параметры для получения данных из массива (по 4 значение типа float на одну вершину)
    glVertexAttribPointer(pointsLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS

    // Создаём буфер для позиций травинок
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);                                            CHECK_GL_ERRORS
    // Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositions.size(), grassPositions.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    //GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
    //glEnableVertexAttribArray(positionLocation);                                 CHECK_GL_ERRORS
    //glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    // // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
    //glVertexAttribDivisor(positionLocation, 1);                                  CHECK_GL_ERRORS

    // Создаём буфер для смещения травинок
    glGenBuffers(1, &grassVariance);                                            CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, grassVariance);                               CHECK_GL_ERRORS
    glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES, grassVarianceData.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

    //GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS
    //glEnableVertexAttribArray(varianceLocation);                                 CHECK_GL_ERRORS
    //glVertexAttribPointer(varianceLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
    //glVertexAttribDivisor(varianceLocation, 1);                                  CHECK_GL_ERRORS

    // Отвязываем VAO
    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    // Отвязываем буфер
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS


    // создание иинициализация фикстивной текстуры для чтения из TBO в шейдере
    glGenTextures(1, &grassTBO);                                                 CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, grassTBO);                                  CHECK_GL_ERRORS;
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer);                    CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, 0);	                                       CHECK_GL_ERRORS;

    // ещё раз ... 
    glGenTextures(1, &grassTBO2);                                                 CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, grassTBO2);                                  CHECK_GL_ERRORS;
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, grassVariance);                    CHECK_GL_ERRORS;
    glBindTexture(GL_TEXTURE_BUFFER, 0);	                                        CHECK_GL_ERRORS;

}

// Создание травы1
void CreateGrass1() {
	uint LOD1 = 1;

	grassTexture1 = SOIL_load_OGL_texture("Texture/grass1.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, grassTexture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS

	// Создаём меш
	vector<VM::vec4> grassPoints1 = GenMesh1(LOD1);
	// Сохраняем количество вершин в меше травы
	grassPointsCount1 = grassPoints1.size();
	// Создаём позиции для травинок
	vector<VM::vec2> grassPositions1 = GenerateGrassPositions1();
	// Инициализация смещений для травинок
	for (uint i = 0; i < GRASS_INSTANCES_1; ++i) {
		grassVarianceData1[i] = VM::vec4(0, 0, 0, 0);
	}

	/* Компилируем шейдеры
	Эта функция принимает на вход название шейдера 'shaderName',
	читает файлы shaders/{shaderName}.vert - вершинный шейдер
	и shaders/{shaderName}.frag - фрагментный шейдер,
	компилирует их и линкует.
	*/
	grassShader1 = GL::CompileShaderProgram("grass1");

	// Здесь создаём буфер
	GLuint pointsBuffer1;
	// Это генерация одного буфера (в pointsBuffer хранится идентификатор буфера)
	glGenBuffers(1, &pointsBuffer1);                                              CHECK_GL_ERRORS
	// Привязываем сгенерированный буфер
	glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer1);                                 CHECK_GL_ERRORS
	// Заполняем буфер данными из вектора
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints1.size(), grassPoints1.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	// Создание VAO
	// Генерация VAO
	glGenVertexArrays(1, &grassVAO1);                                             CHECK_GL_ERRORS
	// Привязка VAO
	glBindVertexArray(grassVAO1);                                                 CHECK_GL_ERRORS

	// Получение локации параметра 'point' в шейдере
	GLuint pointsLocation1 = glGetAttribLocation(grassShader1, "point");           CHECK_GL_ERRORS
	// Подключаем массив атрибутов к данной локации
	glEnableVertexAttribArray(pointsLocation1);                                   CHECK_GL_ERRORS
	// Устанавливаем параметры для получения данных из массива (по 4 значение типа float на одну вершину)
	glVertexAttribPointer(pointsLocation1, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS

	// Создаём буфер для позиций травинок
	GLuint positionBuffer1;
	glGenBuffers(1, &positionBuffer1);                                            CHECK_GL_ERRORS
	// Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer1);                               CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositions1.size(), grassPositions1.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	//GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
	//glEnableVertexAttribArray(positionLocation);                                 CHECK_GL_ERRORS
	//glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
	// // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
	//glVertexAttribDivisor(positionLocation, 1);                                  CHECK_GL_ERRORS

	// Создаём буфер для смещения травинок
	glGenBuffers(1, &grassVariance1);                                            CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, grassVariance1);                               CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES_1, grassVarianceData1.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	//GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS
	//glEnableVertexAttribArray(varianceLocation);                                 CHECK_GL_ERRORS
	//glVertexAttribPointer(varianceLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
	//glVertexAttribDivisor(varianceLocation, 1);                                  CHECK_GL_ERRORS

	// Отвязываем VAO
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	// Отвязываем буфер
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS


	// создание иинициализация фикстивной текстуры для чтения из TBO в шейдере
	glGenTextures(1, &grass1TBO);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, grass1TBO);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer1);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                       CHECK_GL_ERRORS;

	// ещё раз ... 
	glGenTextures(1, &grass1TBO2);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, grass1TBO2);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, grassVariance1);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                        CHECK_GL_ERRORS;

}

// Создание травы2
void CreateGrass2() {
	uint LOD2 = 1;

	grassTexture2 = SOIL_load_OGL_texture("Texture/grass22.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, grassTexture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS

	// Создаём меш
	vector<VM::vec4> grassPoints2 = GenMesh2(LOD2);
	// Сохраняем количество вершин в меше травы
	grassPointsCount2 = grassPoints2.size();
	// Создаём позиции для травинок
	vector<VM::vec2> grassPositions2 = GenerateGrassPositions2();
	// Инициализация смещений для травинок
	for (uint i = 0; i < GRASS_INSTANCES_2; ++i) {
		grassVarianceData2[i] = VM::vec4(0, 0, 0, 0);
	}

	/* Компилируем шейдеры
	Эта функция принимает на вход название шейдера 'shaderName',
	читает файлы shaders/{shaderName}.vert - вершинный шейдер
	и shaders/{shaderName}.frag - фрагментный шейдер,
	компилирует их и линкует.
	*/
	grassShader2 = GL::CompileShaderProgram("grass2");

	// Здесь создаём буфер
	GLuint pointsBuffer2;
	// Это генерация одного буфера (в pointsBuffer хранится идентификатор буфера)
	glGenBuffers(1, &pointsBuffer2);                                              CHECK_GL_ERRORS
	// Привязываем сгенерированный буфер
	glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer2);                                 CHECK_GL_ERRORS
	// Заполняем буфер данными из вектора
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints2.size(), grassPoints2.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	// Создание VAO
	// Генерация VAO
	glGenVertexArrays(1, &grassVAO2);                                             CHECK_GL_ERRORS
	// Привязка VAO
	glBindVertexArray(grassVAO2);                                                 CHECK_GL_ERRORS

	// Получение локации параметра 'point' в шейдере
	GLuint pointsLocation2 = glGetAttribLocation(grassShader2, "point");           CHECK_GL_ERRORS
	// Подключаем массив атрибутов к данной локации
	glEnableVertexAttribArray(pointsLocation2);                                   CHECK_GL_ERRORS
	// Устанавливаем параметры для получения данных из массива (по 4 значение типа float на одну вершину)
	glVertexAttribPointer(pointsLocation2, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS

	// Создаём буфер для позиций травинок
	GLuint positionBuffer2;
	glGenBuffers(1, &positionBuffer2);                                            CHECK_GL_ERRORS
	// Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer2);                               CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositions2.size(), grassPositions2.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	//GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
	//glEnableVertexAttribArray(positionLocation);                                 CHECK_GL_ERRORS
	//glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
	// // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
	//glVertexAttribDivisor(positionLocation, 1);                                  CHECK_GL_ERRORS

	// Создаём буфер для смещения травинок
	glGenBuffers(1, &grassVariance2);                                            CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, grassVariance2);                               CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES_2, grassVarianceData2.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	//GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS
	//glEnableVertexAttribArray(varianceLocation);                                 CHECK_GL_ERRORS
	//glVertexAttribPointer(varianceLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
	//glVertexAttribDivisor(varianceLocation, 1);                                  CHECK_GL_ERRORS

	// Отвязываем VAO
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	// Отвязываем буфер
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS


	// создание иинициализация фикстивной текстуры для чтения из TBO в шейдере
	glGenTextures(1, &grass2TBO);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, grass2TBO);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer2);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                       CHECK_GL_ERRORS;

	// ещё раз ... 
	glGenTextures(1, &grass2TBO2);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, grass2TBO2);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, grassVariance2);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                        CHECK_GL_ERRORS;

}

// Создание травы3
void CreateGrass3() {
	uint LOD3 = 1;

	grassTexture3 = SOIL_load_OGL_texture("Texture/flower.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, grassTexture3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS

	// Создаём меш
	vector<VM::vec4> grassPoints3 = GenMesh3(LOD3);
	// Сохраняем количество вершин в меше травы
	grassPointsCount3 = grassPoints3.size();
	// Создаём позиции для травинок
	vector<VM::vec2> grassPositions3 = GenerateGrassPositions3();
	// Инициализация смещений для травинок
	for (uint i = 0; i < GRASS_INSTANCES_3; ++i) {
		grassVarianceData3[i] = VM::vec4(0, 0, 0, 0);
	}

	/* Компилируем шейдеры
	Эта функция принимает на вход название шейдера 'shaderName',
	читает файлы shaders/{shaderName}.vert - вершинный шейдер
	и shaders/{shaderName}.frag - фрагментный шейдер,
	компилирует их и линкует.
	*/
	grassShader3 = GL::CompileShaderProgram("grass3");

	// Здесь создаём буфер
	GLuint pointsBuffer3;
	// Это генерация одного буфера (в pointsBuffer хранится идентификатор буфера)
	glGenBuffers(1, &pointsBuffer3);                                              CHECK_GL_ERRORS
	// Привязываем сгенерированный буфер
	glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer3);                                 CHECK_GL_ERRORS
	// Заполняем буфер данными из вектора
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * grassPoints3.size(), grassPoints3.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	// Создание VAO
	// Генерация VAO
	glGenVertexArrays(1, &grassVAO3);                                             CHECK_GL_ERRORS
	// Привязка VAO
	glBindVertexArray(grassVAO3);                                                 CHECK_GL_ERRORS

	// Получение локации параметра 'point' в шейдере
	GLuint pointsLocation3 = glGetAttribLocation(grassShader3, "point");           CHECK_GL_ERRORS
	// Подключаем массив атрибутов к данной локации
	glEnableVertexAttribArray(pointsLocation3);                                   CHECK_GL_ERRORS
	// Устанавливаем параметры для получения данных из массива (по 4 значение типа float на одну вершину)
	glVertexAttribPointer(pointsLocation3, 4, GL_FLOAT, GL_FALSE, 0, 0);          CHECK_GL_ERRORS

	// Создаём буфер для позиций травинок
	GLuint positionBuffer3;
	glGenBuffers(1, &positionBuffer3);                                            CHECK_GL_ERRORS
	// Здесь мы привязываем новый буфер, так что дальше вся работа будет с ним до следующего вызова glBindBuffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer3);                               CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec2) * grassPositions3.size(), grassPositions3.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	//GLuint positionLocation = glGetAttribLocation(grassShader, "position");      CHECK_GL_ERRORS
	//glEnableVertexAttribArray(positionLocation);                                 CHECK_GL_ERRORS
	//glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
	// // Здесь мы указываем, что нужно брать новое значение из этого буфера для каждого инстанса (для каждой травинки)
	//glVertexAttribDivisor(positionLocation, 1);                                  CHECK_GL_ERRORS

	// Создаём буфер для смещения травинок
	glGenBuffers(1, &grassVariance3);                                            CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, grassVariance3);                               CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * GRASS_INSTANCES_3, grassVarianceData3.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	//GLuint varianceLocation = glGetAttribLocation(grassShader, "variance");      CHECK_GL_ERRORS
	//glEnableVertexAttribArray(varianceLocation);                                 CHECK_GL_ERRORS
	//glVertexAttribPointer(varianceLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);        CHECK_GL_ERRORS
	//glVertexAttribDivisor(varianceLocation, 1);                                  CHECK_GL_ERRORS

	// Отвязываем VAO
	glBindVertexArray(0);                                                        CHECK_GL_ERRORS
	// Отвязываем буфер
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS


	// создание иинициализация фикстивной текстуры для чтения из TBO в шейдере
	glGenTextures(1, &grass3TBO);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, grass3TBO);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RG32F, positionBuffer3);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                       CHECK_GL_ERRORS;

	// ещё раз ... 
	glGenTextures(1, &grass3TBO2);                                                 CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, grass3TBO2);                                  CHECK_GL_ERRORS;
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, grassVariance3);                    CHECK_GL_ERRORS;
	glBindTexture(GL_TEXTURE_BUFFER, 0);	                                        CHECK_GL_ERRORS;

}

// Создаём камеру (Если шаблонная камера вам не нравится, то можете переделать, но я бы не стал)
void CreateCamera() {
    camera.angle = 45.0f / 180.0f * M_PI;
    camera.direction = VM::vec3(0, 0.3, -1);
    camera.position = VM::vec3(0.5, 0.2, 0);
    camera.screenRatio = (float)screenWidth / screenHeight;
    camera.up = VM::vec3(0, 1, 0);
    camera.zfar = 50.0f;
    camera.znear = 0.05f;
}

// Создаём замлю
void CreateGround() {
    // Земля состоит из двух треугольников
    vector<VM::vec4> meshPoints = {
        VM::vec4(0, 0, 0, 1),
        VM::vec4(3, 0, 0, 1),
        VM::vec4(3, 0, 3, 1),
        VM::vec4(0, 0, 0, 1),
        VM::vec4(3, 0, 3, 1),
        VM::vec4(0, 0, 3, 1),
    };

    // Подробнее о том, как это работает читайте в функции CreateGrass
    groundShader = GL::CompileShaderProgram("ground");

    GLuint pointsBuffer;
	glGenBuffers(1, &pointsBuffer);                                              CHECK_GL_ERRORS
	glBindBuffer(GL_ARRAY_BUFFER, pointsBuffer);                                 CHECK_GL_ERRORS
	glBufferData(GL_ARRAY_BUFFER, sizeof(VM::vec4) * meshPoints.size(), meshPoints.data(), GL_STATIC_DRAW); CHECK_GL_ERRORS

	glGenVertexArrays(1, &groundVAO);                                            CHECK_GL_ERRORS
	glBindVertexArray(groundVAO);                                                CHECK_GL_ERRORS

	GLuint index = glGetAttribLocation(groundShader, "point");                   CHECK_GL_ERRORS
	glEnableVertexAttribArray(index);                                            CHECK_GL_ERRORS
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);                   CHECK_GL_ERRORS

	//
	groundTexture = SOIL_load_OGL_texture("Texture/water3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); CHECK_GL_ERRORS
	//

    glBindVertexArray(0);                                                        CHECK_GL_ERRORS
    glBindBuffer(GL_ARRAY_BUFFER, 0);                                            CHECK_GL_ERRORS
}

int main(int argc, char **argv){

    try {
        cout << "Start" << endl;
        InitializeGLUT(argc, argv);
        cout << "GLUT inited" << endl;
        glewInit();
        cout << "glew inited" << endl;
        CreateCamera();
        cout << "Camera created" << endl;
        CreateGrass();
        cout << "Grass created" << endl;
		CreateGrass1();
		cout << "Grass1 created" << endl;
		CreateGrass2();
		cout << "Grass2 created" << endl;
		CreateGrass3();
		cout << "Grass3 created" << endl;
        CreateGround();
        cout << "Ground created" << endl;
        glutMainLoop();
    } catch (string s) {
        cout << s << endl;
    }
}
