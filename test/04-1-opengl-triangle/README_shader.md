## SHADERS STEP by STEP
<a href="http://docs.gl/"><b>OPEN GL documentation</b><a><br>
<a href="https://www.khronos.org/opengles/sdk/docs/reference_cards/OpenGL-ES-2_0-Reference-card.pdf"><b>ES 2.0</b><a><br>
1. Сначала создаем шейдер вершин, возвращает ненулевое значение, дескриптор:<br>
<a href="http://docs.gl/es2/glCreateShader"><b>glCreateShader</b><a><br>
	
```
GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	
```
2. Создаем строку, в котором прописываем текст шейдера:<br>
  <b>attribute</b> - спецификатор<br>
  переменные могут быть объявлены предварительно в хранилище спецификатора<br>
  Связь между вершинным шейдером и OpenGL ES для первичных данных по вершинам<br>
  <b>vec2</b> - 2 вектора типа float<br>
  <b>gl_Position</b> - Outputs: специальная переменная для вертексного шейдера<br>
	трансформированные вершины <br>
```
const GLchar* vertex_shader_src = R"(
attribute vec2 a_position;
void main()
{
	gl_Position = vec4(a_position, 0.f, 1.f);
})";
	
```  
3. Заменяем исходный код в шейдерном объекте:<br>
<a href="http://docs.gl/es2/glShaderSource"><b>glShaderSource</b><a><br>
	
```
glShaderSource(vert_shader, 1, &vertex_shader_src, NULL);
	
```  
4. Компилируем шейдер:<br>
<a href="http://docs.gl/es2/glCompileShader"><b>glCompileShader</b><a><br>
	
```
glCompileShader(vert_shader);
	
```  
5. Проверка компиляции:<br>
<a href="http://docs.gl/es2/glGetShaderiv"><b>glGetShaderiv</b><a><br>
<a href="http://docs.gl/es2/glGetShaderInfoLog"><b>glGetShaderInfoLog</b><a><br>
	
```
GLint  success;
char infoLog[512];
///returns shader parametr
glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
if(!success)
{
	glGetShaderInfoLog(vert_shader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
}
	
```  
6. После того как скомпилировали вершинный шейдер, можно создать фрагментный шейдер:<br>
<a href="http://docs.gl/es2/glCreateShader"><b>glCreateShader</b><a><br>
	
```
GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	
```
7. Создаем строку, в котором прописываем текст шейдера:<br>
  <b>gl_FragColor</b> - Outputs: цвет фрагмента<br>
	специальные переменные для фрагментного шейдера<br>
```
const GLchar* fragment_shader_src = R"(
void main()
{
	gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
})";

```
8. Заменяем исходный код в шейдерном объекте, компилируем, проверяем:<br>
	
```
glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
glCompileShader(fragment_shader);

glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
if(!success)
{
	glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
}
	
``` 
9. После того как шейдеры успешно скомпилированы, можно создать общий шейдер:<br>
<a href="http://docs.gl/es2/glCreateProgram"><b>glCreateProgram</b><a><br>
если программа не была создана, удаляем наши шейдеры<br>
<a href="http://docs.gl/es2/glDeleteShader"><b>glDeleteShader</b><a><br>
	
```
GLuint shader_program = glCreateProgram();
if (0 == shader_program)
{
	std::cout << "failed to create gl program";
	glDeleteShader(vert_shader);
	glDeleteShader(fragment_shader);
}
	
```
10. Прикрепляем наши шейдеры к программе:<br>
<a href="http://docs.gl/es2/glAttachShader"><b>glAttachShader</b><a><br>
	
```
glAttachShader(shader_program, vert_shader);
glAttachShader(shader_program, fragment_shader);
	
```
11. Связываем атрибут вершины из шейдера с проименованной переменной атрибута:<br>
<a href="http://docs.gl/es2/glBindAttribLocation"><b>glBindAttribLocation</b><a><br>
	
```
glBindAttribLocation(shader_program, 0, "a_position");
	
```
12. Линкуем нашу шейдерную програму:<br>
<a href="http://docs.gl/es2/glLinkProgram"><b>glLinkProgram</b><a><br>
	
```
glLinkProgram(shader_program);
	
```
13. Проверяем слинковалась ли:<br>
<a href="http://docs.gl/es2/glGetProgramiv"><b>glGetProgramiv</b><a><br>
<a href="http://docs.gl/es2/glGetProgramInfoLog"><b>glGetProgramInfoLog</b><a><br>
	
```
glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
if (program_linked != GL_TRUE)
{
	GLsizei log_length = 0;
	GLchar message[1024];
	glGetProgramInfoLog(shader_program, 1024, &log_length, message);
	std::cout << "ERROR::LINKED IS_FAILED\n" << message << std::endl;
}
	
```
14. Устанавливаем наш программный объект как часть текущего состояния рендеринга:<br>
<a href="http://docs.gl/es2/glUseProgram"><b>glUseProgram</b><a><br>
	
```
glUseProgram(shader_program);
	
```
15. после того, как мы отправили входные данные на GPU<br>
мы должны проиструктировать GPU как обрабатывать данные вершин в шейдерах:<br>
<a href="http://docs.gl/es2/glVertexAttribPointer"><b>glVertexAttribPointer</b><a><br>
	
```
te::triangle& t; (наш триугольник)
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(te::vertex), &t.v[0]);
где 0 - это дескриптор

```
16. включаем массив атрибута вершин, теперь этот массив доступен для рендеринга при вызове команд<br>
вершинного массива, таких как glDrawArrays или glDrawElements.<br>
<a href="http://docs.gl/es2/glEnableVertexAttribArray"><b>glEnableVertexAttribArray</b><a><br>
	
```
glEnableVertexAttribArray(0);
	
```
17. рендерит примитивы из массива данных<br>
<a href="http://docs.gl/es2/glDrawArrays"><b>glDrawArrays</b><a><br>
	
```
glDrawArrays(GL_TRIANGLES, 0, 3);
	
```
